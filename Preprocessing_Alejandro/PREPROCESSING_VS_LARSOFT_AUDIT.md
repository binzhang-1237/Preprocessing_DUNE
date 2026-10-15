# TransformerCVN preprocessing 与 LArSoft 重建差异调查

## 1. 文档目的与范围

这份文档记录 Alejandro 提供的标准 preprocessing 链与当前 LArSoft
TransformerCVN 重建链之间已经确认的差异，供后续调查 PID 效率下降时直接使用。

本次源码比较严格限定为：

- `Preprocessing_Alejandro/` 中不含 `atmnu` 的文件：
  - `make_text_file_to_root_trks_shws.C`
  - `preprocess2.py`
  - `CreateFullySparseDataset.ipynb`
  - `Readme.txt`
- 当前重建代码：
  - `../srcs/dunereco/dunereco/TransformerCVN/art/`
  - `../srcs/dunereco/dunereco/TransformerCVN/func/`
  - pixel-map boundary 直接依赖的 `RegCNNBoundary`

不包含 `*atmnu*` 文件，也没有把其他训练仓库中的实现作为本报告的证据。

当前已知性能结果：

- Python preprocessing + training/evaluation：约 95.1% true numu CC PID efficiency。
- LArSoft 原始 V 方向：约 88%。
- LArSoft 完整 400-bin V flip (`new_wire = 399 - old_wire`)：约 90.2%。
- 历史 350-bin LArSoft flip 会丢弃最后 50 列，效率变差。

## 2. 两条数据链

### 2.1 标准 preprocessing 链

```text
ART ROOT
  -> RecoEnergyS flat tree recoEnergy/WC
  -> make_text_file_to_root_trks_shws.C
  -> root2hdf5 /ArrayOfStructures
  -> preprocess2.py
  -> CreateFullySparseDataset.ipynb
  -> training/evaluation HDF5
```

`Readme.txt` 明确说明 macro 是实际制造 pixel maps 的步骤，后续步骤主要负责格式转换。
但源码检查表明 notebook 还会改变 V-plane wire coordinate，因此“后续不改变 pixel map”
并不完全成立。

`RecoEnergyS` 和 `root2hdf5` 源码目前不在本目录中，因此它们产生 branch 的具体定义仍未验证。

### 2.2 当前 LArSoft 链

```text
recob::Hit + Pandora vertex/track/shower associations
  -> TransformerCVNMapper_module.cc
  -> TransformerPixelMapProducer.cxx
  -> TransformerPixelMap U/V/Z arrays
  -> TransformerCVNEvaluator_module.cc
  -> deployed TorchScript
```

当前默认 map 配置见 `TransformerCVNMapper.fcl`：

```fcl
MinClusterHits: 1
TdcWidth: 280
WireLength: 400
TimeResolution: 24
WireResolution: 7
UseRawHitCharge: false
FlipVPlaneWire: false
```

## 3. 已确认的高风险差异

### 3.1 网络实际使用的 charge 不同

标准 macro 同时制造 raw 和 corrected 两张 histogram：

```cpp
h2_evt1[iplane][0]->Fill(..., hit_charge[ihit]);
h2_evt2[iplane][0]->Fill(..., hit_corrcharge[ihit]);
```

并分别写出 `wire_charge` 和 `wire_corrcharge`。但是 `preprocess2.py` 对 event map
和 prong map 都明确选择 `wire_charge`：

```python
fout.create_earray(root, "cvnmap_value", obj=df["wire_charge"][evt_image_indices])
fout.create_earray(root, "png_cvnmap_value", obj=df["wire_charge"][png_image_indices])
```

因此 preprocessing 链最终保存给后续数据集的 pixel value 是 raw `wire_charge`，
不是 `wire_corrcharge`。

当前 LArSoft 默认 `UseRawHitCharge: false`，使用：

```cpp
correctedHitCharge = hit->Integral() * exp(... / ElectronLifetime());
mapHitCharge = fUseRawHitCharge ? hit->Integral() : correctedHitCharge;
```

所以默认网络输入是 LArSoft 内部施加 lifetime correction 后的 `Hit::Integral()`。

需要注意：Python 图的数值虽为 raw charge，macro 决定 sparse row 是否保留时却检查
`phit_corrcharge`：

```cpp
if (at_least_one && phit_corrcharge == 0) continue;
if (phit_corrcharge < -600) continue;
```

因此 Python 路径实际是：

```text
raw charge 作为 pixel value
+ corrected charge 决定部分 pixel 是否输出
```

LArSoft raw-charge 模式只完成了第一项，并没有复制第二项筛选。此前 raw-charge
LArSoft 测试效率下降，不能单独证明 charge 不重要，因为当时其他 map 差异仍存在。

### 3.2 V-plane 的 `349 - y` 与 400-wide shape 冲突

standard macro 本身没有 V flip。V flip 出现在
`CreateFullySparseDataset.ipynb` 的 `sparse_to_sparse()`：

```python
if num_features == 3 and c == 1:
    y = 349 - y
```

其中：

- `c` 是 U/V/Z channel，`c == 1` 是 V plane；
- `y` 来自 `rel_wire`；
- `x` 来自 `rel_tick`。

但是 `preprocess2.py` 将标准 map shape 固定为：

```python
[total_events, 3, 400, 280]
```

所以 preprocessing 链内部同时存在：

```text
wire width = 400
V flip     = 349 - wire
```

其直接结果是：

- 原始 wire `0..349` 映射到 `349..0`；
- 原始 wire `350..399` 映射到 `-1..-50`。

notebook 没有显式删除或拒绝这些负坐标。它们在后续 sparse backend 中究竟报错、
被忽略还是产生别的行为，单凭本目录源码无法确认。还必须检查训练 HDF5 中 V-plane
是否实际存在 `rel_wire >= 350` 的非零 pixel。

当前完整 LArSoft flip 为：

```text
new_wire = 399 - old_wire
```

它保存全部 400 列，但并不逐像素等价于 notebook 的 `349 - y`。对旧 wire
`0..349`，两者相差固定 50 个 pixel。

历史 LArSoft 350-bin 实现则只处理 `0..349` 并把 `350..399` 留为零。这与 notebook
也不完全等价，而且确认会丢失 charge。

### 3.3 event population 不同

standard macro 先要求：

```cpp
if (nhits < 100) continue;
```

当前 LArSoft 要求 `nhits > fMinClusterHits`，默认 `MinClusterHits: 1`。

所以 Python macro 只保留至少 100 hits 的事件，而 LArSoft mapper 接受超过 1 hit 的事件。
这会改变效率统计使用的事件总体，尤其可能将更难分类的低-hit事件只留在 LArSoft 样本中。

LArSoft 还要求找到 Pandora primary vertex；缺 vertex 或最终 map 为空时不产生 event PID。
macro 则依赖 RecoEnergyS 已写出的 `PFVtxGlobal*`，没有同样的运行时 vertex-validity 检查。

### 3.4 standard macro 的 hit-based FV-like veto

macro 遍历所有 hits。只要任一 hit 满足以下条件就设置 `fiducial_cut`：

- global plane 2 且 `tpc < 4 && local_wire < 5`；
- global plane 2 且 `tpc > 19 && local_wire > 474`；
- 任意 plane 的 `hit_peakT > 4482`。

但 veto 只在下面条件成立时执行：

```cpp
if (type == "nue" && fiducial_cut) continue;
```

所以它不是标准的 reconstructed-vertex 3D FV cut，而是一个 hit-boundary event veto；
并且仅影响 `type="nue"`。如果 95.1% 数据由 `type="numu"` 产生，它不会生效。

当前 LArSoft mapper 没有对应的 hit-based veto。

另一个细节是 `f_unique_id++` 发生在 FV veto 之前。因此 `type="nue"` 下被 veto 的
事件会在 `unique_id` 中留下空洞，可能破坏 `preprocess2.py` 对连续 event index 的假设。

### 3.5 prong map 的 charge 归属算法不同

Python macro 在 hit 层面直接将每个 hit 放入对应 prong histogram：

```text
shower: histogram index = prong_tag + 1
track:  histogram index = n_showers + prong_tag - 100 + 1
```

因此同一 coarse pixel 内属于不同 prong 的 hits 仍会分别进入各自的 prong map。

LArSoft 先把所有 hits 累加进 pixel，并在每次写 pixel 时覆盖该 pixel 的 prong tag。
`TransformerPixelMap::Finish()` 再根据最后留下的 tag 决定整个 pixel 是否保留。

如果同一个 pixel 含多个 prong：

- 最后一个 hit 属于目标 prong：可能把其他 prong 的 charge 一起保留；
- 最后一个 hit 不属于目标 prong：可能把目标 prong 的 charge 一起清零。

LArSoft 源码注释本身也把这一点标为 caveat。这是直接改变 prong image 内容的高风险差异。

### 3.6 prong 顺序和前 20 个 prong 不同

Python macro 的输出顺序是：

```text
event map -> all showers -> all tracks
```

当前 LArSoft mapper 的 `prong_pmCol` 顺序是：

```text
all tracks -> all showers
```

evaluator 最多读取 `MaxProngs: 20`。如果一个事件超过 20 个 prongs，两条链送入模型的
前 20 个 prongs 可能不是同一批。Transformer event PID 同时使用 event image 与 prong
images，因此这会直接影响 event score，不只是 prong PID。

LArSoft 还有潜在 ID 上界问题：它把遇到的最大 track/shower ID 保存为 `nTracks` 或
`nShowers`，之后使用 `i < nTracks` 和 `i < nShowers` 遍历。是否漏掉最后一个 ID 取决于
association ID 是 0-based、1-based 还是不连续，当前没有用运行数据验证。

## 4. 坐标、crop 与像素边界差异

### 4.1 map 模式必须确认

standard macro 支持两种模式，最终 shape 都是 `400 x 280`：

| macro type | `x_coarse` | `y_coarse` | 实际覆盖范围 |
|---|---:|---:|---:|
| `nue`（默认） | 1 | 6 | `400 x 1680` raw units |
| `numu` | 7 | 24 | `2800 x 6720` raw units |

当前 LArSoft 为 `WireResolution=7`、`TimeResolution=24`，对应 macro 的 `type="numu"`。

如果产生 95.1% 数据时 macro 使用默认参数、没有显式传入 `"numu"`，那么即使最终 tensor
都是 `400 x 280`，两边每个 pixel 覆盖的物理范围也完全不同。必须从当时运行命令确认。

### 4.2 global coordinate 来源不同

Python macro 直接读取 RecoEnergyS branch：

- `Hit_Global_Wire`
- `Hit_Global_Tick`
- `Hit_Global_Plane`
- `PFVtxGlobalWire/Tick/Plane`

LArSoft 从 `WireID`、`Hit::PeakTime()` 和 Pandora XYZ vertex 现场调用
`GetDUNEGlobalWireTDC()` 计算 global coordinate。

在没有 RecoEnergyS 源码前，以下内容都仍未验证：

- 两者 plane swap 是否一致；
- V global-wire block 方向是否一致；
- tick reversal 与 drift/APA offset 是否一致；
- wire gap 是否一致；
- PF vertex 转换是否一致；
- `Hit_Charge` 是否就是 `Hit::Integral()`；
- `Hit_CorrCharge` 是否等于当前 LArSoft lifetime correction。

### 4.3 crop center 的 nominal shift 相同

两边都使用：

```text
shift = 56 * WireResolution
U center += full_width / 2 - shift
V center -= full_width / 2 - shift
Z center += full_width / 2 - shift
```

公式表面一致，但 center 的输入来源不同，因此最终 crop 仍可能不同。

### 4.4 rounding 与边界可能产生 edge-pixel 差异

Python macro：

1. 将 `Hit_Global_Wire` 从 `double` 截断成 `int`；
2. `rel_time` 也存进 `int`，因此 global tick 小数部分会先被截断；
3. 再执行 `round(rel/resolution)`；
4. 用 `TH2D::Fill()`，范围外坐标进入 underflow/overflow，最终不会被正常-bin循环输出。

LArSoft：

1. 先对 transformed peak time 做 `round()`；
2. 用 `RegCNNBoundary::IsWithin()` 做半开区间检查；
3. 再在 `GlobalToIndexSingle()` 中对 coarse coordinate 做 `round()`。

因此 crop 边缘及 `.5` 附近的 hits 可能落入不同 pixel。这通常不像 charge/V flip 那么大，
但会阻止两边逐像素完全相等。

## 5. preprocessing 后处理中的其他风险

### 5.1 `preprocess2.py` 会静默跳过整个文件

如果下面任一一致性检查失败：

```python
len(events) != len(evt_start_indices)
len(png_E) != len(evt_start_indices)
```

代码会直接 `continue`，静默跳过当前整个输入文件。这会改变最终样本组成。

### 5.2 输入文件顺序被随机打乱

`preprocess2.py` 使用：

```python
random.shuffle(fins)
```

这不改变单个 map，但改变 event 排列。若后续 train/test split 依赖顺序，结果会受影响。

### 5.3 notebook 可能应用外部 selection mask

如果与输入 HDF5 同名的 `.select.npy` 存在，notebook 会额外筛选 event。当前无法仅凭源码
确认生成 95.1% 结果时是否存在该文件。

### 5.4 macro 为每个 plane/prong 保留一条初始零 pixel

`at_least_one` 为 false 时，第一个扫描 pixel 即使 charge 为零也会写出，保证每个
plane/prong 至少有一行。最终 notebook 的 `sparse_to_sparse()` 会跳过 `v == 0`，所以它通常
不改变最终 charge image，但会参与 `preprocess2.py` 对 event/prong 起点的识别。

### 5.5 `.skim.` tree 名称逻辑未实际使用

macro 计算了：

```cpp
if (input.Contains(".skim.")) name_tree = "WC";
else name_tree = "recoEnergy/WC";
```

但随后仍固定构造：

```cpp
TChain *ch = new TChain("recoEnergy/WC");
```

变量 `name_tree` 没有被使用。若输入是只含顶层 `WC` 的 skim 文件，该逻辑不会按注释工作。

## 6. 已确认基本一致的部分

在 macro 确实使用 `type="numu"` 的前提下：

- map shape 均为 `3 x 400 x 280`；
- coarse resolution 均为 wire 7、tick 24；
- plane/channel 顺序均为 U、V、Z；
- 单 plane 内均为 wire-major、tick-minor；
- 同一 event pixel 中多个 hits 的 charge 都会相加；
- LArSoft evaluator flatten 顺序为 U、V、Z；
- event map 都使用 crop 内所有 hits，不要求 hit 已分配给 prong。

这些一致性不代表 map 已逐像素相同，因为 charge、V flip、global coordinate 和边界仍不同。

## 7. PID 差异候选优先级

按当前源码证据和潜在影响排序：

1. **V-plane `349 - y` 与 400-wide shape 的冲突。**
2. **Python 最终使用 raw charge，而 LArSoft 默认使用 lifetime-corrected charge。**
3. **prong map 的 pixel 归属算法、顺序和前 20 个 prong 不一致。**
4. **Python `nhits >= 100` 与 LArSoft `nhits > 1` 导致 event population 不同。**
5. **RecoEnergyS 与 `GetDUNEGlobalWireTDC()` 的 global coordinate 定义未知。**
6. **macro mode 可能误用默认 `nue` 而非与 LArSoft 对应的 `numu`。**
7. **rounding、crop 上界和 TH2 underflow/overflow 的边缘差异。**
8. **`preprocess2.py` 静默跳文件或 notebook 外部 selection mask。**

这些效应目前不能通过总体 PID efficiency 单独解耦。

## 8. 后续问题的直接切入点

### 问题 A：`349` 到底是 bug 还是有效区域约定？

直接检查最终 training/evaluation HDF5 或 `preprocess2.py` 输出：

1. 统计 V plane 中 `rel_wire >= 350` 的非零 pixel 数和 charge 总量；
2. 检查 notebook 输出 coordinates 是否存在负 wire；
3. 对同一批 event 比较：无 flip、`349-y`、`399-y`；
4. 不要再用“仅翻前 350 列、其余清零”的历史实现。

### 问题 B：charge 应该如何与训练输入对齐？

先取得 RecoEnergyS 源码，确认：

1. `Hit_Charge` 的定义；
2. `Hit_CorrCharge` 的完整公式；
3. 是否还有 calibration、gain、lifetime 或负 charge 处理；
4. 用同一 event 比较 `Hit_Charge` 与 LArSoft `Hit::Integral()`；
5. 复制 Python 的“raw value + corrected-value sparse mask”后再做独立测试。

### 问题 C：event sample 是否一致？

为 Python 和 LArSoft 生成 event manifest，至少记录：

- run/subrun/event；
- hit count；
- vertex 是否存在；
- FV-like veto 状态；
- map 是否非空；
- truth class；
- 是否得到网络 result。

只在交集事件上重新计算 paired efficiency，并检查被 Python `nhits < 100` 排除的事件效率。

### 问题 D：prong maps 是否导致 event PID 下降？

对同一个 event：

1. 比较 Python 与 LArSoft 的 prong 数量和顺序；
2. 比较前 20 个 prong 的类型及 charge；
3. 统计一个 pixel 中出现多个 prong hits 的频率；
4. 将 LArSoft 改为 hit-level 按 prong 累加，避免 last-hit tag；
5. 检查 `nTracks/nShowers` 最大 ID 循环是否漏掉最后一个 prong。

### 问题 E：global coordinates 是否一致？

取得 RecoEnergyS 后，逐项对照：

- local plane 到 global plane；
- local wire 到 global wire；
- V block 方向；
- local tick 到 global tick；
- drift direction；
- wire gap 与 APA offset；
- PF vertex global coordinates。

最强验证是同一个 ART event、同一批 hits 的逐 hit coordinate table，而不是只比较最终图片。

### 问题 F：如何做最终端到端 parity test？

建议固定一个输入文件和少量 event：

1. 从 RecoEnergyS/macro 保存 Python dense U/V/Z maps；
2. 从 mapper 的 `PixelMapDiagnostic` 保存最终 `pm.fPEX/fPEY/fPEZ`；
3. 统一 charge 与 V convention；
4. 比较每个 plane 的：非零 pixel 数、charge 总量、坐标交集、L1/L2 difference；
5. 再比较完全相同 tensor 输入时 Python 与 LArSoft TorchScript score。

只有这项测试能把 map-construction 差异与模型/evaluator 差异明确分开。

## 9. 后续调查前必须确认的问题

1. 95.1% 的数据制作时，macro 调用的 `type` 是 `nue` 还是 `numu`？
2. 95.1% 的 efficiency 分母是否经过 `nhits >= 100`、FV-like veto 或 `.select.npy`？
3. 训练 HDF5 的 V plane 是否存在原始 `rel_wire >= 350`？
4. 当时使用的 RecoEnergyS 版本和 geometry 是什么？
5. Python 和 LArSoft 比较是否基于相同的 run/subrun/event 交集？
6. 两边是否使用完全相同的 Pandora hit、track、shower 和 vertex products？

在这些问题回答之前，不能把剩余约 5 个百分点的差距归因于单一因素。
# RecoEnergyS_module.cc 功能解析

本文解析 `RecoEnergyS_module.cc`（`RecoEnergyStudies` 中的旧版实现）。它是一个 LArSoft `art::EDAnalyzer`：对每个输入 ART event 读取重建与 MC 产品，将结果展平写入 ROOT tree `WC`。在通常的 ART 输出结构中，该 tree 的访问路径为 `recoEnergy/WC`。

它不直接生成 pixel map；后续 macro 读取该 tree 的 hit、顶点与 prong 标签 branch 后才生成 pixel map。

## 总体结构

```text
RecoEnergyS_module.cc
|- 类与成员变量：输入产品、services、ROOT branch 缓冲区
|- 构造函数：创建 TTree "WC"，注册输出 branch
|- reconfigure()：读取 FHiCL label 与坐标配置
|- analyze()：逐 event 的主处理流程
|- 坐标工具：XYZ <-> wire/tick/global wire/tick
|- truth 工具：hit、shower、track 的 MC matching
`- reset()：每个 event 前清空状态
```

## 1. 类与输入接口

源码从类声明开始：[`RecoEnergyS_module.cc`](RecoEnergyS_module.cc#L74-L180)。

FHiCL 控制的主要输入为：

- `HitsModuleLabel`：`recob::Hit`；
- `ClusterModuleLabel`：`recob::Cluster`；
- `ShowerModuleLabel`：`recob::Shower`；
- `TrackModuleLabel1`、`TrackModuleLabel2`：两组 `recob::Track`；
- `VertexModuleLabel`、`PFParticleModuleLabel`、`PandoraNuVertexModuleLabel`：vertex / Pandora PFParticle；
- `MCGenModuleLabel`：`simb::MCTruth`；
- `EnergyRecoNueLabel`、`EnergyRecoNumuLabel`、`EnergyRecoNuelseLabel`：`dune::EnergyRecoOutput`；
- `RegCNN*Label`：RegCNN 顶点回归结果；
- `GlobalWireMethod`：global wire/tick 坐标规则。

依赖的 LArSoft services：

- `BackTrackerService`：从 reco hit 查询 SimIDE / track ID；
- `ParticleInventoryService`：从 track ID 查询 `MCParticle`；
- `Geometry`：TPC、wire、plane 与坐标转换；
- `DetectorPropertiesService`：drift velocity、sampling rate、electron lifetime；
- `TFileService`：写 ROOT 输出。

## 2. 构造函数与 ROOT 输出

构造函数位于 [`RecoEnergyS_module.cc`](RecoEnergyS_module.cc#L556)。其核心是：

```cpp
fTree = tfs->make<TTree>("WC", "WC");
```

随后注册大量 branch，可分为：

| 类别 | 代表 branch | 用途 |
|---|---|---|
| event / detector | `ievent`、`det_t0`、`driftvelocity`、`NWires_P*` | event 与探测器参数 |
| neutrino truth | `NuEngTruth`、`NuPDGTruth`、`NuCCNCTruth` | MC neutrino 标签 |
| primary truth | `TrueEnergy`、`TrueVertex*`、`TrueVtxGlobal*` | primary particle 与真顶点 |
| reconstructed vertex | `PFVtxGlobal*`、`CVNVtxGlobal*` | Pandora / RegCNN 顶点 |
| energy reco | `ErecoNu`、`RecoLepEnNu`、`RecoHadEnNu` | DUNE energy-reco 输出 |
| hit level | `Hit_Charge`、`Hit_CorrCharge`、`Hit_Global_*` | pixel-map 的主要输入 |
| shower / track | `all_shw_*`、`all_track_*` | reco object 与 truth matching |
| wire / ADC | `wire_adc`、`wire_corradc`、`wire_tick` | waveform / wire-level study |

后续 pixel-map preprocessing 最直接依赖：`NHits`、`Hit_Global_Wire`、`Hit_Global_Tick`、`Hit_Global_Plane`、`Hit_Charge`、`Hit_CorrCharge`、`Hit_Prong_Tag` 与 `PFVtxGlobal*`。

## 3. `reconfigure()`：FHiCL 配置

[`RecoEnergyS_module.cc`](RecoEnergyS_module.cc#L969-L1005) 从 FHiCL 保存各模块 label 与 `GlobalWireMethod`。该方法不读取 event 数据。

global-coordinate 模式：

- `GlobalWireMethod = 1`：`GetPixelMapWireIdx()`，随后还会运行 `ShiftGlobalWireIdx()`；
- `GlobalWireMethod = 2`：`GetDUNEGlobalWireTDC()`。

这个配置直接影响 `Hit_Global_*` 与 `PFVtxGlobal*`，因而改变后续 pixel map 的坐标。

## 4. Truth helper

### `get_sortedMCParticle()`

见 [`RecoEnergyS_module.cc`](RecoEnergyS_module.cc#L1005-L1028)。输入为 `MCParticle* -> deposited energy` 映射，输出按沉积能量降序排列，并把每项归一化为能量占比。

### `get_bestMCParticle()`

见 [`RecoEnergyS_module.cc`](RecoEnergyS_module.cc#L1030-L1040)。对于一个 hit：

1. 经 `HitToTrackIDEs()` 得到关联 SimIDE；
2. 按 SimIDE energy 聚合至 `MCParticle`；
3. 返回贡献沉积能量最大的 `MCParticle`。

它用于产生 hit 的主导 truth 粒子信息，如 `Hit_Prong_PDG` 与 `Hit_Prong_PDG_Mom`。

## 5. `ConvertXYZtoWireTDC()`：顶点坐标转换

见 [`RecoEnergyS_module.cc`](RecoEnergyS_module.cc#L1042-L1086)。该函数针对三个 wire plane 执行：

```text
XYZ
 -> FindTPCAtPosition
 -> NearestWireID
 -> ConvertXToTicks
 -> local TPC / wire / tick
 -> global plane / wire / tick
```

它被用于 true vertex、RegCNN vertex、Pandora vertex 与 leading-shower 起点。若 XYZ 不在 TPC 内，函数返回 `false`，相关输出数组保留 `reset()` 设定的 sentinel 值。

## 6. `analyze()`：逐 event 主流程

主流程从 [`RecoEnergyS_module.cc`](RecoEnergyS_module.cc#L1088) 开始。

### 6.1 初始化与产品读取

[`RecoEnergyS_module.cc`](RecoEnergyS_module.cc#L1094-L1170) 中依次：

1. 调用 `reset()`；
2. 读取 detector timing、drift velocity、sampling rate；
3. 读取 hit、cluster、shower、track、vertex；
4. 对 MC 读取 `MCTruth`；
5. 读取四组 RegCNN 结果；
6. 创建 hit-to-shower、hit-to-track、vertex-to-PFParticle 等 association helper。

### 6.2 SimChannel 沉积能量

[`RecoEnergyS_module.cc`](RecoEnergyS_module.cc#L1178-L1224) 遍历 `SimChannel` 的 TDC IDE，按 U/V/Z plane 累积 `DepositU/V/Z`。随后在主流程末尾除以 $1000$ 转为 GeV。

### 6.3 Neutrino truth、CC 筛选与 energy reco

[`RecoEnergyS_module.cc`](RecoEnergyS_module.cc#L1228-L1280) 保存 neutrino energy、PDG、vertex、interaction mode、CC/NC 与 lepton truth。

其中存在一个决定数据集组成的硬筛选：

```cpp
if (nuccnc_truth[0] == 1) return;
```

因此，**这个旧版模块会跳过 NC event，且不写入 `WC` entry**。

对于通过筛选的 event，它按 true neutrino flavor 选择相应的 `dune::EnergyRecoOutput`：nue、numu 或 nuelse。

### 6.4 True primary 与 RegCNN vertex

[`RecoEnergyS_module.cc`](RecoEnergyS_module.cc#L1280-L1375) 使用 `ParticleList().Primary(0)` 填充 primary particle 的 energy、vertex、end 与 momentum；随后用 `ConvertXYZtoWireTDC()` 写出 `TrueVtx*`。

RegCNN 的各输出向量前 3 个分量被当作 XYZ 顶点，并转换为 `CVNVtx*`、`CVNVtx2*` 等 local/global wire-tick 坐标。

该旧实现假定 `Primary(0)` 与 `nuccnc_truth[0]` 有效，因而面向有标准 MC truth 的输入，而非 data 的通用 producer。

### 6.5 Pandora neutrino vertex

[`RecoEnergyS_module.cc`](RecoEnergyS_module.cc#L1378-L1450) 使用 `LArPandoraHelper`：

```text
Pandora PFParticle
 -> 保留 IsPrimary() 的 PFParticle
 -> PFParticle -> Vertex association
 -> XYZ
 -> ConvertXYZtoWireTDC
 -> PFVtxTPC/Wire/Tick/Global*
```

`PFVtxGlobalWire` 和 `PFVtxGlobalTick` 是后续 macro 常用的 crop center。若没有可用的 PFParticle-vertex association，顶点坐标可能是无效 sentinel 或由初始值转换来的不可靠结果，应在输出检查中验证。

### 6.6 Track loop 与 truth matching

[`RecoEnergyS_module.cc`](RecoEnergyS_module.cc#L1454-L1615) 遍历第二组 tracks（通常是 Pandora track），保存长度、起点、方向、动量和到 true/reco vertex 的距离。

对每条 track：

1. 收集其关联 hits；
2. 用 SimIDE energy 汇总 truth particle；
3. 保留贡献最大的前两名 truth candidate；
4. 写出 truth PDG、mother PDG、energy fraction、truth momentum 与 reco-true angle。

该部分用于 track/prong study，不是生成 pixel map 的核心输入。

### 6.7 Shower loop 与 shower-hit 汇总

[`RecoEnergyS_module.cc`](RecoEnergyS_module.cc#L1618-L1825) 遍历每个 `recob::Shower`，写出 energy、dEdx、长度、起点和方向；并通过 shower-associated hits 做 truth matching。

它还：

- 累积 U/V/Z raw 与 lifetime-corrected shower charge；
- 保存 shower hit 的 global coordinates；
- 依据 `Energy()[2]` 选择 leading shower；
- 填 leading-shower 的 truth/reco 属性。

该循环构造 shower 相关 branch，但不替代 event-level `Hit_*` 数组。

### 6.8 Hit loop：pixel-map 的直接输入

从 [`RecoEnergyS_module.cc`](RecoEnergyS_module.cc#L1844) 开始，是后续 preprocessing 最重要的部分。

每个 hit 的处理步骤为：

1. 保存 local detector 信息：cryostat、TPC、plane、wire、peak tick、RMS、charge、amplitude；
2. 计算 lifetime-corrected charge：

$$
Q_{\mathrm{corr}} =
Q_{\mathrm{raw}}
\exp\left(
\frac{\mathrm{SampleRate}\cdot \mathrm{PeakTime}}
{\tau_e\cdot 10^3}
\right)
$$

3. 填 `Hit_Charge`、`Hit_CorrCharge` 与 charge-derived `Hit_Energy`；
4. 依据 `GlobalWireMethod` 填 `Hit_Global_Wire`、`Hit_Global_Tick`、`Hit_Global_Plane`；
5. 利用 `get_bestMCParticle()` 写主导 truth PDG 与母粒子 PDG；
6. 通过 hit-to-shower association 写 shower prong tag；
7. 通过 hit-to-track association 写 track prong tag：

```cpp
hit_prong_tag = kMaxClst + trackID;
```

track 与 shower tag 因而处于不同数值区间。若一个 hit 同时关联 shower 和 track，后执行的 track 赋值会覆盖 shower tag。

### 6.9 Wire / ADC loop

[`RecoEnergyS_module.cc`](RecoEnergyS_module.cc#L2050-L2365) 将 hit 汇总成 wire-level 条目，然后读取 `RawDigit` 与 `recob::Wire`：

- 解压 raw ADC；
- 读取 wire signal ROI；
- 保存 raw / corrected ADC 与 tick；
- 根据 tick 是否落在 track/shower 时间窗中，对 waveform tick 做 prong tagging；
- 保存 SimChannel charge。

这些是 waveform 与低层 hit study 数据，通常不是 `make_text_file_to_root_trks_shws.C` 的最小必需输入。

### 6.10 写 tree

处理结束时，[`RecoEnergyS_module.cc`](RecoEnergyS_module.cc#L2360-L2367) 执行：

```cpp
n_clusters = clusters.size();
fTree->Fill();
```

每个成功处理、且未被 NC early return 跳过的 event 向 `WC` 写入一行。

## 7. Global coordinate helpers

### `GetDUNEGlobalWireTDC()`

见 [`RecoEnergyS_module.cc`](RecoEnergyS_module.cc#L2484-L2550)，对应 `GlobalWireMethod = 2`：

- collection plane 使用 480 wires、gap 5；
- induction plane 使用 400 wires、gap 4；
- TPC 1 / 2 会交换 U/V plane；
- V plane 的 global wire 使用反向 TPC block 编号；
- 依据 drift direction 翻转或平移 TDC。

它直接定义 `Hit_Global_*` 与 global vertex coordinates。若 pixel map 坐标错位，应优先比对这里的约定与后续 macro 的假设是否一致。

### `GetPixelMapWireIdx()` 与 `ShiftGlobalWireIdx()`

见 [`RecoEnergyS_module.cc`](RecoEnergyS_module.cc#L2395-L2480) 和 [`RecoEnergyS_module.cc`](RecoEnergyS_module.cc#L2553-L2582)，对应 `GlobalWireMethod = 1`：

- induction wire 由 wire center 转换；
- collection wire 依 TPC block 平移；
- `ShiftGlobalWireIdx()` 通过接近 $t=0$ 的 hit，估算相邻 TPC U/V offset 并修正 global wire。

当配置使用 `GlobalWireMethod = 2` 时，`ShiftGlobalWireIdx()` 不参与最终输出。

## 8. `FindTrackID()` 与 `reset()`

`FindTrackID()` 位于 [`RecoEnergyS_module.cc`](RecoEnergyS_module.cc#L2585-L2597)，从一个 hit 的 SimIDE 中选择 deposited energy 最大的绝对 track ID。当前主 hit loop 并未实际调用它，而是把 `Hit_TrueTrackID` 直接设为 `-1`。

`reset()` 位于 [`RecoEnergyS_module.cc`](RecoEnergyS_module.cc#L2599-L2961)，在每个 event 开始前：

- 将 scalar、vertex 与数组填为零或 sentinel；
- 清空 hit、track、shower 的固定数组；
- 清空 waveform、SimChannel、truth 等 vector branch；
- 初始化 global coordinate offsets 与 RegCNN 输出缓存。

其作用是避免上一 event 的数据残留在当前 `WC` entry 中。

## 9. 对当前 preprocessing 的结论

1. 最关键的输出是 event-level `Hit_*`、`PFVtxGlobal*`、truth 和 `Hit_Prong_Tag`；后续 macro 据此生成 pixel map。
2. 该旧版本会丢弃所有 NC events，因此输出 ROOT entry 数可能少于输入 event 数。
3. `GlobalWireMethod` 决定全局 wire/tick 坐标，必须与下游 macro 的坐标约定一致。
4. FHiCL 中的 module labels 必须对应输入 reco ROOT 实际存在的产品；旧版 `RecoEnergyStudies` 的 labels/API 不应与新的 atmnu 版本混用。
5. 对单 event run，最直接的检查是确认：`recoEnergy/WC` 有 entry，`NHits > 0`，且 `PFVtxGlobalWire/Tick`、`Hit_Global_*` 不全是 sentinel。
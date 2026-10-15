# transformerCVN / RegCNN Preprocessing

根目录：`/home/binzhang/disk_binzhang/preprocessing/`

链路来源：Alejandro 分享的标准预处理链 + iseong 的 `dune_pixelmap_generator`。
RecoEnergyS 模块现有 **两份不同来源的源码**（`RecoEnergyStudies/` 与
`Preprocessing_Alejandro/RecoEnergyS_atmnu/`），差异见 §3.1。
与当前 LArSoft 重建链的差异见 `Preprocessing_Alejandro/PREPROCESSING_VS_LARSOFT_AUDIT.md`。

---

## 1. 目录

```text
preprocessing/
├── RecoEnergyStudies/                            # ★ 新拷入：iseong 原版 dunereco 包
│   ├── Readme.txt                                # 注明来源 DUNE server
│   ├── note                                      # 扁平 tree 列结构简述
│   ├── CMakeLists.txt                            # 同时构建 RecoEnergyS + RegCNNVertexAna
│   ├── RecoEnergyS_module.cc                     # 351 branches
│   ├── RegCNNVertexAna_module.cc                 # RegCNN 顶点分析
│   ├── recoenergystudies.fcl                     # analyzer 参数（linecluster/emshower）
│   ├── recoenergys.fcl                           # 完整 reco + RegCNN mapper/evaluator
│   ├── recoenergys_v1.fcl                        # 同上但用 RegCVN mapper/evaluator
│   ├── RegCNNVertexAna.fcl                       # RegCNNVertexAna 参数
│   ├── regcnnvtxanajob.fcl                       # process_name: RegCNNVtxAna
│   └── regcnnvtxanajob_inc_reco.fcl              # 同 process，含完整 reco 链
├── Preprocessing_Alejandro/
│   ├── RecoEnergyS_atmnu/                         # Alejandro 的 atmnu 修改版
│   ├── RecoEnergyS_atmnu/                         # Alejandro 的 atmnu 修改版
│   │   ├── RecoEnergyS_module.cc                 # 392 branches
│   │   ├── CMakeLists.txt
│   │   ├── recoenergys.fcl / _v1.fcl / _vd.fcl
│   │   ├── recoenergystudies.fcl
│   │   ├── make_text_file_to_root_trks_shws.C    # macro 版本 C
│   │   ├── preprocess_atmnu.py
│   │   └── run_root_processing.sh / run_python_preprocessing.sh
│   ├── make_text_file_to_root_trks_shws.C        # macro 版本 A
│   ├── make_text_file_to_root_trks_shws_atmnu.C  # macro 版本 B
│   ├── preprocess2.py
│   ├── preprocess_atmnu.py
│   ├── CreateFullySparseDataset.ipynb
│   ├── PREPROCESSING_VS_LARSOFT_AUDIT.md
│   └── Readme.txt
└── dune_pixelmap_generator/
    ├── README
    ├── macros/                                   # 版本 A + v2_prongs / v2_nue_vtx_v3 / v2_nue_v6 / 3D
    ├── macros_numu/make_text_file_to_root_v2_numu_v3.C
    ├── standalone/                               # ★ root2hdf5
    │   ├── makefile
    │   └── ConvertToHDF5_{trks_shws,prong,for_numu,loop2,3D}.cc
    ├── Run_BatchJobs_{v3,prong,for_numu_v1}.py
    ├── batch_jobsub_{prong,5,for_numu}.sh
    ├── prodgenie_*.xml / regcvn_reco_*.xml
    └── ana/
```

---

## 2. 流程

```text
reco ROOT
  │ [1] RecoEnergyS_module.cc                    ← 已有两份源码，见 §3.1
  ▼
tree "WC"                                        ← module 实际写出的（TFileService 顶层）
  │ [2] make_text_file_to_root_trks_shws.C       ← 该 macro 硬编码读 "recoEnergy/WC"
  ▼
tree "pixelmap"
  │ [3] standalone/ConvertToHDF5_trks_shws.cc    ← root2hdf5
  ▼
HDF5 dataset "ArrayOfStructures"                 ← RegCNN 到此结束
  │ [4] preprocess2.py
  ▼
HDF5
  │ [5] CreateFullySparseDataset.ipynb
  ▼
训练用 HDF5 + *.baseline.npz                      ← TransformerCVN 需要
```

---

## 3. 各步骤

### 3.1 RecoEnergyS — 现在有两份源码

`art::EDAnalyzer`，作用是把 ART 格式 reco ROOT 里的信息写成扁平 tree。
两份都是 `fTree = tfs->make<TTree>("WC","WC")`。

**两份 module 源码差异很大，不要混用：**

| | `RecoEnergyStudies/RecoEnergyS_module.cc` | `Preprocessing_Alejandro/RecoEnergyS_atmnu/RecoEnergyS_module.cc` |
|---|---|---|
| 大小 | 120 237 B | 135 769 B |
| `fTree->Branch` 数 | 351 | **392** |
| 来源 | DUNE server iseong 原版（dunetpc 时代） | Alejandro 的 atmnu 修改版 |
| 头文件风格 | `art/Framework/Services/Optional/TFileService.h`<br>`nutools/ParticleNavigation/ParticleList.h`<br>`dune/FDSensOpt/...`、`dune/RegCNN/func/...` | `art_root_io/TFileService.h`<br>`nug4/ParticleNavigation/ParticleList.h`<br>`dunereco/FDSensOpt/...`、`dunereco/CVN/func/...` |
| 独有 branch | — | `run`、`subrun`、`cvn*`、`trueDaughter*`、`all_track_end*`、`Hit_SP_*` |
| CMake 依赖写法 | 下划线式 `lardataobj_RawData`… + `BASENAME_ONLY` | `::` 式 `lardataobj::RawData`…，另需 `dunereco::CVN_func` / `dunereco::CVN_art` |
| 能否直接编到 LArSoft v10_23_00 | ❌ 旧头文件路径（`art/`、`nutools/`、`dune/`），需改写 | 较接近，但仍需调整 CMake 依赖 |

**两份 fcl 的差异**（`recoenergystudies.fcl`）：

| 参数 | `RecoEnergyStudies/` | `RecoEnergyS_atmnu/` |
|---|---|---|
| `HitsModuleLabel` / `ClusterModuleLabel` | `linecluster` | `hitfd` |
| `ShowerModuleLabel` | `emshower` | `pandoraShower` |
| `EnergyRecoNueLabel` | `energyreconue` | `energyrecnue` |
| `EnergyRecoNumuLabel` | `energyreconumu` | `energyrecnumu` |
| `EnergyRecoNuelseLabel` | `energyreconuelse` | `energyrecnc` |
| `services.Geometry`（见 `recoenergys.fcl`） | `@local::dune10kt_1x2x6_geo` | `@local::dune10kt_1x2x6_v2_geo` |

> `recoenergys_v1.fcl` 两份完全相同；`CMakeLists.txt` / `recoenergystudies.fcl` / `recoenergys.fcl` 不同。

**`RecoEnergyStudies/` 额外的 fcl：**

| 文件 | 用途 |
|---|---|
| `recoenergys.fcl` | 完整链：`emshowernew` + 3 个 `energyreco*` + 4 组 `regcnnmap*/regcnneval*` |
| `recoenergys_v1.fcl` | 同上但用 `regcvnmap`/`regcvneval` |
| `RegCNNVertexAna.fcl` | `RegCNNVertexAna` 模块参数 |
| `regcnnvtxanajob.fcl` / `_inc_reco.fcl` | `process_name: RegCNNVtxAna`；后者含完整 Pandora/emshower reco 链 |

**当前状态：两份都还跑不起来**

- 都未编进 `srcs/dunereco/dunereco/`；本地 `build_slf7.x86_64/` 和 CVMFS 的 `dunereco` 都没有 `RecoEnergyS` 插件。
- `RecoEnergyStudies/recoenergys.fcl` 还引用了 2019 年的 TF 网络文件
  `duneRegCNNNetwork/dune_nue_vertices_*.pb`，当前环境大概率不存在。
- 运行方式：`lar -n <N> -c <fcl> -s <reco.root>`。

**待实测确认**：两份 module 都建 TFileService 顶层的 tree `WC`，
但 §2 流程里 macro 的输入是硬编码 `TChain("recoEnergy/WC")` —— 树的实际路径要跑一次才能确认。

### 3.2 pixel map macro — `make_text_file_to_root_trks_shws.C`

同名文件有 **3 个内容不同的版本**，支持的 `type` 不同，**不可混用**：

| 版本 | 文件 | 支持的 `type` |
|---|---|---|
| A | `Preprocessing_Alejandro/make_text_file_to_root_trks_shws.C`<br>（与 `dune_pixelmap_generator/macros/` 下同名文件 md5 相同） | `nue`、`numu` |
| B | `Preprocessing_Alejandro/make_text_file_to_root_trks_shws_atmnu.C` | `nue`、`numu`、`atmnu` |
| C | `Preprocessing_Alejandro/RecoEnergyS_atmnu/make_text_file_to_root_trks_shws.C` | `nue`、`numu`、`atmnu`、`nnbar` |

`type` → coarse 参数（macro 内硬编码）：

| `type` | `x_coarse` | `y_coarse` | 覆盖范围 (wire × tick) | 输出 map |
|---|---:|---:|---|---|
| `nue`（默认） | 1 | 6 | 400 × 1680 | 400 × 280 |
| `numu` | 7 | 24 | 2800 × 6720 | 400 × 280 |
| `atmnu` | 16 | 40 | 5600 × 14000 | 350 × 350 |
| `nnbar` | 4 | 25 | 1400 × 8750 | 350 × 350 |

- 输入 tree 硬编码 `recoEnergy/WC`；输出 tree `pixelmap`。
- `type` 不传则按 `nue` 走。**与 LArSoft `WireResolution=7`/`TimeResolution=24` 对应的是 `numu`。**
- 版本 A 的 `else` 分支直接 `std::abort()`；`run_root_processing.sh` 里传的 `"nnbar"` 必须配版本 C。
- 调用：

  ```bash
  root -l -q -b 'make_text_file_to_root_trks_shws.C("<in>.root","<out>.root","numu")'
  ```

### 3.3 root2hdf5 — `dune_pixelmap_generator/standalone/`

- `ConvertToHDF5_trks_shws.cc`：读 tree `pixelmap`，写 HDF5 数据集 `ArrayOfStructures`。
- 编译：`make`（需要 ROOT 的 `h5c++`），目标 `do_convert_trks_shws`。
- 用法：`./do_convert_trks_shws <in>.root <out>.h5`
- README 里提到的 `run_*.sh` 编译脚本在当前目录中不存在。
- 写出的 `data_t` 字段含 `ievt/run/subrun/tpc_id/iplane/unique_id/nwires/nticks/wcoarse/tcoarse/`
  `rel_wire/rel_tick/shw_id/wire_charge/wire_corrcharge/trueVtxGlobal*/pfVtxGlobal*/prong_tag/`
  `prong_*/fCVNResult*` 等。

### 3.4 preprocess2.py — `Preprocessing_Alejandro/preprocess2.py`

- 用法：`python preprocess2.py <out.h5> "<in_glob>" ...`
- 读 `<in>.h5` 的 `ArrayOfStructures`，输出 `cvnmap_shape = [N, 3, 400, 280]`、
  `png_cvnmap_shape = [N, 20, 3, 400, 280]`。
- pixel value 取 **`wire_charge`（raw）**，不是 `wire_corrcharge`。
- `random.shuffle(fins)` 打乱输入文件顺序。
- 一致性检查（`len(events) != len(evt_start_indices)`）失败时**静默跳过整个文件**。

### 3.5 CreateFullySparseDataset.ipynb — `Preprocessing_Alejandro/`

- 生成最终训练用单个 HDF5 和 `*_prong_pixels_minkowski_sparse_<E>_gev.h5`、`.baseline.npz`。
- `sparse_to_sparse()` 中对 V plane（`c == 1`）执行 `y = 349 - y`。
- 若存在与输入同名的 `.select.npy`，会额外筛选事例。

---

## 4. 下一步：跑通 RecoEnergyS

先按 §3.1 的对比决定用哪一份 module 源码（`RecoEnergyStudies/` 是完整包，含 `RegCNNVertexAna`）。

1. 把选定的包复制到 `srcs/dunereco/dunereco/RecoEnergyStudies/`
2. 改 `CMakeLists.txt` 里的依赖到当前 LArSoft v10_23_00 的库名写法
   - `RecoEnergyStudies/` 版是下划线式（`lardataobj_RawData`…），需要改成 `::` 式
  - `RecoEnergyS_atmnu/` 版已是 `::` 式，但引用了 `dunereco::CVN_func` / `CVN_art`
3. 按实际输入文件的 tier 核对 fcl 里的 label（`hitfd`/`linecluster`、`pandoraShower`/`emshower`、
   `energyrecnue`/`energyreconue`）
4. `mrb build` + `mrbsetenv`，确认 `lar --help` 中出现 `RecoEnergyS` 插件
5. 用 1 个 reco 文件小跑，确认输出 ROOT 中的 tree 路径
   （module 写的是 `WC`，macro 要读 `recoEnergy/WC` —— 需要实测对齐）

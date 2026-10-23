# RecoEnergyStudies 输出分析（ROOT MakeClass）

ROOT 已对实际输入树调用：

```cpp
TFile f("/home/binzhang/disk_binzhang/preprocessing_multi_production/RecoEnergyStudies/RecoEnergyS.root");
auto tree = f.Get<TTree>("recoEnergy/WC");
tree->MakeClass("analysis_RecoEnergyStudies_outputs");
```

- `analysis_RecoEnergyStudies_outputs.h`：MakeClass 生成的全部 leaf 成员、branch 指针、Init/SetBranchAddress、GetEntry、LoadTree 等。补充了输出目录成员、std::vector 限定、默认文件绝对路径，以及兼容 2018/2026 样本的数组容量。
- `analysis_RecoEnergyStudies_outputs.cc`：实现生成类的 Loop()；声明每个 leaf 的直方图描述，使用 LoadTree/GetEntry 读取类成员，逐项遍历标量、数组、vector，填充直方图并保存、绘图。修改分析请从这里入手。
- `main_analysis_RecoEnergyStudies_outputs.cc`：校验所有输入的树结构，用 TChain 合并读取，创建分析类并调用一次 Loop()。类放在堆上，避免大数组导致栈溢出；输入文件由 TChain 管理。

无参数执行：

```bash
./build_analysis_RecoEnergyStudies_outputs.sh
./run_analysis_RecoEnergyStudies_outputs.sh
```

默认按文件名顺序读取 `../RecoEnergyStudies/output_root/2018_production/` 目录下全部 `.root` 文件的 `recoEnergy/WC`，将所有记录汇总到同一套直方图。仍输出到 `outputs_2018/`：`RecoEnergyStudies_histograms.root`、`RecoEnergyStudies_histograms.pdf`、`leaf_summary.csv` 和 `analysis.log`，以及 `plot/` 中每个 leaf 单独一张的 PNG（文件名为 `<leaf>.png`），不按输入文件拆分。两遍遍历的范围和统计都基于全部文件。

每个数值元素填充一次。数组、嵌套 vector 展平统计；变长数组只读实际长度，二维固定维度全部纳入。两遍遍历：先测范围，再创建和填充 100 个等宽 bin。不归一化，不筛选 -999 等占位值；NaN/Inf 跳过并统计。空 leaf 保留空直方图。

MakeClass 分支结构保留，数组容量统一与上游 RecoEnergyS_module.cc 的固定缓冲区对齐：kMaxHits=50000、kMax=3000、kMaxClst=100，不再按单个样本最大值分配。Loop 在每次读取数组前先读长度并检查容量，超限报错；输入 schema 改变时也会报错。如上游扩大容量或新增/删除 leaf，需要同步头文件及 Loop。build 不会覆盖手动编写的 Loop。

脚本可选参数为 `[输入目录或单个 ROOT 文件] [输出目录]`，目录只扫描直接包含的 `.root` 文件；源码或头文件更新时自动编译。底层可执行程序参数为 `INPUT.root [INPUT2.root ...] OUTPUT_DIRECTORY`，也支持显式指定多个文件。ROOT_LOCAL、CXX 可覆盖 ROOT 安装位置及编译器。重新运行覆盖同名结果，输入保持只读。

hit/shower-hit/wire 数组第一维使用 kMaxHits；truth/track/shower/cluster 数组使用 kMax；二维 wire 数组第二维使用 kMaxClst。同一可执行程序用于 2018 和 2026 输出，保留完整数据及越界保护。

## 2018 / 2026 逐图叠加比较

```bash
./run_compare_RecoEnergyStudies_outputs.sh
```

默认比较 `outputs_2018/RecoEnergyStudies_histograms.root` 与 `outputs_2026/RecoEnergyStudies_histograms.root`，按相同直方图名字配对。首次运行或修改 C++ 后会自动编译；也可单独运行 `./build_compare_RecoEnergyStudies_outputs.sh`。

可选参数依次为 2018 ROOT、2026 ROOT、输出目录。默认输出 `comparison_2018_vs_2026/`，包含汇总 PDF、`plots/` 中逐变量 PNG、保存原始直方图和对比画布的 ROOT，以及 comparison_summary.csv 和日志。蓝色实线代表 2018，红色虚线代表 2026。

仅叠加原始计数：不归一化，不重分箱，不画比值，保留占位值。两份输入的自动分箱可能不同，程序将坐标范围扩至两者并集，保持各自原始 bin；有差异时图上标注，CSV 也记录。纵轴是每个原始 bin 的计数，分箱不同可能影响峰高比较。空直方图同样输出；输入 ROOT 文件只读。重新运行覆盖同名比较输出。

# RecoEnergyStudies 输出分析（ROOT MakeClass）

ROOT 已对实际输入树调用：

```cpp
TFile f("/home/binzhang/disk_binzhang/preprocessing_multi_production/RecoEnergyStudies/RecoEnergyS.root");
auto tree = f.Get<TTree>("recoEnergy/WC");
tree->MakeClass("analysis_RecoEnergyStudies_outputs");
```

- `analysis_RecoEnergyStudies_outputs.h`：MakeClass 生成的全部 leaf 成员、branch 指针、Init/SetBranchAddress、GetEntry、LoadTree 等。只补充了输出目录成员、std::vector 限定和默认文件绝对路径。
- `analysis_RecoEnergyStudies_outputs.cc`：实现生成类的 Loop()；声明每个 leaf 的直方图描述，使用 LoadTree/GetEntry 读取类成员，逐项遍历标量、数组、vector，填充直方图并保存、绘图。修改分析请从这里入手。
- `main_analysis_RecoEnergyStudies_outputs.cc`：打开输入文件，创建分析类并调用 Loop()。类放在堆上，避免大数组导致栈溢出。

无参数执行：

```bash
./build_analysis_RecoEnergyStudies_outputs.sh
./run_analysis_RecoEnergyStudies_outputs.sh
```

默认读取 `../RecoEnergyStudies/RecoEnergyS.root` 的 `recoEnergy/WC`。输出到 `outputs/`：335 个 leaf 对应的 ROOT 直方图、每个 leaf 一页的 PDF、leaf_summary.csv 和 analysis.log。

每个数值元素填充一次。数组、嵌套 vector 展平统计；变长数组只读实际长度，二维固定维度全部纳入。两遍遍历：先测范围，再创建和填充 100 个等宽 bin。不归一化，不筛选 -999 等占位值；NaN/Inf 跳过并统计。空 leaf 保留空直方图。

MakeClass 数组容量由当前输入文件生成。Loop 在每次读取数组前先读长度并检查容量，超限报错；输入 schema 改变时也会报错。更换文件后若容量不够，应重新 MakeClass 并同步头文件；如新增/删除 leaf，还要同步 Loop 的变量及填充列表。build 不会覆盖手动编写的 Loop。

可通过 INPUT_FILE、OUTPUT_DIR、ROOT_LOCAL、CXX 环境变量覆盖路径或工具。重新运行覆盖同名结果，输入保持只读。build/outputs 被 gitignore 排除。

## 2018 / 2026 逐图叠加比较

```bash
./run_compare_RecoEnergyStudies_outputs.sh
```

默认比较 `outputs_2018/RecoEnergyStudies_histograms.root` 与 `outputs_2026/RecoEnergyStudies_histograms.root`，按相同直方图名字配对。首次运行或修改 C++ 后会自动编译；也可单独运行 `./build_compare_RecoEnergyStudies_outputs.sh`。

可选参数依次为 2018 ROOT、2026 ROOT、输出目录。默认输出 `comparison_2018_vs_2026/`，包含汇总 PDF、`plots/` 中逐变量 PNG、保存原始直方图和对比画布的 ROOT，以及 comparison_summary.csv 和日志。蓝色实线代表 2018，红色虚线代表 2026。

仅叠加原始计数：不归一化，不重分箱，不画比值，保留占位值。两份输入的自动分箱可能不同，程序将坐标范围扩至两者并集，保持各自原始 bin；有差异时图上标注，CSV 也记录。纵轴是每个原始 bin 的计数，分箱不同可能影响峰高比较。空直方图同样输出；输入 ROOT 文件只读。重新运行覆盖同名比较输出。

# CP-Lib

算法竞赛模板库，采用结构体/类封装，方便在比赛中快速复用。

## 快速开始

| 我要…… | 怎么做 |
|---|---|
| 改现有模板 | 改对应目录下的 `.cpp` / `.hpp`，运行 `python tools/build_print.py` |
| 新增模板 | 新建源文件，在 `tools/gen_print.py` 的章节配置里加一行，运行 `python tools/build_print.py` |
| 只改排版 | 改 `print/preamble.tex` / `print/main.tex`，运行 `python tools/build_print.py --no-gen` |
| 改「结论与技巧」 | 改对应目录的 `.md`，运行 `python tools/build_print.py` |

根目录的 `python build.py` 是同一个脚本的快捷入口，参数完全一样。

详细步骤、排版参数和常见问题见单独的小册子：

- [**《CP-Lib 修改指南》PDF**](docs/CP-Lib-Guide.pdf)
- [Markdown 源文件](docs/editing-guide.md)（改完运行 `python tools/build_guide.py` 重新生成 PDF）

## 目录结构

```
CP-Lib/
├── GraphTheory/             # 图论与树论（20 个模板）
├── DataStructure/           # 数据结构（27 个模板）
├── String/                  # 字符串（13 个模板）
├── Math/                    # 数学
│   ├── Combinatorics/
│   ├── NumberTheory/
│   ├── Polynomial/          # FFT / NTT / MTT / FWT / 多项式全家桶
│   ├── LinearAlgebra/       # 高斯消元 / 异或线性基
│   └── Templates/           # 矩阵
├── ComputationalGeometry/   # 计算几何
├── Miscellaneous/           # 离散化、欧拉序、哈希表等
├── DP/                      # 动态规划笔记（暂未收入打印版）
├── print/                   # 打印版（LaTeX）
├── docs/                    # 修改指南（Markdown + PDF）
├── tools/                   # 生成 / 构建脚本
└── CP-Lib.md                # 早期汇总稿（已过时）
```

## 模板目录

### 图论 (GraphTheory)

| 模板 | 文件 |
|---|---|
| 拓扑排序 | `TopoSort.cpp` |
| 树的直径 / 重心 | `TreeDiameter.cpp` / `TreeCentroid.cpp` |
| 倍增 LCA | `BinaryLCA.cpp` |
| 树上启发式合并 | `DSU_on_Tree.cpp` |
| Tarjan：SCC / 割边 / 割点 | `TarjanSCC.cpp` / `Bridges.cpp` / `CutPoints.cpp` |
| 最大流最小割（Dinic） | `Dinic.cpp` |
| 最短路：Dijkstra / SPFA / Floyd | `Dijkstra.cpp` / `SPFA.cpp` / `Floyd.cpp` |
| 分层图最短路 | `LayeredGraph.cpp` |
| 判负环：SPFA / Bellman-Ford | `NegativeCycleSPFA.cpp` / `NegativeCycleBellmanFord.cpp` |
| 二分图：染色判定 / 匈牙利 / KM | `BipartiteColoring.cpp` / `Hungarian.cpp` / `KM.cpp` |
| 基环树：找环 / 最大独立集 | `BaseRingTreeFindCycle.cpp` / `BaseRingTreeMaxIndependentSet.cpp` |

### 数据结构 (DataStructure)

| 模板 | 文件 |
|---|---|
| 手写 bitset | `Bitset/bitset.cpp` |
| 树状数组 | `FenwickTree/FenWickTree.hpp` |
| 并查集（基础 / 可回滚 / 带删除） | `DisjointSetUnion/*` |
| 线段树（基础 / 懒标记 / 动态开点 / 主席树 / 标记永久化） | `SegmemtTree/*` |
| 李超线段树 | `SegmemtTree/Lichao_segment_tree.cpp` |
| 线段树套线段树 | `SegmemtTree/Segment_tree_of_segment_tree.cpp` |
| 字典树（基础 / 01-Trie / 可持久化 01-Trie） | `Trie/*` |
| Splay（模板版 / 数组版） | `Treap/splay.cpp` / `Binary Seaech Tree/splay.cpp` |
| 隐式 Treap / PBDS 平衡树 / rope | `Binary Seaech Tree/*` |
| PBDS 堆 | `Heap/pbds_heap.cpp` |
| 树链剖分 / 点分治 | `Tree Structure Decomposition/*` |
| 稀疏表 | `Sptable/sptable.hpp` |
| 莫队（含回滚莫队） | `Mo's algorithm/Mo_with_rollback.cpp` |
| 凸包上二分 / LineContainer | `Other/*` |

### 字符串 (String)

哈希、KMP（前缀函数 / 匹配 / 周期）、Z 函数、Manacher、Trie、AC 自动机、
后缀数组 SA、ST 表求 LCP、后缀自动机 SAM、广义 SAM、回文自动机 PAM、
最小表示法、Lyndon 分解。

### 数学 (Math)

- **数论**：ModInt、exGCD、exCRT、线性筛、扩展线性筛、分段筛、Miller-Rabin、
  Pollard-rho、BSGS、杜教筛
- **组合数学**：阶乘与组合数预处理
- **多项式**：FFT、NTT、三模 NTT、MTT、FWT/FMT、求导积分、求逆、除法、
  开根、指数/对数、快速幂、多点求值、快速插值、快速阶乘、多点快速阶乘
- **线性代数**：矩阵与列向量、高斯消元（整数 / 实数 / 异或）、异或线性基

### 计算几何 (ComputationalGeometry)

- **二维基础**：`base.hpp`（点线封装、距离、旋转、投影、相交判定、多边形、圆）、
  `polar_sort.hpp`（极角排序）、`Geometry_util.cpp`、`Point_line_ops.cpp`
- **二维进阶**：`Line_equation.cpp`（直线方程转换）、`Triangle_center.cpp`（四心）、
  `Circle_ops.cpp`（交点 / 切线 / 相交面积）、`Polygon_extra.cpp`（Pick 定理 / 网格点）
- **凸包系列**：`Convex_hull.cpp`（Andrew）、`Dynamic_convex_hull.cpp`、
  `Rotating_calipers.cpp`、`Minkowski_sum.cpp`、`Half_plane_intersection.cpp`
- **其他**：`Closest_pair.cpp`（平面最近点对）
- **三维**：`Geometry_3D_base.cpp`（点线面封装与判定）、
  `Geometry_3D_intersect.cpp`（相交与交点）、
  `Geometry_3D_distance.cpp`（距离、夹角、体积）
- **结论**：`Conclusion.md`（平面 / 立体几何结论与公式，纯文字）

#### `base.hpp` 的两个坑

1. **依赖 C++20**：第 7 行 `constexpr Real PI = numbers::pi;` 用了 `std::numbers`，
   `-std=c++17` 会编译失败。要么用 C++20，要么把这行改成 `acosl(-1.0L)`。
2. **没有 include guard**：同一编译单元里重复包含会因 `PI` 重定义报错。
   所以各几何文件都只包含一份上游（如三维三个文件只 include `Geometry_3D_base.cpp`）。

另：`CG.md` 是旧的几何文档，其代码有 13 处缺陷（如点切线用错 `asin`、
`circleIntersection` 校验用错变量、半平面交断言会误触发、动态凸包 `isIntersect`
语义与函数名相反等），新文件里已逐条修掉并在注释中标明原委。**以源码为准。**

### 杂项 (Miscellaneous)

离散化、欧拉序（子树转区间）、反转整数、防卡哈希表、快读快写（`FinFout.cpp`）。

## 使用方式

大多数模板是**代码片段**：直接复制进你的程序即可，需要时自行补齐 `#include`
和类型别名（`ll`、`i128` 等）。部分模板依赖别的模板——例如多项式系列依赖
`Math/NumberTheory/modint.hpp`，`BSGS` 依赖 `Miscellaneous/hashtable.hpp`，
`GaussianElim` 依赖 `Math/Templates/Matrix.hpp`。

## 打印版

`print/` 下是 LaTeX 打印版：A4 纵向单栏，带封面、目录和页码，代码约 8.2pt，正文 11pt。
日常构建只需要一条命令：

```bash
python tools/build_print.py          # 改完模板源码：重新生成章节 + 编译
python tools/build_print.py --no-gen # 只改了 print/ 下的排版：跳过生成
python build.py --no-gen             # 根目录快捷入口，参数相同
```

产物是 `print/CP-Lib.pdf`。必须用 XeLaTeX（`.latexmkrc` 已配置），需要 TeX Live。
目录和页码要多趟编译，通常要等几分钟。

| 参数 | 用途 |
|---|---|
| `--check` / `-c` | 列出还没归入打印版的源文件；有遗漏时返回非 0 |
| `--clean` | 清理所有编译中间文件、`main.pdf` 和 Python 缓存，保留 PDF |
| `--open` / `-o` | 编译完成后自动打开 PDF |
| `--no-gen` / `-n` | 跳过章节生成，直接编译现有 `print/chapters/*.tex` |

完整流程、章节配置格式、排版参数和常见问题见 [修改指南 PDF](docs/CP-Lib-Guide.pdf)。

## 文档与脚本

| 文件 / 脚本 | 角色 |
|---|---|
| `docs/editing-guide.md` | 修改指南的 Markdown 正本 |
| `docs/CP-Lib-Guide.pdf` | 修改指南 PDF（由 `build_guide.py` 生成） |
| `build.py` | 根目录快捷入口，等价于 `tools/build_print.py` |
| `tools/build_print.py` | 一键生成章节并编译打印版 PDF |
| `tools/gen_print.py` | 章节配置 + 从源码生成 `print/chapters/*.tex` |
| `tools/build_guide.py` | 用 pandoc + XeLaTeX 生成修改指南 PDF |
| `tools/md_inline.py` | Markdown 行内元素转 LaTeX（被 `gen_print.py` 复用） |

### 文档约定

- **各目录下的 `.cpp` / `.hpp` 是模板代码的唯一正本。**
- `print/chapters/*.tex` 是 `gen_print.py` 的生成物，不要手改；确需临时手改时用
  `build_print.py --no-gen` 编译，并且之后不要再运行完整生成。
- `GraphTheory/Graph.md`、`DataStructure/DataStructure.md`、`String/String.md`、
  `Math/Math.md`、`ComputationalGeometry/Conclusion.md` 是「结论与技巧」的来源，
  其中旧代码部分可能过时，以源码为准。
- `CP-Lib.md` 是早期汇总稿，已严重过时（缺 bitset、树剖、李超线段树、整章多项式等）。

## 开源协议

本项目采用 **Eclipse Public License (EPL)** 开源协议。

# CP-Lib

算法竞赛模板库，采用结构体/类封装，方便在比赛中快速复用。

## 开源协议

本项目采用 **Eclipse Public License (EPL)** 开源协议。

## 谁说了算

**各个文件夹下的源码文件是唯一正本。** 仓库里的 `.md` 是不同时期写的说明文档，新旧不一，
不要拿它们当准：

| 文件 | 状态 |
|---|---|
| `GraphTheory/*.cpp`、`String/*.cpp` | ✅ 正本（从原 `.md` 抽出，已独立成文件） |
| `DataStructure/`、`Math/`、`ComputationalGeometry/` 等源码 | ✅ 正本 |
| `GraphTheory/Graph.md`、`String/String.md` 等 | 📄 说明文档，代码部分已过时 |
| `CP-Lib.md` | 📄 早期汇总稿，**已严重过时**（缺 bitset、树剖、李超线段树、整章多项式等） |

`CP-Lib.md` 和 `DataStructure.md` 之类只作溯源与文字说明之用，**代码以源码文件为准**。

## 目录结构

```
CP-Lib/
├── GraphTheory/             # 图论与树论（20 个模板）
├── DataStructure/           # 数据结构（21 个，其中「线段树合并」是空文件待补）
├── String/                  # 字符串（13 个模板）
├── Math/                    # 数学
│   ├── Combinatorics/
│   ├── NumberTheory/
│   ├── Polynomial/          # FFT / NTT / MTT / FWT / 多项式全家桶
│   ├── LinearAlgebra/       # 高斯消元 / 异或线性基
│   └── Templates/           # 矩阵
├── ComputationalGeometry/   # 计算几何
├── Miscellaneous/           # 离散化、欧拉序、哈希表等
├── print/                   # 打印版（LaTeX）── 见下方「打印版」
├── tools/                   # 迁移 / 生成脚本（一次性，备查）
└── CP-Lib.md                # 早期汇总稿（已过时）
```

## 图论 (GraphTheory)

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

## 数据结构 (DataStructure)

| 模板 | 文件 |
|---|---|
| 手写 bitset | `Bitset/bitset.cpp` |
| 树状数组 | `FenwickTree/FenWickTree.hpp` |
| 并查集（基础 / 可回滚 / 带删除） | `DisjointSetUnion/*` |
| 线段树（基础 / 懒标记 / 动态开点 / 主席树 / 标记永久化） | `SegmemtTree/*` |
| 李超线段树 | `SegmemtTree/Lichao_segment_tree.cpp` |
| 线段树套线段树 | `SegmemtTree/Segment_tree_of_segment_tree.cpp` |
| 线段树合并 | `SegmemtTree/Segment_tree_merge.cpp` |
| 字典树（基础 / 01-Trie / 可持久化 01-Trie） | `Trie/*` |
| Treap / 隐式 Treap | `Treap/*` |
| 树链剖分 | `Heavy-Light Decomposition/Heavy_light_decomposition.cpp` |
| 稀疏表 | `Sptable/sptable.hpp` |
| 莫队（含回滚莫队） | `Mo's algorithm/Mo_with_rollback.cpp` |

## 字符串 (String)

哈希、KMP（前缀函数 / 匹配 / 周期）、Z 函数、Manacher、Trie、AC 自动机、
后缀数组 SA、ST 表求 LCP、后缀自动机 SAM、广义 SAM、回文自动机 PAM、
最小表示法、Lyndon 分解。

## 数学 (Math)

- **数论**：ModInt、exGCD、exCRT、线性筛、扩展线性筛、分段筛、Miller-Rabin、
  Pollard-rho、BSGS、杜教筛
- **组合数学**：阶乘与组合数预处理
- **多项式**：FFT、NTT、三模 NTT、MTT、FWT/FMT、求导积分、求逆、除法、
  开根、指数/对数、快速幂、多点求值、快速插值、快速阶乘、多点快速阶乘
- **线性代数**：矩阵与列向量、高斯消元（整数 / 实数 / 异或）、异或线性基

## 计算几何 (ComputationalGeometry)

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

### `base.hpp` 的两个坑

1. **依赖 C++20**：第 7 行 `constexpr Real PI = numbers::pi;` 用了 `std::numbers`，
   `-std=c++17` 会编译失败。要么用 C++20，要么把这行改成 `acosl(-1.0L)`。
2. **没有 include guard**：同一编译单元里重复包含会因 `PI` 重定义报错。
   所以各几何文件都只包含一份上游（如三维三个文件只 include `Geometry_3D_base.cpp`）。

另：`CG.md` 是旧的几何文档，其代码有 13 处缺陷（如点切线用错 `asin`、
`circleIntersection` 校验用错变量、半平面交断言会误触发、动态凸包 `isIntersect`
语义与函数名相反等），新文件里已逐条修掉并在注释中标明原委。**以源码为准。**

## 使用方式

大多数模板是**代码片段**：直接复制进你的程序即可，需要时自行补齐 `#include`
和类型别名（`ll`、`i128` 等）。部分模板依赖别的模板——例如多项式系列依赖
`Math/NumberTheory/modint.hpp`，`BSGS` 依赖 `Miscellaneous/hashtable.hpp`，
`GaussianElim` 依赖 `Math/Templates/Matrix.hpp`。

## 打印版

`print/` 下是 **LaTeX 编写的打印版**：A4 纵向双栏，带目录与页码，正文 45 页。

```bash
latexmk print/main.tex     # 仓库根目录；.latexmkrc 已固化参数
```

产物为 `print/CP-Lib.pdf`。**必须用 XeLaTeX**（文档含中文），`.latexmkrc` 已指定，
直接跑 `latexmk` 即可。目录与页码需要多趟编译，`latexmk` 会自动判断。

### 文件分工

| 文件 | 角色 |
|---|---|
| `print/main.tex` | 主文档：引入导言区 + 目录 + 按序 `\input` 各章 |
| `print/preamble.tex` | 宏包、字体、代码高亮、页眉页脚等全部配置 |
| `print/chapters/*.tex` | 正文，6 个文件对应 6 章 |

**要改打印版内容，直接改 `print/chapters/*.tex`。**

### tools/ 下的脚本

这些是**一次性迁移脚本**，当初用来把散在 `.md` 里的代码抽成源文件、
并生成打印版章节。转换已经完成，**留档备查，不要重复运行**——会覆盖手工修改。

| 脚本 | 用途 |
|---|---|
| `extract_sources.py` | 把 `Graph.md` / `String.md` 拆成 `GraphTheory/`、`String/` 下的源文件 |
| `extract_math.py` | 把 `Math.md` 里没有源文件的专题抽成 `Math/` 下的源文件 |
| `md2tex.py` | Markdown 行内元素转 LaTeX（被 `gen_print.py` 复用） |
| `gen_print.py` | 从源码生成 `print/chapters/*.tex`；`--check` 可列出没归入打印版的源文件 |

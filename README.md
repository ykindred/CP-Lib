# CP-Lib

算法竞赛模板库，采用结构体/类封装，方便在比赛中快速复用。

## 快速开始

| 我要…… | 怎么做 |
|---|---|
| 改现有模板 | 改对应目录下的 `.cpp` / `.hpp`，运行 `python tools/build_print.py` |
| 新增模板 | 数学模板登记到 `docs/math-print-manifest.json`，其他模板登记到 `tools/gen_print.py`；再运行 `python tools/build_print.py` |
| 只改排版 | 改 `print/preamble.tex` / `print/main.tex`，运行 `python tools/build_print.py --no-gen` |
| 改「结论与技巧」 | 改对应目录的 `.md`，运行 `python tools/build_print.py` |

根目录的 `python build.py` 是同一个脚本的快捷入口，参数完全一样。

打印脚本已接入 [数学打印清单](docs/math-print-manifest.json)，包含下划线参考模板，并全文收录 `Math/_notes.md`。维护方法见 [数学打印说明](docs/math-printing.md)。

详细步骤、排版参数和常见问题见单独的小册子：

- [《CP-Lib 修改指南》PDF（旧版，待重新生成）](docs/CP-Lib-Guide.pdf)
- [Markdown 源文件](docs/editing-guide.md)（改完运行 `python tools/build_guide.py` 重新生成 PDF）

## 目录结构

```
CP-Lib/
├── GraphTheory/             # 图论与树论（20 个模板）
├── DataStructure/           # 数据结构（27 个模板）
├── String/                  # 字符串（13 个模板）
├── Math/                    # 数学
│   ├── NT/                  # 数论与模运算
│   ├── Comb/                # 组合计数
│   ├── Poly/                # 多项式、插值与线性递推
│   ├── Alg/                 # 线性代数、积分与博弈
│   ├── CG/                  # 二维几何基础与参考文档
│   ├── head.hpp             # 公共头文件与类型定义
│   ├── _notes.md            # 数学知识点，目标为全文打印
│   └── _templates.md        # 维护用接口索引与验证记录
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

`Math/` 当前有 47 份头文件，其中 24 份以下划线开头。下划线表示参考、非本人编写或尚待审核的模板；打印范围包含这些文件，但保留文件名和“参考”标记，不因此改变正式模板身份。

| 目录 | 当前内容 |
|---|---|
| `NT/` | 模运算、扩展欧几里得、不定方程、exCRT、试除分解、筛法、Miller-Rabin、BSGS、类欧几里得；参考模板包括 Pollard-Rho、约数生成、莫比乌斯与欧拉筛、杜教筛、商分块、整除卷积、模平方根、exLucas |
| `Comb/` | 阶乘与组合数、Lucas、斯特林数；参考模板包括常见计数、Burnside、项链手链、Prüfer 编解码 |
| `Poly/` | FFT、NTT、三模和多模卷积、按位卷积、形式幂级数、多点求值与插值；参考模板包括 BM、Kitamasa、Bostan-Mori、连续点插值、幂和、Taylor shift、子集卷积 |
| `Alg/` | 高斯消元；参考模板包括矩阵快速幂、异或线性基、行列式、矩阵求逆、异或消元、矩阵树定理、自适应 Simpson、Nim 与 SG |
| `CG/` | 二维几何基础与极角排序，在计算几何章打印 |

- [数学知识点](Math/_notes.md)：计数推导、反演、树计数、递推、博弈和数值方法，使用标准 LaTeX 公式。
- [补充模板索引](Math/_templates.md)：接口、条件、调用示例与本地验证记录。
- [数学打印同步说明](docs/math-printing.md)与[完整收录清单](docs/math-print-manifest.json)：涵盖全部头文件和知识点正文，由打印脚本直接读取。

以当前 `.hpp` 为正式代码正本。`Math/Math.md` 仅作历史参考；`Math/CG/CG.md` 作为几何参考资料全文打印，包含原有代码和示意图，不因此转为正式模板。下划线文件不能仅因本地测试通过就自动去掉标记。

### 计算几何（Math/CG）

原 `ComputationalGeometry/` 目录已删除，不再参与打印。当前收录：

- `Math/CG/base.hpp`：点、向量、线段、直线、多边形及基础判定。
- `Math/CG/polar_sort.hpp`：极角排序。
- `Math/CG/CG.md`：几何参考资料全文，包括说明、公式、代码块和示意图。

基础库使用 `std::numbers`，需要 C++20；文件没有包含保护，同一编译单元中应避免重复包含。`CG.md` 的示意图使用 `print/assets/` 中已有的本地缓存。

### 杂项 (Miscellaneous)

离散化、欧拉序（子树转区间）、反转整数、随机数生成、快读快写（`FinFout.cpp`）。
防卡哈希表只打印 `DataStructure/hashtable.hpp`，已移除删除的杂项版本条目。
赛时对拍同时收录 `Miscellaneous/stress.py` 脚本和 `Miscellaneous/stress.md` 使用说明，脚本按 Python 代码排版。

## 使用方式

大多数模板是**代码片段**：直接复制进你的程序即可，需要时自行补齐 `#include`
和类型别名（`ll`、`i128` 等）。部分模板依赖别的模板——例如多项式系列依赖
`Math/NT/modint.hpp` 和 `Math/Poly/NTT.hpp`，`Math/NT/BSGS.hpp` 依赖
`DataStructure/hashtable.hpp`。`Math/Alg/gauss.hpp` 直接接收二维增广矩阵，不依赖矩阵类。
下划线模板的参数与返回值见各文件注释；不能因为混合包含检查通过，就把几套同名筛法或卷积实现全部包含进一个程序。

## 打印版

`print/` 下是 LaTeX 打印版：A4 纵向单栏，带封面、目录和页码，代码约 8.2pt，正文 11pt。
日常构建只需要一条命令：

```bash
python tools/build_print.py          # 改完模板源码：重新生成章节 + 编译
python tools/build_print.py --no-gen # 只改了 print/ 下的排版：跳过生成
python build.py --no-gen             # 根目录快捷入口，参数相同
```

产物是 `print/CP-Lib.pdf`。生成数学正文需要 Pandoc；PDF 编译使用 XeLaTeX 和 latexmk（`.latexmkrc` 已配置），需要 TeX Live。
只生成 LaTeX 章节时，可以运行 `python tools/gen_print.py`，无需 TeX。
目录和页码要多趟编译，通常要等几分钟。

| 参数 | 用途 |
|---|---|
| `--check` / `-c` | 检查数学清单、缺失路径和未收录代码；有错误或遗漏时返回非 0 |
| `--clean` | 清理所有编译中间文件、`main.pdf` 和 Python 缓存，保留 PDF |
| `--open` / `-o` | 编译完成后自动打开 PDF |
| `--no-gen` / `-n` | 跳过章节生成，直接编译现有 `print/chapters/*.tex` |

完整流程、章节配置格式、排版参数和常见问题见 [修改指南](docs/editing-guide.md)。数学部分的接入状态单独见 [同步说明](docs/math-printing.md)；配套指南 PDF 尚待重建。

## 文档与脚本

| 文件 / 脚本 | 角色 |
|---|---|
| `docs/editing-guide.md` | 修改指南的 Markdown 正本 |
| `docs/CP-Lib-Guide.pdf` | 修改指南 PDF，本次未重建，以 Markdown 正本为准 |
| `docs/math-printing.md` | 当前数学目录、参考模板和知识点的打印接入说明 |
| `docs/math-print-manifest.json` | 生成器直接读取的数学源码与正文清单 |
| `build.py` | 根目录快捷入口，等价于 `tools/build_print.py` |
| `tools/build_print.py` | 一键生成章节并编译打印版 PDF |
| `tools/gen_print.py` | 章节配置 + 从源码生成 `print/chapters/*.tex` |
| `tools/build_guide.py` | 用 pandoc + XeLaTeX 生成修改指南 PDF |
| `tools/md_inline.py` | Markdown 行内元素转 LaTeX（被 `gen_print.py` 复用） |

### 文档约定

- **各目录下的 `.cpp` / `.hpp` 是模板代码的唯一正本。**
- `print/chapters/*.tex` 是 `gen_print.py` 的生成物，不要手改；确需临时手改时用
  `build_print.py --no-gen` 编译，并且之后不要再运行完整生成。
- `GraphTheory/Graph.md`、`DataStructure/DataStructure.md`、`String/String.md`
  是相应专题的「结论与技巧」来源，其中旧代码部分可能过时，以源码为准。
  数学知识点由 `Math/_notes.md` 全文转换，不再使用旧 `Math/Math.md`。
  几何参考 `Math/CG/CG.md` 和对拍说明 `Miscellaneous/stress.md` 同样全文转换，保留代码块。
- `CP-Lib.md` 是早期汇总稿，已严重过时（缺 bitset、树剖、李超线段树、整章多项式等）。

## 开源协议

本项目采用 **Eclipse Public License (EPL)** 开源协议。

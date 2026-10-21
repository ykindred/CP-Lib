```{=latex}
\pdfbookmark[0]{正文}{main}
```


这份指南说明怎样修改 CP-Lib 的模板内容、怎样把改动同步到打印版 PDF，以及排版出问题时该看哪里。
目标很简单：**改代码只碰 `.cpp/.hpp`，改排版只碰 `print/`，生成物不要手改。**

更完整的项目说明见仓库根目录的 `README.md`。

# 先分清三类文件

| 类型 | 位置 | 说明 |
|---|---|---|
| 模板代码（正本） | 各专题目录下的 `.cpp` / `.hpp` | 日常只改这里 |
| 打印版排版 | `print/main.tex`、`print/preamble.tex` | 封面、目录、字体、颜色、代码块样式 |
| 打印版章节（生成物） | `print/chapters/*.tex` | 由脚本生成，手改会在下次生成时被覆盖 |

专题目录包括 `GraphTheory/`、`DataStructure/`、`String/`、`Math/`、
`ComputationalGeometry/`、`Miscellaneous/`。

一条命令完成“源码 → 章节 → PDF”：

```bash
python tools/build_print.py
```

根目录的 `python build.py` 是同一个脚本的快捷入口，参数完全一样。产物是 `print/CP-Lib.pdf`。

# 常用操作

## 改现有模板的代码

直接编辑源文件，例如 `GraphTheory/Dijkstra.cpp`，然后运行：

```bash
python tools/build_print.py
```

脚本会先按 `tools/gen_print.py` 顶部的章节配置重新生成六章 `.tex`，再调用 latexmk 编译。
目录和页码需要多趟编译，通常等几分钟。

## 只改排版

如果只改了 `print/main.tex` 或 `print/preamble.tex`，跳过生成，直接编译：

```bash
python tools/build_print.py --no-gen
```

`--no-gen` 不会碰 `print/chapters/*.tex`，因此你在章节文件里做的临时手改也会保留。
但下次运行不带 `--no-gen` 的完整构建时，这些手改仍会被生成器覆盖。

## 新增一个模板

1. 在对应目录新建 `.cpp/.hpp`，例如 `GraphTheory/NewAlgo.cpp`。
2. 打开 `tools/gen_print.py`，在对应章节的配置里加一行：

```python
CH_GRAPH = ("图论", "GraphTheory", [
    ...
    ("新算法", "NewAlgo.cpp"),   # (PDF 里的标题, 相对该目录的路径)
])
```

3. 运行：

```bash
python tools/build_print.py
```

配置里的顺序就是 PDF 里的顺序。改标题、调整顺序、删除模板，都是改这份配置。
如果忘了加配置，生成结束后会提示：

```
[提示] 以下源文件未归入打印版：
    GraphTheory/NewAlgo.cpp   (42 行)
```

也可以单独检查：

```bash
python tools/build_print.py --check
```

## 删除或重命名模板

- 删除：从 `tools/gen_print.py` 的配置里删掉那一行，再按需删除源文件。
- 重命名：同时改源文件名和配置里的相对路径。
- 配置里写了但文件不存在时，`gen_print.py` 会报告 `[缺失]` 并以非零状态退出，
  `build_print.py` 会停下来，不会生成一个悄悄缺模板的 PDF。

## 新增一整章

除了在 `tools/gen_print.py` 里新增章节配置（`CH_XXX` 并加入 `CHAPTERS`），还要：

1. 在 `gen_print.py` 的 slug 映射里加新章节的输出文件名，例如 `"动态规划": "07-dp"`。
2. 在 `print/main.tex` 末尾加一行：

```latex
\input{chapters/07-dp.tex}
```

## 改「结论与技巧」

每章末尾的「结论与技巧」是生成器从 `.md` 里抓的：标题中含
「结论 / 技巧 / 注意 / 速查 / 归档 / 性质」的小节会被收进来，子标题会降级保留。

对应关系：

| 章节 | 来源 |
|---|---|
| 图论 | `GraphTheory/Graph.md` |
| 数据结构 | `DataStructure/DataStructure.md` |
| 字符串 | `String/String.md` |
| 数学 | `Math/Math.md` |
| 计算几何 | `ComputationalGeometry/Conclusion.md` |
| 杂项 | 无（不抓取） |

改完 `.md` 后运行 `python tools/build_print.py` 即可。结论里的代码块会被丢弃，只保留文字、公式、列表和图片。

# build_print.py 的参数

| 命令 | 用途 |
|---|---|
| `python tools/build_print.py` | 改完模板源码后：重新生成章节 + 编译 |
| `python tools/build_print.py --no-gen`（`-n`） | 只改了 `print/` 下的排版时：跳过生成，直接编译 |
| `python tools/build_print.py --check`（`-c`） | 只列出还没归入打印版的源文件；有遗漏时返回非 0 |
| `python tools/build_print.py --clean` | 清理所有编译中间文件、`main.pdf` 和 Python 缓存，保留 PDF |
| `python tools/build_print.py --open`（`-o`） | 编译完成后自动打开 PDF |

根目录的 `python build.py` 与 `python tools/build_print.py` 完全等价，参数相同。
构建脚本会自动调用 XeLaTeX，并检查 `print/CP-Lib.pdf` 是否真的生成；
失败时会提示查看 `print/CP-Lib.log`。

# 排版文件里改什么

## 代码字号

`print/preamble.tex` 中的：

```latex
basicstyle=\ttfamily\fontsize{8.2}{9.9}\selectfont,
```

第一个数是字号，第二个数是行距。当前单栏布局在 A4 上约能放下 107 列；
把字号调大会让可容纳列数减少，超长行会折行（行首出现灰色 $\hookrightarrow$）。

## 页边距

`print/preamble.tex` 顶部的：

```latex
\usepackage[a4paper,top=16mm,bottom=15mm,left=16mm,right=16mm,
            headheight=15pt,headsep=4mm]{geometry}
```

`top` 必须至少容纳 `headheight + headsep`，否则页眉会被裁掉。

## 颜色

`print/preamble.tex` 的“颜色”一节集中定义了主色、正文色、代码高亮色：

```latex
\definecolor{accent}{RGB}{28,62,106}   % 章节号、链接、代码左侧竖条
\definecolor{ink}{RGB}{24,24,24}       % 正文黑
\definecolor{codebg}{RGB}{247,248,251} % 代码底色
```

改一处即可全局生效。`accent2` 只用于「结论与技巧」标题。

## 代码块样式

`lstset` 里可以改：

- `frame=l` / `framerule` / `rulecolor`：左侧竖条的粗细和颜色。
- `backgroundcolor`：代码底色。
- `breakindent`：折行续行的缩进。
- `keywordstyle`、`commentstyle`、`stringstyle`：语法高亮颜色。
- `aboveskip`、`belowskip`：代码块上下的空白。

## 封面和目录

- 封面在 `print/main.tex` 的“封面”一段，用 TikZ 画色带和圆环。
- 目录由 `\cplibcontents` 输出，`print/preamble.tex` 里的 `\titlecontents` 控制缩进、点线和页码颜色。
- 章节标题由 `\titleformat{\section}` 和 `\titleformat{\subsection}` 控制。

# 源文件书写注意事项

## 行宽

单栏代码区约 107 列。源文件单行超过约 105 列时会在 PDF 里折行，
行首出现灰色 $\hookrightarrow$。折行不影响内容正确性，但会降低可读性。

## 代码块是逐字排版的

`gen_print.py` 把源文件原样塞进 `lstlisting`，不做转义。
因此源码里的 `\end{lstlisting}` 会提前结束代码块，正常 C++ 代码不会出现这种情况。

## 中文注释

代码里的中文注释使用黑体（SimHei），随代码字号缩放；不需要额外处理。

# 速查表

| 我想…… | 改哪里 | 然后运行 |
|---|---|---|
| 改模板代码 | 对应的 `.cpp/.hpp` | `python tools/build_print.py` |
| 加模板 | 新源文件 + `tools/gen_print.py` 配置 | `python tools/build_print.py` |
| 改模板标题/顺序 | `tools/gen_print.py` 配置 | `python tools/build_print.py` |
| 改字号/颜色/边距 | `print/preamble.tex` | `python tools/build_print.py --no-gen` |
| 改封面/目录 | `print/main.tex` + `print/preamble.tex` | `python tools/build_print.py --no-gen` |
| 改结论与技巧 | 对应目录的 `.md` | `python tools/build_print.py` |

# 常见问题

## 编译失败，提示找不到 latexmk

安装 TeX Live，并把它的 `bin/windows` 目录加入 `PATH`。本仓库用 XeLaTeX，
不能用 pdfLaTeX 编译（文档含中文）。

## 编译报错，想看到具体原因

打开 `print/CP-Lib.log`，搜索第一个以 `!` 开头的行。也可以直接运行：

```bash
latexmk print/main.tex
```

查看完整输出。

## 我手改了 print/chapters/*.tex，重新生成后改动没了

这是预期行为：那些文件是生成物。需要长期保留的内容应写回源码或对应的 `.md`。
只想临时编译一次手改版本时，用 `--no-gen`，并且之后不要再运行完整生成。

## 新模板没有出现在 PDF 里

检查 `tools/gen_print.py` 的章节配置里有没有它。运行 `--check` 可以看到所有未归入打印版的源文件。

## 目录或页码不对

目录、页码、`LastPage` 需要多趟编译。`build_print.py` 调用的 latexmk 会自动跑足够多趟；
不要手动只跑一遍 `xelatex` 后就判断页码错了。

## 代码行被折成两行

这是超过版心宽度后的自动折行，行首的灰色 $\hookrightarrow$ 表示续行。建议把源码行宽控制在 105 列以内。

## PDF 页数变了

新增模板、改排版、调整字号都会影响分页，页数变化是正常的。README 不写死页数。

## 封面页码重复 / xdvipdfmx 报 Object @page.1 already defined

不要用 `titlepage` 环境做封面：它会把页码重置为 1，导致封面和下一页产生重复的 `page.1` 锚点。
当前实现用普通页 + `\thispagestyle{empty}`，见 `print/main.tex` 顶部注释。

## 图片放在哪里

生成器会把结论 `.md` 里的 `<img src="URL">` 或 `![...](URL)` 转成
`print/assets/文件名` 的图片引用。新增图片时，把图片存到 `print/assets/`，
并让 URL 的文件名与本地文件名一致。

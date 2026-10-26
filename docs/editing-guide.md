```{=latex}
\pdfbookmark[0]{正文}{main}
```


这份指南说明怎样修改 CP-Lib 的模板内容、怎样把改动同步到打印版 PDF，以及排版出问题时该看哪里。
目标很简单：**改代码只碰 `.cpp/.hpp`，改排版只碰 `print/`，生成物不要手改。**

更完整的项目说明见仓库根目录的 `README.md`。

## 数学目录调整后的维护入口

数学源码已改用 `Math/NT/`、`Math/Comb/`、`Math/Poly/`、`Math/Alg/` 和 `Math/CG/`。当前有 47 份头文件，其中 24 份以下划线标记，仍属于参考或待审核模板。打印范围包含这些参考文件与 `Math/_notes.md`，并保留参考身份。

完整目录和接入步骤见 [数学打印同步说明](math-printing.md)，逐文件清单见 `docs/math-print-manifest.json`。`Math/_templates.md` 用于查找接口；`Math/Math.md` 和 `Math/CG/CG.md` 只作历史参考。

打印脚本已直接读取新清单，数学知识点由 Pandoc 全文转换。新增或改名数学模板时应维护清单，其他专题仍维护脚本中的章节配置。原 `ComputationalGeometry/` 目录已删除，计算几何章收录 `Math/CG/` 中的两份基础头文件和 `CG.md` 全文。杂项章另收录 `stress.py` 和 `stress.md`，分别作为 Python 代码与使用说明。

本次未重建配套指南 PDF，阅读维护说明时请以本 Markdown 为准。


# 先分清三类文件

| 类型 | 位置 | 说明 |
|---|---|---|
| 模板代码（正本） | 各专题目录下的 `.cpp` / `.hpp` | 日常只改这里 |
| 打印版排版 | `print/main.tex`、`print/preamble.tex` | 封面、目录、字体、颜色、代码块样式 |
| 打印版章节（生成物） | `print/chapters/*.tex` | 由脚本生成，手改会在下次生成时被覆盖 |

专题目录包括 `GraphTheory/`、`DataStructure/`、`String/`、`Math/`、
`Miscellaneous/`；计算几何基础保存在 `Math/CG/`。

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

数学模板请在 `docs/math-print-manifest.json` 中登记，字段与示例见 [数学打印说明](math-printing.md)。以下脚本配置示例用于其他专题。

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

也可以单独检查未列入配置的代码文件：

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

图论、数据结构和字符串章末尾的「结论与技巧」由生成器从 `.md` 中提取：标题中含
「结论 / 技巧 / 注意 / 速查 / 归档 / 性质」的小节会被收进来，子标题会降级保留。

对应关系：

| 章节 | 来源 |
|---|---|
| 图论 | `GraphTheory/Graph.md` |
| 数据结构 | `DataStructure/DataStructure.md` |
| 字符串 | `String/String.md` |
| 数学 | `Math/_notes.md`，由 Pandoc 全文转换 |
| 计算几何 | `Math/CG/CG.md`，全文收录代码、说明、公式和示意图 |
| 杂项 | `Miscellaneous/stress.md`，全文收录使用说明与代码示例 |

图论、数据结构和字符串改完 `.md` 后，按现有规则运行 `python tools/build_print.py`。旧提取器会丢弃结论中的代码块，只保留选中的文字、公式、列表和图片。

数学的 `_notes.md`、几何的 `CG.md` 和对拍的 `stress.md` 都不按标题关键词筛选，而是全文转换。生成器保留标题、公式和代码块，并使用现有 `listings` 排版代码。几何图像引用 `print/assets/` 中的本地缓存，缺图时会报告具体路径。修改正文后正常构建即可。

`Miscellaneous/stress.py` 作为独立源码条目打印，使用 Python 高亮。`Miscellaneous/hashtable.hpp` 已删除，列表中只保留 `DataStructure/hashtable.hpp`。

# build_print.py 的参数

| 命令 | 用途 |
|---|---|
| `python tools/build_print.py` | 改完模板源码后：重新生成章节 + 编译 |
| `python tools/build_print.py --no-gen`（`-n`） | 只改了 `print/` 下的排版时：跳过生成，直接编译 |
| `python tools/build_print.py --check`（`-c`） | 检查数学清单、缺失路径和未收录代码；有错误或遗漏时返回非 0 |
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

代码里的中文注释使用黑体（SimHei），随代码字号缩放。源码由 `lstlisting` 逐字打印，其中的 LaTeX 公式仍显示为源码；知识点 Markdown 的公式则应作为正文渲染。

接口注释应简要说明用途、输入和返回值。正文行内公式使用单美元符号，独立公式使用双美元符号；行内代码使用单反引号，代码块使用三反引号。

# 速查表

| 我想…… | 改哪里 | 然后运行 |
|---|---|---|
| 改模板代码 | 对应的 `.cpp/.hpp` | `python tools/build_print.py` |
| 加模板 | 新源文件 + 数学清单或其他章节配置 | `python tools/build_print.py` |
| 改模板标题/顺序 | 数学清单或其他章节配置 | `python tools/build_print.py` |
| 改字号/颜色/边距 | `print/preamble.tex` | `python tools/build_print.py --no-gen` |
| 改封面/目录 | `print/main.tex` + `print/preamble.tex` | `python tools/build_print.py --no-gen` |
| 改结论与技巧 | 对应目录的 `.md` | `python tools/build_print.py` |

# 常见问题

## 生成数学正文时提示找不到 pandoc

安装 Pandoc 并加入 `PATH`。数学知识点需要完整的 Markdown 转换，不能绕过正文后继续生成缺页的打印版。

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

检查 `tools/gen_print.py` 的章节配置里有没有它。下划线文件也必须登记，打印标题会保留“参考”标记。运行 `--check` 会检查数学清单及正文路径、全部章节的缺失源码，并列出未收录的代码。正文公式的实际转换则在生成时完成。

对于数学部分，核对 `docs/math-print-manifest.json` 的路径、标题与参考标记，生成器会据此更新输出。

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

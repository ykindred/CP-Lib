# 数学部分的目录与打印

打印生成器直接读取 `docs/math-print-manifest.json`。本文说明清单的维护方法、正文转换规则和检查命令，所有路径都相对于仓库根目录。

## 当前收录范围

数学目录共有 47 份头文件，其中 24 份以下划线开头。45 份位于数学章，`Math/CG/base.hpp` 和 `Math/CG/polar_sort.hpp` 位于计算几何章。原 `ComputationalGeometry/` 目录已删除，打印脚本不再引用该目录，也不会恢复它。几何章另全文收录仍保留在 `Math/CG/CG.md` 中的参考资料。

下划线模板参与打印，但仍是参考或待审核模板。生成器在标题中保留“参考”标记，并在数学源码标题下显示实际文件路径。打印收录不会改变模板身份，也不会重命名源文件。

`Math/_notes.md` 全文加入数学章末尾，保留标题、列表、链接、行内公式和独立公式。`Math/_templates.md` 是维护用接口索引，不重复打印。`Math/Math.md` 仍只作历史参考。`Math/CG/CG.md` 按原文打印，包含说明、公式、93 个代码块和 5 张示意图，身份仍为参考资料。

## 清单字段

- `code_entries` 列出源码的标题、路径、目标章节和参考标记。数组顺序就是章节内的打印顺序，`group` 用于说明所属专题。
- `path` 相对于仓库根目录。不要再另外拼接 `Math/`。
- `reference` 必须与文件名是否以下划线开头一致。参考条目的标题若未写“参考”，生成器会补上。
- `prose_entries` 指定数学与几何正文来源。`Math/_notes.md` 和 `Math/CG/CG.md` 都使用 `full_document`，表示全文转换。
- `heading_shift` 控制正文标题降级，范围为零到四。数学知识点从一级标题开始，设为一；几何原文从二级标题开始，设为零，保证两者的总标题都作为章内小节。
- `maintenance_only` 和 `legacy_references` 记录维护索引与历史资料，不作为打印正文。

## 新增、改名和调整顺序

新增数学头文件时，在清单的 `code_entries` 中增加一项。例如：

```json
{
  "chapter": "数学",
  "group": "数论",
  "title": "新算法（参考）",
  "path": "Math/NT/_new_algorithm.hpp",
  "reference": true
}
```

这只是字段示例，不是实际收录项。修改文件名时同步更新 `path`；改变顺序时移动清单条目；修改源码内容时无需重复修改清单。

所有数学头文件都必须列入清单。新增后忘记登记、路径不存在、文件为空、重复收录或参考身份不符，都会使检查和生成返回非零状态。`Math/_notes.md` 也不能从正文条目中遗漏。

其他专题继续维护 `tools/gen_print.py` 中的章节配置。`Miscellaneous/stress.py` 在杂项代码列表中登记；`Miscellaneous/stress.md` 在 `OTHER_TEXT_SOURCES` 中登记，并全文转换。杂项 Python 脚本也纳入未收录源码检查。

## 检查与生成

在仓库根目录运行：

```bash
python tools/build_print.py --check
```

检查模式不写入文件，也不要求安装 TeX。它校验数学清单、全部章节的缺失源码，并列出整个仓库中尚未收录的代码文件。

只生成 LaTeX 章节，不编译 PDF：

```bash
python tools/gen_print.py
```

这一步需要 Python 和 Pandoc。生成器先检查所有路径并完成正文转换，成功后才写入 `print/chapters/`。因此，缺失文件或正文转换失败不会把原章节覆盖成不完整结果。

生成并编译算法库 PDF：

```bash
python tools/build_print.py
```

完整构建还需要 TeX Live 中的 XeLaTeX、latexmk，以及排版配置使用的字体。只改排版时可以使用 `--no-gen`；修改源码、清单或知识点后应使用完整生成。

## 知识点的转换方式

数学知识点由 Pandoc 从完整 Markdown 转成 LaTeX 片段，直接追加到 `04-math.tex`。几何参考资料追加到 `05-geometry.tex`，对拍说明追加到 `06-misc.tex`。不再从旧 `Math/Math.md` 按关键词提取内容，也不需要额外维护一个知识点 `.tex` 文件。

Markdown 行内公式使用 `$...$`，独立公式使用单独成行的 `$$`；行内代码使用单反引号，代码块使用三反引号。生成器保留公式内容，并提供 Pandoc 紧凑列表和行内代码需要的命令定义。围栏代码块使用 `listings`，与现有打印样式保持一致。

图论、数据结构和字符串的结论仍沿用原来的标题关键词提取方式。`tools/md_inline.py` 继续服务这些旧式片段，不负责数学整篇正文的转换。

源码使用 `lstlisting` 逐字排版，其中的 LaTeX 注释按源码显示；知识点文档作为正常正文渲染。`stress.py` 明确使用 Python 语言设置。

`CG.md` 中的网络图片地址会映射为 `print/assets/` 中同名缓存，HTML 图片与 Markdown 图片都可处理。生成器不在编译时访问网络；缺少缓存会停止生成并提示具体文件。

## 当前源码目录

### 数学


**公共定义**

- 公共头文件：`Math/head.hpp`。

**数论**

- 模运算类：`Math/NT/modint.hpp`。
- 模运算与整数整除工具：`Math/NT/tools.hpp`。
- 扩展欧几里得与不定方程：`Math/NT/exgcd.hpp`。
- 扩展中国剩余定理：`Math/NT/excrt.hpp`。
- 试除分解与约数枚举：`Math/NT/factor.hpp`。
- Miller-Rabin 素性测试：`Math/NT/primetest.hpp`。
- 线性筛：`Math/NT/sieve.hpp`。
- 扩展线性筛：`Math/NT/sieve_ext.hpp`。
- 分段筛：`Math/NT/sieve_seg.hpp`。
- 类欧几里得求和：`Math/NT/euclid_like.hpp`。
- BSGS 与扩展 BSGS：`Math/NT/BSGS.hpp`。
- Pollard-Rho 分解（参考）：`Math/NT/_pollard_rho.hpp`。
- 由质因数分解生成约数（参考）：`Math/NT/_divisors.hpp`。
- 莫比乌斯函数与欧拉函数筛（参考）：`Math/NT/_mobius.hpp`。
- 杜教筛（参考）：`Math/NT/_dujiao_sieve.hpp`。
- 数论分块与带权求和（参考）：`Math/NT/_quotient_blocks.hpp`。
- 整除变换与最大公约数、最小公倍数卷积（参考）：`Math/NT/_divisor_transform.hpp`。
- Tonelli-Shanks 模平方根（参考）：`Math/NT/_mod_sqrt.hpp`。
- exLucas 合数模组合数（参考）：`Math/NT/_exlucas.hpp`。

**组合计数**

- 阶乘、组合数与 Lucas：`Math/Comb/factorial.hpp`。
- 斯特林数：`Math/Comb/stirling.hpp`。
- 错排、拆分、Bell、Catalan 与树计数（参考）：`Math/Comb/_numbers.hpp`。
- Burnside、项链与手链（参考）：`Math/Comb/_burnside.hpp`。
- Prüfer 编解码（参考）：`Math/Comb/_prufer.hpp`。

**多项式与线性递推**

- FFT：`Math/Poly/FFT.hpp`。
- NTT 与原根：`Math/Poly/NTT.hpp`。
- 三模 NTT 与精确整数卷积：`Math/Poly/MTT.hpp`。
- 多模 NTT 与 Garner 合并：`Math/Poly/MTT_large.hpp`。
- 按位卷积：`Math/Poly/FWHT.hpp`。
- 多项式与形式幂级数运算：`Math/Poly/poly_major.hpp`。
- BM 与 Kitamasa 线性递推（参考）：`Math/Poly/_linear_recurrence.hpp`。
- Bostan-Mori 与快速线性递推（参考）：`Math/Poly/_bostan_mori.hpp`。
- 连续点插值与幂和（参考）：`Math/Poly/_lagrange.hpp`。
- Taylor shift 多项式平移（参考）：`Math/Poly/_taylor_shift.hpp`。
- 子集卷积（参考）：`Math/Poly/_subset_convolution.hpp`。

**线性代数、数值积分与博弈**

- 高斯消元：`Math/Alg/gauss.hpp`。
- 矩阵与矩阵快速幂（参考）：`Math/Alg/_Matrix.hpp`。
- 异或线性基（参考）：`Math/Alg/_linear_basis.hpp`。
- 行列式（参考）：`Math/Alg/_determinant.hpp`。
- 矩阵求逆（参考）：`Math/Alg/_matrix_inverse.hpp`。
- 异或高斯消元（参考）：`Math/Alg/_gauss_xor.hpp`。
- 矩阵树定理（参考）：`Math/Alg/_kirchhoff.hpp`。
- 自适应 Simpson 积分（参考）：`Math/Alg/_integrate.hpp`。
- Nim 与 SG（参考）：`Math/Alg/_sg.hpp`。
### 计算几何


**二维基础**

- 二维几何基础：`Math/CG/base.hpp`。
- 极角排序：`Math/CG/polar_sort.hpp`。

## 验证记录

本次已通过两种 `--check` 入口，并实际生成六章 LaTeX。逐份核对了全部 47 个源码块、24 个参考标题、知识点正文及公式；图论、数据结构和字符串的输出与接入前一致。

BSGS 实际依赖的 `DataStructure/hashtable.hpp` 原先未收录，本次将它加入杂项章，杂项中的重复版本现已删除，打印列表也已移除，只保留数据结构中的版本。新增的 `Miscellaneous/RNG.hpp`、`stress.py` 和 `stress.md` 已收录。算法源码和对拍源文件没有改动。

生成器测试命令为：

```bash
python tools/test_gen_print.py -v
```

测试覆盖清单错误、重复加载、参考标记、普通标题、列表与公式转换、CG 的代码和本地图片、对拍说明、Python 排版、缺少 Pandoc 或图片缓存，以及转换失败时保留旧章节。

当前环境缺少 XeLaTeX 和 latexmk，因此只生成并检查了 LaTeX，未重新编译算法库 PDF。配套指南 PDF 也未重建，维护说明以 Markdown 正本为准。

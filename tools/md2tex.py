#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
一次性迁移脚本：CP-Lib.md  ->  print/chapters/*.tex

⚠️ 这是一次性工具。转换完成后 print/chapters/*.tex 即为打印版主文档，
   此后请直接手改 .tex，不要再运行本脚本（会覆盖手工修改）。

为什么用脚本而不是手抄：正文里有 4939 行 C++ 代码，逐字手抄必然引入错字，
机械转换可以保证代码零失真。

用法：
    python tools/md2tex.py
"""

import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
SRC = ROOT / "CP-Lib.md"
OUTDIR = ROOT / "print" / "chapters"

# 12 个 h1 章节对应的文件名 slug（按出现顺序）
CHAPTER_SLUGS = [
    "01-topo",            # 拓扑排序
    "02-tree",            # 树
    "03-tarjan",          # Tarjan
    "04-flow",            # 网络流
    "05-datastructure",   # 数据结构模板库
    "06-geometry",        # 几何
    "07-combinatorics",   # 组合数学
    "08-modint",          # 模运算
    "09-numbertheory",    # 数论
    "10-algebra",         # 代数
    "11-string",          # 字符串
    "12-misc",            # 杂
]

HEADING_CMDS = {1: "section", 2: "subsection", 3: "subsubsection", 4: "paragraph"}

# ---------------------------------------------------------------- 转义

ESCAPES = {
    "\\": r"\textbackslash{}",
    "{": r"\{",
    "}": r"\}",
    "$": r"\$",
    "&": r"\&",
    "#": r"\#",
    "%": r"\%",
    "_": r"\_",
    "~": r"\textasciitilde{}",
    "^": r"\textasciicircum{}",
    "<": r"\textless{}",
    ">": r"\textgreater{}",
}
_ESC_RE = re.compile(r"[\\{}$&#%_~^<>]")


def esc(s: str) -> str:
    """单趟替换，避免二次转义（\\textbackslash{} 自身含花括号）。"""
    return _ESC_RE.sub(lambda m: ESCAPES[m.group()], s)


# ---------------------------------------------------------------- 行内切分


def _find_closing_dollar(s: str, i: int) -> int:
    """从 s[i] 起找未被反斜杠转义的 $，返回下标或 -1。
    注意要跳过 \\\\ （矩阵换行）与 \\{ 之类的转义序列。"""
    while i < len(s):
        if s[i] == "\\":
            i += 2
            continue
        if s[i] == "$":
            return i
        i += 1
    return -1


def tokenize(s: str):
    """把行内文本切成 (kind, value)，kind ∈ {text, code, math}。

    数学段跨行也算一个整体 —— CP-Lib.md 第 1950-1953、1963-1966 行的
    $...$ 就是跨行的，全文仅有的 _ \\ { } 都在里面，按行处理会直接编挂。
    """
    out, buf = [], []
    i, n = 0, len(s)

    def flush():
        if buf:
            out.append(("text", "".join(buf)))
            buf.clear()

    while i < n:
        c = s[i]
        if c == "\\":
            buf.append(s[i: i + 2])
            i += 2
            continue
        if c == "`":
            j = s.find("`", i + 1)
            if j == -1:
                buf.append(c)
                i += 1
                continue
            flush()
            out.append(("code", s[i + 1: j]))
            i = j + 1
            continue
        if c == "$":
            j = _find_closing_dollar(s, i + 1)
            if j == -1:
                buf.append(c)
                i += 1
                continue
            flush()
            out.append(("math", s[i + 1: j]))
            i = j + 1
            continue
        buf.append(c)
        i += 1
    flush()
    return out


def render_inline(s: str) -> str:
    parts = []
    for kind, val in tokenize(s):
        if kind == "math":
            body = val.strip()
            # 含环境或多行的公式用行间公式，可读性更好
            if "\n" in val or "\\begin{" in body:
                parts.append("\\[" + body + "\\]")
            else:
                parts.append("$" + val + "$")
        elif kind == "code":
            parts.append("\\texttt{" + esc(val) + "}")
        else:
            parts.append(esc(val))
    return "".join(parts)


# ---------------------------------------------------------------- 占位符机制

# 先把图片/加粗/链接抠成占位符，再统一转义剩余文本，最后回填。
# 占位符用 \x00 包数字，esc() 不会碰它。
_HOLD_RE = re.compile(r"\x00(\d+)\x00")

_IMG_TAG_RE = re.compile(r'<img\s+[^>]*?src="([^"]+)"[^>]*?/?>', re.I)
_MD_IMG_RE = re.compile(r"!\[[^\]]*\]\(([^)]+)\)")
_BOLD_RE = re.compile(r"\*\*(.+?)\*\*")
_LINK_RE = re.compile(r"\[([^\]]+)\]\(([^)]+)\)")


def _figure(url: str) -> str:
    """本地缓存的图 + 原始 URL（放脚注在 multicols 下不可靠，改为图下一行小字）。

    用 center 环境而不是 \\centerline：\\centerline 按 \\hsize 定宽，
    在 quote/itemize 里不会扣除缩进，会溢出版心（实测正好溢出缩进量）。
    """
    name = url.rsplit("/", 1)[-1]
    return (
        "\\begin{center}"
        "\\includegraphics[width=0.86\\linewidth]{assets/" + name + "}\\\\[1pt]"
        "{\\tiny\\color{gray}\\url{" + url + "}}"
        "\\end{center}"
    )


def render_prose(s: str) -> str:
    holders = []

    def stash(latex: str) -> str:
        holders.append(latex)
        return "\x00%d\x00" % (len(holders) - 1)

    # 顺序要紧：图片 -> 加粗 -> 链接。先抠出来的变成占位符，
    # 后续 render_inline 看到占位符会原样放行，天然支持嵌套。
    s = _IMG_TAG_RE.sub(lambda m: stash(_figure(m.group(1))), s)
    s = _MD_IMG_RE.sub(lambda m: stash(_figure(m.group(1))), s)
    s = _BOLD_RE.sub(lambda m: stash("\\textbf{" + render_inline(m.group(1)) + "}"), s)
    s = _LINK_RE.sub(
        lambda m: stash("\\href{" + m.group(2) + "}{" + render_inline(m.group(1)) + "}"), s
    )

    out = render_inline(s)
    return _HOLD_RE.sub(lambda m: holders[int(m.group(1))], out)


# ---------------------------------------------------------------- 块解析


def parse(lines):
    """把行序列解析成块列表。"""
    blocks = []
    i, n = 0, len(lines)
    while i < n:
        line = lines[i]

        # 代码围栏
        if line.startswith("```"):
            lang = line[3:].strip()
            j = i + 1
            body = []
            while j < n and not lines[j].startswith("```"):
                body.append(lines[j])
                j += 1
            blocks.append(("code", lang, body))
            i = j + 1
            continue

        # 标题
        m = re.match(r"^(#{1,6})\s+(.*)$", line)
        if m:
            blocks.append(("heading", len(m.group(1)), m.group(2).strip()))
            i += 1
            continue

        # 空行
        if not line.strip():
            i += 1
            continue

        # 分隔线
        if re.match(r"^-{3,}\s*$", line):
            blocks.append(("rule",))
            i += 1
            continue

        # 引用块
        if line.lstrip().startswith(">"):
            j = i
            body = []
            while j < n and (lines[j].lstrip().startswith(">") or not lines[j].strip()):
                if lines[j].lstrip().startswith(">"):
                    body.append(re.sub(r"^\s*>\s?", "", lines[j], count=1))
                else:
                    body.append("")
                j += 1
                # 引用块后必须紧跟引用行才继续
                if j < n and not lines[j].strip():
                    if j + 1 < n and lines[j + 1].lstrip().startswith(">"):
                        continue
                    break
            while body and not body[-1].strip():
                body.pop()
            blocks.append(("quote", body))
            i = j
            continue

        # 列表
        if re.match(r"^\s*([-*]|\d+\.)\s+", line):
            items = []
            cur = None
            j = i
            while j < n:
                l = lines[j]
                m2 = re.match(r"^\s*([-*]|\d+\.)\s+(.*)$", l)
                if m2:
                    if cur is not None:
                        items.append(cur)
                    cur = [m2.group(2)]
                    j += 1
                    continue
                if not l.strip():
                    # 空行：若后面还有缩进的续行或新列表项，则继续
                    k = j + 1
                    while k < n and not lines[k].strip():
                        k += 1
                    if k < n and (
                        lines[k].startswith((" ", "\t"))
                        or re.match(r"^\s*([-*]|\d+\.)\s+", lines[k])
                    ):
                        if cur is not None:
                            cur.append("")
                        j = k
                        continue
                    break
                if l.startswith((" ", "\t")) and cur is not None:
                    cur.append(l.strip())
                    j += 1
                    continue
                break
            if cur is not None:
                items.append(cur)
            ordered = bool(re.match(r"^\s*\d+\.\s+", line))
            blocks.append(("list", ordered, items))
            i = j
            continue

        # 普通段落：连续非空、非特殊行
        j = i
        para = []
        while j < n:
            l = lines[j]
            if not l.strip():
                break
            if (
                l.startswith("```")
                or re.match(r"^#{1,6}\s+", l)
                or re.match(r"^-{3,}\s*$", l)
                or l.lstrip().startswith(">")
                or re.match(r"^\s*([-*]|\d+\.)\s+", l)
            ):
                break
            para.append(l.strip())
            j += 1
        blocks.append(("para", para))
        i = j
    return blocks


# ---------------------------------------------------------------- 输出


def emit_blocks(blocks, out):
    for b in blocks:
        kind = b[0]

        if kind == "heading":
            lvl, text = b[1], b[2]
            cmd = HEADING_CMDS.get(lvl)
            if cmd is None:
                continue
            out.append("\\%s{%s}" % (cmd, render_inline(text)))
            out.append("")

        elif kind == "code":
            out.append("\\begin{lstlisting}")
            out.extend(b[2])
            out.append("\\end{lstlisting}")
            out.append("")

        elif kind == "rule":
            # \addvspace 只能在垂直模式用；\rule 之后处于水平模式，
            # 必须先 \par 回到垂直模式，否则报 "missing \item"
            out.append(
                "\\par\\addvspace{3pt}\\noindent\\rule{\\linewidth}{0.4pt}"
                "\\par\\addvspace{3pt}"
            )
            out.append("")

        elif kind == "quote":
            out.append("\\begin{quote}")
            for l in b[1]:
                out.append(render_prose(l) if l.strip() else "")
            out.append("\\end{quote}")
            out.append("")

        elif kind == "list":
            env = "enumerate" if b[1] else "itemize"
            out.append("\\begin{%s}" % env)
            for item in b[2]:
                text = " ".join(x for x in item if x.strip())
                out.append("  \\item " + render_prose(text))
            out.append("\\end{%s}" % env)
            out.append("")

        elif kind == "para":
            out.append(render_prose(" ".join(b[1])))
            out.append("")


def main():
    raw = SRC.read_bytes().decode("utf-8")
    raw = raw.replace("\r\n", "\n").replace("\r", "\n")
    lines = raw.split("\n")

    # 按 h1 切章
    chapters = []
    cur = None
    for idx, line in enumerate(lines):
        m = re.match(r"^#\s+(.*)$", line)
        if m:
            if cur is not None:
                chapters.append(cur)
            cur = (m.group(1).strip(), [])
            continue
        if cur is not None:
            cur[1].append(line)
    if cur is not None:
        chapters.append(cur)

    if len(chapters) != len(CHAPTER_SLUGS):
        print(
            "警告：h1 章节数 %d 与预设 slug 数 %d 不符" % (len(chapters), len(CHAPTER_SLUGS)),
            file=sys.stderr,
        )

    OUTDIR.mkdir(parents=True, exist_ok=True)
    inputs = []
    for k, (title, body) in enumerate(chapters):
        slug = CHAPTER_SLUGS[k] if k < len(CHAPTER_SLUGS) else "zz-%02d" % k
        blocks = [("heading", 1, title)] + parse(body)
        out = []
        emit_blocks(blocks, out)
        text = "\n".join(out).rstrip() + "\n"
        path = OUTDIR / (slug + ".tex")
        path.write_text(text, encoding="utf-8")
        inputs.append("chapters/" + slug + ".tex")
        print("%-24s %5d 行  %s" % (slug + ".tex", text.count("\n"), title))

    print("\n共 %d 章" % len(chapters))
    print("main.tex 里按顺序 \\input：")
    for p in inputs:
        print("  \\input{" + p + "}")


if __name__ == "__main__":
    main()

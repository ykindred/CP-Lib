#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Markdown 行内元素 -> LaTeX 的转换函数（被 gen_print.py 复用）。

只处理行内内容：转义、`code`、$math$、**粗体**、[链接](url)、图片。
整篇 Markdown 的块级解析（标题、列表、代码围栏）在 gen_print.py 里按打印版需要另做，
不在这里。
"""

import re

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

    注意要跳过 \\\\（矩阵换行）与 \\{ 之类的转义序列。
    """
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

    数学段跨行也算一个整体：旧文档里有 $...$ 跨行的情况，按行处理会编挂。
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
            text = esc(val)
            # 中文文档里的逻辑符号在西文正文字体里没有字形，换成数学符号
            text = text.replace("⇔", r"$\Leftrightarrow$").replace("⇒", r"$\Rightarrow$")
            parts.append(text)
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
    """本地缓存的图 + 原始 URL（图下一行小字）。"""
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

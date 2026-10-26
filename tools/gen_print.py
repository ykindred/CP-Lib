#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
从仓库里的真实源码生成打印版章节 print/chapters/*.tex。

⚠️ 生成物会覆盖 print/chapters/ 下的同名文件。模板代码更新后运行本脚本；
   如果手工改过 .tex，请先把改动回写到源码，否则会被覆盖。
   日常更推荐直接运行 tools/build_print.py（生成 + 编译一步到位）。

用法：
    python tools/gen_print.py            # 生成全部章节
    python tools/gen_print.py --check    # 检查清单、缺失文件和未归入打印版的源码
"""

import argparse
import json
import os
import re
import shutil
import subprocess
import sys
from pathlib import Path


def setup_console():
    """Windows 控制台可能是 GBK；统一成 UTF-8，避免中文输出乱码。"""
    if sys.platform.startswith("win"):
        os.system("chcp 65001 >nul")
    for stream in (sys.stdout, sys.stderr):
        try:
            stream.reconfigure(encoding="utf-8", errors="replace")
        except (AttributeError, ValueError, OSError):
            pass


# 不生成 tools/__pycache__：这个脚本经常被 build_print.py 调用
sys.dont_write_bytecode = True
sys.path.insert(0, str(Path(__file__).resolve().parent))
from md_inline import render_prose  # 行内 Markdown -> LaTeX

ROOT = Path(__file__).resolve().parent.parent
OUTDIR = ROOT / "print" / "chapters"

# ============================================================
#  章节配置：(标题, 源文件相对路径)
#  顺序即打印顺序。空文件会自动跳过。
# ============================================================
CH_GRAPH = ("图论", "GraphTheory", [
    ("拓扑排序", "TopoSort.cpp"),
    ("树的直径", "TreeDiameter.cpp"),
    ("树的重心", "TreeCentroid.cpp"),
    ("倍增 LCA", "BinaryLCA.cpp"),
    ("树上启发式合并 (DSU on Tree)", "DSU_on_Tree.cpp"),
    ("SCC（强连通分量）", "TarjanSCC.cpp"),
    ("割边（Bridges）", "Bridges.cpp"),
    ("割点（Cut Points）", "CutPoints.cpp"),
    ("最大流 / 最小割（Dinic）", "Dinic.cpp"),
    ("Dijkstra（堆优化，非负权）", "Dijkstra.cpp"),
    ("SPFA（可处理负权边）", "SPFA.cpp"),
    ("Floyd（多源最短路）", "Floyd.cpp"),
    ("分层图最短路", "LayeredGraph.cpp"),
    ("SPFA 判负环", "NegativeCycleSPFA.cpp"),
    ("Bellman-Ford 判负环", "NegativeCycleBellmanFord.cpp"),
    ("染色法判定二分图", "BipartiteColoring.cpp"),
    ("匈牙利算法（最大匹配）", "Hungarian.cpp"),
    ("KM 算法（最大权完美匹配）", "KM.cpp"),
    ("基环树找环（拓扑剪叶法）", "BaseRingTreeFindCycle.cpp"),
    ("基环树最大独立集", "BaseRingTreeMaxIndependentSet.cpp"),
])

CH_DS = ("数据结构", "DataStructure", [
    ("手写 bitset", "Bitset/bitset.cpp"),
    ("树状数组", "FenwickTree/FenWickTree.hpp"),
    ("基础并查集", "DisjointSetUnion/Disjoint_set_unoin.cpp"),
    ("可回滚并查集", "DisjointSetUnion/Disjoint_set_union_with_rollback.cpp"),
    ("带删除并查集", "DisjointSetUnion/Disjoint_set_unoin_with_delete.cpp"),
    ("基础线段树", "SegmemtTree/Segment_tree.cpp"),
    ("懒标记线段树", "SegmemtTree/Segment_tree_with_lazytag.cpp"),
    ("动态开点线段树", "SegmemtTree/Segment_tree_with_dynamic_points.cpp"),
    ("可持久化线段树（主席树）", "SegmemtTree/Persistent_segment_tree.cpp"),
    ("标记永久化线段树", "SegmemtTree/Segment_tree_with_persistent_marks.cpp"),
    ("李超线段树", "SegmemtTree/Lichao_segment_tree.cpp"),
    ("线段树套线段树", "SegmemtTree/Segment_tree_of_segment_tree.cpp"),
    ("基础字典树", "Trie/Trie.cpp"),
    ("01 字典树", "Trie/01-Trie.cpp"),
    ("可持久化 01 字典树", "Trie/Persistent_Trie.cpp"),
    ("Splay（模板版）", "Treap/splay.cpp"),
    ("Splay（数组版）", "Binary Seaech Tree/splay.cpp"),
    ("隐式 Treap（FHQ-Treap）", "Binary Seaech Tree/Implicit_treap.cpp"),
    ("PBDS 平衡树（名次树）", "Binary Seaech Tree/treap_pbds.cpp"),
    ("PBDS 堆", "Heap/pbds_heap.cpp"),
    ("rope（块状链表）", "Binary Seaech Tree/rope.cpp"),
    ("树链剖分", "Tree Structure Decomposition/Heavy_light_decomposition.cpp"),
    ("点分治 / 点分树", "Tree Structure Decomposition/Centroid_decomposition.cpp"),
    ("稀疏表", "Sptable/sptable.hpp"),
    ("莫队（含回滚莫队思路）", "Mo's algorithm/Mo_with_rollback.cpp"),
    ("凸包上二分（动态维护直线最值）", "Other/Binary_Search_on_Convex.cpp"),
    ("LineContainer（动态直线容器）", "Other/LineContainer.cpp"),
])

CH_STRING = ("字符串", "String", [
    ("字符串哈希", "StringHash.cpp"),
    ("KMP（前缀函数 / 匹配 / 周期）", "PrefixFunction.cpp"),
    ("Z 函数（扩展 KMP）", "ZFunction.cpp"),
    ("Manacher", "Manacher.cpp"),
    ("Trie", "Trie.cpp"),
    ("AC 自动机", "ACAutomaton.cpp"),
    ("后缀数组 SA", "SuffixArray.cpp"),
    ("ST 表求任意两后缀 LCP", "SuffixArrayLCP.cpp"),
    ("后缀自动机 SAM", "SuffixAutomaton.cpp"),
    ("广义 SAM（多串）", "GeneralizedSAM.cpp"),
    ("回文自动机 PAM", "PalindromicTree.cpp"),
    ("最小表示法", "MinimalRepresentation.cpp"),
    ("Lyndon 分解（Duval）", "Lyndon.cpp"),
])

CH_MISC = ("杂项", ".", [
    ("离散化", "Miscellaneous/Discretization.cpp"),
    ("欧拉序（子树转区间）", "Miscellaneous/GenerateEulerTourOrder.cpp"),
    ("反转整数", "Miscellaneous/ReverseInt.cpp"),
    ("防卡哈希表（BSGS 依赖）", "DataStructure/hashtable.hpp"),
    ("快读快写（FinFout）", "Miscellaneous/FinFout.cpp"),
    ("随机数生成（RNG）", "Miscellaneous/RNG.hpp"),
    ("赛时对拍脚本（stress.py）", "Miscellaneous/stress.py"),
])

# 杂项正文单独登记；数学和几何正文由数学清单提供。
OTHER_TEXT_SOURCES = [
    {"chapter": "杂项", "title": "赛时对拍使用说明", "path": "Miscellaneous/stress.md",
     "mode": "full_document", "heading_shift": 1},
]

# 数学与几何基础条目由清单提供，运行时配置，避免保留另一份手写目录。
CHAPTERS = []
FULL_TEXT_SOURCES = []


def configure_chapters():
    """读取数学清单，校验输入路径和参考身份，更新代码章节与全文正文配置。"""
    global CHAPTERS, FULL_TEXT_SOURCES
    path = ROOT / "docs" / "math-print-manifest.json"
    manifest = json.loads(path.read_text(encoding="utf-8"))
    if not isinstance(manifest, dict) or manifest.get("schema_version") != 1 or manifest.get("path_base") != "repository_root":
        raise ValueError("数学清单的版本或路径基准不正确")

    def source(rel):
        """接收仓库相对路径，返回实际文件；拒绝越出 Math 目录或缺失的文件。"""
        if not isinstance(rel, str) or Path(rel).is_absolute():
            raise ValueError(f"清单需要相对路径：{rel}")
        target = (ROOT / rel).resolve()
        try:
            target.relative_to((ROOT / "Math").resolve())
        except ValueError:
            raise ValueError(f"清单路径不在 Math 目录内：{rel}") from None
        if not target.is_file():
            raise ValueError(f"清单文件不存在：{rel}")
        if not target.read_text(encoding="utf-8").strip():
            raise ValueError(f"清单文件为空：{rel}")
        return target

    entries = {"数学": [], "计算几何": []}
    seen = set()
    for entry in manifest["code_entries"]:
        rel, title, chapter = entry["path"], entry["title"], entry["chapter"]
        if not isinstance(title, str) or not title.strip():
            raise ValueError(f"打印标题不能为空：{rel}")
        target = source(rel)
        if target in seen:
            raise ValueError(f"清单重复收录：{rel}")
        seen.add(target)
        if target.suffix != ".hpp" or chapter not in entries:
            raise ValueError(f"不支持的数学代码条目：{rel}")
        reference = target.name.startswith("_")
        if type(entry["reference"]) is not bool or entry["reference"] != reference:
            raise ValueError(f"参考标记与文件名不符：{rel}")
        if reference and "（参考）" not in title:
            title += "（参考）"
        entries[chapter].append((title, target.relative_to(ROOT.resolve()).as_posix()))

    missing = {p.resolve() for p in (ROOT / "Math").rglob("*.hpp")} - seen
    if missing:
        names = ", ".join(sorted(p.relative_to(ROOT.resolve()).as_posix() for p in missing))
        raise ValueError(f"数学清单遗漏头文件：{names}")

    prose = manifest["prose_entries"]
    prose_seen = set()
    for entry in prose:
        if not isinstance(entry["title"], str) or not entry["title"].strip():
            raise ValueError(f"正文标题不能为空：{entry['path']}")
        target = source(entry["path"])
        if target in prose_seen or entry["chapter"] not in entries:
            raise ValueError(f"重复或无效的正文条目：{entry['path']}")
        prose_seen.add(target)
        if entry["mode"] != "full_document" or target.suffix != ".md":
            raise ValueError(f"正文必须使用完整 Markdown：{entry['path']}")
        shift = entry["heading_shift"]
        if type(shift) is not int or not 0 <= shift <= 4:
            raise ValueError(f"正文标题降级必须在 0 到 4 之间：{entry['path']}")
    if not any((ROOT / entry["path"]).resolve() == (ROOT / "Math" / "_notes.md").resolve()
               and entry["chapter"] == "数学" for entry in prose):
        raise ValueError("数学清单必须全文收录 Math/_notes.md")

    geometry = entries["计算几何"]
    CHAPTERS = [CH_GRAPH, CH_DS, CH_STRING, ("数学", ".", entries["数学"]),
                ("计算几何", ".", geometry), CH_MISC]
    FULL_TEXT_SOURCES = prose + OTHER_TEXT_SOURCES


def prepare_images(markdown):
    """接收 Markdown，将远程示意图改用已有打印缓存，返回可直接排版的正文。"""
    def image(url):
        """接收图片地址，返回本地图片引用；缓存缺失时明确报错。"""
        if not url.startswith(("https://", "http://")):
            return f"![]({url})"
        name = url.split("?", 1)[0].rsplit("/", 1)[-1]
        if not (ROOT / "print" / "assets" / name).is_file():
            raise RuntimeError(f"缺少打印图片缓存：print/assets/{name}，来源：{url}")
        return f"![](assets/{name}){{width=86%}}"

    markdown = re.sub(r'!\[[^\]]*\]\((https?://[^)]+)\)',
                      lambda m: image(m.group(1)), markdown)
    return re.sub(r'<img\b[^>]*?src="([^\"]+)"[^>]*?/?>',
                  lambda m: image(m.group(1)), markdown, flags=re.I)


def emit_full_text(entry):
    """接收全文正文条目，调用 Pandoc，返回保留标题、列表、链接和公式的 LaTeX 行。"""
    pandoc = shutil.which("pandoc")
    if not pandoc:
        raise RuntimeError("找不到 pandoc，无法转换 Markdown 正文；请安装并加入 PATH")
    path = ROOT / entry["path"]
    markdown = prepare_images(path.read_text(encoding="utf-8"))
    # 清单指定打印标题，保留原开头标题的级别，不重复插入标题。
    heading = re.match(r"^(#{1,6}) +[^\n]*", markdown)
    if heading:
        markdown = heading.group(1) + " " + entry["title"] + markdown[heading.end():]
    else:
        markdown = "# " + entry["title"] + "\n\n" + markdown
    result = subprocess.run(
        [pandoc, "--from=markdown+tex_math_dollars", "--to=latex", "--wrap=none", "--listings",
         f"--shift-heading-level-by={entry['heading_shift']}"],
        input=markdown, encoding="utf-8", stdout=subprocess.PIPE, stderr=subprocess.PIPE,
        cwd=path.parent,
    )
    if result.returncode:
        raise RuntimeError(f"正文转换失败：{entry['path']}\n{result.stderr.strip()}")
    if result.stderr.strip():
        print(result.stderr.strip(), file=sys.stderr)
    # Pandoc 完整模板中的列表与行内代码辅助命令，需要在片段中补上定义。
    return ["", r"\providecommand{\tightlist}{\setlength{\itemsep}{0pt}\setlength{\parskip}{0pt}}",
            r"\providecommand{\passthrough}[1]{#1}", *result.stdout.splitlines()]


def missing_sources():
    """检查全部代码和全文正文，返回所属章节及缺失文件的相对路径列表。"""
    sources = [(title, (Path(folder) / rel).as_posix())
               for title, folder, entries in CHAPTERS for _, rel in entries]
    sources += [(entry["chapter"], entry["path"]) for entry in FULL_TEXT_SOURCES]
    return [(title, rel) for title, rel in sources if not (ROOT / rel).is_file()]


# 每个章节末尾追加的「结论与技巧」，从对应 .md 里按标题抓
CONCLUSION_SOURCES = {
    "图论": "GraphTheory/Graph.md",
    "数据结构": "DataStructure/DataStructure.md",
    "字符串": "String/String.md",
}
CONCLUSION_PAT = re.compile(r"(结论|技巧|注意|速查|归档|性质)")


def harvest_conclusions(md_path: Path):
    """抓出 .md 里标题带「结论/技巧/注意/速查/归档/性质」的小节。

    要处理嵌套：像 Math.md 的「## 杂项结论」下面挂着好多个「### xxx定理」，
    只有把这些子小节一并收进来才不会漏内容。子标题降级成 #### 保留。
    """
    if not md_path.exists():
        return []
    lines = md_path.read_text(encoding="utf-8").splitlines()
    sections, cur, cur_level = [], None, 0
    in_code = False

    for line in lines:
        m = re.match(r"^(#{1,6})\s+(.*)$", line)
        if m:
            level, title = len(m.group(1)), m.group(2)
            if cur is not None and level <= cur_level:
                sections.append(cur)          # 同级或更高级标题 -> 本节结束
                cur = None
            if cur is None:
                if CONCLUSION_PAT.search(title):
                    cur_level, cur, in_code = level, [], False
            else:
                cur.append("#### " + title)   # 子标题降级保留
            continue
        if cur is None:
            continue
        if line.startswith("```"):
            in_code = not in_code             # 结论里的代码块整段丢弃
            continue
        if not in_code:
            cur.append(line)
    if cur is not None:
        sections.append(cur)
    return [s for s in sections if any(x.strip() for x in s)]


def emit_conclusions(title, md_rel):
    if not md_rel:
        return []                    # 该章没有对应的 .md（如「杂项」）
    md = ROOT / md_rel
    if not md.is_file():
        return []
    secs = harvest_conclusions(md)
    if not secs:
        return []
    out = ["", "\\subsection*{结论与技巧}", "\\addcontentsline{toc}{subsection}{结论与技巧}"]
    for body in secs:
        out += _emit_conclusion_body(body)
    return out


_ITEM_RE = re.compile(r"^(\s*)([-*]|\d+\.)\s+(.*)$")


def _emit_conclusion_body(body):
    """逐行扫描正文。

    不能按「空行分段」再判断是不是列表：几何结论里图片是列表项的续行，
    而图片行后面紧跟着下一个列表项、中间没有空行，于是「图片 + 后面所有
    条目」会被当成一个段落，既进不了 itemize，条目之间还会被压成一行。
    所以这里按行走，遇到列表标记就开一个列表，缩进行的续行并进当前项。
    """
    lines = [l.rstrip() for l in body]
    out, i, n = [], 0, len(lines)

    while i < n:
        line = lines[i]
        if not line.strip():
            i += 1
            continue

        if line.startswith("#### "):
            out.append("\\par\\addvspace{2pt}\\noindent\\textbf{%s}\\par"
                       % render_prose(line[5:].strip()))
            i += 1
            continue

        m = _ITEM_RE.match(line)
        if m:
            ordered = m.group(2)[0].isdigit()
            items, cur = [], None
            while i < n:
                mm = _ITEM_RE.match(lines[i])
                if mm:
                    if cur is not None:
                        items.append(cur)
                    cur = mm.group(3)
                    i += 1
                    continue
                if not lines[i].strip():
                    j = i + 1
                    while j < n and not lines[j].strip():
                        j += 1
                    if j < n and (_ITEM_RE.match(lines[j]) or lines[j][:1] in " \t"):
                        i = j
                        continue
                    break
                # 缩进或图片行 -> 当前项的续行
                if lines[i][:1] in " \t" or lines[i].lstrip().startswith(("<img", "![")):
                    if cur is not None:
                        cur += " " + lines[i].strip()
                    i += 1
                    continue
                break
            if cur is not None:
                items.append(cur)
            env = "enumerate" if ordered else "itemize"
            out.append("\\begin{%s}" % env)
            for it in items:
                out.append("  \\item " + render_prose(it))
            out.append("\\end{%s}" % env)
            continue

        para = []
        while i < n and lines[i].strip() and not _ITEM_RE.match(lines[i]) \
                and not lines[i].startswith("#### "):
            para.append(lines[i].strip())
            i += 1
        out.append(render_prose(" ".join(para)))
        out.append("")

    return out


def emit_chapter(title, folder, entries):
    out = ["\\section{%s}" % title, ""]
    missing, empty = [], []
    for sub, rel in entries:
        path = (ROOT / folder / rel) if folder != "." else (ROOT / rel)
        if not path.exists():
            missing.append(rel)
            continue
        code = path.read_text(encoding="utf-8").rstrip("\n")
        if not code.strip():
            empty.append(rel)
            continue
        out.append("\\subsection{%s}" % sub)
        if folder == "." and rel.startswith("Math/"):
            source_path = (ROOT / folder / rel).relative_to(ROOT).as_posix()
            out.append(r"\noindent{\small " + render_prose("`" + source_path + "`") + r"}\par")
        out.append("")
        language = "[language=Python]" if path.suffix == ".py" else ""
        out.append("\\begin{lstlisting}" + language)
        out.extend(code.split("\n"))
        out.append("\\end{lstlisting}")
        out.append("")
    return out, missing, empty


def listed_paths():
    """章节配置里列到的源文件绝对路径。"""
    listed = set()
    for title, folder, entries in CHAPTERS:
        for _, rel in entries:
            listed.add(((ROOT / folder / rel) if folder != "." else (ROOT / rel)).resolve())
    return listed


def unlisted_sources():
    """仓库里存在、但章节配置没有列到的源文件（打印版不会包含）。"""
    listed = listed_paths()
    out = []
    for p in sorted(ROOT.rglob("*")):
        if not p.is_file():
            continue
        rp = p.relative_to(ROOT).as_posix()
        if p.suffix not in {".cpp", ".hpp", ".h"} and not (p.suffix == ".py" and rp.startswith("Miscellaneous/")):
            continue
        if rp.startswith(("print/", "tools/")):
            continue
        if p.resolve() not in listed:
            n = len(p.read_text(encoding="utf-8", errors="replace").splitlines())
            out.append((rp, n))
    return out


def main():
    """处理命令行，检查收录情况或生成全部章节；失败时返回非零状态。"""
    ap = argparse.ArgumentParser()
    ap.add_argument("--check", action="store_true",
                    help="检查数学清单、缺失文件和未收录源码，不写文件")
    args = ap.parse_args()
    try:
        configure_chapters()
    except (OSError, ValueError, KeyError, TypeError) as exc:
        print(f"[数学清单错误] {exc}", file=sys.stderr)
        return 1

    missing = missing_sources()
    if missing:
        print("[缺失] 配置里列了但文件不存在：", file=sys.stderr)
        for title, rel in missing:
            print(f"    {title}: {rel}", file=sys.stderr)
    unlisted = unlisted_sources()
    if args.check:
        print("=== 未归入打印版的源文件 ===")
        if not unlisted:
            print("    （无）")
        for rel, n in unlisted:
            print(f"    {rel}   ({n} 行)")
        return 1 if missing or unlisted else 0
    if missing:
        return 1

    # 先完成所有转换，确认成功后再写入，避免转换失败时留下半套章节。
    slugs = {
        "图论": "01-graph", "数据结构": "02-datastructure", "字符串": "03-string",
        "数学": "04-math", "计算几何": "05-geometry", "杂项": "06-misc",
    }
    outputs, empty = [], []
    try:
        for title, folder, entries in CHAPTERS:
            body, _, skipped = emit_chapter(title, folder, entries)
            body += emit_conclusions(title, CONCLUSION_SOURCES.get(title, ""))
            for entry in FULL_TEXT_SOURCES:
                if entry["chapter"] == title:
                    body += emit_full_text(entry)
            outputs.append((slugs[title] + ".tex", title, "\n".join(body).rstrip() + "\n"))
            empty.extend((title, rel) for rel in skipped)
    except (OSError, RuntimeError) as exc:
        print(f"[生成失败] {exc}", file=sys.stderr)
        return 1

    OUTDIR.mkdir(parents=True, exist_ok=True)
    for filename, title, text in outputs:
        (OUTDIR / filename).write_text(text, encoding="utf-8", newline="\n")
        print(f"{filename:<26}{text.count(chr(10)):>6} 行   {title}")
    if empty:
        print("\n[空文件] 已跳过：")
        for title, rel in empty:
            print(f"    {title}: {rel}")
    if unlisted:
        print("\n[提示] 以下源文件未归入打印版：")
        for rel, n in unlisted:
            print(f"    {rel}   ({n} 行)")
        print("    数学文件请更新 docs/math-print-manifest.json，其他源码请更新章节配置。")
    return 0


if __name__ == "__main__":
    setup_console()
    sys.exit(main())

#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
从仓库里的真实源码生成打印版章节 print/chapters/*.tex。

⚠️ 这是生成器，不是一次性脚本。但注意：生成物会覆盖 print/chapters/ 下的同名文件，
   如果你已经在 .tex 里手工改过东西，跑这个脚本会丢掉那些改动。
   仓库当前的做法是「.tex 为打印版正本」，所以正常情况下不要再跑它。

用法：
    python tools/gen_print.py            # 生成全部章节
    python tools/gen_print.py --check    # 只检查：列出没有归入打印版的源文件
"""

import argparse
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from md2tex import render_prose  # 复用 md -> LaTeX 的行内转换

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
    ("线段树合并", "SegmemtTree/Segment_tree_merge.cpp"),
    ("基础字典树", "Trie/Trie.cpp"),
    ("01 字典树", "Trie/01-Trie.cpp"),
    ("可持久化 01 字典树", "Trie/Persistent_Trie.cpp"),
    ("Treap", "Treap/treap.cpp"),
    ("隐式 Treap（FHQ-Treap）", "Treap/Implicit_treap.cpp"),
    ("树链剖分", "Heavy-Light Decomposition/Heavy_light_decomposition.cpp"),
    ("稀疏表", "Sptable/sptable.hpp"),
    ("莫队（含回滚莫队思路）", "Mo's algorithm/Mo_with_rollback.cpp"),
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

CH_MATH = ("数学", "Math", [
    ("组合数（阶乘预处理）", "Combinatorics/base.hpp"),
    ("ModInt 模运算类", "NumberTheory/modint.hpp"),
    ("exGCD 扩展欧几里得", "NumberTheory/exgcd.hpp"),
    ("exCRT 扩展中国剩余定理", "NumberTheory/excrt.hpp"),
    ("线性筛", "NumberTheory/sieve.hpp"),
    ("扩展线性筛（积性函数）", "NumberTheory/sieve_ext.hpp"),
    ("分段筛", "NumberTheory/sieve_seg.hpp"),
    ("Miller-Rabin 素性测试", "NumberTheory/primetest.hpp"),
    ("Pollard-rho 质因数分解", "NumberTheory/PollardRho.cpp"),
    ("BSGS 离散对数", "NumberTheory/BSGS.cpp"),
    ("杜教筛", "NumberTheory/dujiao_sieve.hpp"),
    ("FFT", "Polynomial/FFT.cpp"),
    ("NTT", "Polynomial/NTT.cpp"),
    ("三模 NTT", "Polynomial/NTT_3mod.cpp"),
    ("MTT（任意模数）", "Polynomial/MTT.cpp"),
    ("FWT / FMT", "Polynomial/FWT.cpp"),
    ("多项式求导积分", "Polynomial/Derivative.cpp"),
    ("多项式求逆", "Polynomial/Inverse.cpp"),
    ("多项式除法", "Polynomial/Division.cpp"),
    ("多项式开根", "Polynomial/Sqrt.cpp"),
    ("多项式指数 / 对数", "Polynomial/ExpLog.cpp"),
    ("多项式快速幂", "Polynomial/Pow.cpp"),
    ("多点求值", "Polynomial/MultiEval.cpp"),
    ("快速插值", "Polynomial/Interpolation.cpp"),
    ("快速阶乘", "Polynomial/FastFactorial.cpp"),
    ("多点快速阶乘", "Polynomial/FastFactorialMulti.cpp"),
    ("矩阵与列向量", "Templates/Matrix.hpp"),
    ("高斯消元（整数）", "LinearAlgebra/GaussianElim_int.cpp"),
    ("高斯消元（实数）", "LinearAlgebra/GaussianElim_real.cpp"),
    ("高斯消元（异或）", "LinearAlgebra/GaussianElim_xor.cpp"),
    ("异或线性基", "LinearAlgebra/XORBasis.cpp"),
])

CH_CG = ("计算几何", "ComputationalGeometry", [
    ("二维几何基础库（base.hpp）", "base.hpp"),
    ("极角排序", "polar_sort.hpp"),
    ("平面几何杂项（sgn / cmp / 读入）", "Geometry_util.cpp"),
    ("平面点线运算", "Point_line_ops.cpp"),
    ("平面直线方程转换", "Line_equation.cpp"),
    ("平面三角形相关（四心）", "Triangle_center.cpp"),
    ("平面圆相关（交点 / 切线 / 相交面积）", "Circle_ops.cpp"),
    ("平面多边形补充（Pick 定理 / 网格点）", "Polygon_extra.cpp"),
    ("二维凸包（Andrew）", "Convex_hull.cpp"),
    ("二维动态凸包", "Dynamic_convex_hull.cpp"),
    ("旋转卡壳", "Rotating_calipers.cpp"),
    ("平面最近点对", "Closest_pair.cpp"),
    ("闵可夫斯基和", "Minkowski_sum.cpp"),
    ("半平面交", "Half_plane_intersection.cpp"),
    ("三维几何：点线面封装与基础判定", "Geometry_3D_base.cpp"),
    ("三维几何：相交判定与交点", "Geometry_3D_intersect.cpp"),
    ("三维几何：距离、夹角与体积", "Geometry_3D_distance.cpp"),
])

CH_MISC = ("杂项", ".", [
    ("离散化", "Miscellaneous/Discretization.cpp"),
    ("欧拉序（子树转区间）", "Miscellaneous/GenerateEulerTourOrder.cpp"),
    ("反转整数", "Miscellaneous/ReverseInt.cpp"),
    ("防卡哈希表", "Miscellaneous/hashtable.hpp"),
])

CHAPTERS = [CH_GRAPH, CH_DS, CH_STRING, CH_MATH, CH_CG, CH_MISC]

# 每个章节末尾追加的「结论与技巧」，从对应 .md 里按标题抓
CONCLUSION_SOURCES = {
    "图论": "GraphTheory/Graph.md",
    "数据结构": "DataStructure/DataStructure.md",
    "字符串": "String/String.md",
    "数学": "Math/Math.md",
    # 几何的结论已单独整理到 Conclusion.md（CG.md 是旧稿）
    "计算几何": "ComputationalGeometry/Conclusion.md",
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
        out.append("")
        out.append("\\begin{lstlisting}")
        out.extend(code.split("\n"))
        out.append("\\end{lstlisting}")
        out.append("")
    return out, missing, empty


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--check", action="store_true",
                    help="只列出未归入打印版的源文件，不写文件")
    args = ap.parse_args()

    # 记录配置里列到的绝对路径，用于 --check
    listed = set()
    for title, folder, entries in CHAPTERS:
        for _, rel in entries:
            listed.add(((ROOT / folder / rel) if folder != "." else (ROOT / rel)).resolve())

    if args.check:
        print("=== 未归入打印版的源文件 ===")
        for p in sorted(ROOT.rglob("*")):
            if not p.is_file() or p.suffix not in {".cpp", ".hpp", ".h"}:
                continue
            rp = p.relative_to(ROOT).as_posix()
            if rp.startswith(("print/", "tools/")):
                continue
            if p.resolve() not in listed:
                n = len(p.read_text(encoding="utf-8", errors="replace").splitlines())
                print(f"    {rp}   ({n} 行)")
        return

    OUTDIR.mkdir(parents=True, exist_ok=True)
    total_missing, total_empty = [], []
    for title, folder, entries in CHAPTERS:
        body, missing, empty = emit_chapter(title, folder, entries)
        body += emit_conclusions(title, CONCLUSION_SOURCES.get(title, ""))
        slug = {
            "图论": "01-graph", "数据结构": "02-datastructure", "字符串": "03-string",
            "数学": "04-math", "计算几何": "05-geometry", "杂项": "06-misc",
        }[title]
        text = "\n".join(body).rstrip() + "\n"
        (OUTDIR / (slug + ".tex")).write_text(text, encoding="utf-8")
        n = text.count("\n")
        print(f"{slug+'.tex':<26}{n:>6} 行   {title}")
        total_missing += [(title, m) for m in missing]
        total_empty += [(title, m) for m in empty]

    if total_missing:
        print("\n[缺失] 配置里列了但文件不存在：")
        for t, m in total_missing:
            print(f"    {t}: {m}")
    if total_empty:
        print("\n[空文件] 已跳过：")
        for t, m in total_empty:
            print(f"    {t}: {m}")


if __name__ == "__main__":
    main()

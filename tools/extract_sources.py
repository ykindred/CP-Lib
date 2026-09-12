#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
一次性脚本：把 GraphTheory/Graph.md 与 String/String.md 里的代码块
拆成一个个独立的模板源文件。

⚠️ 一次性工具。跑完之后 GraphTheory/*.cpp、String/*.cpp 就是这些模板的
   正本，.md 退化为说明文档。不要再重复运行（会覆盖手工修改）。

规则：
  * 代码块归属于它前面最近的那个标题。
  * 同一标题下的多个代码块：第一个是「实现」，其余是「使用示例」，
    合并进同一个文件，中间用注释分隔，冗余的 #include/using 去掉，
    合并结果是一个自包含可编译的程序。
  * 标题没有代码块的（如「# 树」这种纯分组标题）不产生文件。
"""

import re
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent

# 标题 -> 文件名（不含扩展名）。按 .md 中出现顺序。
GRAPH_FILES = {
    "拓扑排序": "TopoSort",
    "树的直径": "TreeDiameter",
    "树的重心": "TreeCentroid",
    "倍增LCA": "BinaryLCA",
    "树上启发式合并 (DSU on Tree)": "DSU_on_Tree",
    "SCC (强连通分量)": "TarjanSCC",
    "割边 (Bridges)": "Bridges",
    "割点 (Cut Points)": "CutPoints",
    "最大流/最小割 (Dinic)": "Dinic",
    "1.1 Dijkstra（堆优化，非负权）": "Dijkstra",
    "1.2 SPFA（可处理负权边）": "SPFA",
    "1.3 Floyd（多源最短路）": "Floyd",
    "二、分层图最短路": "LayeredGraph",
    "3.1 SPFA 判负环（检测全图，P3385 风格）": "NegativeCycleSPFA",
    "3.2 Bellman-Ford 判负环": "NegativeCycleBellmanFord",
    "4.1 染色法判定二分图": "BipartiteColoring",
    "4.2 匈牙利算法（最大匹配）": "Hungarian",
    "4.3 KM 算法（最大权完美匹配，O(n^3) BFS 版）": "KM",
    "5.1 找环（拓扑排序剪叶法）": "BaseRingTreeFindCycle",
    "5.2 基环树最大独立集（点权版，经典题：骑士）": "BaseRingTreeMaxIndependentSet",
}

STRING_FILES = {
    "1. 字符串哈希": "StringHash",
    "2.1 前缀函数": "PrefixFunction",
    # 2.2/2.3 是 KMP 的配套片段（匹配、周期），并进同一个文件
    "2.2 匹配": "PrefixFunction",
    "2.3 周期相关": "PrefixFunction",
    "3. Z 函数（扩展 KMP）": "ZFunction",
    "4. Manacher": "Manacher",
    "5. Trie": "Trie",
    "6. AC 自动机": "ACAutomaton",
    "7. 后缀数组 SA": "SuffixArray",
    "7.1 ST 表：任意两后缀 LCP": "SuffixArrayLCP",
    "8. 后缀自动机 SAM": "SuffixAutomaton",
    "8.1 广义 SAM（多串）": "GeneralizedSAM",
    "9. 回文自动机 PAM": "PalindromicTree",
    "10. 最小表示法": "MinimalRepresentation",
    "11. Lyndon 分解（Duval）": "Lyndon",
}

USAGE_MARK = "// ================= 使用示例 ================="
CONTINUE_MARK = "// ================= 续 ================="


def parse_blocks(md_path):
    """返回 [(heading_text, [block, ...]), ...]，保持出现顺序。"""
    lines = Path(md_path).read_text(encoding="utf-8").splitlines()
    sections = []          # [(title, [blocks])]
    index = {}             # title -> 在 sections 中的下标
    cur = None
    i = 0
    while i < len(lines):
        line = lines[i]
        m = re.match(r"^#{1,6}\s+(.*)$", line)
        if m:
            title = m.group(1).strip()
            if title in index:
                cur = index[title]
            else:
                sections.append([title, []])
                index[title] = len(sections) - 1
                cur = len(sections) - 1
            i += 1
            continue
        if line.startswith("```"):
            j = i + 1
            body = []
            while j < len(lines) and not lines[j].startswith("```"):
                body.append(lines[j])
                j += 1
            if cur is not None:
                sections[cur][1].append(body)
            i = j + 1
            continue
        i += 1
    return [s for s in sections if s[1]]


def strip_std_boilerplate(block, impl=None):
    """去掉使用示例里重复的 #include / using namespace std; / 已声明过的 using 别名，
    否则合并后会出现重复的 `using ll = long long;`。"""
    seen_using = set()
    for line in (impl or []):
        s = line.strip()
        if s.startswith("using ") and s.endswith(";"):
            seen_using.add(s)
    out = []
    for line in block:
        s = line.strip()
        if s.startswith("#include") or s == "using namespace std;":
            continue
        if s.startswith("using ") and s.endswith(";") and s in seen_using:
            continue
        out.append(line)
    while out and not out[0].strip():
        out.pop(0)
    while out and not out[-1].strip():
        out.pop()
    return out


def write_template(section, out_dir, filename, written):
    """写模板文件；同一文件名第二次出现时改为追加（用于把多个小节并进一个文件）。"""
    title, blocks = section
    path = out_dir / (filename + ".cpp")

    if filename in written:
        prev = path.read_text(encoding="utf-8").rstrip("\n")
        add = ["", CONTINUE_MARK, f"// 来自小节：{title}"]
        for b in blocks:
            add.append("")
            add.extend(b)
        text = prev + "\n" + "\n".join(add).rstrip() + "\n"
        path.write_text(text, encoding="utf-8")
        return path, len(text.splitlines())

    impl = blocks[0]
    usages = blocks[1:]

    body = list(impl)
    for u in usages:
        body.append("")
        body.append(USAGE_MARK)
        body.extend(strip_std_boilerplate(u, impl))

    while body and not body[-1].strip():
        body.pop()

    text = "\n".join(body).rstrip() + "\n"
    path.write_text(text, encoding="utf-8")
    return path, len(text.splitlines())


def main():
    total = 0
    for md, mapping, out_dir in [
        ("GraphTheory/Graph.md", GRAPH_FILES, ROOT / "GraphTheory"),
        ("String/String.md", STRING_FILES, ROOT / "String"),
    ]:
        out_dir.mkdir(exist_ok=True)
        sections = parse_blocks(ROOT / md)
        print(f"===== {md} -> {out_dir.name}/ =====")
        made_titles = set()
        written_files = set()      # 已写过的「文件名」，用于判断是否改成追加
        for title, blocks in sections:
            name = mapping.get(title)
            if not name:
                print(f"  [跳过] 标题无映射：{title}")
                continue
            path, n = write_template((title, blocks), out_dir, name, written_files)
            made_titles.add(title)
            written_files.add(name)
            total += 1
            print(f"  {path.name:<44}{n:>4} 行   ({len(blocks)} 块)")
        missing = set(mapping) - made_titles
        for t in missing:
            print(f"  [警告] 映射里的标题在 .md 中没找到：{t}")
    print(f"\n共生成 {total} 个源文件")


if __name__ == "__main__":
    main()

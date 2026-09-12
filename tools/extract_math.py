#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
一次性脚本：把 Math.md 里「尚无对应源文件」的模板抽成 .cpp。

Math.md 有 20 多个专题，但 Math/ 下只有 10 个源文件。这个脚本会：
  1. 解析 Math.md 的每个小节及其代码块；
  2. 把 Math/ 下现有源文件全文规范化（去空白）后拼在一起，作为「已覆盖」集合；
  3. 小节的首个代码块若已能在现有源文件中找到 → 跳过；
  4. 找不到 → 按映射表写出新文件。

⚠️ 一次性工具，跑完 Math/*.cpp 即为正本，不要重复运行。
"""

import re
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
MATH_MD = ROOT / "Math" / "Math.md"
MATH_DIR = ROOT / "Math"

# 小节标题（去层级） -> 目标文件（相对 Math/，不含扩展名）
TARGETS = {
    "BSGS": "NumberTheory/BSGS",
    "FFT": "Polynomial/FFT",
    "NTT": "Polynomial/NTT",
    "三模NTT": "Polynomial/NTT_3mod",
    "MTT": "Polynomial/MTT",
    "FWT/FMT": "Polynomial/FWT",
    "求导积分": "Polynomial/Derivative",
    "求逆": "Polynomial/Inverse",
    "除法": "Polynomial/Division",
    "开根": "Polynomial/Sqrt",
    "指数/对数": "Polynomial/ExpLog",
    "幂": "Polynomial/Pow",
    "多点求值": "Polynomial/MultiEval",
    "快速插值": "Polynomial/Interpolation",
    "快速阶乘": "Polynomial/FastFactorial",
    "多点快速阶乘": "Polynomial/FastFactorialMulti",
    "高斯消元(整数)": "LinearAlgebra/GaussianElim_int",
    "高斯消元(实数)": "LinearAlgebra/GaussianElim_real",
    "高斯消元(异或)": "LinearAlgebra/GaussianElim_xor",
    "异或线性基": "LinearAlgebra/XORBasis",
    "基础": "Combinatorics/base",
    "模运算类": "NumberTheory/modint",
    "exCRT": "NumberTheory/excrt",
    "exGCD": "NumberTheory/exgcd",
    "线性筛": "NumberTheory/sieve",
    "分段筛": "NumberTheory/sieve_seg",
    "Miller-Rabin": "NumberTheory/primetest",
    # primetest.hpp 只实现了 Miller-Rabin，没有 rho/factorize，所以单列一个目标
    "Pollard-rho": "NumberTheory/PollardRho",
    "杜教筛": "NumberTheory/dujiao_sieve",
    "矩阵和列向量": "Templates/Matrix",
}

USAGE_MARK = "// ================= 使用示例 ================="


def parse_sections(md_path):
    lines = Path(md_path).read_text(encoding="utf-8").splitlines()
    sections, index, cur = [], {}, None
    i = 0
    while i < len(lines):
        m = re.match(r"^#{1,6}\s+(.*)$", lines[i])
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
        if lines[i].startswith("```"):
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


def main():
    """判定「已覆盖」的依据：映射到的目标 .hpp 是否已存在。

    试过两种更「聪明」的办法，都不靠谱：
      * 整段文本比对 —— .md 与 .hpp 常是同一算法的不同写法（exgcd 的
        ll 版与 i128 版），文本对不上，会把已有的重复抽一遍；
      * 比对声明的符号名 —— 正则会把函数「调用」当成「声明」，例如
        BSGS 里调用了 exgcd/mul，就被误判成已有实现而漏抽。
    目标文件是否存在，简单且可预测。
    """
    sections = parse_sections(MATH_MD)

    covered, created, skipped, unknown = [], [], [], []
    for title, blocks in sections:
        target = TARGETS.get(title)
        if not target:
            unknown.append(title)
            continue
        if (MATH_DIR / (target + ".hpp")).exists():
            covered.append((title, target))
            continue
        # 未覆盖：写出
        body = list(blocks[0])
        for u in blocks[1:]:
            body.append("")
            body.append(USAGE_MARK)
            body.extend(u)
        while body and not body[-1].strip():
            body.pop()
        path = MATH_DIR / (target + ".cpp")
        path.parent.mkdir(parents=True, exist_ok=True)
        if path.exists():
            skipped.append((title, path.relative_to(ROOT)))
            continue
        path.write_text("\n".join(body).rstrip() + "\n", encoding="utf-8")
        created.append((title, path.relative_to(ROOT), len(body)))

    print(f"已有源文件覆盖，跳过（{len(covered)}）:")
    for t, p in covered:
        print(f"    {t:<22} <- Math/{p}.hpp 已存在")
    print(f"\n新抽出（{len(created)}）:")
    for t, p, n in created:
        print(f"    {t:<22} -> {p}   {n} 行")
    if skipped:
        print(f"\n目标文件已存在，未覆盖（{len(skipped)}）:")
        for t, p in skipped:
            print(f"    {t:<22} -> {p}")
    if unknown:
        print(f"\n映射表里没有的小节（{len(unknown)}）:")
        for t in unknown:
            print(f"    {t}")


if __name__ == "__main__":
    main()

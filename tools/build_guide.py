#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
把 docs/editing-guide.md 渲染成 docs/CP-Lib-Guide.pdf。

用法：
    python tools/build_guide.py

依赖：pandoc（任意较新版本）+ TeX Live（XeLaTeX）。
Markdown 是这份指南的唯一正本；改完 .md 重新运行本脚本即可。
"""

import os
import shutil
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent


def setup_console():
    """Windows 控制台可能是 GBK；统一成 UTF-8，避免中文输出乱码。"""
    if sys.platform.startswith("win"):
        os.system("chcp 65001 >nul")
    for stream in (sys.stdout, sys.stderr):
        try:
            stream.reconfigure(encoding="utf-8", errors="replace")
        except (AttributeError, ValueError, OSError):
            pass
MD = ROOT / "docs" / "editing-guide.md"
PDF = ROOT / "docs" / "CP-Lib-Guide.pdf"
HEADER = ROOT / "docs" / "guide-header.tex"
COVER = ROOT / "docs" / "guide-cover.tex"


def main():
    setup_console()

    pandoc = shutil.which("pandoc")
    if not pandoc:
        print("找不到 pandoc。请先安装 pandoc（https://pandoc.org/），"
              "并确保它在 PATH 中。", file=sys.stderr)
        return 127
    if not shutil.which("xelatex"):
        print("找不到 xelatex。请安装 TeX Live，并把它的 bin 目录加入 PATH。",
              file=sys.stderr)
        return 127

    for path in (MD, HEADER, COVER):
        if not path.exists():
            print(f"缺少文件：{path.relative_to(ROOT)}", file=sys.stderr)
            return 1

    cmd = [
        pandoc, str(MD),
        "-o", str(PDF),
        "--pdf-engine=xelatex",
        "--standalone",
        "--number-sections",
        "--toc",
        "--toc-depth=2",
        "-V", "documentclass=ctexart",
        "-V", "fontsize=11pt",
        "-V", "papersize=a4",
        "-V", "geometry:top=16mm",
        "-V", "geometry:bottom=15mm",
        "-V", "geometry:left=16mm",
        "-V", "geometry:right=16mm",
        "-V", "geometry:headheight=15pt",
        "-V", "geometry:headsep=4mm",
        "-V", "toc-title=目录",
        "--syntax-highlighting=idiomatic",
        "-H", str(HEADER),
        "--include-before-body", str(COVER),
    ]

    print("[1/1] 用 pandoc + XeLaTeX 渲染指南 ...", flush=True)
    print("$ " + " ".join(cmd), flush=True)
    rc = subprocess.run(cmd, cwd=ROOT).returncode
    if rc != 0:
        print("\n渲染失败。pandoc 会把 LaTeX 的原始报错留在上面；"
              "也可以先单独渲染成 .tex 检查：", file=sys.stderr)
        print(f"  pandoc {MD.relative_to(ROOT)} -o /tmp/guide.tex --standalone "
              f"-H {HEADER.relative_to(ROOT)}", file=sys.stderr)
        return rc

    if not PDF.exists():
        print("pandoc 返回成功，但没有生成 docs/CP-Lib-Guide.pdf", file=sys.stderr)
        return 1

    size_kb = PDF.stat().st_size / 1024
    print(f"\n完成：{PDF.relative_to(ROOT)}（{size_kb:.0f} KB）")
    return 0


if __name__ == "__main__":
    sys.exit(main())

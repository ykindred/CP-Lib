#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
一键生成打印版 PDF：重新生成 print/chapters/*.tex，然后调用 XeLaTeX 编译。

用法（在仓库任意位置执行）：
    python tools/build_print.py            # 改完模板源码后：重新生成章节 + 编译 PDF
    python tools/build_print.py --no-gen   # 只改了 print/ 下的排版时：直接用现有章节编译
    python tools/build_print.py --check    # 只检查哪些源码还没归入打印版
    python tools/build_print.py --clean    # 清理所有编译中间文件（保留 PDF）
    python tools/build_print.py --open     # 编译完成后自动打开 PDF

日常修改流程：
    1. 改模板源码（GraphTheory/*.cpp、DataStructure/...、Math/... 等）。
       —— 这些源文件是唯一正本，print/chapters/*.tex 是生成物，不要手改。
    2. 新增模板时，把 (标题, 相对路径) 加到 tools/gen_print.py 顶部的章节配置。
    3. 运行本脚本，产物是 print/CP-Lib.pdf。

需要：Python 3.8+、TeX Live（latexmk + XeLaTeX）。
"""

import argparse
import os
import shutil
import subprocess
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
GEN = ROOT / "tools" / "gen_print.py"
PDF = ROOT / "print" / "CP-Lib.pdf"

# latexmk / xelatex 可能留下的中间文件；这些都可以安全删除
INTERMEDIATE_PATTERNS = (
    "*.aux", "*.log", "*.out", "*.toc", "*.lof", "*.lot",
    "*.fls", "*.fdb_latexmk", "*.synctex.gz", "*.synctex(busy)",
    "*.xdv", "*.bbl", "*.blg", "*.run.xml", "*.nav", "*.snm", "*.vrb",
)

_UTF8_CONSOLE = False


def setup_console():
    """Windows 控制台默认可能是 GBK；统一成 UTF-8，避免中文输出乱码。"""
    global _UTF8_CONSOLE
    ok = True
    if sys.platform.startswith("win"):
        # chcp 只在当前控制台有效；失败也不影响脚本逻辑
        ok = os.system("chcp 65001 >nul") == 0
    for stream in (sys.stdout, sys.stderr):
        try:
            stream.reconfigure(encoding="utf-8", errors="replace")
            _UTF8_CONSOLE = True
        except (AttributeError, ValueError, OSError):
            _UTF8_CONSOLE = _UTF8_CONSOLE or ok


def run(cmd):
    """在当前仓库根目录执行命令，返回退出码。输出直接透传给终端。"""
    print("$ " + " ".join(str(x) for x in cmd), flush=True)
    env = os.environ.copy()
    env["PYTHONDONTWRITEBYTECODE"] = "1"
    if _UTF8_CONSOLE:
        # 让作为子进程的 gen_print.py 也用 UTF-8 输出
        env["PYTHONIOENCODING"] = "utf-8"
    try:
        return subprocess.run([str(x) for x in cmd], cwd=ROOT, env=env).returncode
    except FileNotFoundError:
        print(f"找不到命令：{cmd[0]}", file=sys.stderr)
        return 127


def clean_intermediates():
    """删除 print/、docs/ 下的编译中间文件和 Python 缓存，保留所有 PDF。"""
    removed = []

    for directory in (ROOT / "print", ROOT / "docs"):
        if not directory.is_dir():
            continue
        for pattern in INTERMEDIATE_PATTERNS:
            for path in sorted(directory.glob(pattern)):
                try:
                    path.unlink()
                except OSError as exc:
                    print(f"无法删除 {path.relative_to(ROOT)}：{exc}", file=sys.stderr)
                else:
                    removed.append(path.relative_to(ROOT).as_posix())

    # 直接跑 xelatex main.tex 时留下的“另一个 PDF”，不是正式产物
    stray_main_pdf = ROOT / "print" / "main.pdf"
    if stray_main_pdf.exists():
        try:
            stray_main_pdf.unlink()
            removed.append(stray_main_pdf.relative_to(ROOT).as_posix())
        except OSError as exc:
            print(f"无法删除 {stray_main_pdf.relative_to(ROOT)}：{exc}", file=sys.stderr)

    for cache in (ROOT / "tools" / "__pycache__",
                  ROOT / "__pycache__",
                  ROOT / "docs" / "__pycache__"):
        if cache.is_dir():
            shutil.rmtree(cache, ignore_errors=True)
            removed.append(cache.relative_to(ROOT).as_posix() + "/")

    return removed


def pdf_pages(pdfinfo):
    if not pdfinfo:
        return ""
    try:
        # pdfinfo 会把 PDF 元数据原样输出，编码不一定是控制台的编码；
        # 这里只解析 ASCII 的 "Pages:" 行，其余用 replace 兜底。
        out = subprocess.run(
            [pdfinfo, str(PDF)], encoding="utf-8", errors="replace",
            stdout=subprocess.PIPE, stderr=subprocess.DEVNULL,
        ).stdout
    except OSError:
        return ""
    for line in out.splitlines():
        if line.startswith("Pages:"):
            return line.split(":", 1)[1].strip()
    return ""


def open_pdf():
    try:
        if sys.platform.startswith("win"):
            os.startfile(PDF)  # noqa: 仅 Windows
        elif sys.platform == "darwin":
            subprocess.run(["open", str(PDF)], check=False)
        else:
            subprocess.run(["xdg-open", str(PDF)], check=False)
    except OSError as exc:
        print(f"无法自动打开 PDF：{exc}", file=sys.stderr)


def main():
    setup_console()

    ap = argparse.ArgumentParser(
        description="重新生成打印版章节并编译 print/CP-Lib.pdf")
    ap.add_argument("-n", "--no-gen", "--no_gen", action="store_true",
                    help="不运行 gen_print.py，直接用现有 print/chapters/*.tex 编译")
    ap.add_argument("-c", "--check", action="store_true",
                    help="只列出未归入打印版的源文件；有遗漏时返回非 0")
    ap.add_argument("--clean", action="store_true",
                    help="清理所有编译中间文件（保留 PDF）")
    ap.add_argument("-o", "--open", action="store_true",
                    help="编译完成后自动打开 PDF")
    args = ap.parse_args()

    if args.check:
        return run([sys.executable, GEN, "--check"])

    if args.clean:
        removed = clean_intermediates()
        if removed:
            print("已清理：", flush=True)
            for item in removed:
                print("    " + item)
        else:
            print("没有需要清理的中间文件。", flush=True)
        if PDF.exists():
            print(f"保留：{PDF.relative_to(ROOT)}")
        return 0

    latexmk = shutil.which("latexmk")
    if not latexmk:
        print("找不到 latexmk。请安装 TeX Live，并把它的 bin 目录加入 PATH。",
              file=sys.stderr)
        return 127

    started = time.time()

    # 1. 从源码重新生成各章 .tex
    if not args.no_gen:
        print("[1/2] 从源文件生成 print/chapters/*.tex ...", flush=True)
        rc = run([sys.executable, GEN])
        if rc != 0:
            print("生成章节失败，已停止。", file=sys.stderr)
            return rc
    else:
        print("[1/2] 跳过生成，使用现有 print/chapters/*.tex", flush=True)

    # 2. 编译 PDF（latexmk 会按需跑多趟，解决目录/页码/引用）
    print("[2/2] 编译 PDF ...", flush=True)
    rc = run([latexmk, "print/main.tex"])
    if rc != 0:
        print("\n编译失败。完整日志见 print/CP-Lib.log", file=sys.stderr)
        if sys.platform.startswith("win"):
            print("如果 CP-Lib.pdf 正在阅读器中打开，Windows 可能锁定文件；"
                  "请关闭后重试。", file=sys.stderr)
        return rc

    if not PDF.exists():
        print("latexmk 返回成功，但 print/CP-Lib.pdf 不存在，请检查日志。",
              file=sys.stderr)
        return 1

    pages = pdf_pages(shutil.which("pdfinfo"))
    size_mb = PDF.stat().st_size / 1024 / 1024
    detail = f"{pages} 页, " if pages else ""
    print(f"\n完成：print/CP-Lib.pdf（{detail}{size_mb:.1f} MB，"
          f"用时 {time.time() - started:.0f} 秒）", flush=True)

    if args.open:
        open_pdf()
    return 0


if __name__ == "__main__":
    sys.exit(main())

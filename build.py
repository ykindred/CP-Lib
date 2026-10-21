#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
快捷入口：python build.py [参数] 等价于 python tools/build_print.py [参数]。

保留这个文件是为了让根目录下的 `python build.py --no-gen` 之类的命令也能直接用；
真正的实现维护在 tools/build_print.py。
"""

import runpy
import sys
from pathlib import Path

TARGET = Path(__file__).resolve().parent / "tools" / "build_print.py"
sys.argv[0] = str(TARGET)
runpy.run_path(str(TARGET), run_name="__main__")

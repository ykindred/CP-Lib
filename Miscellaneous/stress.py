#!/usr/bin/env python3
# python3 stress.py          : std vs brute（不限制时间）
# python3 stress.py time 2   : 只测 std，2 秒超时
import subprocess as sp
import sys
import time
from pathlib import Path

root = Path(__file__).resolve().parent
if len(sys.argv) == 1:
    limit = None
elif len(sys.argv) == 3 and sys.argv[1] == 'time':
    try:
        limit = float(sys.argv[2])
        assert limit > 0
    except (ValueError, AssertionError):
        sys.exit('用法: python3 stress.py [time 秒数]')
else:
    sys.exit('用法: python3 stress.py [time 秒数]')

for name in (('gen', 'std') if limit else ('gen', 'std', 'brute')):
    if sp.run(['g++', f'{name}.cpp', '-std=c++17', '-O2', '-o', name], cwd=root).returncode:
        sys.exit(1)

mx = 0.0
try:
    for i in range(1, 10**18):
        gen = sp.run(['./gen'], cwd=root, stdout=sp.PIPE)
        if gen.returncode:
            sys.exit(f'gen RE #{i}')
        data = gen.stdout
        if limit:
            t = time.perf_counter()
        try:
            a = sp.run(['./std'], cwd=root, input=data,
                       stdout=sp.DEVNULL if limit else sp.PIPE, timeout=limit)
        except sp.TimeoutExpired:
            (root / 'failed.in').write_bytes(data)
            sys.exit(f'TLE #{i} (> {limit}s), see failed.in')
        if limit:
            mx = max(mx, time.perf_counter() - t)
        if a.returncode:
            (root / 'failed.in').write_bytes(data)
            sys.exit(f'std RE #{i}, see failed.in')
        if not limit:
            b = sp.run(['./brute'], cwd=root, input=data, stdout=sp.PIPE)
            if b.returncode or a.stdout.split() != b.stdout.split():
                (root / 'failed.in').write_bytes(data)
                (root / 'std.out').write_bytes(a.stdout)
                (root / 'brute.out').write_bytes(b.stdout)
                sys.exit(f'WA/RE #{i}, see failed.in, std.out, brute.out')
        if i % 100 == 0:
            print(f'{i} OK' + (f', max {mx:.3f}s' if limit else ''), flush=True)
except KeyboardInterrupt:
    print('\nStopped')

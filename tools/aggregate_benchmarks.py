#!/usr/bin/env python3
import os
import json
import subprocess

def find_csvs(root='benchmarks'):
    out = []
    if not os.path.isdir(root):
        return out
    for fn in os.listdir(root):
        if fn.endswith('.csv'):
            out.append(os.path.join(root, fn))
    return out

def main():
    csvs = find_csvs()
    summary = {}
    for c in csvs:
        res = subprocess.run(['python3','tools/bench_stats.py',c], capture_output=True, text=True)
        try:
            j = json.loads(res.stdout)
            summary[c] = j
        except Exception as e:
            print('failed to parse', c, e)

    with open('benchmarks/summary.json','w') as f:
        json.dump(summary, f, indent=2)
    print('Wrote benchmarks/summary.json')

if __name__ == '__main__':
    main()

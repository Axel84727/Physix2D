#!/usr/bin/env python3
import sys
import csv
import json
import statistics

def analyze(path):
    frames = []
    total = []
    broad = []
    narrow = []
    resolve = []
    with open(path, newline='') as csvf:
        r = csv.DictReader(csvf)
        for row in r:
            total.append(float(row.get('total_us', 0)))
            broad.append(float(row.get('broad_us', 0)))
            narrow.append(float(row.get('narrow_us', 0)))
            resolve.append(float(row.get('resolve_us', 0)))
            frames.append(int(row.get('frame', 0)))

    def stats(a):
        if not a: return {'mean':0,'std':0}
        return {'mean': statistics.mean(a), 'std': statistics.pstdev(a)}

    out = {
        'file': path,
        'frames': len(frames),
        'total': stats(total),
        'broad': stats(broad),
        'narrow': stats(narrow),
        'resolve': stats(resolve)
    }
    print(json.dumps(out, indent=2))

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print('usage: bench_stats.py <csv>')
        sys.exit(1)
    analyze(sys.argv[1])

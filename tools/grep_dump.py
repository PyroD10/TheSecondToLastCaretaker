#!/usr/bin/env python3
"""Search the UE4SS object dump for a pattern. Usage: python tools/grep_dump.py Character"""
import re, sys, pathlib
dump = pathlib.Path(__file__).resolve().parent.parent / "dumps" / "UE4SS_ObjectDump.txt"
if not dump.exists():
    sys.exit(f"no dump at {dump} — see docs/04-dev-setup.md §3")
pat = re.compile(sys.argv[1], re.I) if len(sys.argv) > 1 else sys.exit("usage: grep_dump.py <regex>")
seen = set()
with dump.open(encoding="utf-8", errors="replace") as f:
    for line in f:
        if pat.search(line):
            key = line.strip()
            if key not in seen:
                seen.add(key); print(key)
print(f"-- {len(seen)} unique matches", file=sys.stderr)

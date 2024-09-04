#!/bin/python3

with open('patts_rm.txt', 'r') as f:
    lines = f.readlines()

# sort the lines alphabetically
lines.sort()

with open('patts_sorted.txt', 'w') as f:
    f.writelines(lines)

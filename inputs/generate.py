#!/bin/python3.6
import math
import random
import sys

def generate(name, nb):
    f = open(name, "w+")
    for i in range(0, nb):
        x = random.uniform(-179.0, 179.0)
        y = random.uniform(-179.0, 179.0)
        id = str(i)
        f.write("%s %f %f\n" % (id, x, y))
    f.close()

if len(sys.argv) != 3:
    print("usage: ./generate.py <filename> <nb_pts>")
else:
    generate(sys.argv[1], int(sys.argv[2]))
    


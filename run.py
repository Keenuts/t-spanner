#!/bin/python3.6

import tspanner
import networkx as nx
import numpy as np
import matplotlib.pyplot as plt



# Output to a file
# tspanner.compute_greedy(1.0, "inputs/complet.graph", "output.dot")

# Get graph in python
# graph = tspanner.compute_greedy(1.0, "inputs/complet.graph")
graph = tspanner.compute_wspd(2.0, "inputs/complet.graph")

G = nx.DiGraph()

print("Graph k=%d edges=%d" % (graph[0], graph[2]))

pos = { }

for n in graph[1]:
    pos[n[0]] = (n[1], n[2])
for e in graph[3]:
    G.add_edge(e[0], e[1])

nx.draw(G, pos=pos, with_label=True, arrows=False);
# plt.show()
plt.savefig("output.png", format="PNG")

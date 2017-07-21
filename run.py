#!/bin/python3.6

import tspanner
import networkx as nx
import numpy as np
import matplotlib.pyplot as plt
import profiling



# Output to a file
# tspanner.compute_greedy(1.0, "inputs/complet.graph", "output.dot")

graph = tspanner.compute_greedy(1.5, "inputs/us_250.graph")

print("Graph computations done.")
G = nx.DiGraph()

print("Graph k=%d edges=%d" % (graph[0], graph[2]))

pos = { }

for n in graph[1]:
    pos[n[0]] = (n[1], n[2])
for e in graph[3]:
    G.add_edge(e[0], e[1])

print("Drawing...")
nx.draw(G, pos=pos, node_size=0.1, with_label=True, arrows=False);
# plt.show()
# plt.figure(figsize=(500, 500))
print("Output to png...")
plt.savefig("output.png", format="PNG", dpi=1000)

pr = profiling.Profiling()
pr.set_args(1.2, "inputs/us_250.graph")

pr.set_method("compute_greedy")
pr.run()
pr.print_stats()


pr.set_args(1.2, "inputs/test.graph")


pr.set_method("compute_wspd")
pr.run()
pr.print_stats()

pr.set_method("compute_wspd_parallel")
pr.run()
pr.print_stats()

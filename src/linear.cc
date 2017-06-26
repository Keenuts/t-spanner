#include <vector>
#include <iostream>
#include <cstring>

#include "error.h"
#include "pathfinding.h"
#include "types.h"

//This function only test Dijkstra for now.
int greedy_linear(struct graph_t *graph, struct graph_t *output) {

  std::vector<struct edge_t> edges(graph->edge_nbr);
  std::vector<uint32_t> path;

  std::memcpy(edges.data(), graph->edges, sizeof(struct edge_t) * graph->edge_nbr);

  int res = dijkstra(edges, path, graph->k, 0, 20);
  if (!res)
    printf("[!] %s: Dijkstra returned with error code : 0x%x.\n", __func__, res);
  printf("Path returned has %zu nodes.\n", path.size());

  output->edge_nbr = path.size() - 1;
  output->k = path.size();
  output->edges = new struct edge_t[path.size() - 1];

  for (unsigned int i = 0; i < path.size() - 1; i++) {
    struct edge_t e {
      .a = path[i],
      .b = path[i + 1],
      .w = 1
    };
    output->edges[i] = e;
  }

  return STATUS_SUCCESS;
}

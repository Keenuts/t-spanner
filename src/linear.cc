#include <algorithm>
#include <cstring>
#include <iostream>
#include <unordered_set>
#include <vector>

#include "error.h"
#include "pathfinding.h"
#include "types.h"

static void sort(std::vector<struct edge_t> &edges)
{
  struct {
    bool operator()(struct edge_t a, struct edge_t b) const
    {
      return a.w < b.w;
    }
  } less_edge;

  std::sort(edges.begin(), edges.end(), less_edge);
}

static float node_distance(struct node_t a, struct node_t b)
{
  return std::sqrt(std::pow(b.x - a.x, 2) + std::pow(b.y - a.y, 2));
}

static float path_length(std::vector<struct edge_t> &edges,
                        uint32_t k, uint32_t a, uint32_t b)
{
  float cost;
  std::vector<uint32_t> path;

  int res = dijkstra(edges, path, k, a, b, &cost);
  if (res)
    return std::numeric_limits<float>::infinity();
  return cost;
}

static uint32_t count_nodes(std::vector<struct edge_t> &edges) {
  std::unordered_set<uint32_t> total;

  for (struct edge_t e : edges) {
    total.insert(e.a);
    total.insert(e.b);
  }
  return total.size();
}

int greedy_linear(struct graph_t *graph, struct graph_t *output, float t)
{
  std::vector<struct edge_t> edges_a;
  std::vector<struct edge_t> edges_b;
  printf("%p\n", graph);
  for (size_t i = 0; i < graph->k; ++i)
    for (size_t j = 0; j < graph->k; ++j)
      if (i == j)
	continue;
      else {
	auto a = graph->nodes[i];
	auto b = graph->nodes[j];
	edge_t e(a.id, b.id, node_distance(a, b));
	edges_a.push_back(e);
      }

  std::cout << "done" << std::endl;
  sort(edges_a);

  for (struct edge_t e : edges_a) {
    float len = path_length(edges_b, graph->k, e.a, e.b);
    float d = t * node_distance(graph->nodes[e.a], graph->nodes[e.b]);
    if (len > d) {
      edges_b.push_back(e);
    }
  }
  std::cout << "compute done" << std::endl;

  output->k = count_nodes(edges_b);
  output->edge_nbr = edges_b.size();
  output->edges = new struct edge_t[edges_b.size()];
  std::memcpy(output->edges, edges_b.data(), sizeof(struct edge_t) * edges_b.size());

  output->nodes = new struct node_t[graph->k];
  std::memcpy(output->nodes, graph->nodes, sizeof(struct node_t) * graph->k);

  return STATUS_SUCCESS;
}

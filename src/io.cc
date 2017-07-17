#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include <cstring>
#include <stdio.h>
#include <cmath>

#include "error.h"
#include "types.h"

static float node_distance(struct node_t a, struct node_t b)
{
  return std::sqrt(std::pow(b.x - a.x, 2) + std::pow(b.y - a.y, 2));
}

static int parse_file(
  std::ifstream &in,
  std::vector<struct edge_t> &edges,
  std::vector<struct node_t> &nodes,
  uint32_t *k
  )
{
  std::map<std::string, struct node_t> names;
  uint32_t index = 0;

  for (uint32_t line = 0; !in.eof(); line++) {
    std::string name;
    float x, y;

    in >> name;
    if (in.eof())
      break;

    in >> x;
    in >> y;

    if (in.eof()) {
      printf("[!] %s: node redefined at the line : %u\n", __func__, line);
      return STATUS_ERROR_PARSING_INCOMPLETE;
    }

    node_t node(index, x, y);

    auto res = names.emplace(std::make_pair(name, node));
    if (res.second)
      index++;
  }

  for (const auto &a : names) {
    nodes.push_back(a.second);
    for (const auto &b : names) {
      if (a.second.id == b.second.id)
        continue;

      edge_t e(a.second.id, b.second.id, node_distance(a.second, b.second));
      edges.push_back(e);
    }
  }

  *k = index;
  return STATUS_SUCCESS;
}

int load_graph(const char* path, struct graph_t *graph) {

  std::ifstream in(path);
  std::vector<struct edge_t> edges;
  std::vector<struct node_t> nodes;
  uint32_t res, k;

  if (!in.is_open())
    return STATUS_ERROR_IO;

  res = parse_file(in, edges, nodes, &k);
  if (res != STATUS_SUCCESS)
    return res;

  if (graph->edges)
    printf("[!] %s: graph pointer not null\n", __func__);
  graph->edge_nbr = edges.size();
  graph->k = k;
  graph->nodes = new struct node_t[k];
  graph->edges = new struct edge_t[edges.size()];
  std::memcpy(graph->edges, edges.data(), edges.size() * sizeof(struct edge_t));
  std::memcpy(graph->nodes, nodes.data(), nodes.size() * sizeof(struct node_t));

  printf("[*] Loading a graph with %u edges.\n", graph->edge_nbr);
  return STATUS_SUCCESS;
}

int output_graph(struct graph_t *graph, const char *path) {
  FILE *f = fopen(path, "w");
  if (!f)
    return STATUS_ERROR_IO;

  fprintf(f, "digraph {\n");
  for (uint32_t i = 0; i < graph->edge_nbr; i++) { 
    fprintf(f, "\t%d -> %d [label=\"%d\", weight=\"%d\"];\n",
      graph->edges[i].a, graph->edges[i].b, (uint32_t)graph->edges[i].w,
      (uint32_t)graph->edges[i].w);
  }
  fprintf(f, "}\n");

  fclose(f);

  return STATUS_SUCCESS;
}

#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include <cstring>
#include <stdio.h>

#include "error.h"
#include "types.h"

static int edge_exists(std::vector<struct edge_t> &edges, struct edge_t edge) {
  for (struct edge_t e : edges)
    if (e.a == edge.a && e.b == edge.b && e.w == edge.w)
      return 1;
  return 0;
};

static int parse_file(std::ifstream &in, std::vector<struct edge_t> &edges, uint32_t *k) {
  std::map<std::string, uint32_t> names;
  uint32_t ids[2] = { 0 };
  uint32_t index = 0;
  uint32_t weight = -1;

  for (uint32_t line = 0; !in.eof(); line++) {
    std::string n;

    for (uint32_t i = 0; i < 2; i++) {
      in >> n;
      if (in.eof() && i == 0)
        break;
      else if (in.eof())
        return STATUS_ERROR_PARSING_INCOMPLETE;

      auto e = names.find(n);
      if (e == names.end()) {
        ids[i] = index++;
        names.emplace(std::make_pair(n, ids[i]));
      }
      else
        ids[i] = e->second;
    }
    if(in.eof()) //EOF only arise when reading on EOF, not before...
      break;

    weight = -1;
    in >> weight;
    if (weight == (uint32_t)-1)
      return STATUS_ERROR_PARSING_NUMBER;

    struct edge_t e = {
      .a = ids[0],
      .b = ids[1],
      .w = weight,
    };
    if (!edge_exists(edges, e))
      edges.push_back(e);
    else {
      printf("[!] %s: Edge at line %d already exists.\n", __func__, line);
      return STATUS_ERROR_PARSING_EXISTANT_EDGE;
    }
  }

  *k = index;
  return STATUS_SUCCESS;
}

int load_graph(const char* path, struct graph_t *graph) {

  std::ifstream in(path);
  std::vector<struct edge_t> edges;
  uint32_t res, k;

  if (!in.is_open())
    return STATUS_ERROR_IO;

  res = parse_file(in, edges, &k);
  if (res != STATUS_SUCCESS)
    return res;

  if (graph->edges)
    printf("[!] %s: graph pointer not null\n", __func__);
  graph->edge_nbr = edges.size();
  graph->k = k;
  graph->edges = new struct edge_t[edges.size()];
  std::memcpy(graph->edges, edges.data(), edges.size() * sizeof(struct edge_t));

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
      graph->edges[i].a, graph->edges[i].b, graph->edges[i].w, graph->edges[i].w);
  }
  fprintf(f, "}\n");

  fclose(f);

  return STATUS_SUCCESS;
}

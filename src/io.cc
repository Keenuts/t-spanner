#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include <cstring>

#include "error.h"
#include "types.h"

static int edge_exists(std::vector<struct edge_t> &edges, struct edge_t edge) {
  for (struct edge_t e : edges)
    if (e.a == edge.a && e.b == edge.b && e.w == edge.w)
      return 1;
  return 0;
};

static int parse_file(std::ifstream &in, std::vector<struct edge_t> &edges) {
  std::map<std::string, int> names;
  int ids[2] = { 0 };
  int index = 0;
  int weight = -1;

  for (int line = 0; !in.eof(); line++) {
    std::string n;

    for (int i = 0; i < 2; i++) {
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
    if (weight == -1)
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
  return STATUS_SUCCESS;
}

int load_graph(const char* path, struct graph_t *graph) {

  std::ifstream in(path);
  std::vector<struct edge_t> edges;
  int res;

  if (!in.is_open())
    return STATUS_ERROR_IO;

  res = parse_file(in, edges);
  if (res != STATUS_SUCCESS)
    return res;

  if (graph->edges)
    printf("[!] %s: graph pointer not null\n", __func__);
  graph->edge_nbr = edges.size();
  graph->edges = new struct edge_t[edges.size()];
  std::memcpy(graph->edges, edges.data(), edges.size() * sizeof(struct edge_t));

  printf("[*] Loading a graph with %zu edges.\n", graph->edge_nbr);
  return STATUS_SUCCESS;
}

int output_graph(struct graph_t *graph, const char *path) {
  (void)graph;
  (void)path;

  return STATUS_SUCCESS;
}

#include <cstring>
#include <iostream>
#include <cstdlib>

#include "gpu.h"
#include "io.h"
#include "linear.h"
#include "messages.h"
#include "types.h"
#include "wspd.hh"

static inline void delete_graph(graph_t *g)
{
  delete[] g->nodes;
  delete[] g->edges;
}

int main(int argc, char** argv)
{

  int res;
  struct graph_t graph, output;
  std::memset(&graph, 0, sizeof(struct graph_t));
  std::memset(&output, 0, sizeof(struct graph_t));

  if (argc != 3) {
    std::cout << MSG_USAGE_INTRO << argv[0] << MSG_USAGE_PARAMS << std::endl;
    return 1;
  }

  res = load_graph(argv[1], &graph);
  if (res) {
    printf("[!] Graph loading returned with error 0x%x\n", res);
    return res;
  }

  printf("[*] Input graph has %u nodes.\n", graph.k);
  printf("[*] Input graph has %u edges.\n", graph.edge_nbr);

  tbb::tick_count t0 = tbb::tick_count::now();
  res = wspd_linear(&graph, &output, 20.0 / 3.0);
  //res = greedy_linear(&graph, &output, 4.0);
  tbb::tick_count t1 = tbb::tick_count::now();
  std::cout << (t1 - t0).seconds() << " s" << std::endl;
  if (res) {
    printf("[!] Greedy (linear) returned with error 0x%x\n", res);
    return res;
  }

  printf("[*] Output graph has %u nodes.\n", output.k);
  printf("[*] Output graph has %u edges.\n", output.edge_nbr);
  res = output_graph(&output, argv[2]);

  delete_graph(&graph);
  delete_graph(&output);
  if (res)
    return res;
  return 0;

  (void)argc;
  (void)argv;
  return 0;
}


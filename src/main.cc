#include <iostream>
#include <cstring>

#include "io.h"
#include "linear.h"
#include "messages.h"
#include "types.h"
#include "wspd.hh"

int main(int argc, char** argv) {
#if 0
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

  res = greedy_linear(&graph, &output, 2);
  if (res) {
    printf("[!] Greedy (linear) returned with error 0x%x\n", res);
    return res;
  }

  printf("[*] Output graph has %u nodes.\n", output.k);
  printf("[*] Output graph has %u edges.\n", output.edge_nbr);
  res = output_graph(&output, argv[2]);
  if (res)
    return res;
  return 0;
#endif
  (void)argc;
  (void)argv;
  Point<double> p(0.0, 1.0);
  Point<double> p1(1.0, 1.0);
  Point<double> p2(1.0, 3.0);

  std::vector<Point<double>> v;
  v.push_back(p);
  v.push_back(p1);
  v.push_back(p2);

  WSPD wpsd;
  auto test = wpsd.split_tree<double>(v);
  std::cout << "hello" << std::endl;
};

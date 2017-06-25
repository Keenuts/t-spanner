#include <iostream>
#include <cstring>

#include "io.h"
#include "messages.h"
#include "types.h"

int main(int argc, char** argv) {
  int res;
  struct graph_t graph;
  std::memset(&graph, 0, sizeof(struct graph_t));

  if (argc != 3) {
    std::cout << MSG_USAGE_INTRO << argv[0] << MSG_USAGE_PARAMS << std::endl;
    return 1;
  }

  res = load_graph(argv[1], &graph);
  if (res) {
    printf("[!] Graph loading returned with error 0x%x\n", res);
    return res;
  }

  //TODO: ADD GRAPH COMP

  res = output_graph(&graph, argv[2]); 
  if (res)
    return res;
  return 0;
};

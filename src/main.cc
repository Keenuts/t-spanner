#include <iostream>
#include <cstring>

#include "io.h"
#include "linear.h"
#include "messages.h"
#include "types.h"

int main(int argc, char** argv) {
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

  res = greedy_linear(&graph, &output);
  if (res) {
    printf("[!] Greedy (linear) returned with error 0x%x\n", res);
    return res;
  }

  res = output_graph(&output, argv[2]); 
  if (res)
    return res;
  return 0;
};

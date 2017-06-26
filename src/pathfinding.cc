#include <limits>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <vector>

#include "error.h"
#include "types.h"

static uint32_t find_min(std::vector<uint32_t> &nodes, std::vector<float> &weights)
{
  float min = std::numeric_limits<float>::infinity();
  uint32_t id = -1;

  for (uint32_t i = 0; i < nodes.size(); i++) {
    if (weights[nodes[i]] < min) {
      min = weights[nodes[i]];
      id = i;
    }
  }

  return id;
}

int dijkstra(std::vector<struct edge_t> &edges, std::vector<uint32_t> &path,
             uint32_t k, uint32_t a, uint32_t b, float *cost)
{
  std::vector<float> weight(k);
  std::vector<uint32_t> predecessor(k);
  std::vector<uint32_t> q(k);

  for (uint32_t i = 0; i < k; i++) {
    weight[i] = std::numeric_limits<float>::infinity();
    q[i] = i;
  }
  weight[a] = 0;

  while (q.size()) {
    uint32_t i1 = find_min(q, weight);
    if (i1 == (uint32_t)-1)
      return STATUS_NO_PATH;

    uint32_t s1 = q[i1];
    q.erase(q.begin() + i1);

    for (struct edge_t e : edges) {
      if (e.a == s1 && weight[e.b] > weight[s1] + e.w) {
        weight[e.b] = weight[s1] + e.w;
        predecessor[e.b] = s1;
      }
    }
    if (s1 == b)
      break;
  }

  uint32_t p = b;
  *cost = weight[b];
  do {
    path.insert(path.begin(), p);
    p = predecessor[p];
  } while (path[0] != a);

  return STATUS_SUCCESS;
}

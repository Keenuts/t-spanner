#pragma once

#include <stdint.h>
#include <vector>

#include "types.h"

int dijkstra(std::vector<struct edge_t> &edges, std::vector<uint32_t> &path,
             uint32_t k, uint32_t a, uint32_t b);

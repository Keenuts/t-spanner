#pragma once

#include <CL/cl.hpp>
#include <cmath>
#include <sys/types.h>

struct edge_t {
  uint32_t a, b;
  float w;
  edge_t() {}
  edge_t(uint32_t _a, uint32_t _b, float _w) : a(_a), b(_b), w(_w) {}
};

struct node_t {
  uint32_t id;
  float x, y;
  node_t() {}
  node_t(uint32_t _id, float _x, float _y) : id(_id), x(_x), y(_y) {}
};

struct graph_t {
  uint32_t edge_nbr;
  uint32_t k;
  struct node_t *nodes;
  struct edge_t *edges;
};

struct gpu_t {
  cl::Context context;
  cl::Device device;
  cl::Platform platform;
  cl::Program kernel;
};

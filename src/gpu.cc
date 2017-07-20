#include <CL/cl.hpp>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>

#include "error.h"
#include "types.h"
#include "gpu.h"

int load_kernel(const char *filename, struct gpu_t *gpu)
{
  cl_int res;
  std::ifstream f(filename);
  if (!f.is_open()) {
    std::cout << "[!] Kernel not found : " << filename << ":\n";
    return STATUS_ERROR_IO;
  }
  std::string content((std::istreambuf_iterator<char>(f)),
                       std::istreambuf_iterator<char>()); 
  f.close();

  cl::Program k(gpu->context, content);
  res = k.build({ gpu->device });
  if (res != CL_SUCCESS) {
    std::cout << "[!] Unable to build the kernel : " << filename << ":\n";
    std::cout << k.getBuildInfo<CL_PROGRAM_BUILD_LOG>(gpu->device) << "\n";
    return STATUS_ERROR_KERNEL_COMPILATION;
  }

  gpu->kernel = k;
  std::cout << "[!] Kernel " << filename << " successfuly loaded.\n";
  return STATUS_SUCCESS;
}

int initialize_gpu(const char *kernel_path, struct gpu_t *gpu)
{
  cl_int res = CL_SUCCESS;

  std::vector<cl::Platform> platforms;
  std::vector<cl::Device> devices;

  res = cl::Platform::get(&platforms);
  if (res != CL_SUCCESS || platforms.size() == 0)
    return STATUS_ERROR_GPU_INITIALIZATION;

  for (cl::Platform p : platforms) {
    res = p.getDevices(CL_DEVICE_TYPE_ALL, &devices);
    if (res == CL_SUCCESS) {
      if (devices.size() == 0)
        return STATUS_ERROR_GPU_NOT_COMPATIBLE;

      gpu->device = devices[0];
      gpu->platform = p;
      break;
    }
    else if (res == CL_DEVICE_NOT_FOUND)
      std::cout << "[!] Device type not found\n";
    else
      std::cout << "[!] Unspecified error\n";
      
  }

  if (res != CL_SUCCESS)
    return STATUS_ERROR_GPU_NO_DEVICE_FOUND;

  gpu->context = cl::Context({ gpu->device });

  return load_kernel(kernel_path, gpu);
}

int greedy_gpu(struct graph_t *graph, struct graph_t *output, float t)
{
  const int ker_nbr_w = graph->edge_nbr;
  const uint64_t edge_size = sizeof(struct edge_t) * graph->edge_nbr;
  const uint64_t node_size = sizeof(struct node_t) * graph->k;
  struct gpu_t gpu;

  int res = initialize_gpu("kernels/greedy.comp", &gpu);
  if (res != STATUS_SUCCESS)
    return res;

  struct gpu_settings_t settings {
    .per_ker_width = 1,
    .k = graph->k,
    .edge_nbr = graph->edge_nbr,
  };

  cl::Buffer params(gpu.context, CL_MEM_READ_ONLY, sizeof(struct gpu_settings_t));
  cl::Buffer edge_buffer(gpu.context, CL_MEM_READ_ONLY, edge_size);
  cl::Buffer node_buffer(gpu.context, CL_MEM_READ_ONLY, node_size);
  cl::Buffer output_buffer(gpu.context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY,
                           edge_size);

  cl::CommandQueue queue(gpu.context, gpu.device);
  queue.enqueueWriteBuffer(params, CL_TRUE, 0, sizeof(struct gpu_settings_t), &settings);
  queue.enqueueWriteBuffer(edge_buffer, CL_TRUE, 0, edge_size, graph->edges);
  queue.enqueueWriteBuffer(node_buffer, CL_TRUE, 0, node_size, graph->nodes);
  cl::Kernel k = cl::Kernel(gpu.kernel, "greedy_tspanner");
  k.setArg(0, t);
  k.setArg(1, params);
  k.setArg(2, edge_buffer);
  k.setArg(3, node_buffer);
  k.setArg(4, output_buffer);


  queue.enqueueNDRangeKernel(k, cl::NullRange, cl::NDRange(ker_nbr_w, 1), cl::NullRange);
  queue.finish();

  output->k = graph->k;
  output->nodes = new struct node_t[output->k];
  output->edge_nbr = graph->edge_nbr;
  output->edges = new struct edge_t[graph->edge_nbr];
  std::memcpy(output->nodes, graph->nodes, sizeof(struct node_t) * graph->k);

  queue.enqueueReadBuffer(output_buffer, CL_TRUE, 0, edge_size, output->edges);

  for (uint32_t i = 0; i < graph->edge_nbr; i++) {
    printf("| %d -> %d [%f]\n",
           output->edges[i].a, output->edges[i].b, output->edges[i].w);
  }
  return STATUS_SUCCESS;
}

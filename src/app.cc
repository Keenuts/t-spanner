#include <cstring>
#include <iostream>
#include <python3.6m/Python.h>
#include <stdio.h>

#include "io.h"
#include "linear.h"
#include "messages.h"
#include "types.h"
#include "wspd.hh"

static int compute_greedy(const char *input_file, const char *output_file)
{
  int res;
  struct graph_t graph, output;
  std::memset(&graph, 0, sizeof(struct graph_t));
  std::memset(&output, 0, sizeof(struct graph_t));

  res = load_graph(input_file, &graph);
  if (res) {
    printf("[!] Graph loading returned with error 0x%x\n", res);
    return res;
  }

  res = greedy_linear(&graph, &output, 5);
  if (res) {
    printf("[!] Greedy (linear) returned with error 0x%x\n", res);
    return res;
  }

  printf("[*] Output graph has %u nodes.\n", output.k);
  printf("[*] Output graph has %u edges.\n", output.edge_nbr);

  res = output_graph(&output, output_file);
  if (res)
    return res;
  return 0;
}

static PyObject* tspanner_greedy(PyObject *self, PyObject *args)
{
  const char *input_file;
  const char *output_file;

  if (!PyArg_ParseTuple(args, "ss", &input_file, &output_file))
      return NULL;

  (void)self;
  return PyLong_FromLong(compute_greedy(input_file, output_file));
}

static PyMethodDef tspanner_methods[] = {
  { "compute_greedy", tspanner_greedy, METH_VARARGS, "Linear greedy algorithm" },
  { NULL, NULL, 0, NULL }
};

static struct PyModuleDef tspanner_module = {
  .m_base = PyModuleDef_HEAD_INIT,
  .m_name = "tspanner",
  .m_doc = NULL,
  .m_size = -1,
  .m_methods = tspanner_methods,
  .m_slots = NULL,
  .m_traverse = NULL,
  .m_clear = NULL,
  .m_free = NULL,
};

PyMODINIT_FUNC
PyInit_tspanner(void)
{
  return PyModule_Create(&tspanner_module);
}

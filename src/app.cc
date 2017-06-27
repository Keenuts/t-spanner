#include <cstring>
#include <iostream>
#include <python3.6m/Python.h>
#include <stdio.h>

#include "io.h"
#include "linear.h"
#include "messages.h"
#include "types.h"
#include "wspd.hh"

static int compute_linear_greedy(const char* input_file, struct graph_t *out, float t)
{
  int res;
  struct graph_t graph;
  std::memset(&graph, 0, sizeof(struct graph_t));

  res = load_graph(input_file, &graph);
  if (res) {
    printf("[!] Graph loading returned with error 0x%x\n", res);
    return res;
  }

  return greedy_linear(&graph, out, t);
}

static int greedy_file(const char *input_file, const char *output_file, float t)
{
  int res;
  struct graph_t output;
  std::memset(&output, 0, sizeof(struct graph_t));

  res = compute_linear_greedy(input_file, &output, t);
  if (res) {
    printf("[!] Greedy (linear) returned with error 0x%x\n", res);
    return res;
  }

  printf("[*] Output graph has %u nodes.\n", output.k);
  printf("[*] Output graph has %u edges.\n", output.edge_nbr);
  return output_graph(&output, output_file);
}

PyObject* greedy_object(const char *input_file, float t)
{
  int res;
  struct graph_t output;
  std::memset(&output, 0, sizeof(struct graph_t));

  res = compute_linear_greedy(input_file, &output, t);
  if (res) {
    printf("[!] Greedy (linear) returned with error 0x%x\n", res);
    //FIXME: Python needs error setting
    return NULL;
  }

  PyObject *nodes = PyList_New(output.k);

  for (uint32_t i = 0; i < output.k; i++) {
    PyObject *t = PyTuple_New(3);
    PyTuple_SetItem(t, 0, PyLong_FromLong(output.nodes[i].id));
    PyTuple_SetItem(t, 1, PyLong_FromLong(output.nodes[i].x));
    PyTuple_SetItem(t, 2, PyLong_FromLong(output.nodes[i].y));
    PyList_SET_ITEM(nodes, i, t);
  }

  PyObject *edges = PyList_New(output.edge_nbr);
  for (uint32_t i = 0; i < output.edge_nbr; i++) {
    PyObject *e = PyTuple_New(3);
    PyTuple_SetItem(e, 0, PyLong_FromLong(output.edges[i].a));
    PyTuple_SetItem(e, 1, PyLong_FromLong(output.edges[i].b));
    PyTuple_SetItem(e, 2, PyFloat_FromDouble(output.edges[i].w));
    PyList_SET_ITEM(edges, i, e);
  }

  PyObject *tuple = PyTuple_New(4);
  PyTuple_SetItem(tuple, 0, PyLong_FromLong(output.k));
  PyTuple_SetItem(tuple, 1, nodes);
  PyTuple_SetItem(tuple, 2, PyLong_FromLong(output.edge_nbr));
  PyTuple_SetItem(tuple, 3, edges);

  return tuple;
}

static PyObject* tspanner_greedy(PyObject *self, PyObject *args)
{
  float t = 0.0f;
  const char *input_file = NULL;
  const char *output_file = NULL;
  (void)self;

  if (!PyArg_ParseTuple(args, "fs|s", &t, &input_file, &output_file))
      return NULL;

  if (output_file)
    return PyLong_FromLong(greedy_file(input_file, output_file, t));
  else
    return greedy_object(input_file, t);
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

#include <cstring>
#include <new>
#include "wspd.hh"
#include "error.h"

template <class Iterator, class _Point>
Iterator vector_get(Iterator begin, Iterator end, const _Point& p)
{
  for (; begin != end; ++begin) {
    const auto elt = *begin;
    if (elt.x == p.x && elt.y == p.y)
      break;
  }
  return begin;
}

/* returns iterator on added element */
template <class _Point>
auto add_node(std::vector<node_t>& nodes, const _Point& p, uint32_t& idx)
-> decltype(nodes.begin())
{
  auto it = vector_get(nodes.begin(), nodes.end(), p);
  if (it != nodes.end())
    return it;
  nodes.push_back(node_t(idx++, p.x, p.y));
  return std::prev(nodes.end());
}

/* Graph<Point<float>> g;
 * for (auto p : pairs) {
 *   g.add_edge(p.first->elt, p.second->elt, 0.0f);
 * }
 * return g;
 */
template <typename Pairs>
static int fill_graph(graph_t *out, const Pairs& sep_pairs)
{

  uint32_t index = 0;
  std::vector<edge_t> edge_out;
  std::vector<node_t> node_out;

  for (const auto pair: sep_pairs) {
    auto p1 = (pair.first)->elt;
    auto p2 = (pair.second)->elt;
    auto a = add_node<decltype(p1)>(node_out, p1, index);
    auto id = a->id;
    auto b = add_node<decltype(p2)>(node_out, p2, index);
    edge_out.push_back(edge_t(id, b->id, p1.euclidean_distance(p2)));
  }

  out->k = node_out.size();
  out->edge_nbr = edge_out.size();

  try {
    out->nodes = new node_t[out->k];
    out->edges = new edge_t[out->edge_nbr];
  } catch (const std::bad_alloc& ex) {
    return STATUS_ERROR_GRAPH_ALLOC;
  }

  std::memcpy(out->edges, edge_out.data(), out->edge_nbr * sizeof(edge_t));
  std::memcpy(out->nodes, node_out.data(), out->k * sizeof(node_t));
  return STATUS_SUCCESS;
}

int wspd_linear(graph_t *graph, graph_t *output, float t)
{
  std::vector<Point<float>> points;

  for (size_t i = 0; i < graph->k; ++i) {
    node_t p = graph->nodes[i];
    auto it = vector_get(points.begin(), points.end(), p);
    if (it == points.end())
      points.emplace_back(Point<float>(p.x, p.y, p.id));
  }

  WSPD<float> wspd(points, t);
  //auto sep_pairs = wspd.compute_parallel_tree();
  auto sep_pairs = wspd.compute();

  return fill_graph<decltype(sep_pairs)>(output, sep_pairs);
}

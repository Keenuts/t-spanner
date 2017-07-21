#pragma once

#include <memory>
#include <vector>
#include <algorithm>
#include <iostream>
#include <tbb/tbb.h>
#include <atomic>

#include "types.h"

template <typename T>
struct Point {
  T x, y;
  int id;
  Point() {}
  Point(T _x, T _y) : x(_x), y(_y), id(0) {}
  Point(T _x, T _y, int _id) : x(_x), y(_y), id(_id) {}
  double euclidean_distance(const Point<T>& p) const {
    return std::sqrt(std::pow(p.x - x, 2.0) + std::pow(p.y - y, 2.0));
  }
  friend std::ostream& operator<<(std::ostream& os, const Point<T>& rhs) {
    os << "(" << rhs.x << ", " << rhs.y << ")";
    return os;
  }
};

template <typename T>
struct HyperRect {
  std::vector<std::pair<T, T>> intervals;

  HyperRect<T>() {}
  HyperRect<T>(const std::vector<std::pair<T, T>>& i) : intervals(i) {}
  HyperRect<T>(const std::vector<Point<T>>& s);
  HyperRect<T>(const std::vector<Point<T>>& s, const HyperRect<T>& rhs);
  HyperRect<T>(const HyperRect<T>& rhs) {
    std::copy(rhs.intervals.begin(), rhs.intervals.end(),
	      std::back_inserter(intervals));
  }

  bool is_in(const Point<T>& p) const;
  bool is_well_separated(const HyperRect<T>& rhs, double stretch);
  std::pair<HyperRect<T>, HyperRect<T>> split() const;
  T max_dim() const;
};

template <typename T, typename U = T>
struct Tree {
  T elt;
  HyperRect<U> box;
  std::shared_ptr<Tree<T, U>> l;
  std::shared_ptr<Tree<T, U>> r;

  Tree<T, U>(const T& _elt) : elt(_elt), box(HyperRect<U>()), l(nullptr),
			      r(nullptr)
  {}

  Tree<T, U>(const T& _elt, const HyperRect<U>& _box) : elt(_elt), box(_box),
		  				 l(nullptr), r(nullptr)
  {}
};

template <typename T>
struct Sphere {
  Point<T> center;
  T radius;
  Sphere<T>(const HyperRect<T>& rect);
  double min_dist(const Sphere<T>& s) const;
  friend std::ostream& operator<<(std::ostream& os, const Sphere<T>& rhs) {
    os << "Center: " << rhs.center << std::endl;
    os << "Radius: " << rhs.radius << std::endl;
    return os;
  }
};

template <typename T>
using tree_ptr = std::shared_ptr<Tree<Point<T>, T>>;

template <typename T>
using ws_pair = std::pair<tree_ptr<T>, tree_ptr<T>>;

template <typename T>
struct WSPD {
  std::vector<Point<T>> points;
  double stretch;

  WSPD<T>(const std::vector<Point<T>>& s, double _stretch) {
    points = std::move(s);
    stretch = _stretch;
  }

  std::vector<ws_pair<T>> compute() const;
  std::vector<ws_pair<T>> compute_parallel_tree() const;

private:
  tree_ptr<T> split_tree(const std::vector<Point<T>>& s, const HyperRect<T>& box) const;

  void find_pairs(const tree_ptr<T>& l, const tree_ptr<T>& r,
		   std::vector<ws_pair<T>>& res)
  const;

  void compute_rec(const tree_ptr<T>& u, std::vector<ws_pair<T>>& res) const;


};

template <typename T>
struct SplitTreeTask : tbb::task {
  const std::vector<Point<T>> s;
  const HyperRect<T> box;
  tree_ptr<T>& u;
  std::atomic_ulong next;

  SplitTreeTask<T>(const std::vector<Point<T>>& _s, const HyperRect<T>& _box,
		   tree_ptr<T>& _u, long unsigned int n)
  : s(_s), box(_box), u(_u), next(n)
  {}

  virtual tbb::task* execute();
};

int wspd_linear(graph_t *graph, graph_t *output, float t);

#include "wspd.hxx"

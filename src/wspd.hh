#pragma once

#include <memory>
#include <vector>
#include <algorithm>

template <typename T>
struct Point {
  T x, y;
  Point(T _x, T _y) : x(_x), y(_y) {}
};

template <typename T>
struct HyperRect {
  std::vector<std::pair<T, T>> intervals;

  HyperRect<T>() {}
  HyperRect<T>(const std::vector<std::pair<T, T>>& i) : intervals(i) {}
  HyperRect<T>(const std::vector<Point<T>>& s);

  bool is_in(const Point<T>& p);
  std::pair<HyperRect<T>, HyperRect<T>> split() const;
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
using tree_ptr = std::shared_ptr<Tree<Point<T>, T>>;

struct WSPD {
  /* ... */
  template <typename T>
  tree_ptr<T> split_tree(const std::vector<Point<T>>& s);
};

#include "wspd.hxx"

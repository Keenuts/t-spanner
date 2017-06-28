#pragma once

#include <memory>
#include <vector>
#include <algorithm>
#include <iostream>

template <typename T>
struct Point {
  T x, y;
  Point() {}
  Point(T _x, T _y) : x(_x), y(_y) {}
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

  bool is_in(const Point<T>& p) const;
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
struct Sphere {
  Point<T> center;
  T rayon;
  Sphere<T>(const HyperRect<T>& rect);
};

template <typename T>
using tree_ptr = std::shared_ptr<Tree<Point<T>, T>>;

struct WSPD {
  /* ... */
  template <typename T>
  tree_ptr<T> split_tree(const std::vector<Point<T>>& s);
};

#include "wspd.hxx"

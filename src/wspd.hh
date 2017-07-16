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
    std::copy(s.begin(), s.end(), std::back_inserter(points));
    stretch = _stretch;
  }

  std::vector<ws_pair<T>> compute() const;

private:
  tree_ptr<T> split_tree(const std::vector<Point<T>>& s) const;

  void find_pairs(const tree_ptr<T>& l, const tree_ptr<T>& r,
		   std::vector<ws_pair<T>>& res)
  const;

  void compute_rec(const tree_ptr<T>& u, std::vector<ws_pair<T>>& res) const;


};

#include "wspd.hxx"

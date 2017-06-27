#pragma once

template <typename T>
HyperRect<T>::HyperRect(const std::vector<Point<T>>& s) {
  if (s.size() == 1) {
    /* HyperRect has no meaning if sz == 1 it's a leaf */
    intervals.push_back(std::make_pair(s[0].x, s[0].y));
    intervals.push_back(std::make_pair(s[0].x, s[0].y));
    return;
  }

  const auto min = std::numeric_limits<T>::min();
  const auto max = std::numeric_limits<T>::max();
  std::pair<T, T> range_x(max, min);
  std::pair<T, T> range_y(max, min);

  for (const auto p : s) {
    if (p.x < range_x.first)
      range_x.first = p.x;
    else if (p.x > range_x.second)
      range_x.second = p.x;
    if (p.y < range_y.first)
      range_y.first = p.y;
    else if (p.y > range_y.second)
      range_y.second = p.y;
  }

  intervals.push_back(range_x);
  intervals.push_back(range_y);
}

/* Only work in 2D */
/* Split rectangle on maximum dimension */
template <typename T>
std::pair<HyperRect<T>, HyperRect<T>> HyperRect<T>::split() const {
  std::pair<T, T> l_x;
  std::pair<T, T> l_y;
  std::pair<T, T> r_x;
  std::pair<T, T> r_y;

  auto x = intervals[0].second - intervals[0].first;
  auto y = intervals[1].second - intervals[1].first;
  auto max = std::max(x, y);

  if (max == x) {
    auto max_x = (intervals[0].first + intervals[0].second) / 2;
    l_x = std::make_pair(intervals[0].first, max_x);
    r_x = std::make_pair(max_x, intervals[0].second);
    l_y = std::make_pair(intervals[1].first, intervals[1].second);
    r_y = std::make_pair(intervals[1].first, intervals[1].second);
  } else {
    auto max_y = (intervals[1].first + intervals[1].second) / 2;
    l_x = std::make_pair(intervals[0].first, intervals[0].second);
    r_x = std::make_pair(intervals[0].first, intervals[0].second);
    l_y = std::make_pair(intervals[1].first, max_y);
    r_y = std::make_pair(max_y, intervals[1].second);
  }

  auto v1 = std::vector<std::pair<T, T>>();
  auto v2 = std::vector<std::pair<T, T>>();
  v1.push_back(l_x);
  v1.push_back(l_y);
  v2.push_back(r_x);
  v2.push_back(r_y);

  return std::make_pair(HyperRect(v1), HyperRect(v2));
}

template <typename T>
bool HyperRect<T>::is_in(const Point<T>& p) {
  auto dim_x = intervals[0];
  auto dim_y = intervals[1];
  return (p.x >= dim_x.first && p.x <= dim_x.second)
	 && (p.y >= dim_y.first && p.y <= dim_y.second);
}

template <typename T>
tree_ptr<T> WSPD::split_tree(const std::vector<Point<T>>& s) {
  HyperRect<T> rect(s);
  if (s.size() == 1)
    return tree_ptr<T>(new Tree<Point<T>, T>(s[0], rect));
  auto pr = rect.split();
  auto rect_left = pr.first;
  auto rect_right = pr.second;
  auto left = std::vector<Point<T>>();
  auto right = std::vector<Point<T>>();
  for (const auto p : s)
    if (rect_left.is_in(p))
      left.push_back(p);
    else if (rect_right.is_in(p))
      right.push_back(p);

  auto tree = new Tree<Point<T>, T>(left[0] /* unused */, rect);
  tree->l = split_tree(left);
  tree->r = split_tree(right);

  return tree_ptr<T>(tree);
}

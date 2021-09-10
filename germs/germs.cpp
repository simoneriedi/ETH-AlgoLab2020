#include<iostream>
#include<limits>
#include<vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef K::Point_2 P;
typedef K::Segment_2 S;

void testcase(int n) {
  int left, bottom, right, top;
  cin >> left >> bottom >> right >> top;
  vector<P> points(n);
  for (int i=0; i<n; i++) {
    int x, y; cin >> x >> y;
    points[i] = P(x, y);
  }
  Triangulation T; 
  T.insert(points.begin(), points.end());
  vector<K::FT> min_seg_sizes;
  for (auto v = T.finite_vertices_begin(); v != T.finite_vertices_end(); v++) {
    // Find smallest edge from vertex
    double min_size = numeric_limits<double>::max();
    auto e = T.incident_edges(v);
    do {
      if (!T.is_infinite(e)) min_size = min(min_size, T.segment(e).squared_length()/4);
    } while (++e != T.incident_edges(v));
    min_size = min(min_size, (v->point().x()-left)*(v->point().x()-left));
    min_size = min(min_size, (v->point().x()-right)*(v->point().x()-right));
    min_size = min(min_size, (v->point().y()-top)*(v->point().y()-top));
    min_size = min(min_size, (v->point().y()-bottom)*(v->point().y()-bottom));
    min_seg_sizes.push_back(min_size);
  }
  int size = min_seg_sizes.size();
  sort(min_seg_sizes.begin(), min_seg_sizes.end());
  long first = ceil(sqrt((CGAL::sqrt(min_seg_sizes[0])-0.5)));
  long mid = ceil(sqrt((CGAL::sqrt(min_seg_sizes[size/2])-0.5)));
  long last = ceil(sqrt((CGAL::sqrt(min_seg_sizes[size-1])-0.5)));
  cout << first << " " << mid << " " << last << '\n';
}

int main() {
  ios_base::sync_with_stdio(false);
  int n; cin >> n;
  while (n) {
    testcase(n);
    cin >> n;
  }
}
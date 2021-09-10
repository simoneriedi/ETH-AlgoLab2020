#include<iostream>
#include<vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

using namespace std;



void testcase(int n) {
  vector<K::Point_2> points(n);
  for (int i=0; i<n; i++) {
    int x, y; cin >> x >> y;
    points[i] = K::Point_2(x, y);
  }
  Triangulation t;
  t.insert(points.begin(), points.end());
  int m; cin >> m;
  for (int i=0; i<m; i++) {
    int x, y; cin >> x >> y;
    K::Point_2 q = K::Point_2(x, y);
    Triangulation::Vertex_handle v = t.nearest_vertex(q);
    K::Point_2 p = v->point();
    cout << (long)CGAL::squared_distance(p, q) << '\n';
    
  }
  
}
int main() {
  int n; cin >> n;
  while (n) {
    testcase(n);
    cin >> n;
  }
}
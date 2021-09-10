#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <vector>
#include<limits>
#include<queue>
#include <tuple>
#include <algorithm>
#include <iostream>

using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K>   Vb;
typedef CGAL::Triangulation_face_base_with_info_2<K::FT, K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Triangulation;
typedef K::Point_2 P;
typedef Triangulation::Face_handle F;



class Compare {
  public:
    bool operator() (pair<K::FT, F> p1, pair<K::FT, F> p2) {
      return p1.first < p2.first;
    }
};


void precomp(Triangulation &T) {
  priority_queue<pair<K::FT, F>, vector<pair<K::FT, F>>, Compare> q;
  for (auto f = T.all_faces_begin(); f != T.all_faces_end(); f++) {
    if (T.is_infinite(f)) f->info() = numeric_limits<double>::max();
    else f->info() = CGAL::squared_radius(f->vertex(0)->point(), f->vertex(1)->point(), f->vertex(2)->point());
    q.push({f->info(), f});
  }
  while(!q.empty()) {
    auto curr = q.top(); q.pop();
    if (curr.first != curr.second->info()) continue;
    for (int i=0; i<3; i++) {
      F next = curr.second->neighbor(i);
      K::FT potential = min(curr.first, CGAL::squared_distance(curr.second->vertex((i+1)%3)->point(), curr.second->vertex((i+2)%3)->point()));
      if (potential > next->info()) {
        next->info() = potential;
        q.push({potential, next});
      }
    }
  }
}




void testcase() {
  int n, m;
  K::FT r;
  cin >> n >> m >> r;
  vector<P> points(n);
  for (int i=0; i<n; i++) {
    long x, y; cin >> x >> y;
    points[i] = P(x, y);
  }
  Triangulation T;
  T.insert(points.begin(), points.end());
  
  precomp(T);
  for (int i=0; i<m; i++) {
    long x, y; cin >> x >> y;
    P query(x, y);
    K::FT s; cin >> s;
    if (CGAL::squared_distance(T.nearest_vertex(query)->point(), query) < CGAL::square(r+s)) {
      cout << 'n';
      continue;
    }
    if (T.locate(query)->info() < 4*CGAL::square(r+s)) cout << 'n';
    else cout << 'y';
  }
  cout << '\n';
}




int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for (int i=0; i<t; i++) testcase();
}
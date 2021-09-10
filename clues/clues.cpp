#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>
#include <vector>
#include <queue>
#include<utility>
#include <tuple>
#include <algorithm>
#include <iostream>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<pair<int,int>,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Triangulation;
typedef K::Point_2 P;

bool precomp(Triangulation &T, K::FT dist) {
  vector<vector<P>> coloring(2, vector<P>());
  int comp = 0;
  for (auto v = T.finite_vertices_begin(); v != T.finite_vertices_end(); v++) {
    if (v->info().first != -1) continue;
    queue<Triangulation::Vertex_handle> q;
    q.push(v);
    v->info() = {0, comp};
    while (!q.empty()) {
      auto curr = q.front(); q.pop();
      coloring[curr->info().first].push_back(curr->point());
      int other_color = (curr->info().first+1)%2;
      auto next = T.incident_vertices(curr);
      do {
        if (!T.is_infinite(next) && next->info().first == -1 && CGAL::squared_distance(next->point(), curr->point()) <= dist) {
          q.push(next);
          next->info() = {other_color, comp};
        }
      } while (++next != T.incident_vertices(curr));
    }
    comp++;
  }
  bool bipartite = true;
  for (int i=0; i<2; i++) {
    Triangulation t;
    t.insert(coloring[i].begin(), coloring[i].end());
    for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); e++) {
      if (t.segment(e).squared_length() <= dist) bipartite = false;
    }
  }
  return bipartite;
}



void testcase() {
  int n, m; K::FT r;
  cin >> n >> m >> r;
  vector<pair<P,pair<int,int>>> points(n);
  for (int i=0; i<n; i++) {
    int x, y; cin >> x >> y;
    points[i] = {P(x, y), {-1, -1}};
  }
  Triangulation T;
  T.insert(points.begin(), points.end());
  bool bipartite = precomp(T, CGAL::square(r));
  for (int i=0; i<m; i++) {
    int x1, y1, x2, y2;
    cin >> x1 >> y1 >> x2 >> y2;
    if (!bipartite) {
      cout << 'n';
      continue;
    }
    P holmes(x1, y1), watson(x2, y2);
    bool close = CGAL::squared_distance(holmes, watson) <= CGAL::square(r);
    auto v_holmes = T.nearest_vertex(holmes);
    auto v_watson = T.nearest_vertex(watson);
    bool can_comunicate = CGAL::squared_distance(holmes, v_holmes->point()) <= CGAL::square(r) && CGAL::squared_distance(watson, v_watson->point()) <= CGAL::square(r);
    bool same_comp = v_holmes->info().second == v_watson->info().second;
    if (close || (can_comunicate&&same_comp)) cout << 'y';
    else cout << 'n';
    
  }
  cout << '\n';
}




int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for (int i=0; i<t; i++) testcase();
}
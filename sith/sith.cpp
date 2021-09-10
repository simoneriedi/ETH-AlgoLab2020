#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>
#include <vector>
#include<queue>
#include <tuple>
#include <algorithm>
#include <iostream>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Triangulation;
typedef K::Point_2 P;



pair<int,bool> largest_component(int k, vector<pair<P, int>> &points, K::FT r){
  Triangulation T;
  T.insert(points.begin()+k, points.end());
  vector<bool> vis((int)points.size());
  int max_comp = 0;
  for (auto v = T.finite_vertices_begin(); v != T.finite_vertices_end(); v++) {
    if (vis[v->info()]) continue;
    vis[v->info()] = true;
    queue<Triangulation::Vertex_handle> q;
    q.push(v);
    int comp_size = 1;
    while(!q.empty()) {
      auto curr = q.front(); q.pop();
      auto next = T.incident_vertices(curr);
      do {
        if (!T.is_infinite(next) && !vis[next->info()] && CGAL::squared_distance(curr->point(), next->point()) <= r) {
          q.push(next);
          vis[next->info()] = true;
          comp_size++;
        }
      } while(++next != T.incident_vertices(curr));
    }
    max_comp = max(max_comp, comp_size);
  }
  return {min(max_comp, k), max_comp > k};
}

void solve(vector<pair<P, int>> &points, K::FT r) {
  int left = 1; // Will point to last value of k such that .second is true
  int right = points.size()-1; // Will point to first value of k such that .second is false, i.e. waited too long
  while(left+1<right) {
    int mid = (left+right)/2;
    auto val = largest_component(mid, points, r);
    if (val.second) left = mid;
    else right = mid;
  }
  int res = max(largest_component(left, points, r).first, largest_component(right, points, r).first);
  cout << res << '\n';
}

void testcase() {
  int n; K::FT r;
  cin >> n >> r;
  vector<pair<P, int>> points(n);
  for (int i=0; i<n; i++) {
    int x, y; cin >> x >> y;
    points[i] = {P(x, y), i};
  }
  solve(points, r*r);
}



int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for (int i=0; i<t; i++) testcase();
}


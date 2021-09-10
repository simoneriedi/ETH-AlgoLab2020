#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>
#include <vector>
#include <tuple>
#include<queue>
#include <algorithm>
#include <iostream>
using namespace std;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Triangulation;
typedef K::Point_2 P;



int count_bones(K::FT dist, vector<K::FT> bones) {
  auto p = upper_bound(bones.begin(), bones.end(), dist);
  int val = p - bones.begin();
  return val;
}


int find_bones(Triangulation &T, vector<vector<K::FT>> &bones_here, int n, K::FT dist) {
  int max_bones = 0;
  vector<bool> vis(n, false);
  for (auto v = T.finite_vertices_begin(); v != T.finite_vertices_end(); v++) {
    if (vis[v->info()]) continue;
    int bones = 0;
    queue<Triangulation::Vertex_handle> q;
    q.push(v);
    vis[v->info()] = true;
    while (!q.empty()) {
      auto curr = q.front(); q.pop();
      bones += count_bones(dist/4, bones_here[curr->info()]);
      auto next = T.incident_vertices(curr);
      do {
        if (!T.is_infinite(next) && !vis[next->info()] && CGAL::squared_distance(curr->point(), next->point()) <= dist) {
          q.push(next);
          vis[next->info()] = true;
        }
      } while (++next != T.incident_vertices(curr));
    }
    max_bones = max(max_bones, bones);
  }
  return max_bones;
}



long rounding(const K::FT& x) {
  double a = std::ceil(CGAL::to_double(x));
  while (a-1 >= x) a -= 1;
  while (a < x) a += 1;
  return a;
}


void testcase() {
  int n, m, k;
  K::FT s; 
  cin >> n >> m >> s >> k;
  vector<pair<P, int>> points(n);
  for (int i=0; i<n; i++) {
    int x, y; cin >> x >> y;
    points[i] = {P(x, y), i};
  }
  Triangulation T;
  T.insert(points.begin(), points.end());
  vector<vector<K::FT>> bones(n, vector<K::FT>());
  vector<K::FT> candidates;
  for (int i=0; i<m; i++) {
    int x, y; cin >> x >> y;
    P bone(x, y);
    auto vertex = T.nearest_vertex(bone);
    K::FT dist = CGAL::squared_distance(bone, vertex->point());
    candidates.push_back(4*dist);
    bones[vertex->info()].push_back(dist);
  }
  for (int i=0; i<n; i++) sort(bones[i].begin(), bones[i].end());
  for (auto e = T.finite_edges_begin(); e != T.finite_edges_end(); ++e) {
    candidates.push_back(T.segment(e).squared_length());
  }
  sort(candidates.begin(), candidates.end());
  int initial_bones = find_bones(T, bones, n, s);
  int left = 0; 
  int right = candidates.size()-1;
  while(left < right) {
    int mid = (left+right)/2;
    if (find_bones(T, bones, n, candidates[mid]) >= k) right = mid;
    else left = mid + 1;
  }
  cout << initial_bones << " " << rounding(candidates[left]) << '\n';
}


int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for (int i=0; i<t; i++) testcase();
}
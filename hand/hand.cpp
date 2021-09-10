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


long rounding(const K::FT& x) {
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}



int compute_f(Triangulation &T, K::FT dist, int k, int n) {
  // Store in comp_sizes[i] the number of components of size i+1. For all components of size larger than
  // k, just store in comp_sizes[k-1].
  vector<int> comp_sizes(k);
  vector<bool> vis(n);
  for (auto v = T.finite_vertices_begin(); v != T.finite_vertices_end(); v++) {
    if (vis[v->info()]) continue;
    int size = 1;
    vis[v->info()] = true;
    queue<Triangulation::Vertex_handle> q;
    q.push(v);
    while (!q.empty()) {
      auto curr = q.front(); q.pop();
      auto next = T.incident_vertices(curr);
      do {
        if (!T.is_infinite(next) && !vis[next->info()] && CGAL::squared_distance(curr->point(), next->point()) < dist) {
          size++;
          q.push(next);
          vis[next->info()] = true;
        }
      } while(++next != T.incident_vertices(curr));
    }
    comp_sizes[min(size, k)-1]++;
  }
  // Compute the result
  if (k==1) return comp_sizes[0];
  if (k==2) return comp_sizes[1] + comp_sizes[0]/2;
  if (k==3) {
    if (comp_sizes[1] < comp_sizes[0]) return comp_sizes[2] + comp_sizes[1] + (comp_sizes[0]-comp_sizes[1])/3;
    else return comp_sizes[2] + comp_sizes[0] + (comp_sizes[1]-comp_sizes[0])/2;
  }
  else {
    int res = comp_sizes[3] + comp_sizes[1]/2 + min(comp_sizes[0], comp_sizes[2]);
    int rem2 = comp_sizes[1]%2;
    if (comp_sizes[0] < comp_sizes[2]) return res + (rem2+comp_sizes[2]-comp_sizes[0])/2;
    else return res + (comp_sizes[0]-comp_sizes[2] + 2*rem2)/4;
  }
}


void testcase() {
  int n, k, f;
  long s;
  cin >> n >> k >> f >> s;
  vector<pair<P, int>> points(n);
  for (int i=0; i<n; i++) {
    int x, y; cin >> x >> y;
    points[i] = {P(x, y), i};
  }
  Triangulation T;
  T.insert(points.begin(), points.end());
  int f0 = compute_f(T, s, k, n);
  
  vector<K::FT> candidates;
  for (auto e = T.finite_edges_begin(); e != T.finite_edges_end(); e++) candidates.push_back(T.segment(e).squared_length());
  sort(candidates.begin(), candidates.end());
  int left = 0;
  int right = candidates.size()-1;
  while (left < right) {
    int mid = (left+right)/2 + 1;
    if (compute_f(T, candidates[mid], k, n) >= f) left = mid;
    else right = mid-1;
  }
  cout << rounding(candidates[left]) << " " << f0 << '\n';
}

int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for (int i=0; i<t; i++) testcase();
}
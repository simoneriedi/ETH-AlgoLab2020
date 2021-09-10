#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <boost/pending/disjoint_sets.hpp>
#include <vector>
#include <utility>
#include<queue>
#include <algorithm>
#include <iostream>
using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K>   Vb;
typedef CGAL::Triangulation_face_base_with_info_2<K::FT, K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Triangulation;
typedef K::Point_2 P;



class Compare {
  public:
    bool operator() (pair<Triangulation::Face_handle, K::FT> f1, pair<Triangulation::Face_handle, K::FT> f2) {
      return f1.second < f2.second;
    }
};

void preprocessing(Triangulation &T) {
  priority_queue<pair<Triangulation::Face_handle, K::FT>, vector<pair<Triangulation::Face_handle, K::FT>>, Compare> q;
  // insert finite faces
  for (auto f = T.faces_begin(); f != T.faces_end(); f++) q.push({f, f->info()});
  // insert infinite faces
  auto c = T.incident_faces(T.infinite_vertex());
  do { q.push({c, c->info()});
  } while (++c != T.incident_faces(T.infinite_vertex()));
  // iterate the priority queue to update all faces
  while (!q.empty()) {
    auto curr = q.top(); q.pop();
    // Important: here we check if the inserted value is out-dated
    if (curr.first->info() > curr.second) continue;
    auto curr_face = curr.first;
    // Check if we can update any neighboring face
    for (int i=0; i<3; i++) {
      auto max_size = min(CGAL::squared_distance(curr_face->vertex((i+1)%3)->point(), curr_face->vertex((i+2)%3)->point()), curr_face->info());
      if (curr_face->neighbor(i)->info() < max_size) {
        curr_face->neighbor(i)->info() = max_size;
        q.push({curr_face->neighbor(i), max_size});
      }
    }
  }
}


void testcase(int n) {
  vector<P> points(n);
  for (int i=0; i<n; i++) {
    int x, y; cin >> x >> y;
    points[i] = P(x, y);
  }
  Triangulation T;
  T.insert(points.begin(), points.end());
  for (auto f = T.finite_faces_begin(); f != T.finite_faces_end(); f++) f->info() = 0;
  auto c = T.incident_faces(T.infinite_vertex());
  do { c->info() = numeric_limits<double>::max();
  } while (++c != T.incident_faces(T.infinite_vertex()));
  
  preprocessing(T);
  int m; cin >> m;
  for (int i=0; i<m; i++) {
    int x, y; K::FT d; cin >> x >> y >> d;
    P point(x, y);
    if (CGAL::squared_distance(T.nearest_vertex(point)->point(), point) < d) {
      cout << "n";
      continue;
    }
    auto f = T.locate(point);
    if (f->info() < 4*d) cout << "n";
    else cout << "y";
  }
  cout << '\n';
}



int main() {
  ios_base::sync_with_stdio(false);
  int n; cin >>n;
  while (n) {
    testcase(n);
    cin >> n;
  }
}
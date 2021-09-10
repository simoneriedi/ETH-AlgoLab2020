#include<iostream>
#include<vector>
#include<limits.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;



struct T {
  P p0, p1, p2, p3, p4, p5;
};


bool contains(P q, T t) {
  bool cond1 = (CGAL::orientation(t.p0, t.p1, q) == CGAL::orientation(t.p0, t.p1, t.p2)) || (CGAL::orientation(t.p0, t.p1, q)==CGAL::COLLINEAR);
  bool cond2 = (CGAL::orientation(t.p2, t.p3, q) == CGAL::orientation(t.p2, t.p3, t.p4)) || (CGAL::orientation(t.p2, t.p3, q)==CGAL::COLLINEAR);
  bool cond3 = (CGAL::orientation(t.p4, t.p5, q) == CGAL::orientation(t.p4, t.p5, t.p0)) || (CGAL::orientation(t.p4, t.p5, q)==CGAL::COLLINEAR);
  return cond1&&cond2&&cond3;
}

int update_count(vector<int> &legs, vector<int> &count, bool add) {
  int changes = 0;
  for (int i=0; i<(int)legs.size(); i++) {
    if (add) {
      changes += !count[legs[i]];
      count[legs[i]]++;
    }
    else {
      count[legs[i]]--;
      changes += !count[legs[i]];
    }
  }
  return changes;
}

void sliding_window(vector<vector<int>> &legs_in_map, int required) {
  int n = legs_in_map.size();
  int best_size = INT_MAX;
  int left = 0;
  int size = 0;
  vector<int> count(required, 0);
  while(left+size <= n) {
    if (!required) {
      best_size = min(size, best_size);
      required += update_count(legs_in_map[left], count, false);
      left++; size--;
    }
    else {
      if (left+size < n) required -= update_count(legs_in_map[left+size], count, true);
      size++;
    }
  }
  cout << best_size << '\n';
}

void testcase() {
  int m, n; cin >> m >> n;
  vector<P> points(m);
  for (int i=0; i<m; i++) {
    int x, y; cin >> x >> y;
    points[i] = P(x, y);
  }
  vector<vector<int>> legs_in_map(n, vector<int>());
  for (int i=0; i<n; i++) {
    int x0, y0, x1, y1, x2, y2, x3, y3, x4, y4, x5, y5;
    cin >> x0 >> y0 >> x1 >> y1 >> x2 >> y2 >> x3 >> y3 >> x4 >> y4 >> x5 >> y5;
    T map = {P(x0, y0), P(x1, y1), P(x2, y2), P(x3, y3), P(x4, y4), P(x5, y5)};
    bool pred = contains(points[0], map);
    for (int j=1; j<m; j++) {
      bool curr = contains(points[j], map);
      if (pred && curr) legs_in_map[i].push_back(j-1);
      pred = curr;
    }
  }
  sliding_window(legs_in_map, m-1);
  
}


int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for (int i=0; i<t; i++) testcase();
}
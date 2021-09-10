#include<iostream>
#include<vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Ray_2 R;


void testcase(int n) {
  long x, y, a, b;
  cin >> x >> y >> a >> b;
  R ray(P(x, y), P(a, b));
  bool intersect = false;
  for (int i=0; i<n; i++) {
    long r, s, t, u;
    cin >> r >> s >> t >> u;
    if (!intersect) {
      S seg(P(r, s), P(t, u));
      if (CGAL::do_intersect(seg, ray)) intersect = true;
    }
  }
  if (intersect) cout << "yes\n";
  else cout << "no\n";
}


int main() {
  ios_base::sync_with_stdio(false);
  int n; cin >> n;
  while (n) {
    testcase(n);
    cin >> n;
  }
}
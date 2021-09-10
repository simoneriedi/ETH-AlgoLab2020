#include<iostream>
#include<vector>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Ray_2 R;


long rounding(const K::FT& x) {
  double a = floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}


void testcase(int n) {
  long x, y, a, b; cin >> x >> y >> a >> b;
  vector<S> segments(n);
  for (int i=0; i<n; i++) {
    long a, b, c, d; cin >> a >> b >> c >> d;
    segments[i] = S(P(a, b), P(c, d));
  }
  random_shuffle(segments.begin(), segments.end());
  P source(x, y);
  R ray(source, P(a, b));
  S seg;
  bool intersect = false;
  P intersection;
  for (int i=0; i<n; i++) {
    if ((!intersect && CGAL::do_intersect(ray, segments[i])) || (intersect && CGAL::do_intersect(seg, segments[i]))) {
      auto o = CGAL::intersection(ray, segments[i]);
      if (const P* op = boost::get<P>(&*o)) intersection = *op;
      else if (const S* os = boost::get<S>(&*o)) {
        // two options for closest point
        if (CGAL::squared_distance(os->source(), source) < CGAL::squared_distance(os->target(), source)) {
          intersection = os->source();
        }
        else intersection = os->target();
      }
      intersect = true;
      seg = S(source, intersection);
    }
  }
  // Print result
  if (!intersect) cout << "no\n";
  else cout << rounding(intersection.x()) << " " << rounding(intersection.y()) << '\n';
}


int main() {
  ios_base::sync_with_stdio(false);
  int n; cin >>n;
  while (n) {
    testcase(n);
    cin >> n;
  }
}
#include<iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <vector>

using namespace std;


// typedefs
typedef  CGAL::Exact_predicates_exact_constructions_kernel K;
typedef  CGAL::Min_circle_2_traits_2<K>  Traits;
typedef  CGAL::Min_circle_2<Traits>      Min_circle;
typedef K::Point_2 P;



double ceil_to_double(const K::FT& x) {
  double a = std::ceil(CGAL::to_double(x));
  while (a-1 >= x) a -= 1;
  while (a < x) a += 1;
  return a;
}

void testcase(int n) {
  vector<P> points(n);
  for (int i=0; i<n; i++) {
    long x, y; cin >> x >> y;
    points[i] = P(x, y);
  }
  Min_circle mc(points.begin(), points.end(), true);
  Traits::Circle c = mc.circle();
  K::FT sq_radius = c.squared_radius();
  unsigned long left = 0;
  unsigned long right = 1UL << 50;
  while (left<right) {
    unsigned long mid = (left+right)/2;
    K::FT square = (K::FT)mid*(K::FT)mid;
    if (square < sq_radius) left = mid+1;
    else right = mid;
  }
  cout << left << '\n';
}




int main() {
  ios_base::sync_with_stdio(false);
  int n; cin >> n;
  while (n) {
    testcase(n);
    cin >> n;
  }
}
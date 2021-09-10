#include<iostream>
#include<vector>
#include<algorithm>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
typedef CGAL::Exact_predicates_exact_constructions_kernel K;

using namespace std;

struct Line {
  int index;
  long y;
  K::FT slope;
};

void testcase() {
  int n; cin >> n;
  vector<Line> bikers(n);
  for (int i=0; i<n; i++) {
    long y, x1, y1;
    cin >> y >> x1 >> y1;
    K::FT slope = ((K::FT)y1-(K::FT)y)/(K::FT)x1;
    bikers[i] = {i, y, slope};
  }
  sort(bikers.begin(), bikers.end(), [](auto b1, auto b2) {return b1.y > b2.y;});
  // Iterate bikers from top most down
  vector<Line> remaining;
  for (int i=0; i<n; i++) {
    int index = remaining.size() - 1;
    if (bikers[i].slope < 0) {
      // Negative slope => always add it, because it can only intersect with previous bikers whose slope is even more negative
      while (index>=0 && remaining[index].slope < bikers[i].slope) {
        index--;
        remaining.pop_back();
      }
      remaining.push_back(bikers[i]);
    }
    else {
      // Positive slope => New biker intersecates with all previous bikers with negative slope => keep removing them until
      // one has lower absolute value slope or it is positive. If previous biker is positive it will intersect only
      // if the new biker has a larger slope => we don't add it
      while (index>=0 && bikers[i].slope <= -remaining[index].slope) {
        index--;
        remaining.pop_back();
      }
      if (index<0 || remaining[index].slope >= bikers[i].slope) remaining.push_back(bikers[i]);
    }
  }
  sort(remaining.begin(), remaining.end(), [](auto b1, auto b2){return b1.index < b2.index;});
  for (int i=0; i<(int)remaining.size(); i++) cout << remaining[i].index << " ";
  cout << '\n';
}


int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for (int i=0; i<t; i++) testcase();
}
#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

struct Bev {
  int cost;
  int liters;
};
struct Info {
  int cost;
  int distinct;
  vector<bool> present;
};



void testcase() {
  int n, k; cin >> n >> k;
  vector<Bev> bev(n);
  for (int i=0; i<n; i++) {
    int c, l; cin >> c >> l;
    bev[i] = {c, l};
  }
  // Store here the best punch that can be done (according to problem) with i liters
  vector<Info> best_punch(k+1, {-1, 0, vector<bool>(n, false)});
  best_punch[0].cost = 0;
  for (int i=0; i<n; i++) {
    for (int j=0; j<k; j++) {
      // does adding to the best punch with j liters beverage i improve it?
      if (best_punch[j].cost < 0) continue; // Could no get here just yet
      int next = min(j+bev[i].liters, k);
      int new_cost = best_punch[j].cost + bev[i].cost;
      int old_cost = best_punch[next].cost;
      int new_distinct = best_punch[j].distinct + !best_punch[j].present[i];
      int old_distinct = best_punch[next].distinct;
      if (old_cost == -1 || old_cost > new_cost || (old_cost == new_cost && old_distinct < new_distinct)) {
        best_punch[next] = {new_cost, new_distinct, best_punch[j].present};
        best_punch[next].present[i] = true;
      }
    }
  }
  cout << best_punch[k].cost << " " << best_punch[k].distinct << '\n';
}






int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for (int i=0; i<t; i++) testcase();
}
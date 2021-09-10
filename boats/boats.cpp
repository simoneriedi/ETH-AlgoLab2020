#include<iostream>
#include<vector>
#include<limits.h>
#include<algorithm>

using namespace std;

struct Boat {
  int len;
  int pos;
};



int greedy(vector<Boat> &boats, int n) {
  int n_boats = 0;
  int curr_pos = INT_MIN;
  int next_pos = INT_MAX;
  for (int i=0; i<n; i++) {
    if (next_pos > boats[i].pos) next_pos = min(max(boats[i].pos, curr_pos+boats[i].len), next_pos);
    else {
      curr_pos = next_pos;
      next_pos = max(boats[i].pos, curr_pos+boats[i].len);
      n_boats++;
    }
  }
  return n_boats + (next_pos < INT_MAX);
}



void testcase() {
  int n; cin >> n;
  vector<Boat> boats(n);
  for (int i=0; i<n; i++) {
    int l, p; cin >> l >> p;
    boats[i] = {l, p};
  }
  sort(boats.begin(), boats.end(), [](auto b1, auto b2) {return b1.pos < b2.pos;});
  cout << greedy(boats, n) << '\n';
}



int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for (int i=0; i<t; i++) testcase();
}
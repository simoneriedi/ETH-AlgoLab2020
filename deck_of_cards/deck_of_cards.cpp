#include<iostream>
#include<vector>
#include<limits.h>

using namespace std;




void sliding_window(vector<int> &values, int k) {
  int n = values.size();
  pair<int, int> sol = {-1, -1};
  int best_abs = INT_MAX;
  int curr_sum = values[0];
  int left = 0;
  int size = 1;
  while (left + size <= n) {
    if (abs(curr_sum - k) < best_abs) {
      best_abs = abs(curr_sum - k);
      sol = {left, left+size-1};
    }
    if (curr_sum <= k) {
      if (left+size < n) curr_sum += values[left+size];
      size++;
    }
    else {
      curr_sum -= values[left];
      left++;
      size--;
    }
  }
 cout <<  sol.first << " " << sol.second << '\n';
}

void testcase() {
  int n, k; cin >> n >> k;
  vector<int> values(n);
  for (int i=0; i<n; i++) cin >> values[i];
  sliding_window(values, k);
}

int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for (int i=0; i<t; i++) testcase();
}
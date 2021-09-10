#include<iostream>
#include<vector>
#include<limits.h>
using namespace std;

vector<vector<int>> edges;
vector<int> cost;
vector<vector<int>> memo;

// Case 0: Current node is safe 
// Case 1: Current node is not safe but father is safe, because another node was forced to save it
// Case 2: Father is not safe and the current node is forced to save it
int dp(int curr, int curr_case) {
  if (memo[curr][curr_case] != -1) return memo[curr][curr_case];
  int out_going = edges[curr].size();
  if (!out_going) return (curr_case==0) ? 0 : cost[curr];
  // Should always consider the case where the current node is repaired
  int res = cost[curr];
  for (int i=0; i<out_going; i++) res += dp(edges[curr][i], 0);
  if (curr_case == 1) {
    int min_diff = INT_MAX;
    int index = -1;
    for (int i=0; i<out_going; i++) {
      int diff = dp(edges[curr][i], 2) - dp(edges[curr][i], 1);
      if (diff < min_diff) { min_diff = diff; index = i;}
    }
    int res1 = 0;
    for (int i=0; i<out_going; i++) res1 += dp(edges[curr][i], 1 + (i==index));
    res = min(res1, res);
  }
  if (curr_case == 0) {
    int res1 = 0;
    for (int i=0; i<out_going; i++) res1 += dp(edges[curr][i], 1);
    res = min(res1, res);
  }
  memo[curr][curr_case] = res;
  return res;
}



void testcase() {
  int n; cin >> n;
  edges = vector<vector<int>>(n, vector<int>());
  cost = vector<int>(n);
  memo = vector<vector<int>>(n, vector<int>(3, -1));
  for (int i=0; i<n-1; i++) {
    int from, to; cin >> from >> to;
    edges[from].push_back(to);
  }
  for (int i=0; i<n; i++) cin >> cost[i];
  cout << dp(0, 1) << '\n';
}


int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for (int i=0; i<t; i++) testcase();
}
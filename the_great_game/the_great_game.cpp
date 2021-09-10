#include<iostream>
#include<vector>
#include<limits.h>

using namespace std;

int n, m;
vector<vector<int>> edges;
vector<vector<int>> memo;

int dp(int pos, bool maximize) {
  if (pos == (int)edges.size() - 1) return 0;
  if (memo[pos][maximize] != -1) return memo[pos][maximize];
  int res = maximize ? -1 : INT_MAX; // Works because we can assume that every non-target position has at least one outgoing edge
  for (int i=0; i<(int)edges[pos].size(); i++) {
    res = maximize ? max(1+dp(edges[pos][i], false), res) : min(1+dp(edges[pos][i], true), res);
  }
  memo[pos][maximize] = res;
  return res;
}


void testcase() {
  int holmes, moriarty; 
  cin >> n >> m >> holmes >> moriarty;
  holmes--; moriarty--;
  edges = vector<vector<int>>(n, vector<int>());
  memo = vector<vector<int>>(n, vector<int>(2, -1));
  for (int i=0; i<m; i++) {
    int from, to; cin >> from >> to;
    from--; to--;
    edges[from].push_back(to);
  }
  int holmes_turns = dp(holmes, false);
  int moriarty_turns = dp(moriarty, false);
  if (holmes_turns < moriarty_turns || (holmes_turns == moriarty_turns && holmes_turns%2)) cout << 0 << '\n';
  else cout << 1 << '\n';
  
}

int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for (int i=0; i<t; i++) testcase();
}
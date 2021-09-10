#include<iostream>
#include<vector>
#include<limits.h>

using namespace std;




vector<vector<long>> memo;
vector<vector<pair<int,long>>> edges;
int moves;


// return maximum score in given moves
long dp(int pos, int moves_left) {
  if (!moves_left) return 0;
  if (memo[pos][moves_left] != -1) return memo[pos][moves_left];
  int out_going = edges[pos].size();
  long res = -1;
  if (!out_going) res = dp(0, moves_left);
  else {
    for (int i=0; i<out_going; i++) {
      res = max(res, edges[pos][i].second + dp(edges[pos][i].first, moves_left-1));
    }
  }
  memo[pos][moves_left] = res;
  return res;
}

void testcase() {
  int n, m;
  long score;
  cin >> n >> m >> score >> moves;
  edges = vector<vector<pair<int,long>>>(n, vector<pair<int,long>>());
  for (int i=0; i<m; i++) {
    int from, to;
    long val; 
    cin >> from >> to >> val;
    edges[from].push_back({to, val});
  }
  memo = vector<vector<long>>(n, vector<long> (moves+1, -1));
  long max_score = dp(0, moves);
  if (max_score < score) {
    cout << "Impossible" << '\n';
    return;
  }
  int left = 1;
  int right = moves;
  while (left < right) {
    int mid = (left+right)/2;
    if (dp(0, mid) >= score) right = mid;
    else left = mid+1;
  }
  cout << left << '\n';
}

int main() {
  int t; cin >> t;
  for (int i=0; i<t; i++) testcase();
}
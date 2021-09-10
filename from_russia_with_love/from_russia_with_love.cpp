#include<vector>
#include<iostream>
using namespace std;


int n, m, k;
vector<int> values;
vector<vector<int>> memo;

int dp(int curr, int left, int right) {
  if (left > right) return 0;
  if (memo[left][right] != -1) return memo[left][right];
  int sol1 = dp((curr+1)%m, left+1, right);
  int sol2 = dp((curr+1)%m, left, right-1);
  int sol = (curr == k) ? max(sol1 + values[left], sol2 + values[right]) : min(sol1, sol2);
  memo[left][right] = sol;
  return sol;
}

void testcase() {
  cin >> n >> m >> k;
  values = vector<int>(n);
  memo = vector<vector<int>>(n, vector<int>(n, -1));
  for (int i=0; i<n; i++) cin >> values[i];
  cout << dp(0, 0, n-1) << '\n';
}




int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for (int i=0; i<t; i++) testcase();
}
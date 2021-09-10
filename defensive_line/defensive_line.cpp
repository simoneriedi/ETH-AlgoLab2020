#include<vector>
#include<iostream>
#include<limits.h>
using namespace std;


int n, m, k;
vector<int> sum;
vector<vector<int>> memo;

// Find position of next defender
int find_next(int curr) {
  int to_find = k + (curr!=0)*sum[curr-1];
  auto p = lower_bound(sum.begin(), sum.end(), to_find);
  if (p==sum.end() || *p != to_find) return -1;
  return 1 + p - sum.begin();
}


int dp(int curr, int att_left) {
  if (curr == (int)sum.size() || !att_left) return (!att_left) ? 0 : INT_MIN;
  if (memo[curr][att_left] != -1) return memo[curr][att_left];
  int res = dp(curr+1, att_left);
  int next = find_next(curr);
  if (next != -1) res = max(res, next-curr + dp(next, att_left-1));
  memo[curr][att_left] = res;
  return res;
}

void testcase() {
  cin >> n >> m >> k;
  sum = vector<int>(n);
  memo = vector<vector<int>>(n, vector<int>(m+1, -1));
  int pred = 0;
  for (int i=0; i<n; i++) {
    cin >> sum[i];
    sum[i] += pred;
    pred = sum[i];
  }
  int res = dp(0, m);
  if (res < 0) cout << "fail\n";
  else cout << res << '\n';
}





int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for (int i=0; i<t; i++) testcase();
}
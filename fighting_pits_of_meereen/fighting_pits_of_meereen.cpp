#include<iostream>
#include<vector>
#include<limits.h>
using namespace std;

int n, k, m;
vector<int> values;
vector<vector<vector<vector<vector<vector<int>>>>>> memo;


int compute_fun(int abs_diff, int val1, int val2, int val3) {
  int res = 0;
  vector<bool> present(k);
  if (m==3 && val1 >= 0 && !present[val1]) {res += 1000; present[val1] = true;}
  if (val2 >= 0 && !present[val2]) {res += 1000; present[val2] = true;}
  if (!present[val3]) res += 1000;
  int count = 1;
  while (abs_diff > 0) {
    count *= 2;
    abs_diff--;
    if (res - count < 0) return -1;
  }
  return res - count;
}


int dp(int curr, int size1, int size2, int l1, int l2, int r1, int r2) {
  if (curr == n) return 0;
  if (abs(size1-size2) > 11) return INT_MIN;
  int diff = (size1-size2 < 0) ? min(size2-size1, 11) + 11 : min(size1-size2, 11);
  bool can_access = (m==2) ? l2 != -1 && r2 != -1 : l1 != -1 && l2 != -1 && r1!= -1 && r2 != -1;
  if (can_access && memo[curr][diff][l1][l2][r1][r2] != -1) return memo[curr][diff][l1][l2][r1][r2];
  // Compute fun value for the two alternatives for the current fighter
  int res1 = compute_fun(abs(size1+1-size2), l1, l2, values[curr]);
  int res2 = compute_fun(abs(size1-1-size2), r1, r2, values[curr]);
  if (res1 > 0) res1 += dp(curr+1, size1+1, size2, (m==2) ? 0 : l2, values[curr], r1, r2);
  if (res2 > 0) res2 += dp(curr+1, size1, size2+1, l1, l2, (m==2) ? 0 : r2, values[curr]);
  int res = max(res1, res2);
  if (can_access) memo[curr][diff][l1][l2][r1][r2] = res;
  return res;
}

void testcase() {
  cin >> n >> k >> m;
  values = vector<int>(n);
  for (int i=0; i<n; i++) cin >> values[i];
  memo = vector<vector<vector<vector<vector<vector<int>>>>>>(n, vector<vector<vector<vector<vector<int>>>>>(23, 
       vector<vector<vector<vector<int>>>> (k, vector<vector<vector<int>>>(k, vector<vector<int>>(k, vector<int>(k, -1))))));
  
  if (m==2) cout << dp(0, 0, 0, 0, -1, 0, -1) << '\n';
  else cout << dp(0, 0, 0, -1, -1, -1, -1) << '\n';
}


int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for (int i=0; i<t; i++) testcase();
}
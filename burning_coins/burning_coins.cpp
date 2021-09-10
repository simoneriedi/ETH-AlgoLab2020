#include<iostream>
#include<vector>


using namespace std;

int solve(vector<vector<int>> &memo, vector<int> &coins, int n, int left, int right) {
  if(left==right) {
    return coins[left];
  }
  else if (left>right) {
    return 0;
  }
  else if (memo[left][right] != -1) {
    return memo[left][right];
  }
  int res1 = min(solve(memo,coins,n,left+2,right), solve(memo,coins,n,left+1,right-1)) + coins[left];
  int res2 = min(solve(memo,coins,n,left,right-2), solve(memo,coins,n,left+1,right-1)) + coins[right];
  memo[left][right] = max(res1,res2);
  return memo[left][right];
}

void testcase() {
  int n; cin>>n;
  vector<int> coins(n);
  for (int i=0;i <n; i++) {
    int el; cin>>el;
    coins[i] = el;
  }
  vector<vector<int>> memo(n,vector<int>(n,-1));
  //for (int i=0; i<n; i++) {
  //  for (int j=0; j<n; j++) {
  //    cout << memo[i][j] << '\t';
  //  }
  //  cout << '\n';
  //}
  cout << solve(memo, coins, n, 0, n-1) << '\n';
}
int main() {
  ios_base::sync_with_stdio(false);
  int t; cin>>t;
  for (int i=0; i< t; i++) {
    testcase();
  }
}
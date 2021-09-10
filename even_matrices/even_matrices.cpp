#include<vector>
#include<iostream>

using namespace std;

// Find all solutions blocks where j1 = from and j2 = to from the problem description
int solve_between(vector<vector<int>> &sum, int from, int to) {
  int n = sum.size();
  int evens = 0;
  int pred = 0;
  for (int i=0; i<n; i++) {
    int val = sum[i][to] - (from != 0)*sum[i][from-1] + pred;
    pred = val;
    evens += (val%2==0);
  }
  int odds = n-evens;
  return (evens*(evens-1))/2 + evens + (odds*(odds-1))/2;
}


int solve(vector<vector<int>> &sum) {
  int n = sum.size();
  int res = 0;
  for (int j1=0; j1<n; j1++) {
    for (int j2=j1; j2<n; j2++) {
      res += solve_between(sum, j1, j2);
    }
  }
  return res;
}

void testcase() {
  int n; cin >> n;
  vector<vector<int>> matrix(n, vector<int>(n));
  vector<vector<int>> sum(n, vector<int>(n)); // This is a precomputation vector
  for (int i=0; i<n; i++) {
    for (int j=0; j<n; j++) {
      cin >> matrix[i][j];
      sum[i][j] = matrix[i][j] + (j!=0)*sum[i][j-1];
    }
  }
  cout << solve(sum) << '\n';
}




int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for (int i=0; i<t; i++) testcase();
}
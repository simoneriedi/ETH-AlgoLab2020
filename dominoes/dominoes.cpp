#include<iostream>
#include<stack>
#include<algorithm>
#include<vector>
#include<utility>


int domino(std::vector<int> v) {
  int s = 1;
  int n = v.size();
  int credit = v[0]-1;
  for (int i=1;i<n; i++) {
    if (credit > 0) {
      s++;
      credit = std::max(credit, v[i]) -1;
    }
  }
  return s;
}

void testcase() {
  int n; std::cin>>n;
  std::vector<int> v(n);
  for (int i=0;i<n;i++) {
    int k; std::cin>>k;
    v[i] = k;
  }
  std::cout << domino(v) << '\n';
}


int main() {
  int t; std::cin>>t;
  for (int i=0;i<t;i++) testcase();
}
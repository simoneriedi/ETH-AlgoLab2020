#include<iostream>
#include<stack>
#include<algorithm>
#include<vector>
#include<utility>




void testcase() {
  int n; std::cin>>n;
  int sum = 0;
  for (int i=0;i<n;i++) {
    int k; std::cin>>k;
    sum += k;
  }
  std::cout << sum << '\n';
}


int main() {
  int t; std::cin>>t;
  for (int i=0;i<t;i++) testcase();
}
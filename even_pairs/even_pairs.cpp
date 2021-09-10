#include<iostream>
#include<stack>
#include<algorithm>
#include<vector>
#include<utility>


int even_pairs(std::vector<int> s) {
  int n = s.size();
  int evens = 0;
  for (int i=0; i<n;i++) {
    evens += (s[i]%2==0);
  }
  int odds = n-evens;
  int pairs = evens + (evens*(evens-1))/2 + (odds*(odds-1))/2;
  return pairs;
}

void testcase() {
  int n; std::cin>>n;
  std::vector<int> v(n);
  for (int i=0;i<n;i++) {
    int k; std::cin>>k;
    v[i] = k;
  }
  std::vector<int> s(n);
  s[0] = v[0];
  for (int i=1;i<n;i++) {
    s[i] = v[i]+s[i-1];
  }
  std::cout << even_pairs(s) << '\n';
}


int main() {
  int t; std::cin>>t;
  for (int i=0;i<t;i++) testcase();
}
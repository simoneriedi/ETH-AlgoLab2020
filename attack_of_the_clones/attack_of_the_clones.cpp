#include<vector>
#include<iostream>
#include<map>
#include<limits.h>
#include<algorithm>
using namespace std;


struct Jedi {
  int from;
  int to;
};


void testcase() {
  int n, m; cin >> n >> m;
  vector<Jedi> jedis(n);
  map<int, int> mapping;
  for (int i=0; i<n; i++) {
    int a, b; cin >> a >> b;
    a--; b--;
    jedis[i] = {a, b};
    mapping[a]++;
    mapping[(b+1)%m]--;
  }
  int s = -1;
  int c = INT_MAX;
  int curr = 0;
  for (auto val : mapping) {
    curr += val.second;
    if (curr < c) {
      c = curr;
      s = val.first;
    }
  }
  vector<Jedi> segments;
  vector<Jedi> extra;
  for (int i=0; i<n; i++) {
    jedis[i] = {(jedis[i].from-s+m)%m, (jedis[i].to-s+m)%m};
    if (jedis[i].from <= jedis[i].to) segments.push_back(jedis[i]);
    else extra.push_back(jedis[i]);
  }
  
  sort(segments.begin(), segments.end(), [](auto j1, auto j2){return j1.to < j2.to;});
  int count = 0;
  int start = -1;
  for (int i=0; i<(int)segments.size(); i++) {
    if (segments[i].from > start) {
      start = segments[i].to;
      count++;
    }
  }
  for (int i=0; i<(int)extra.size(); i++) {
    if (!segments.size() || (extra[i].from > start && extra[i].to < segments[0].from)) {
      count++;
      break;
    }
  }
  cout << count << '\n';
}





int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for (int i=0; i<t; i++) testcase();
}
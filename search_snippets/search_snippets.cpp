#include<vector>
#include<iostream>
#include<algorithm>
#include<limits.h>

using namespace std;

struct Val {
  int word;
  int pos;
};

vector<Val> values;


void sliding_window(int required) {
  int best_size = INT_MAX;
  vector<int> count(required, 0);
  int left = 0;
  int size = 0;
  while (left + size <= (int)values.size()) {
    if (!required) {
      best_size = min(values[left+size-1].pos-values[left].pos + 1, best_size);
      count[values[left].word]--;
      required += !count[values[left].word];
      size--; left++;
    }
    else {
      if (left+size < (int)values.size()) {
        required -= !count[values[left+size].word];
        count[values[left+size].word]++;
      }
      size++;
    }
  }
  cout << best_size << '\n';
}



void testcase() {
  int n; cin >> n;
  vector<int> occ(n);
  for (int i=0; i<n; i++) cin >> occ[i];
  values = vector<Val>();
  for (int i=0; i<n; i++) {
    for (int j=0; j<occ[i]; j++) {
      int pos; cin >> pos;
      values.push_back({i, pos});
    }
  }
  sort(values.begin(), values.end(), [](auto v1, auto v2){return v1.pos < v2.pos;});
  sliding_window(n);
}


int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for (int i=0; i<t; i++) testcase();
}
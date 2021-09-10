#include<vector>
#include<iostream>
#include<algorithm>
#include<utility>
using namespace std;

int detonate(int index, vector<bool> &detonated, int n) {
  if (detonated[index]) return 0;
  detonated[index] = true;
  if (index >= (n-1)/2) return 1;
  else return 1 + detonate(2*index+1, detonated, n) + detonate(2*index+2, detonated, n);
}

struct Bomb {
  int index;
  int exp_time;
};

void testcase() {
  int n; cin >> n;
  vector<Bomb> bombs(n);
  for (int i=0; i<n; i++) {
    cin >> bombs[i].exp_time;
    bombs[i].index = i;
  }
  sort(bombs.begin(), bombs.end(), [](auto b1, auto b2){return b1.exp_time < b2.exp_time;});
  vector<bool> detonated(n);
  int curr_time = 0;
  bool explodes = false;
  for (int i=0; i<n; i++) {
    curr_time += detonate(bombs[i].index, detonated, n);
    if (curr_time > bombs[i].exp_time) explodes = true;
  }
  if(explodes) cout << "no\n";
  else cout << "yes\n";
}


int main() {
  ios_base::sync_with_stdio(false);
  int t; cin>>t;
  for (int i=0; i<t; i++) testcase();
}



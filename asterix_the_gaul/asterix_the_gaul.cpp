#include<vector>
#include<algorithm>
#include<iostream>

using namespace std;




struct M {
  long d;
  long t;
};


// Return all subsets of moves
vector<M> get_sums(vector<M> &moves, long extra) {
  int n = moves.size();
  vector<M> res;
  for (int s = 0; s < 1<<n; ++s) { // Iterate through all subsets
    long sum_d = 0;
    long sum_t = 0;
    for (int i = 0; i < n; ++i) {
      if (s & 1<<i) {
        sum_d += moves[i].d+extra; 
        sum_t += moves[i].t;
      }
    }
    res.push_back({sum_d, sum_t});
  }
  return res;
}


bool can_do(long D, long T, vector<M> &moves, long extra) {
  int n = moves.size();
  int mid = n/2+1;
  vector<M> moves1(moves.begin(), moves.begin()+mid);
  vector<M> moves2(moves.begin()+mid, moves.end());
  // Get sums for both moves
  vector<M> sums1 = get_sums(moves1, extra);
  vector<M> sums2 = get_sums(moves2, extra);
  // Order sums 2 from smallest distance to largest
  sort(sums2.begin(),sums2.end(), [](auto m1, auto m2){return m1.d<m2.d;});
  for (int i=(int)sums2.size()-2; i>=0; i--) sums2[i].t = min(sums2[i].t, sums2[i+1].t);
  // Iterate sums1 for a match
  for (int i=0; i<(int)sums1.size(); i++) {
    long curr_time = sums1[i].t;
    if (curr_time >= T) continue;
    long required_dist = D - sums1[i].d;
    if (required_dist <= 0) return true;
    M target = {required_dist, 0};
    auto p = lower_bound(sums2.begin(), sums2.end(), target, [](auto m1, auto m2){return m1.d < m2.d;});
    if (p == sums2.end()) continue;
    int index = p - sums2.begin();
    if (sums2[index].t + curr_time < T) return true;
  }
  return false;
}

void binary_search(long D, long T, vector<M> &moves, vector<long> &potion) {
  int m = potion.size();
  
  int left = 0;
  int right = m-1;
  if (!can_do(D, T, moves, potion[right])) {
    cout << "Panoramix captured\n";
    return;
  }
  while (left < right) {
    int mid = (left+right)/2;
    if (can_do(D, T, moves, potion[mid])) right = mid;
    else left = mid+1;
  }
  cout << left << '\n';
}




void testcase() {
  int n, m;
  long D, T;
  cin >> n >> m >> D >> T;
  vector<M> moves(n);
  for (int i=0; i<n; i++) {
    long d, t; cin >> d >> t;
    moves[i] = {d, t};
  }
  vector<long> potion(m+1, 0);
  for (int i=0; i<m; i++) cin >> potion[i+1];
  binary_search(D, T, moves, potion);
}




int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for (int i=0; i<t; i++) testcase();
}
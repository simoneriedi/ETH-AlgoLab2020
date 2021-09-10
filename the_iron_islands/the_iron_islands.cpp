#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

void print_vec(vector<pair<int,int>> &v) {
  for (int i=0; i< (int)v.size(); i++) {
    cout << "[" << v[i].first << " ," << v[i].second << "]  ";
  }
  cout << '\n';
}


// Sliding along the given waterway 
int sliding_window(int k, vector<int> &values) {
  int n = values.size();
  reverse(values.begin(), values.end());
  int best_size = -1;
  int left = 0;
  int size = 1;
  int curr = values[left];
  while (left+size <= n) {
    if (curr < k) {
      if (left + size < n) curr += values[left + size];
      size++;
    }
    else if (curr > k) {
      curr -= values[left];
      left++;
      size--;
    }
    else {
      best_size = max(best_size, size);
      curr -= values[left];
      left++;
      size--;
    }
  }
  reverse(values.begin(), values.end());
  return best_size;
}


int solve(int k, vector<vector<int>> &waterways) {
  int max_islands = 0;
  for (int i=0; i<(int)waterways.size(); i++) {
    max_islands = max(max_islands, sliding_window(k, waterways[i]));
  }
  vector<pair<int,int>> most_islands(k, {-1, -1});
  vector<pair<int,int>> second_most_islands(k, {-1, -1});
  for (int i=0; i<(int)waterways.size(); i++) {
    int men = 0;
    int in = 1;
    while (men < k && in <= (int)waterways[i].size()) {
      if (most_islands[men].first < in-1) most_islands[men] = {in-1, i};
      else if (second_most_islands[men].first < in-1) second_most_islands[men] = {in-1, i};
      if (in < (int)waterways[i].size()) men += waterways[i][in];
      in++;
    }
  }
  int base_men = waterways[0][0];
  for (int i=1; i<k; i++) {
    if (k-i-base_men < 0) continue;
    if (most_islands[i].second != most_islands[k-i-base_men].second) {
      if (most_islands[i].first != -1 && most_islands[k-i-base_men].first != -1) {
        max_islands = max(max_islands, 1 + most_islands[i].first + most_islands[k-i-base_men].first);
      }
    }
    else {
      if (second_most_islands[i].first != -1) {
        max_islands = max(max_islands, 1 + second_most_islands[i].first + most_islands[k-i-base_men].first);
      }
      if (second_most_islands[k-i-base_men].first != -1) {
        max_islands = max(max_islands, 1 + most_islands[i].first + second_most_islands[k-i-base_men].first);
      }
    }
  }
  return max_islands;
}







void testcase() {
  int n, k, w;
  cin >> n >> k >> w;
  vector<int> men(n);
  for (int i=0; i<n; i++) cin >> men[i];
  vector<vector<int>> waterways(w, vector<int>());
  for (int i=0; i<w; i++) {
    int l; cin >> l;
    for (int j=0; j<l; j++) {
      int island; cin >> island;
      waterways[i].push_back(men[island]);
    }
  }
  cout << solve(k, waterways) << '\n';
}






int main() {
  int t; cin >> t;
  for (int i=0; i<t; i++) testcase();
}
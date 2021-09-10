#include<iostream>
#include<vector>
#include<limits.h>
#include<algorithm>

using namespace std;
int n;
vector<int> parasols;


void sliding_window() {
  int n = parasols.size();
  // Variables corresponding to the output
  vector<int> centers;
  int max_parasols = -1;
  int min_distance = INT_MAX;
  // Sliding Window helper variables
  int left = 0;
  int size = 1;
  while (left + size <= n) {
    int c = (parasols[left+size-1] + parasols[left])/2;
    int dist = max(c-parasols[left], parasols[left+size-1]-c);
    if (parasols[left+size-1] - parasols[left] > 200) {left++; size--;}
    else {
      if (size > max_parasols || (size == max_parasols && dist < min_distance)) centers.clear();
      if (size > max_parasols || (size == max_parasols && dist <= min_distance)) {
        max_parasols = size;
        min_distance = dist;
        // Check if we can also add the other two possible centers that arise from rounding
        if (max(c-1-parasols[left], parasols[left+size-1]-c+1) == min_distance) centers.push_back(c-1);
        centers.push_back(c);
        if (max(c+1-parasols[left], parasols[left+size-1]-c-1) == min_distance) centers.push_back(c+1);
        // Note: This already checks that we don't "lose" parasols by schifting the center, because that happens only if the distance
        // to the furthest parasol increases
      }
      size++;
    }
  }
  cout << max_parasols << " " << min_distance << '\n';
  for (int i=0; i<(int)centers.size(); i++) cout << centers[i] << " ";
  cout << '\n';
}




void testcase() {
  cin >> n;
  parasols = vector<int>(n);
  for (int i=0; i<n; i++) cin >> parasols[i];
  sort(parasols.begin(), parasols.end());
  sliding_window();
}






int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for (int i=0; i<t; i++) testcase();
}
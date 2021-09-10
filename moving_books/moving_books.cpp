#include<iostream>
#include<vector>
#include<algorithm>
#include<queue>





using namespace std;




void testcase() {
  int n, m; cin >> n >> m;
  // Save input
  vector<int> friends(n);
  vector<int> boxes(m);
  for (int i=0; i<n; i++) cin >> friends[i];
  for (int i=0; i<m; i++) cin >> boxes[i];
  // Sort both lists
  sort(friends.begin(), friends.end());
  sort(boxes.begin(), boxes.end(), [](int b1, int b2) {return b1 > b2;});
  
  // Iterate boxes from heaviest to lightest and add friends to a priority_queue
  if (friends[n-1] < boxes[0]) {
    cout << "impossible" << '\n';
    return;
  }
  priority_queue<int, vector<int>, greater<int>> q;
  int last_contained = n;
  int max_boxes = -1;
  for (int i=0; i<m; i++) {
    // Look if new box can be lifted by additional friends
    auto p = lower_bound(friends.begin(), friends.end(), boxes[i]); // First element NOT smaller
    int curr = p - friends.begin();
    for (int i=curr; i<last_contained; i++) q.push(0);
    last_contained = curr;
    // Give the box to friend that carried least amount of boxes so far
    int val = q.top(); q.pop();
    max_boxes = max(val+1, max_boxes);
    q.push(val+1);
  }
  cout << 3*max_boxes-1 << '\n';
}


int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for (int i=0; i<t; i++) testcase();
}









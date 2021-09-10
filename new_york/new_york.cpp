#include<set>
#include<vector>
#include<iostream>
using namespace std;


void testcase() {
  int n, m, k; cin >> n >> m >> k;
  vector<int> temp(n);
  for (int i=0; i<n; i++) cin >> temp[i];
  vector<bool> is_leaf(n, true);
  vector<int> parent(n, -1);
  for (int i=0; i<n-1; i++) {
    int from, to; cin >> from >> to;
    parent[to] = from;
    is_leaf[from] = false;
  }
  
  vector<bool> res(n, false);
  vector<bool> vis(n, false);
  for (int i=0; i<n; i++) {
    if (!is_leaf[i]) continue;
    // Go up this path
    int tail = i;
    int size = 0;
    int next = i;
    multiset<int> h;
    while (!vis[tail]) {
      h.insert(temp[next]); 
      size++;
      if (*(h.rbegin()) - *(h.begin()) > k) {
        h.erase(h.find(temp[tail]));
        vis[tail] = true;
        tail = parent[tail];
        size--;
      }
      else if (size == m) {
        h.erase(h.find(temp[tail]));
        vis[tail] = true;
        res[next] = true;
        tail = parent[tail];
        size--;
      }
      // Reached the root, keep removing and marking as visited
      if (parent[next] == -1) vis[tail] = true;
      else next = parent[next];
    }
  }
  bool exists = false;
  for (int i=0; i<n; i++) {
    if (res[i]) {
      cout << i << " ";
      exists = true;
    }
  }
  exists ? cout << '\n' : cout << "Abort mission\n";
}



int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for (int i=0; i<t; i++) testcase();
}
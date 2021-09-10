#include<vector>
#include<iostream>
#include<algorithm>
using namespace std;

vector<vector<int>> edges;
vector<int> dfs_tag;
vector<int> low;
vector<vector<int>> dfs_tree;
vector<int> father;
int number;

int dfs(int current) {
  dfs_tag[current] = number;
  low[current] = number;
  number++;
  for (int i=0; i<(int)edges[current].size(); i++) {
    // Launch dfs for all neighboring vertices
    int next = edges[current][i];
    // Skip visit to my father in the dfs tree
    if (father[current] == next) continue;
    // Already been visited, see if we can take his low value
    if (dfs_tag[next] != -1) low[current] = min(low[current], low[next]);
    else {
      father[next] = current;
      low[current] = min(low[current], dfs(next));
      dfs_tree[current].push_back(next);
    }
  }
  return low[current];
}

void print_solution(int n) {
  int n_bridges = 0;
  vector<vector<int>> bridges(n, vector<int>());
  for (int i=0; i<n; i++) {
    for (int j=0; j<(int)dfs_tree[i].size(); j++) {
      int u = dfs_tree[i][j];
      bool cond = (dfs_tag[i] < dfs_tag[u]) ? low[u] > dfs_tag[i] : dfs_tag[u] < low[i];
      if (cond) {
        n_bridges++;
        bridges[min(i, u)].push_back(max(i, u));
      }
    }
  }
  cout << n_bridges << '\n';
  for (int i=0; i<n; i++) {
    sort(bridges[i].begin(), bridges[i].end());
    for (int j=0; j<(int)bridges[i].size(); j++) cout << i << " " << bridges[i][j] << '\n';
  }
}



void testcase() {
  int n, m; cin >> n >> m;
  edges = vector<vector<int>>(n, vector<int>());
  dfs_tag = vector<int>(n, -1);
  low = vector<int>(n);
  father = vector<int>(n, -1);
  dfs_tree = vector<vector<int>>(n, vector<int>());
  number = 0;
  for (int i=0; i<m; i++) {
    int a, b; cin >> a >> b;
    edges[a].push_back(b);
    edges[b].push_back(a);
  }
  if (!n) {
    cout << 0 << '\n';
    return;
  }
  dfs(0);
  print_solution(n);
}






int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for (int i=0; i<t; i++) testcase();
}
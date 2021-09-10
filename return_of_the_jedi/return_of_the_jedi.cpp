#include<iostream>
#include<vector>
#include<queue>
#include<utility>
#include<tuple>
#include<limits.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

using namespace std;

vector<vector<int>> kruskal(const weighted_graph &G, int n) {
  std::vector<edge_desc> mst;    // vector to store MST edges (not a property map!)
  boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
  vector<vector<int>> res(n, vector<int>());
  for (std::vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); ++it) {
    res[boost::source(*it, G)].push_back(boost::target(*it, G));
    res[boost::target(*it, G)].push_back(boost::source(*it, G));
  }
  return res;
}



void dfs(int curr, int prev, int mark_from, int max_on_path, vector<vector<int>> &edges, vector<vector<int>> &mst, vector<vector<int>> &largest_on_path) {
  for (int i=0; i<(int)mst[curr].size(); i++) {
    int next = mst[curr][i];
    if (next == prev) continue;
    int new_max = max(max_on_path, edges[curr][next]);
    largest_on_path[mark_from][next] = new_max;
    dfs(next, curr, mark_from, new_max, edges, mst, largest_on_path);
  }
}


void testcase() {
  int n, start;
  cin >> n >> start;
  vector<vector<int>> edges = vector<vector<int>>(n, vector<int>(n));
  weighted_graph G(n);
  weight_map weights = boost::get(boost::edge_weight, G);
  edge_desc e;
  for (int j=1; j<n; j++) {
    for (int k=1; k<=n-j; k++) {
      int cost; cin >> cost;
      edges[j-1][k+j-1] = cost;
      edges[k+j-1][j-1] = cost;
      e = boost::add_edge(j-1, k+j-1, G).first; weights[e]=cost;
    }
  }
  vector<vector<int>> mst = kruskal(G, n);
  int mst_value = 0;
  vector<vector<bool>> exists(n, vector<bool>(n, false));
  for (int i=0; i<n; i++) {
    for (int j=0; j<(int)mst[i].size(); j++) {
      exists[i][mst[i][j]] = true;
      exists[mst[i][j]][i] = true;
      mst_value += edges[i][mst[i][j]];
    }
  }
  
  vector<vector<int>> largest_on_path(n, vector<int>(n, 0));
  for (int i=0; i<n; i++) dfs(i, -1, i, -1, edges, mst, largest_on_path);
  int min_diff = INT_MAX;
  for (int i=0; i<n; i++) {
    for (int j=i+1; j<n; j++) {
      if (i != j && !exists[i][j]){
        min_diff = min(min_diff, edges[i][j] - largest_on_path[i][j]);
      } 
    }
  }
  cout << mst_value/2 + min_diff << '\n';
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for (int i=0; i<t; i++) testcase();
}
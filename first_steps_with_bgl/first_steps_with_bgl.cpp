#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>


using namespace std;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property,             
  boost::property<boost::edge_weight_t, int>> weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;


int dijkstra_max_dist(const weighted_graph &G, int s) {
  int n = boost::num_vertices(G);
  std::vector<int>         dist_map(n);

  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));
  
  int max_dist = -1;
  for (int i=0; i<n; i++) max_dist = max(max_dist, dist_map[i]);
  return max_dist;
}

int kruskal_cost(const weighted_graph &G, weight_map &weights) {
  std::vector<edge_desc> mst;    // vector to store MST edges (not a property map!)
  boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
  int cost = 0;
  for (std::vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); ++it) cost += weights[*it];
  return cost;
}

void testcase() {
  int n, m; cin >> n >> m;
  weighted_graph G(n);
  weight_map weights = boost::get(boost::edge_weight, G);
  edge_desc e;
  for (int i=0; i<m; i++) {
    int from, to, w; cin >> from >> to >> w;
    e = boost::add_edge(from, to, G).first; weights[e]=w;
  }
  int max_dist = dijkstra_max_dist(G, 0);
  int mst_cost = kruskal_cost(G, weights);
  cout << mst_cost << " " << max_dist << '\n';
}

int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for (int i=0; i<t; i++) testcase();
}
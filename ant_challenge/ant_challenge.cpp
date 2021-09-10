#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>


using namespace std;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;
typedef boost::graph_traits<weighted_graph>::edge_iterator edge_it;



void kruskal(const weighted_graph &G, vector<vector<int>> &final_edges, weight_map &weights) {
  std::vector<edge_desc> mst;    // vector to store MST edges (not a property map!)
  boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
  for (std::vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); ++it) {
    int a = boost::source(*it, G);
    int b = boost::target(*it, G);
    bool condition = final_edges[a][b]==-1 || final_edges[a][b] > weights[*it];
    if (condition) final_edges[a][b] = weights[*it]; 
  }
}

int dijkstra_dist(const weighted_graph &G, int s, int t) {
  int n = boost::num_vertices(G);
  std::vector<int> dist_map(n);
  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));
  return dist_map[t];
}


void testcase() {
  int n, m, s, start, finish;
  cin >> n >> m >> s >> start >> finish;
  vector<vector<int>> final_edges(n, vector<int>(n, -1));
  vector<vector<vector<int>>> species_weights(s, vector<vector<int>>(n, vector<int>(n, -1)));
  weighted_graph G(n);
  for (int i=0; i<m; i++) {
    int from, to; cin >> from >> to;
    boost::add_edge(from, to, G);
    for (int j=0; j<s; j++) {
      int w; cin >> w;
      species_weights[j][from][to] = w; 
      species_weights[j][to][from] = w; 
    }
  }
  weight_map weights = boost::get(boost::edge_weight, G);
  for (int i=0; i<s; i++) {
    // Add weights to the graph for current specie
    edge_it e_beg, e_end;
    for (boost::tie(e_beg, e_end) = boost::edges(G); e_beg != e_end; ++e_beg) {
      weights[*e_beg] = species_weights[i][boost::source(*e_beg, G)][boost::target(*e_beg, G)];
    }
    // Run kruskal to select edges
    kruskal(G, final_edges, weights);
  }
  // No need for the species hives, just discard them
  int dummy; for (int i=0; i<s; i++) cin >> dummy;
  // Construct final graph and run dijkstra
  weighted_graph G_final(n);
  weight_map weights_final = boost::get(boost::edge_weight, G_final);
  edge_desc e;
  for (int i=0; i<n; i++) {
    for (int j=0; j<n; j++) {
      if (final_edges[i][j] != -1) {
        e = boost::add_edge(i, j, G_final).first; weights_final[e]=final_edges[i][j];
      }
    }
  }
  cout << dijkstra_dist(G_final, start, finish) << '\n';
}



int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for (int i=0; i<t; i++) testcase();
}

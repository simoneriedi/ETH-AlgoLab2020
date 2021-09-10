#include<iostream>
#include<vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

using namespace std;


int dijkstra_dist(const weighted_graph &G, int s, int dest) {
  int n = boost::num_vertices(G);
  std::vector<int> dist_map(n);
  
  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  return dist_map[dest];
}

void testcase() {
  int n, m, k, x, y, a, b, c, d;
  cin >> n >> m >> k >> x >> y;
  weighted_graph G(n*(k+1));
  weight_map weights = boost::get(boost::edge_weight, G);
  edge_desc e;
  for (int i=0; i<m; i++) {
    cin >> a >> b >> c >> d;
    if (d) {
      for (int j=1; j<k+1; j++) {
        e = boost::add_edge(a + n*(j-1), b + n*j, G).first; weights[e]=c;
        e = boost::add_edge(b + n*(j-1), a + n*j, G).first; weights[e]=c;
      }
    }
    for (int j=0; j<k+1; j++) {
      e = boost::add_edge(a + n*j, b + n*j, G).first; weights[e]=c;
    }
  }
  cout << dijkstra_dist(G, x, y+n*k) << '\n';
}

int main() {
  ios_base::sync_with_stdio(false);
  int t; cin>>t;
  for (int i=0; i<t; i++) testcase();
}
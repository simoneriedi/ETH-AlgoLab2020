#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include<vector>
#include <boost/graph/max_cardinality_matching.hpp>

using namespace std;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor                       vertex_desc;

int maximum_matching(const graph &G) {
  int n = boost::num_vertices(G);
  std::vector<vertex_desc> mate_map(n);  // exterior property map
  boost::edmonds_maximum_cardinality_matching(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
  int matching_size = boost::matching_size(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
  return matching_size;
}

void testcase() {
  int n; cin >> n;
  vector<vector<bool>> is_valid(n, vector<bool>(n));
  int tot_valid = 0;
  for (int i=0; i<n; i++) { // rows
    for (int j=0; j<n; j++) { // columns
      int valid; cin >> valid;
      tot_valid += valid;
      is_valid[i][j] = valid;
    }
  }
  // Graph, map (i, j) to i*n + j
  graph G(n*n);
  for (int i=0; i<n; i++) { // rows
    for (int j=0; j<n; j++) {  // columns
      if (!is_valid[i][j]) continue;
      // Connect edge with all other conflicting positions
      if (i>0 && j>1 && is_valid[i-1][j-2]) boost::add_edge(i*n+j, (i-1)*n+j-2, G);
      if (i>0 && j<n-2 && is_valid[i-1][j+2]) boost::add_edge(i*n+j, (i-1)*n+j+2, G);
      if (i<n-1 && j>1 && is_valid[i+1][j-2]) boost::add_edge(i*n+j, (i+1)*n+j-2, G);
      if (i<n-1 && j<n-2 && is_valid[i+1][j+2]) boost::add_edge(i*n+j, (i+1)*n+j+2, G);
      if (i>1 && j>0 && is_valid[i-2][j-1]) boost::add_edge(i*n+j, (i-2)*n+j-1, G);
      if (i>1 && j<n-1 && is_valid[i-2][j+1]) boost::add_edge(i*n+j, (i-2)*n+j+1, G);
      if (i<n-2 && j>0 && is_valid[i+2][j-1]) boost::add_edge(i*n+j, (i+2)*n+j-1, G);
      if (i<n-2 && j<n-1 && is_valid[i+2][j+1]) boost::add_edge(i*n+j, (i+2)*n+j+1, G);
    }
  }
  cout << tot_valid - maximum_matching(G) << '\n';
}









int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for (int i=0; i<t; i++) testcase();
}
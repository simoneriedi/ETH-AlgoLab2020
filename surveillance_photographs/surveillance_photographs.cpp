#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

using namespace std;

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

// Custom edge adder class, highly recommended
class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};





void testcase() {
  // nodes, edges, stations, photographs
  int n, m, k, l;
  cin >> n >> m >> k >> l;
  // Graph: nodes {0, n-1} represent intersections where no photograph has yet been recovered
  // {n, 2*n-1} the opposite
  graph G(2*n + 2);
  edge_adder adder(G);
  int source = 2*n;
  int sink = 2*n+1;
  // Police stations in the first layer are connected with the source and in the second layor with the sink
  for (int i=0; i<k; i++) {
    int x; cin >> x;
    adder.add_edge(source, x, 1);
    adder.add_edge(n+x, sink, 1);
  }
  // In the location of the photographs we jump down to the next level
  for (int i=0; i<l; i++) {
    int x; cin >> x;
    adder.add_edge(x, n+x, 1);
  }
  // Add all basic connections
  for (int i=0; i<m; i++) {
    int x, y; cin >> x >> y;
    adder.add_edge(x, y, k);
    adder.add_edge(n+x, n+y, 1);
  }
  long flow = boost::push_relabel_max_flow(G, source, sink);
  cout << flow << '\n';
}

int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for (int i=0; i<t; i++) testcase();
}

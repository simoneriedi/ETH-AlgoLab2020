#include <iostream>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

using namespace std;

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

// Custom edge adder class
class edge_adder {
 graph &G;
 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   // new assign cost
    w_map[rev_e] = -cost;   // new negative cost
  }
};


void testcase() {
  int b, s, p; cin >> b >> s >> p;
  graph G(b+s+2);
  edge_adder adder(G);
  int source = b+s;
  int sink = b+s+1;
  // Connect each boat with the source
  for (int i=0; i<b; i++) adder.add_edge(source, i, 1, 0);
  // Connect each sailor with the sink
  for (int i=b; i<b+s; i++) adder.add_edge(i, sink, 1, 0);
  // Add special edge to each sailor to always ensure a maximal flow of s
  for (int i=b; i<b+s; i++) adder.add_edge(source, i, b, 50);
  // Add pairings of boats with sailors
  for (int i=0; i<p; i++) {
    int boat, sailor, c;
    cin >> boat >> sailor >> c;
    adder.add_edge(boat, b+sailor, 1, 50-c);
  }
  // Run flow algorithm
  boost::successive_shortest_path_nonnegative_weights(G, source, sink);
  int C = 50*s - boost::find_flow_cost(G);
  cout << C << '\n';
}



int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for (int i=0; i<t; i++) testcase();
}
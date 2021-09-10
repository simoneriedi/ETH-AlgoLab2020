#include <iostream>
#include<vector>
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
  int c, g, start, end;
  long b;
  cin >> c >> g >> b >> start >> end;
  int max_cap = 0;
  graph G(c+1);
  int source = c;
  int sink = end;
  edge_adder adder(G); 
  for (int i=0; i<g; i++) {
    int x, y, d, e;
    cin >> x >> y >> d >> e;
    adder.add_edge(x, y, e, d);
    if (x == start) max_cap += e;
  }
  // Set flow from source to start
  auto c_map = boost::get(boost::edge_capacity, G);
  auto r_map = boost::get(boost::edge_reverse, G);
  auto w_map = boost::get(boost::edge_weight, G); // new!
  const edge_desc e = boost::add_edge(source, start, G).first;
  const edge_desc rev_e = boost::add_edge(start, source, G).first;
  c_map[rev_e] = 0; // reverse edge has no capacity!
  r_map[e] = rev_e;
  r_map[rev_e] = e;
  w_map[e] = 0;   // new assign cost
  w_map[rev_e] = 0;   // new negative cost
  
  
  int left = 0;
  int right = max_cap;
  while (left < right) {
    int mid = (left+right)/2+1;
    c_map[e] = mid;
    int flow = boost::push_relabel_max_flow(G, source, sink);
    boost::successive_shortest_path_nonnegative_weights(G, source, sink);
    long cost = boost::find_flow_cost(G);
    bool ok = (cost <= b) && (flow == mid);
    if (ok) left = mid;
    else right = mid-1;
  }
  cout << left << '\n';
}




int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for (int i=0; i<t; i++) testcase();
}

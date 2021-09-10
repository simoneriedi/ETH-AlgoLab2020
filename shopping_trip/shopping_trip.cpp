#include <iostream>

// BGL include
#include <boost/graph/adjacency_list.hpp>

// BGL flow include *NEW*
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
  int n, m, s; cin >> n >> m >> s;
  graph G(n+1);
  edge_adder adder(G);
  int source = 0;
  int sink = n;
  for (int i=0; i<s; i++) {
    int store; cin >> store;
    adder.add_edge(store, sink, 1);
  }
  for (int i=0; i<m; i++) {
    int a, b; cin >> a >> b;
    adder.add_edge(a, b, 1);
    adder.add_edge(b, a, 1);
  }
  long flow = boost::push_relabel_max_flow(G, source, sink);
  if (flow == s) cout << "yes\n";
  else cout << "no\n"; 
}
  
int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for (int i=0; i<t; i++) testcase();
}
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
  int n, m; cin >> n >> m;
  vector<int> demand(n);
  for (int i=0; i<n; i++) {
    int have, need; cin >> have >> need;
    demand[i] = need - have;
  }
  graph G(n+2);
  edge_adder adder(G);
  int source = n;
  int sink = n+1;
  for (int i=0; i<m; i++) {
    int from, to, min_c, max_c;
    cin >> from >> to >> min_c >> max_c;
    adder.add_edge(from, to, max_c - min_c);
    demand[from] += min_c;
    demand[to] -= min_c;
  }
  long objective = 0;
  for (int i=0; i<n; i++) {
    if (demand[i] > 0) {
      adder.add_edge(i, sink, demand[i]);
      objective += demand[i];
    }
    else if (demand[i] < 0) adder.add_edge(source, i, -demand[i]);
  }
  long flow = boost::push_relabel_max_flow(G, source, sink);
  if (flow == objective) cout << "yes\n";
  else cout << "no\n";
}


int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for (int i=0; i<t; i++) testcase();
}
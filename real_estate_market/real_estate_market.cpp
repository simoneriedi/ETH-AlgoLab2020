#include<iostream>
#include<vector>

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
  // buyers, sites, states
  int N, M, S; cin >> N >> M >>  S;
  vector<int> l(S);
  vector<int> s(M);
  vector<vector<int>> b(N, vector<int>(M));
  
  for (int i=0; i<S; i++) cin >> l[i];
  for (int i=0; i<M; i++) cin >> s[i];
  for (int i=0; i<N; i++) {
    for (int j=0; j<M; j++) cin >> b[i][j];
  }
  
  // Graph
  // {0,...N-1} are the buyers
  // {N,...N+M-1} are the sites
  // {N+M,...N+M+S-1} are the states
  // {N+M+S} is the source and {N+M+S+1} the sink
  const int n=N+M+S+2;
  const int source = N+M+S;
  const int sink = N+M+S+1;

  graph G(n);
  edge_adder adder(G);
    
  // Adding edges from source to buyers
  for (int i=0; i<N; i++) adder.add_edge(source, i, 1, 0);
  
  // Adding edges from each buyer to each site
  for (int i=0; i<N; i++) {
    for (int j=N; j<N+M; j++) adder.add_edge(i, j, 1, 100-b[i][j-N]);
  }
  
  // Adding edges from each site to corresponding state
  for (int i=N; i<N+M; i++) adder.add_edge(i, s[i-N]+N+M-1, 1, 0);
  
  // Adding edges from each state to sink
  for (int i=N+M; i<N+M+S; i++) adder.add_edge(i, sink, l[i-N-M], 0);

  
  // Min Cost Max Flow with successive_shortest_path_nonnegative_weights 
  int flow = boost::push_relabel_max_flow(G, source, sink);
  boost::successive_shortest_path_nonnegative_weights(G, source, sink);
  int cost = 100*flow - boost::find_flow_cost(G);
  cout << flow << " " << cost << '\n';
}
int main() {
  int t; cin >> t;
  for (int i=0; i<t; i++) testcase();
}
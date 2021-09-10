#include<iostream>
#include<vector>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace std;
// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator  out_edge_it; // Iterator


// Dijkstra includes
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;

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


int dijkstra_dist(const weighted_graph &G, int s, int t) {
  int n = boost::num_vertices(G);
  std::vector<int> dist_map(n);

  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  return dist_map[t];
}

struct Edge {
  int from;
  int to;
  int wid;
  int len;
};


void set_capacity(const edge_desc &e, const edge_desc &rev_e, graph &G, int capacity) {
  auto c_map = boost::get(boost::edge_capacity, G);
  auto r_map = boost::get(boost::edge_reverse, G);
  auto w_map = boost::get(boost::edge_weight, G);
  c_map[rev_e] = 0; 
  r_map[e] = rev_e;
  r_map[rev_e] = e;
  w_map[e] = 0; 
  w_map[rev_e] = 0; 
  c_map[e] = capacity;
}

void testcase() {
  int n, m, start, end;
  cin >> n >> m >> start >> end;
  vector<Edge> edges(m);
  int max_flow = 0;
  for (int i=0; i<m; i++) {
    int a, b, c, d;
    cin >> a >> b >> c >> d;
    edges[i] = {a, b, c , d};
    if (a == start || b == start) max_flow += c;
  }
  // Part 1: Compute length of the shortest path from start to end
  weighted_graph G_dijkstra(n);
  weight_map weights = boost::get(boost::edge_weight, G_dijkstra);
  edge_desc e;
  for (int i=0; i<m; i++) {
    e = boost::add_edge(edges[i].from, edges[i].to, G_dijkstra).first; weights[e]=edges[i].len;
    e = boost::add_edge(edges[i].to, edges[i].from, G_dijkstra).first; weights[e]=edges[i].len;
  }
  int shortest_path_len = dijkstra_dist(G_dijkstra, start, end);
  // Part 2: Solve the task, by using Min_cost-Max_flow
  graph G(n+1);
  int source = n;
  int sink = end;
  edge_adder adder(G);
  // Add all edges of the graph
  for (int i=0; i<m; i++) {
    adder.add_edge(edges[i].from, edges[i].to, edges[i].wid, edges[i].len);
    adder.add_edge(edges[i].to, edges[i].from, edges[i].wid, edges[i].len);
  }
  // Add edge from source to start with the desired capacity. Done manually to change capacity later.
  e = boost::add_edge(source, start, G).first;
  const edge_desc rev_e = boost::add_edge(start, source, G).first;
  // Binary search to find the correct capacity, i.e. the number of runners that can partecipate
  int left = 0;
  int right = max_flow;
  while (left < right) {
    int mid = (left+right)/2+1;
    set_capacity(e, rev_e, G, mid);
    boost::successive_shortest_path_nonnegative_weights(G, source, sink);
    int cost = boost::find_flow_cost(G);
    int flow = boost::push_relabel_max_flow(G, source, sink);
    bool ok = flow==mid && cost == flow*shortest_path_len;
    if (ok) left = mid;
    else right = mid-1;
  }
  cout << left << '\n';
}

int main() {
  int t; cin >> t;
  for (int i=0; i<t; i++) testcase();
}
#include <iostream>
#include<vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include<limits.h>
using namespace std;

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;
// DIJKSTRA
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;
typedef boost::graph_traits<weighted_graph>::edge_iterator edge_it;

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


struct Edge {
  int from, to, cost;
};

void dijkstra_dist(const weighted_graph &G, int s, vector<int> &shelters, vector<int> &dist_to_shelter) {
  int n = boost::num_vertices(G);
  std::vector<int> dist_map(n);
  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));
  
  for (int i=0; i<(int)shelters.size(); i++) dist_to_shelter[i] = dist_map[shelters[i]];
}

void testcase() {
  // nodes, edges, agents, shelters, capacity, waiting time
  int N, M, A, S, C, D;
  cin >> N >> M >> A >> S >> C >> D;
  weighted_graph G_dijkstra(N);
  weight_map weights = boost::get(boost::edge_weight, G_dijkstra);
  edge_desc e;
  // Add edges to djkstra graph
  for (int i=0; i<M; i++) {
    char w; int from, to, cost;
    cin >> w >> from >> to >> cost;
    e = boost::add_edge(from, to, G_dijkstra).first; weights[e]=cost;
    if (w == 'L') {
      e = boost::add_edge(to, from, G_dijkstra).first; weights[e]=cost;
    }
  }
  vector<int> agents(A);
  for (int i=0; i<A; i++) cin >> agents[i];
  vector<int> shelters(S);
  for (int i=0; i<S; i++) cin >> shelters[i];
  // First: for each agent find his fastest way to reach each shelter
  vector<vector<int>> dist(A, vector<int>(S));
  for (int i=0; i<A; i++) dijkstra_dist(G_dijkstra, agents[i], shelters, dist[i]);
  // Store viable edges in a list, then sort them
  vector<Edge> edges;
  for (int i=0; i<A; i++) {
    for (int j=0; j<S; j++) {
      if (dist[i][j] != INT_MAX) {
        edges.push_back({i, A+j, dist[i][j]});
        if (C==2) edges.push_back({i, A+S+j, dist[i][j]+D});
      }
    }
  }
  sort(edges.begin(), edges.end(), [](auto e1, auto e2) {return e1.cost < e2.cost;});
  // Binary search on the amount of edges we need to add to find the solution
  int left = 0;
  int right = edges.size()-1;
  while (left < right) {
    int mid = (left+right)/2;
    // Can we bring all agents to safety with (edges.begin(), edges.begin() + mid) edges?
    graph G(A+C*S+2);
    edge_adder adder(G);
    int source = A+C*S;
    int sink = A+C*S+1;
    for (int i=0; i<= mid; i++) adder.add_edge(edges[i].from, edges[i].to, 1);
    for (int i=0; i<A; i++) adder.add_edge(source, i, 1);
    for (int i=A; i<C*S+A; i++) adder.add_edge(i, sink, 1);
    long flow = boost::push_relabel_max_flow(G, source, sink);
    if (flow < A) left = mid+1;
    else right = mid;
  }
  cout << edges[left].cost+D << '\n';
}

int main () {
  ios_base::sync_with_stdio(false);
  int t; cin >> t; 
  for (int i=0; i<t; i++) testcase();
}


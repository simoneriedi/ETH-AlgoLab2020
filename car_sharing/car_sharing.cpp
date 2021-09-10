#include <iostream>
#include<vector>
#include<limits>
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


struct B {
  int from, to, start, end, profit;
};
struct Node {
  int time_, id;
};



void testcase() {
  int n, s; cin >> n >> s;
  // Store how many cars are in each station initially
  vector<int> stations(s);
  for (int i=0; i<s; i++) cin >> stations[i];
  int tot_cars = accumulate(stations.begin(), stations.end(), 0);
  // map every node of the graph to a unique identifier
  // mapping[{from, start}] = id
  int min_time = INT_MAX;
  int max_time = -1;
  int id = s;
  vector<B> bookings(n);
  map<pair<int, int>, int> mapping;
  vector<vector<Node>> nodes_in_stat(s, vector<Node>());
  for (int i=0; i<n; i++) {
    int from, to, start, end, profit;
    cin >> from >> to >> start >> end >> profit;
    bookings[i] = {from-1, to-1, start, end, profit};
    min_time = min(min_time, start);
    max_time = max(max_time, end);
    if (mapping.insert({{from-1, start}, id}).second) {nodes_in_stat[from-1].push_back({start, id}); id++;}
    if (mapping.insert({{to-1, end}, id}).second) {nodes_in_stat[to-1].push_back({end, id}); id++;}
  }
  // Sort times for each station
  for (int i=0; i<s; i++) sort(nodes_in_stat[i].begin(), nodes_in_stat[i].end(), [](auto p1, auto p2) {return p1.time_ < p2.time_;});
  // Create graph: {0,..s-1} are the stations. {s, ... id-1} are the nodes
  graph G(id+2);
  int source = id;
  int sink = id+1;
  edge_adder adder(G);  
  // Add connections from source to all stations
  for (int i=0; i<s; i++) adder.add_edge(source, i, stations[i], 0);
  // Add all booking connections
  for (int i=0; i<n; i++) {
    int node1 = mapping[{bookings[i].from, bookings[i].start}];
    int node2 = mapping[{bookings[i].to, bookings[i].end}];
    adder.add_edge(node1, node2, 1, (bookings[i].end - bookings[i].start)*100 - bookings[i].profit);
  }
  // Add all connections between nodes in the same station
  for (int i=0; i<s; i++) {
    for (int j=0; j<(int)nodes_in_stat[i].size(); j++) {
      if (!j) adder.add_edge(i, nodes_in_stat[i][j].id, tot_cars, (nodes_in_stat[i][j].time_-min_time)*100);
      else adder.add_edge(nodes_in_stat[i][j-1].id, nodes_in_stat[i][j].id, tot_cars, (nodes_in_stat[i][j].time_-nodes_in_stat[i][j-1].time_)*100);
      if (j == (int)nodes_in_stat[i].size()-1) adder.add_edge(nodes_in_stat[i][j].id, sink, tot_cars, (max_time - nodes_in_stat[i][j].time_)*100);
    }
  }
  // Solve the task
  long flow = boost::push_relabel_max_flow(G, source, sink);
  boost::successive_shortest_path_nonnegative_weights(G, source, sink);
  cout << flow*(long)(max_time-min_time)*100 - (long)boost::find_flow_cost(G) << '\n';
}


int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for (int i=0; i<t; i++) testcase();
}


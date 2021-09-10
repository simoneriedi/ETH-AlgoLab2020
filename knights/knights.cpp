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
  int cols, rows, k, c;
  cin >> cols >> rows >> k >> c;
  // Let (i, j) be an intersection in column = i and row = j
  // mapping (i, j) -> 2*(j*cols+i) for the incoming and 2*(j*cols+i)+1 for the outgoing flows
  graph G(2*cols*rows+2);
  edge_adder adder(G);
  int source = 2*cols*rows;
  int sink = 2*cols*rows + 1;
  // Connect knights to the source
  for (int z=0; z<k; z++) {
    int i, j; cin >> i >> j;
    adder.add_edge(source, 2*(j*cols+i), 1);
  }
  for (int i=0; i<cols; i++) {
    for (int j=0; j<rows; j++) {
      // Connect incoming flow to outgoing flow for this node
      adder.add_edge(2*(j*cols+i), 2*(j*cols+i)+1, c);
      // Do all outgoing connections
      if (i==0) adder.add_edge(2*(j*cols+i)+1, sink, 1);
      else adder.add_edge(2*(j*cols+i)+1, 2*(j*cols +i-1), 1);
      if (i==cols-1) adder.add_edge(2*(j*cols+i)+1, sink, 1);
      else adder.add_edge(2*(j*cols+i)+1, 2*(j*cols +i+1), 1);
      if (j==0) adder.add_edge(2*(j*cols+i)+1, sink, 1);
      else adder.add_edge(2*(j*cols+i)+1, 2*((j-1)*cols +i), 1);
      if (j==rows-1) adder.add_edge(2*(j*cols+i)+1, sink, 1);
      else adder.add_edge(2*(j*cols+i)+1, 2*((j+1)*cols +i), 1);
    }
  }
  long flow = boost::push_relabel_max_flow(G, source, sink);
  cout << flow << '\n';
}

int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for (int i=0; i<t; i++) testcase();
}
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
  int cols, rows; cin >> cols >> rows;
  vector<bool> is_valid(cols*rows, true);
  int tot = 0;
  for (int j=0; j<rows; j++) {
    for (int i=0; i<cols; i++) {
      char val; cin >> val;
      if (val == 'x') is_valid[j*cols+i] = false;
      else tot++;
    }
  }
  
  graph G(cols*rows+2);
  edge_adder adder(G);
  int source = cols*rows;
  int sink = cols*rows+1;
  for (int j=0; j<rows; j++) {
    for (int i=0; i<cols; i++) {
      if (!is_valid[j*cols+i]) continue;
      if ((j+i) % 2 == 0) {
        adder.add_edge(source, j*cols+i, 1);
        if (i > 0 && is_valid[j*cols+i-1]) adder.add_edge(j*cols+i, j*cols+i-1, 1);
        if (i < cols-1 && is_valid[j*cols+i+1]) adder.add_edge(j*cols+i, j*cols+i+1, 1);
        if (j > 0 && is_valid[(j-1)*cols+i]) adder.add_edge(j*cols+i, (j-1)*cols+i, 1);
        if (j < rows-1 && is_valid[(j+1)*cols+i]) adder.add_edge(j*cols+i, (j+1)*cols+i, 1);
      }
      else adder.add_edge(j*cols+i, sink, 1);
    }
  }
  long flow = boost::push_relabel_max_flow(G, source, sink);
  if (flow*2 == tot) cout << "yes\n";
  else cout << "no\n";
}


int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for (int i=0; i<t; i++) testcase();
}
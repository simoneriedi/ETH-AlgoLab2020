#include<iostream>
#include<vector>
#include<string>
#include<map>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>
using namespace std;


typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor                       vertex_desc;

bool is_maximal(const graph &G) {
  int n = boost::num_vertices(G);
  std::vector<vertex_desc> mate_map(n); 
  boost::edmonds_maximum_cardinality_matching(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
  int matching_size = boost::matching_size(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
  return n == matching_size*2;
}



bool can_improve(int in_common, vector<vector<int>> &common) {
  int n = common.size();
  // Create Graph by connecting two students if they share at least in_common characteristics
  graph G(n);
  for (int i=0; i<n; i++) {
    for (int j=i+1; j<n; j++) {
      if (common[i][j] >= in_common) boost::add_edge(i, j, G);
    }
  }
  return is_maximal(G);
}

void testcase() {
  int n, c, f;
  cin >> n >> c >> f;
  multimap<string, int> m;
  vector<vector<int>> common(n, vector<int>(n, 0));
  for (int i=0; i<n; i++) {
    for (int j=0; j<c; j++) {
      string x; cin >> x;
      m.insert({x,i});
    }
  }
  vector<int> people;
  for (auto it = m.begin(); it != m.end(); it++) {
    people.push_back(it->second);
    auto next = it; next++;
    if (next->first != it->first || next == m.end()) {
      for (int i=0; i<(int)people.size(); i++) {
        for (int j=i+1; j<(int)people.size(); j++) {
          common[people[i]][people[j]]++;
          common[people[j]][people[i]]++;
        }
      }
      people.clear();
    }
  }
  can_improve(f+1, common) ? cout << "not optimal\n" : cout << "optimal\n";
}

int main() {
  ios_base::sync_with_stdio(false);
  int t; cin>>t;
  for (int i=0; i<t; i++) testcase();
}
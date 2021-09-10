#include<iostream>
#include<vector>
#include<algorithm>
#include<map>
#include<string>

using namespace std;

struct Q {
  int age;
  int query_id;
};


vector<vector<int>> edges;
vector<vector<Q>> queries;
vector<int> result;
vector<int> age;
vector<int> current_path;
vector<int> ages_on_path;


bool compare(int v, int w) {
  return v > w;
}
void dfs(int current, int level) {
  current_path[level] = current;
  ages_on_path[level] = age[current];
  int n_queries = queries[current].size();
  for (int i=0; i<n_queries; i++) {
    auto p = lower_bound(ages_on_path.begin(), ages_on_path.begin()+level+1, queries[current][i].age, compare);
    int index = p - ages_on_path.begin();
    result[queries[current][i].query_id] = current_path[index];
  }
  // Recursive calls for dfs
  for (int i=0; i<(int)edges[current].size(); i++) dfs(edges[current][i], level+1);
}





void testcase() {
  int n, q; cin >> n >> q;
  age = vector<int>(n);
  map<string,int> mapping;
  vector<string> name(n);
  edges = vector<vector<int>>(n, vector<int>());
  queries = vector<vector<Q>>(n, vector<Q>());
  result = vector<int>(q);
  current_path = vector<int>(n);
  ages_on_path = vector<int>(n);
  for (int i=0; i<n; i++) {
    string n; int a;
    cin >> n >> a;
    mapping.insert({n, i});
    age[i] = a;
    name[i] = n;
  }
  vector<bool> is_root(n, true);
  for (int i=0; i<n-1; i++) {
    string to, from; cin >> to >> from;
    edges[mapping[from]].push_back(mapping[to]);
    is_root[mapping[to]] = false;
  }
  for (int i=0; i<q; i++) {
    string n; int a; cin >> n >> a;
    queries[mapping[n]].push_back({a, i});
  }
  int root = 0;
  for (int i=0; i<n; i++) if (is_root[i]) root = i;
  dfs(root, 0);
  for (int i=0; i<q; i++) cout << name[result[i]] << " ";
  cout << '\n';
}




int main () {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for (int i=0; i<t; i++) testcase();
}
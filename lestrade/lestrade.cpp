#include<iostream>
#include<vector>
#include<utility>
#include<tuple>
#include<limits.h>
#include<algorithm>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
// choose input type (input coefficients must fit)
typedef long IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;
// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>  Triangulation;
typedef K::Point_2 P;

using namespace std;

struct agent {
  int fee;
  int info1;
  int info2;
  int info3;
};



long round_and_print(CGAL::Quotient<ET> res) {
  double a = ceil(CGAL::to_double(res));
  while (a-1 >= res) a--;
  while (a<res) a++;
  return a;
}

// For each agent find closest criminal and push it in return vector.
// If there are multiple agents assigned to the same criminal, only keep the one with the lowest fee among them
vector<agent> get_input(int a, int criminals) {
  vector<agent> res(a);
  vector<pair<P, int>> members(criminals);
  vector<tuple<int,int,int>> mem_info(criminals);
  for (int i=0; i<criminals; i++) {
    int x, y, u, v, w;
    cin >> x >> y >> u >> v >> w;
    members[i] = {P(x, y), i};
    mem_info[i] = {u, v, w};
  }
  Triangulation T;
  T.insert(members.begin(), members.end());
  // Read the agents
  vector<pair<P,int>> ag(a);
  for (int i=0; i<a; i++) {
    P p; int fee;
    cin >> p >> fee;
    ag[i] = {p, fee};
  }
  sort(ag.begin(), ag.end(), [](auto a1, auto a2) {return a1.second<a2.second;});
  vector<bool> already_marked(criminals, false);
  for (int i=0; i<a; i++) {
    auto v = T.nearest_vertex(ag[i].first);
    int in = v->info();
    if (already_marked[in]) continue;
    auto t = mem_info[in];
    res[i] = {ag[i].second, get<0>(t), get<1>(t), get<2>(t)};
    already_marked[in] = true;
  }
  return res;
}

vector<agent> agents;

// Note that we have already filtered the list of agents such that there are no two agents assigned to the same criminal
int linear_program(int info1, int info2, int info3) {
  int n = agents.size(); // also the number of variables
  
  Program lp (CGAL::SMALLER, true, 0, true, 24);
  
  // Set objective function
  for (int i=0; i<n; i++) {
    lp.set_c(i, agents[i].fee);
  }
  // Info 1 constraint
  for (int i=0; i<n; i++) {
    lp.set_a(i, 0, -agents[i].info1); lp.set_b(0, -info1);
  }
  // Info 2 constraint
  for (int i=0; i<n; i++) {
    lp.set_a(i, 1, -agents[i].info2); lp.set_b(1, -info2);
  }
  // Info 3 constraint
  for (int i=0; i<n; i++) {
    lp.set_a(i, 2, -agents[i].info3); lp.set_b(2, -info3);
  }
  
  Solution s = CGAL::solve_linear_program(lp, ET());
  if (s.is_infeasible()) {/*cout << "infeasible with the poor agents"; */return -1;}
  else if (s.is_optimal()) {/*cout << s.objective_value(); */return round_and_print(s.objective_value());}
}

void testcase() {
  int holmes_cost, info1, info2, info3;
  cin >> holmes_cost >> info1 >> info2 >> info3;
  int a, criminals;
  cin >> a >> criminals;
  agents = get_input(a, criminals);
  int result = linear_program(info1,info2,info3);
  //cout << " holmes has: " << holmes_cost << '\n';
  if (result != -1 && result <= holmes_cost) cout << "L" << '\n';
  else cout << "H" << '\n';
}


int main() {
  std::ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for (int i=0; i<t; i++) testcase();
}
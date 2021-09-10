#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include<iostream>
#include<vector>
#include<utility>
using namespace std;
// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;
// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


// Water canal
const int a = 0;
const int c = 1;
const int f = 2;
const int eps = 3;
long s;
int constraint;
int n;
vector<pair<int, int>> nobles;
int m;
vector<pair<int, int>> commons;



bool add_cersei(Program &lp) {
  for (int i=0; i<n; i++) {
    lp.set_a(a, constraint,  -nobles[i].second);
    lp.set_a(f, constraint,  1); lp.set_b(constraint, -nobles[i].first);
    constraint++;
  }
  for (int i=0; i<m; i++) {
    lp.set_a(a, constraint,  commons[i].second);
    lp.set_a(f, constraint,  -1); lp.set_b(constraint, commons[i].first);
    constraint++;
  }
  Solution s = CGAL::solve_linear_program(lp, ET());
  return !s.is_infeasible();
}
void add_tywin(Program &lp) {
  long sum_x = s;
  long sum_y = 0;
  for (int i=0; i<n; i++) {
    sum_x += nobles[i].first;
    sum_y -= nobles[i].second;
  }
  for (int i=0; i<m; i++) {
    sum_x -= commons[i].first;
    sum_y += commons[i].second;
  }
  lp.set_a(a, constraint, -sum_y); lp.set_a(f, constraint, m-n); lp.set_b(constraint, sum_x);
  constraint++;
}

void add_jamie(Program &lp) {
  for (int i=0; i<n; i++) {
    lp.set_a(a, constraint, nobles[i].first); lp.set_a(c, constraint, 1);
    lp.set_a(eps, constraint, -1); lp.set_b(constraint, -nobles[i].second);
    constraint++;
    lp.set_a(a, constraint, -nobles[i].first); lp.set_a(c, constraint, -1);
    lp.set_a(eps, constraint, -1); lp.set_b(constraint, nobles[i].second);
    constraint++;
  }
  for (int i=0; i<m; i++) {
    lp.set_a(a, constraint, commons[i].first); lp.set_a(c, constraint, 1);
    lp.set_a(eps, constraint, -1); lp.set_b(constraint, -commons[i].second);
    constraint++;
    lp.set_a(a, constraint, -commons[i].first); lp.set_a(c, constraint, -1);
    lp.set_a(eps, constraint, -1); lp.set_b(constraint, commons[i].second);
    constraint++;
  }
  lp.set_c(eps, 1);
}

long rounding(CGAL::Quotient<ET> res) {
  double a = ceil(CGAL::to_double(res));
  while (a-1 >= res) a--;
  while (a<res) a++;
  return a;
}

void testcase() {
  cin >> n >> m >> s;
  nobles = vector<pair<int,int>>(n);
  commons = vector<pair<int,int>>(m);
  for (int i=0; i<n; i++) {
    int x, y; cin >> x >> y;
    nobles[i] = {x, y};
  }
  for (int i=0; i<m; i++) {
    int x, y; cin >> x >> y;
    commons[i] = {x, y};
  }
  constraint = 0;
  Program lp (CGAL::SMALLER, false, 0, false, 0); 
  if (!add_cersei(lp)) {
    cout << "Yuck!\n";
    return;
  }
  if (s != -1) add_tywin(lp);
  add_jamie(lp);
  Solution s = CGAL::solve_linear_program(lp, ET());
  if (s.is_infeasible()) cout << "Bankrupt!\n";
  else cout << rounding(s.objective_value()) << '\n';
}

int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for (int i=0; i<t; i++) testcase();
}
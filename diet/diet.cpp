#include<iostream>
#include<vector>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
using namespace std;
// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;
// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;



long rounding(CGAL::Quotient<ET> x) {
  ET val = x.numerator()/x.denominator();
  return CGAL::to_double(val);
}

struct N {
  int min, max;
};
struct F {
  int cost;
  vector<int> amounts;
};

void testcase(int n, int m) {
  vector<N> nutrient(n);
  for (int i=0; i<n; i++) {
    int mi, ma; cin >> mi >> ma;
    nutrient[i] = {mi, ma};
  }
  vector<F> food(m);
  for (int i=0; i<m; i++) {
    int cost; cin >> cost;
    vector<int> amounts(n);
    for (int j=0; j<n; j++) cin >> amounts[j];
    food[i] = {cost, amounts};
  }
  Program lp (CGAL::SMALLER, true, 0, false, 0); 
  // Objective function
  for (int i=0; i<m; i++) lp.set_c(i, food[i].cost);
  // Constraints
  for (int j=0; j<n; j++) {
    for (int i=0; i<m; i++) {
      lp.set_a(i, 2*j, food[i].amounts[j]);
      lp.set_a(i, 2*j+1, -food[i].amounts[j]);
    }
    lp.set_b(2*j, nutrient[j].max);
    lp.set_b(2*j+1, -nutrient[j].min);
  }
  // Solve the LP
  Solution s = CGAL::solve_linear_program(lp, ET());
  if (s.is_optimal()) cout << rounding(s.objective_value()) << '\n';
  else if (s.is_unbounded()) cout << "How could this be?\n";
  else if (s.is_infeasible()) cout << "No such diet.\n";
}






int main() {
  ios_base::sync_with_stdio(false);
  int n; cin >> n;
  int m; cin >> m;
  while (n) {
    testcase(n, m);
    cin >> n;
    cin >> m;
  }
}
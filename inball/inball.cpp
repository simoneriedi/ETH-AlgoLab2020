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
  return CGAL::to_double(x.numerator()/x.denominator());
}


void testcase(int n) {
  int d; cin >> d;
  vector<vector<int>> inequalities(n, vector<int>(d));
  vector<int> norms(n);
  vector<int> bias(n);
  for (int i=0; i<n; i++) {
    int norm = 0;
    for (int j=0; j<d; j++) {
      int a; cin >> a;
      inequalities[i][j] = a;
      norm += a*a;
    }
    norms[i] = sqrt(norm);
    cin >> bias[i];
  }
  
  // create an LP with Ax <= b, lower bound 0 and no upper bounds
  Program lp (CGAL::SMALLER, false, 0, false, 0); 
  
  // Variables {0, 1, .. d-1} is the center of the circle
  const int r = d;  // radius
  // Set objective function
  lp.set_c(r, -1);
  // Set constraints
  for (int i=0; i<n; i++) {
    for (int j=0; j<d; j++) lp.set_a(j, i, inequalities[i][j]);
    lp.set_a(r, i, norms[i]);
    lp.set_b(i, bias[i]);
  }
  lp.set_l(r, true, 0);                                   
  // solve the program, using ET as the exact type
  Solution s = CGAL::solve_linear_program(lp, ET());
  if (s.is_optimal()) cout << rounding(-s.objective_value()) << '\n';
  else if (s.is_unbounded()) cout << "inf\n";
  else cout << "none\n";
}





int main() {
  ios_base::sync_with_stdio(false);
  int n; cin >> n;
  while(n) {
    testcase(n);
    cin >> n;
  }
}
#include<iostream>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


using namespace std;



void round_and_print(CGAL::Quotient<ET> res, int p) {
  ET num = res.numerator();
  ET den = res.denominator();
  if (num<0) {
    if (num % (-den) == 0) cout << num/den << '\n';
    else {
      if (p==1) cout << num/den-1 << '\n';
      else cout << num/den << '\n';
    }
  }
  else {
    if (num % den == 0) cout << num/den << '\n';
    else {
      if (p==1) cout << num/den << '\n';
      else cout << num/den-1 << '\n';
    }
  }
}

void testcase(int p) {
  const int X = 0;
  const int Y = 1;
  const int Z = 2;
  int a, b; cin >> a >> b;
  if (p==1) {
    Program lp (CGAL::SMALLER, true, 0, false, 0);
    lp.set_a(X, 0, 1); lp.set_a(Y, 0, 1); lp.set_b(0,4);
    lp.set_a(X, 1, 4); lp.set_a(Y, 1, 2); lp.set_b(1,a*b);
    lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2,1);
    lp.set_c(X, a); lp.set_c(Y, -b);
    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_linear_program(lp, ET());
    if (s.is_optimal()) round_and_print(-s.objective_value(),1);
    if (s.is_unbounded()) cout << "unbounded" << '\n';
    if (s.is_infeasible()) cout << "no" << '\n';
    return;
  }
  else {
    Program lp (CGAL::LARGER, false, 0, true, 0);
    lp.set_a(X, 0, 1); lp.set_a(Y, 0, 1); lp.set_b(0,-4);
    lp.set_a(X, 1, 4); lp.set_a(Y, 1, 2); lp.set_a(Z, 1, 1); lp.set_b(1,-a*b);
    lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2,-1);
    lp.set_c(X, a); lp.set_c(Y, b); lp.set_c(Z, 1);
    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_linear_program(lp, ET());
    if (s.is_optimal()) round_and_print(s.objective_value(),2);
    if (s.is_unbounded()) cout << "unbounded" << '\n';
    if (s.is_infeasible()) cout << "no" << '\n';
    return;
  }
  
} 




int main() {
  int p; cin >> p;
  while (p) {
    testcase(p);
    cin >> p;
  }
}
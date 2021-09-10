#include<iostream>
#include<vector>
#include<utility>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <CGAL/number_utils.h>

// choose input type (input coefficients must fit)
typedef CGAL::Gmpz IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


using namespace std;



void round_and_print(CGAL::Quotient<ET> res) {
  ET num = res.numerator();
  ET den = res.denominator();
  cout << num/den << '\n';
}


void testcase() {
  // Starting postion, number of equations
  int x, y, n; cin >> x >> y >> n;
  
  const int t = 0;
  const int x_pos = 1;
  const int y_pos = 2;
  
  Program lp (CGAL::LARGER, false, 0, false, 0);
  lp.set_c(t, -1);
  for (int i=0; i<n; i++) {
    CGAL::Gmpz a, b, c, v; 
    cin >> a >> b >> c >> v;
    if (a*x + b*y + c < 0) {
      lp.set_a(x_pos, i, -a);
      lp.set_a(y_pos, i, -b);
      lp.set_a(t, i, -v*CGAL::sqrt(a*a+b*b));
      lp.set_b(i, c);
    } 
    else {
      lp.set_a(x_pos, i, a);
      lp.set_a(y_pos, i, b);
      lp.set_a(t, i, -v*CGAL::sqrt(a*a+b*b));
      lp.set_b(i, - c);
    }
    
  }
  Solution s = CGAL::solve_linear_program(lp, ET());
  round_and_print(-s.objective_value());
} 




int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for (int i=0; i<t; i++) testcase();
}
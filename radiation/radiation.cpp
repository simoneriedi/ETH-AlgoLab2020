#include<iostream>
#include<vector>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
using namespace std;
// choose input type (input coefficients must fit)
typedef CGAL::Gmpz IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;
// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

struct P {
  long x, y, z;
};

vector<P> h_cells;
vector<P> c_cells;


bool is_sep(int degree) {
  Program lp (CGAL::SMALLER, false, 0, false, 0);  
  CGAL::Quadratic_program_options options;
  options.set_pricing_strategy(CGAL::QP_BLAND);
  int constraint = 0;
  int variable = 0;
  for (int i=0; i<(int)h_cells.size(); i++) {
    for (int x=0; x<=degree; x++) {
      for (int y=0; y+x<=degree; y++) {
        for (int z=0; z+x+y<= degree; z++) {
          CGAL::Gmpz coef = pow(h_cells[i].x, x)*pow(h_cells[i].y, y)*pow(h_cells[i].z, z);
          lp.set_a(variable, constraint, -coef);
          variable++;
        }
      }
    }
    lp.set_b(constraint, -1);
    constraint++;
    variable = 0;
  }
  for (int i=0; i<(int)c_cells.size(); i++) {
    for (int x=0; x<=degree; x++) {
      for (int y=0; y+x<=degree; y++) {
        for (int z=0; z+x+y<= degree; z++) {
          CGAL::Gmpz coef = pow(c_cells[i].x, x)*pow(c_cells[i].y, y)*pow(c_cells[i].z, z);
          lp.set_a(variable, constraint, coef);
          variable++;
        }
      }
    }
    lp.set_b(constraint, -1);
    constraint++;
    variable = 0;
  }
  Solution s = CGAL::solve_linear_program(lp, ET(), options);
  return !s.is_infeasible();
}



void testcase() {
  int h, t; cin >> h >> t;
  h_cells = vector<P>(h);
  c_cells = vector<P>(t);
  for (int i=0; i<h; i++) {
    int x, y, z; cin >> x >> y >> z;
    h_cells[i] = {x, y, z};
  }
  for (int i=0; i<t; i++) {
    int x, y, z; cin >> x >> y >> z;
    c_cells[i] = {x, y, z};
  }
  
  // Start with exponential search
  int min_d = 1;
  while(min_d < 31 && !is_sep(min_d)) min_d *= 2;
  if (min_d > 30 && !is_sep(30)) {
    cout << "Impossible!\n";
    return;
  }
  // Binary search 
  int left = min_d/2;
  int right = min(min_d, 30);
  while(left < right) {
    int mid = (left+right)/2;
    if (is_sep(mid)) right = mid;
    else left = mid+1;
  }
  cout << left << '\n';
}


int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for (int i=0; i<t; i++) testcase();
}
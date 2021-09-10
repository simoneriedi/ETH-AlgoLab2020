#include<iostream>
#include<vector>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

using namespace std;
// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
// Triangulation
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef K::Point_2 P;


struct W {
  P pos;
  int supply;
  int alc;
};
struct S {
  P pos;
  int demand;
  int up_limit;
};
struct C {
  P center;
  long r;
};

vector<W> warehouse;
vector<S> stadium;
vector<vector<int>> revenue; // revenue[warehouse][stadium]
vector<C> contour;

long rounding(CGAL::Quotient<ET> x) {
  double a = floor(CGAL::to_double(x));
  while (a+1 <= x) a++;
  while (a>x) a--;
  return a;
}


void compute_rev() {
  int S = stadium.size();
  int W = warehouse.size();
  // Flow between warehouse w and stadium s is (w, s) -> w*S + s
  Program lp (CGAL::SMALLER, true, 0, false, 0); 
  // Objective function
  for (int w=0; w<W; w++) { 
    for (int s=0; s<S; s++) lp.set_c(w*S + s, -revenue[w][s]); 
  }
  // Constraints {0,..W-1}: Warehouses supply 
  for (int w=0; w<W; w++) {
    for (int s=0; s<S; s++) lp.set_a(w*S + s, w, 1);
    lp.set_b(w, warehouse[w].supply);
  }
  // Constraints {W,..W+2*S-1}: Stadiums equality of liters
  for (int s=0; s<S; s++) {
    for (int w=0; w<W; w++) {
      lp.set_a(w*S + s, W+s, 1);
      lp.set_a(w*S + s, W+S+s, -1);
    }
    lp.set_b(W+s, stadium[s].demand);
    lp.set_b(W+S+s, -stadium[s].demand);
  }
  // Constraints {W+2*S,..W+3*S-1}: Alcohol constrains on stadiums
  for (int s=0; s<S; s++) {
    for (int w=0; w<W; w++) lp.set_a(w*S + s, W+2*S + s, warehouse[w].alc);
    lp.set_b(W+2*S + s, stadium[s].up_limit);
  }
  // Solve LP
  Solution s = CGAL::solve_linear_program(lp, ET());
  if (s.is_optimal()) cout << rounding(-s.objective_value()/100) << '\n';
  else cout << "RIOT!\n";
}

void update_revenues() {
  int W = warehouse.size();
  int S = stadium.size();
  int C = contour.size();
  Triangulation T;
  for (int s=0; s<S; s++) T.insert(stadium[s].pos);
  for (int w=0; w<W; w++) T.insert(warehouse[w].pos);
  for (int c=0; c<C; c++) {
    long squared_r = contour[c].r*contour[c].r;
    auto nearest = T.nearest_vertex(contour[c].center);
    if (CGAL::squared_distance(nearest->point(), contour[c].center) > squared_r) continue;
    for (int w=0; w<W; w++) {
      for (int s=0; s<S; s++) {
        auto in_w =  CGAL::squared_distance(warehouse[w].pos, contour[c].center) <= squared_r;
        auto in_s =  CGAL::squared_distance(stadium[s].pos, contour[c].center) <= squared_r;
        revenue[w][s] -= (in_w != in_s);
      }
    }
  }
}

void testcase() {
  // warehouses, stadiums, contours
  int n, m, c; cin >> n >> m >> c;
  // Save all the input
  warehouse = vector<W>(n);
  stadium = vector<S>(m);
  revenue = vector<vector<int>>(n, vector<int>(m)); // revenue[warehouse][stadium]
  contour = vector<C>(c);
  for (int i=0; i<n; i++) {
    int x, y, s, a;
    cin >> x >> y >> s >> a;
    warehouse[i] = {P(x, y), s, a};
  }
  for (int i=0; i<m; i++) {
    int x, y, d, u;
    cin >> x >> y >> d >> u;
    stadium[i] = {P(x, y), d, 100*u};
  }
  for (int i=0; i<n; i++) {
    for (int j=0; j<m; j++) {
      int r; cin >> r;
      revenue[i][j] = 100*r;
    }
  }
  for (int i=0; i<c; i++) {
    int x, y, r; 
    cin >> x >> y >> r;
    contour[i] = {P(x, y), r};
  }
  update_revenues();
  compute_rev();
}


int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for (int i=0; i<t; i++) testcase();
}

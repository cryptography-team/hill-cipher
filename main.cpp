#include "matrix.h"
#include <iostream>
using namespace std;
int main() {
  int n;
  cin >> n;
  matrix<int> x(n, n);
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      cin >> x(i, j);
  cout << x;
}

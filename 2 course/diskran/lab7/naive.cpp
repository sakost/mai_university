#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
  int n, m;
  cin >> n >> m;
  int count = 0;
  for (long long i(n); i > 0ll; --i) {
    auto s1 = to_string(i);
    auto s2 = to_string(n);
    if (i % m == 0 &&
        lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end())) {
      ++count;
    }
  }
  cout << count << endl;
}

#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

using namespace std;

template <typename T>
int find_all_sub(const vector<T> &str, const vector<T> &pat) {
  int ans(0);
  for (size_t i(0); i < str.size(); i++) {
    bool eq = true;
    for (size_t j(0); j < pat.size(); j++) {
      if (str[i + j] != pat[j]) {
        eq = false;
        break;
      }
    }
    ans += (int)eq;
  }
  return ans;
}

auto main() -> int {
  vector<uint64_t> pat, s;
  string line;
  getline(cin, line);
  istringstream ss(line);
  copy(std::istream_iterator<uint64_t>(ss), std::istream_iterator<uint64_t>(),
       std::back_inserter(pat));
  copy(istream_iterator<uint64_t>(cin), istream_iterator<uint64_t>(),
       back_inserter(s));
  cout << find_all_sub(s, pat);
}

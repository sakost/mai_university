#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>


int main() {
    std::int64_t n, m;
    std::cin >> n >> m;

    std::vector<std::int64_t> dp;
    dp.resize(std::to_string(n).size()+1);
    dp[0] = -bool(n%m==0);

    std::int64_t i(1);
    while(n > 0){
        dp[i] = n/m - ((std::int64_t)std::pow(10, std::to_string(n).size()-1) - 1) / m + dp[i-1];
        n /= 10;
        ++i;
    }

    std::cout << std::max(dp.back(), (std::int64_t)0) << std::endl;
}

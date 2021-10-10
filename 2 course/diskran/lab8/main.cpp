#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <limits>

using data_type = double;
using cost_type = std::uint_fast32_t;

const cost_type MAX_MIN_COST = 50;

struct Row: std::vector<data_type>{
    std::size_t num;
    cost_type cost;
};

int main() {
    std::size_t m, n;
    std::cin >> m >> n;
    std::vector<Row> matrix(m);

    std::set<std::size_t> ans;

    for (std::size_t i(0); i < matrix.size(); ++i) {
        auto& row = matrix[i];
        row.num = i+1;
        row.resize(n);
        for (auto &el: row) {
            std::cin >> el;
        }
        std::cin >> row.cost;
    }

    // just find similar upper-triangle matrix
    for(std::size_t col(0); col < n; ++col){
        cost_type min_cost = MAX_MIN_COST + 1;
        std::size_t min_idx;
        for (std::size_t row(col); row < m; ++row) {
            static data_type epsilon = std::numeric_limits<data_type>::epsilon();
            if(std::abs(matrix[row][col]) > epsilon && matrix[row].cost < min_cost){
                min_idx = row;
                min_cost = matrix[row].cost;
            }
        }
        if(min_cost == MAX_MIN_COST + 1){
            std::cout << -1 << std::endl;
            return 0;
        }

        ans.insert(matrix[min_idx].num);

        std::swap(matrix[col/*from which we started*/], matrix[min_idx/*found*/]);

        for (std::size_t row(col + 1); row < m; ++row) { // change all "bottom"(for current) rows
            data_type coefficient = matrix[row][col] / matrix[col][col]; // coefficient for current row

            for (std::size_t col2 = col; col2 < n; ++col2) { // start from our first "not calculated" column
                matrix[row][col2] -= matrix[col][col2] * coefficient;
            }
        }
    }

//    std::copy(ans.begin(), ans.end(), std::ostream_iterator<std::size_t>(std::cout, " "));
    for (const auto& elem: ans) {
        std::cout << elem;
        if(elem != *std::prev(ans.end())){
            std::cout << ' ';
        }
    }
    std::cout << std::endl;

    return 0;
}

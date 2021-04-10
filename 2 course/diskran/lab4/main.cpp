#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

using TValue = uint64_t;
const TValue shebang = (1ull << 32ull);

// todo: can replace with getline with three args
vector<TValue> GetClause() {
    TValue c;
    vector<TValue> cl;
    std::string line;
    if(!std::getline(cin, line)){
        throw exception();
    }
    std::istringstream iss(line);
    while ( iss >> c) {
        cl.push_back(c);
    }
    return cl;
}

void BuildZFunction(vector<TValue> &pattern, vector<TValue> &pi){
    for (size_t i = 1; i < pattern.size(); ++i) {
        TValue j = pi.at(i-1);
        while(j > 0 && pattern.at(i) != pattern.at(j)){
            j = pi.at(j-1);
        }
        if(pattern.at(i) == pattern.at(j)) j++;
        pi.at(i) = j;
    }
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    vector<TValue> pattern = GetClause();
    pattern.push_back(shebang);
    vector<TValue> pi(pattern.size(), 0);
    BuildZFunction(pattern, pi);

    vector<size_t> rowsLength;

    size_t wordInd(0), rowInd(0);
    TValue curPi = 0;

    while(true){
        vector<TValue> row;
        try {
            row = GetClause();
        } catch (exception& err) {
            break;
        }
//        if(row.empty()) continue;
        for (auto& el: row) {
            while(curPi > 0 && el != pattern.at(curPi)){
                curPi = pi.at(curPi - 1);
            }
            if(el == pattern.at(curPi)) ++curPi;
            if(curPi == pattern.size()-1){
                size_t ro = rowInd + 1, wo = wordInd + 1;
                int64_t ind = -1;
                size_t pattern_size = pattern.size()-1;
                while(pattern_size > wo){
                    pattern_size -= wo;
                    ro--;
                    wo = *next(rowsLength.end(), ind--);
                }
                wo -= pattern_size - 1;
                cout << ro << ", " << wo << '\n';
            }
            wordInd++;
        }
        wordInd = 0;
        rowInd++;
        rowsLength.push_back(row.size());
    }
}

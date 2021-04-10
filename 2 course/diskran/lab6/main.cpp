#include <iostream>
#include <string>
#include <vector>

//#define USE_FFT
#include "longarithmetic.h"

using namespace std;


int main() {
    ios_base::sync_with_stdio(false);
    cout.tie(nullptr);
    cin.tie(nullptr);

    TLongArithmetic a, b;
    std::vector<std::string> result = {"false", "true"};
    
    char op;
    while(std::cin >> a >> b >> op){
        try {
            switch (op) {
                case '+':
                    cout << a + b << '\n';
                    break;
                case '-':
                    cout << a - b << '\n';
                    break;
                case '*':
                    cout << a * b << '\n';
                    break;
                case '^':
                    cout << a.Power(b) << '\n';
                    break;
                case '/':
                    cout << a / b << '\n';
                    break;
                case '>':
                    cout << result[a > b] << '\n';
                    break;
                case '<':
                    cout << result[a < b] << '\n';
                    break;
                case '=':
                    cout << result[a == b] << '\n';
                    break;
                default:
                    cout << "Error\n";
            }
        } catch (TInvalidOperands& e) {
            cout << "Error\n";
        }
    }
    cout.flush();
    return 0;
}

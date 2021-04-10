/**
 * Саженов Константин
 * на github sakost
 * Создать класс Modulo для работы с целыми числами по модулю N. В классе должно быть два поля: число и N.
 * Реализовать все арифметические операции. Реализовать операции сравнения.
 * */

#include <iostream>
#include <string>

/**
* `Module` class implements arithmetical operations by modulo
 * `T` is the data type
*/
template <typename T>
class Modulo{
public:
    // default constructor
    Modulo(){
        number = 0;
        N = 1;
    }
    Modulo(T number_, T N_){
        N = N_;
        number = number_ % N;
    }
    // copy operator
    Modulo& operator=(const Modulo<T> &other){
        number = other.number;
        N = other.N;
        return *this;
    }

    // arithmetical operations
    Modulo operator+(const Modulo<T> &other) const{
        return Modulo<T>((number + other.number) % N, N);
    }
    Modulo operator-(const Modulo<T> &other) const{
        return (*this) + (-other);
    }
    Modulo operator-() const{
        return Modulo<T>((N-number) % N, N);
    }
    Modulo operator*(const Modulo<T> &other) const{
        return Modulo<T>((number * other.number) % N, N);
    }
    // compare operators
    bool operator<(const Modulo<T> &other) const{
        return number < other.number;
    }
    bool operator!=(const Modulo<T> &other) const{
        return number != other.number;
    }
    bool operator>(const Modulo<T> &other) const{
        return !((*this) < other) && (*this) != other;
    }
    bool operator==(const Modulo<T> &other) const{
        return !((*this) != other);
    }
    bool operator>=(const Modulo<T> &other) const{
        return ((*this) > other) || ((*this) == other);
    }
    bool operator<=(const Modulo<T> &other) const{
        return ((*this) < other) || ((*this) == other);
    }
    // binary power by modulo
    [[nodiscard]] Modulo<T> pow(Modulo<T> base, Modulo<T> exp) const{
        if (exp.number == 0)
            return Modulo<T>(1, N);
        if (exp.number % 2 == 1)
            return Modulo((pow(base, Modulo(exp.number - 1, N)) * base).number % N, N);
        Modulo<T> tmp = pow(base, Modulo(exp.number / 2, N));
        return Modulo<T>((tmp.number * tmp.number) % N, N);
    }

    // division by modulo using the Euler theorem
    Modulo<T> operator/(Modulo<T> other) const{
        return Modulo<T>(((*this) * pow(other, Modulo(N - 2, N))).number % N, N);
    }

    // prints the number of given ostream
    friend std::ostream& operator<< (std::ostream &out, const Modulo<T> &n){
        out << n.number;
        return out;
    }

private:
    T number, N;
};


/**
 * help function prints help message to std::cout
 **/
void help(){
    std::cout << "plus num1 num2 mod" << std::endl;
    std::cout << "minus num1 num2 mod" << std::endl;
    std::cout << "mul num1 num2 mod" << std::endl;
    std::cout << "div num1 num2 mod" << std::endl;
    std::cout << "pow num1 num2 mod" << std::endl;
    std::cout << "exit to quit" << std::endl;
    std::cout << "help to get help" << std::endl;
}

using Number = long long;

int main() {
    help();
    std::string cmd;

    while (true){
        std::cout << ">>> ";
        std::cin >> cmd;
        // exit commands
        if(cmd == "quit" || cmd == "exit") {
            break;
        }
        // all available commands
        else if(cmd != "plus" && cmd != "minus" && cmd != "mul" && cmd != "div" && cmd != "pow" || cmd == "help"){
            help();
            continue;
        }

        // create three numbers
        Number num1, num2, mod;
        std::cin >> num1 >> num2 >> mod;
        // create three numbers by modulo
        Modulo<Number> n1(num1, mod), n2(num2, mod), n3(0, mod);

        // given operations
        if(cmd == "plus"){
            n3 = n1 + n2;
        }
        else if(cmd == "minus"){
            n3 = n1 - n2;
        }
        else if(cmd == "mul"){
            n3 = n1 * n2;
        }
        else if(cmd == "div"){
            n3 = n1 / n2;
        } else if(cmd == "pow"){
            n3 = n3.pow(n1, n2);
        }
        // prints result
        std::cout << n3 << std::endl;
    }
}

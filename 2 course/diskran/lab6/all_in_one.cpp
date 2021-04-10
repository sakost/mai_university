//
// Created by sakost on 08.03.2021.
//
#define FAST_MUL

//
// Created by sakost on 06.03.2021.
//

#ifndef DA_LAB_6_LONGARITHMETIC_H
#define DA_LAB_6_LONGARITHMETIC_H

#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

constexpr long double PI = std::acos(-1.l);

class TInvalidOperands : public std::exception {
public:
  const char *what() const noexcept override { return "Invalid operands"; }
};

#ifdef FAST_MUL

#include <complex>

#endif

// template<typename T>
class TLongArithmetic {
  using T = long long;
  //    static_assert(std::is_integral_v<T>, "Integral required.");

  static const T BASE_COUNT_DIGITS = 5;
  static const T BASE = std::pow(10, BASE_COUNT_DIGITS);

  friend std::ostream &operator<<(std::ostream &out,
                                  const TLongArithmetic &lhs);

  friend std::istream &operator>>(std::istream &in, TLongArithmetic &rhs);

  using ssizeType = typename std::string::size_type;
  using vsizeType = typename std::vector<T>::size_type;

public:
  TLongArithmetic() : TLongArithmetic(T()) {}

  TLongArithmetic(T n) { // NOLINT(google-explicit-constructor)
    mData.clear();
    if (n < BASE)
      mData.push_back(n);
    else {
      for (; n > 0; n /= BASE)
        mData.push_back(n % BASE);
    }
    Normalize();
  }

  explicit TLongArithmetic(const std::vector<T> &data) { mData = data; }

  explicit TLongArithmetic(std::string &input) {
    if (input.empty()) {
      mData.push_back(0);
      return;
    }

    input = TrimLeadingZeroes(input);

    std::stringstream tempSS;
    mData.clear();

    for (long long i = (long long)input.size() - 1; i >= 0;
         i -= BASE_COUNT_DIGITS) {
      ssizeType start = std::max(i - (long long)BASE_COUNT_DIGITS + 1ll, 0ll);
      ssizeType end = i - start + 1;

      tempSS << input.substr(start, end);
      T radix = 0;
      tempSS >> radix;
      mData.push_back(radix);
      tempSS.clear();
    }
  }

  explicit operator std::string() const {
    if (mData.empty())
      return std::string("0");
    std::stringstream res;
    res << mData.back();

    for (auto el = std::next(mData.crbegin()); el != mData.crend(); ++el) {
      res << std::setfill('0') << std::setw(BASE_COUNT_DIGITS) << *el;
    }

    return res.str();
  }

  bool operator==(const TLongArithmetic &rhs) const {
    return this == &rhs || mData == rhs.mData ||
           ((mData.empty() || (mData.size() == 1 && mData.front() == 0)) &&
            (rhs.mData.empty() ||
             (rhs.mData.size() == 1 && rhs.mData.front() == 0)));
  }

  bool operator!=(const TLongArithmetic &rhs) const {
    return mData != rhs.mData;
  }

  bool operator<(const TLongArithmetic &rhs) const {
    if (mData.size() != rhs.mData.size()) {
      return mData.size() < rhs.mData.size();
    }
    return std::lexicographical_compare(mData.rbegin(), mData.rend(),
                                        rhs.mData.rbegin(), rhs.mData.rend());
  }

  bool operator>(const TLongArithmetic &rhs) const {
    return !(*this < rhs) && (*this != rhs);
  }

  bool operator<=(const TLongArithmetic &rhs) const {
    return (*this == rhs) || (*this < rhs);
  }

  bool operator>=(const TLongArithmetic &rhs) const {
    return (*this == rhs) || (*this > rhs);
  }

  TLongArithmetic &operator+=(const TLongArithmetic &rhs) {
    auto maxSize = std::max(mData.size(), rhs.mData.size()) + 1;
    mData.resize(maxSize, (T)0);

    T carry = (T)0;
    for (vsizeType i = 0; i < maxSize || carry != 0; i++) {
      if (i >= mData.size()) {
        mData.push_back((T)0);
      }
      T cur = mData[i] + rhs.At(i) + carry;
      mData[i] = cur % BASE;
      carry = cur >= BASE;
    }
    this->Normalize();
    return *this;
  }

  TLongArithmetic operator+(const TLongArithmetic &rhs) const {
    TLongArithmetic temp = *this;
    temp += rhs;
    return temp;
  }

  TLongArithmetic &operator-=(const TLongArithmetic &rhs) {
    if (*this < rhs) {
      throw TInvalidOperands();
    }
    auto min_size = rhs.mData.size();

    T carry = (T)0;
    for (vsizeType i = 0; i < min_size || carry != 0; i++) {
      T cur = mData[i] - rhs.At(i) - carry;
      carry = cur < 0;
      if (carry != 0) {
        cur += BASE;
      }
      mData[i] = cur;
    }
    this->Normalize();
    return *this;
  }

  TLongArithmetic operator-(const TLongArithmetic &rhs) const {
    TLongArithmetic temp = *this;
    temp -= rhs;
    return temp;
  }

  TLongArithmetic &operator--() {
    *this -= 1;
    return *this;
  }

  TLongArithmetic operator--(int) { return --(*this); }

  TLongArithmetic &operator++() {
    *this += 1;
    return *this;
  }

  TLongArithmetic operator++(int) { return ++(*this); }

  TLongArithmetic &operator*=(const T &rhs) {
    T carry = (T)0;
    for (typename std::vector<T>::size_type i = 0;
         i < mData.size() || carry != 0; i++) {
      if (i == mData.size()) {
        mData.push_back(0);
      }
      T cur = carry + mData[i] * rhs;
      mData[i] = cur % BASE;
      carry = cur / BASE;
    }
    this->Normalize();
    return *this;
  }

  TLongArithmetic operator*(const T &rhs) const {
    TLongArithmetic temp = *this;
    temp *= rhs;
    return temp;
  }

  TLongArithmetic &operator*=(const TLongArithmetic &rhs) {
    *this = (*this * rhs);
    return *this;
  }

  TLongArithmetic operator*(const TLongArithmetic &rhs) const {
    TLongArithmetic res(0);

#ifdef FAST_MUL
    std::vector<base> fa(mData.begin(), mData.end()),
        fb(rhs.mData.begin(), rhs.mData.end());
    std::size_t n = 1;
    while (n < std::max(fa.size(), fb.size())) {
      n <<= 1;
    }
    n <<= 1;
    fa.resize(n), fb.resize(n);

    FFT(fa, false), FFT(fb, false);
    for (std::size_t i = 0; i < n; ++i) {
      fa[i] *= fb[i];
    }

    FFT(fa, true);

    res.mData.resize(n);
    T carry = 0;
    for (std::size_t i = 0; i < n || carry != 0; ++i) {
      T cur = std::llroundl(fa[i].real()) + carry;
      res.mData[i] = cur % BASE;
      carry = cur / BASE;
    }

#else
    vsizeType lhsSize = mData.size(), rhsSize = rhs.mData.size();

    res.mData.resize(lhsSize + rhsSize, 0);

    for (vsizeType i = 0; i < lhsSize; ++i) {
      T carry = 0;
      if (mData[i] == 0) {
        continue;
      }
      for (vsizeType j = 0; j < rhsSize || carry != 0; ++j) {
        T cur = mData[i] * rhs.At(j) + carry + res.mData[i + j];
        carry = cur / BASE;
        res.mData[i + j] = cur % BASE;
      }
    }
#endif

    res.Normalize();
    return res;
  }

  TLongArithmetic operator/(const TLongArithmetic &rhs) const {
    if (rhs == 0) {
      throw TInvalidOperands();
    }
    if (*this == rhs)
      return 1;
    if (*this < rhs)
      return 0;

    TLongArithmetic res, cv = 0;
    res.mData.resize(mData.size());

    for (long long i = mData.size() - 1; i >= 0; --i) {
      cv.mData.insert(cv.mData.begin(), mData[i]);
      if (cv.mData.back() == 0) {
        cv.mData.pop_back();
      }
      T x = 0, l = 0, r = BASE;
      while (l <= r) {
        T m = (l + r) / 2;
        TLongArithmetic cur(rhs * m);
        if (cur <= cv) {
          x = m;
          l = m + 1;
        } else {
          r = m - 1;
        }
      }
      res.mData[i] = x;
      cv = cv - rhs * x;
    }
    res.Normalize();
    return res;
  }

  TLongArithmetic &operator/=(const TLongArithmetic &rhs) {
    *this = *this / rhs;
    return *this;
  }

  TLongArithmetic operator%(const TLongArithmetic &rhs) {
    if (rhs == 2) {
      return mData.front() % 2;
    }
    if (rhs == 0) {
      throw TInvalidOperands();
    }
    if (mData.empty() || rhs == 1 || *this == rhs) {
      return 0;
    }

    if (*this < rhs) {
      return *this;
    }
    auto temp = *this / rhs;
    return *this - temp * (*this);
  }

  TLongArithmetic Power(const TLongArithmetic &rhs) {
    if (*this == 0) {
      if (rhs == 0) {
        throw TInvalidOperands();
      }
      return 0;
    }
    if (rhs == 0 || *this == 1) {
      return 1;
    }
    if (rhs == 1) {
      return *this;
    }
    TLongArithmetic result = 1, power = rhs, number = *this;
    while (power > 0) {
      if (power % 2 > 0) {
        result *= number;
      }
      number *= number;
      power /= 2;
    }
    return result;
  }

private:
  void Normalize() {
    if (mData.empty()) {
      mData.push_back((T)0);
      return;
    }
    while (mData.back() == 0 && mData.size() > 1) {
      mData.pop_back();
    }
  }

  T At(const vsizeType &i) const {
    if (i < 0 || i >= mData.size())
      return (T)0;
    return mData[i];
  }

  std::string TrimLeadingZeroes(const std::string &input) {
    ssizeType i = 0;
    while (input.size() != i - 1 && input[i] == '0') {
      i++;
    }
    return input.substr(i);
  }

#ifdef FAST_MUL
  using double_base = long double;
  using base = std::complex<double_base>;

  void FFT(std::vector<base> &a, bool inverse = false) const {
    std::vector<base>::size_type n = a.size();
    if (n == 1)
      return;

    std::vector<base> a0(n / 2), a1(n / 2);
    for (std::size_t i(0), j(0); i < n; i += 2, ++j) {
      a0[j] = a[i];
      a1[j] = a[i + 1];
    }

    FFT(a0, inverse);
    FFT(a1, inverse);

    double_base ang = 2 * PI / n * (inverse ? -1 : 1);
    base w(1), wn(std::cos(ang), std::sin(ang));
    for (std::size_t i = 0; i < n / 2; ++i) {
      a[i] = a0[i] + w * a1[i];
      a[i + n / 2] = a0[i] - w * a1[i];
      if (inverse) {
        a[i] /= 2;
        a[i + n / 2] /= 2;
      }
      w *= wn;
    }
  }

#endif

  std::vector<T> mData;
};

std::ostream &operator<<(std::ostream &out, const TLongArithmetic &lhs) {
  out << (std::string)lhs;
  return out;
}

std::istream &operator>>(std::istream &in, TLongArithmetic &rhs) {
  std::string inp;
  in >> inp;
  rhs = TLongArithmetic(inp);
  return in;
}

#endif // DA_LAB_6_LONGARITHMETIC_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
  ios_base::sync_with_stdio(false);
  cout.tie(nullptr);
  cin.tie(nullptr);

  TLongArithmetic a, b;
  std::vector<std::string> result = {"false", "true"};

  char op;
  while (std::cin >> a >> b >> op) {
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
    } catch (TInvalidOperands &e) {
      cout << "Error\n";
    }
  }
  cout.flush();
  return 0;
}

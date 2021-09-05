#include <cstdlib>

class Rational {
private:
    int num;
    int denom;

    void reduct();

public:
    Rational();
    Rational(int, int);

    int numerator() const;
    int denominator() const;

    Rational operator+() const;
    Rational operator-() const;

    Rational& operator+=(const Rational&);
    Rational& operator+=(int);

    Rational& operator-=(const Rational&);
    Rational& operator-=(int);

    Rational& operator*=(const Rational&);
    Rational& operator*=(int);

    Rational& operator/=(const Rational&);
    Rational& operator/=(int);

    Rational& operator++();
    Rational& operator--();

    Rational operator++(int);
    Rational operator--(int);
};

int calc_gcd(int a, int b) {
    while (a && b) {
        if (a > b) {
            a %= b;
        } else {
            b %= a;
        }
    }
    return a + b;
}

int calc_lcm(int a, int b) {
    return a*b / calc_gcd(a, b);
}

Rational::Rational(): num(0), denom(1) {}

Rational::Rational(int _num, int _denom) {
    if (_denom < 0) {
        _num = -_num;
        _denom = -_denom;
    }
    num = _num;
    denom = _denom;
    reduct();
}

void Rational::reduct() {
    int gcd = calc_gcd(std::abs(num), denom);
    num /= gcd;
    denom /= gcd;
}

int Rational::numerator() const {
    return num;
}

int Rational::denominator() const {
    return denom;
}

Rational Rational::operator-() const {
    return {-num, denom};
}

Rational Rational::operator+() const {
    return *this;
}

Rational operator+(const Rational& a, const Rational& b) {
    int common_denom = calc_lcm(a.denominator(), b.denominator());
    int new_num = common_denom / a.denominator() * a.numerator() +
                  common_denom / b.denominator() * b.numerator();
    return {new_num, common_denom};
}

Rational operator+(const Rational& a, int b) {
    return {a.numerator() + b * a.denominator(),
            a.denominator()};
}

Rational operator+(int a, const Rational& b) {
    return b + a;
}

Rational operator-(const Rational& a, const Rational& b) {
    return a + (-b);
}

Rational operator-(const Rational& a, int b) {
    return a + (-b);
}

Rational operator-(int a, const Rational& b) {
    return a + (-b);
}

Rational operator*(const Rational& a, int b) {
    return {a.numerator() * b, a.denominator()};
}

Rational operator*(int a, const Rational& b) {
    return b * a;
}

Rational operator*(const Rational& a, const Rational& b) {
    return {a.numerator() * b.numerator(),
            a.denominator() * b.denominator()};
}

Rational operator/(const Rational& a, int b) {
    return {a.numerator(), a.denominator() * b};
}

Rational operator/(int a, const Rational& b) {
    return a * Rational(b.denominator(), b.numerator());
}

Rational operator/(const Rational& a, const Rational& b) {
    return a * Rational(b.denominator(), b.numerator());
}

Rational& Rational::operator+=(const Rational& a) {
    return (*this = *this + a);
}

Rational& Rational::operator+=(int a) {
    return (*this = *this + a);
}

Rational& Rational::operator-=(const Rational& a) {
    return (*this = *this - a);
}

Rational& Rational::operator-=(int a) {
    return (*this = *this - a);
}

Rational& Rational::operator*=(const Rational& a) {
    return (*this = *this * a);
}

Rational& Rational::operator*=(int a) {
    return (*this = *this * a);
}

Rational& Rational::operator/=(const Rational& a) {
    return (*this = *this / a);
}

Rational& Rational::operator/=(int a) {
    return (*this = *this / a);
}

Rational& Rational::operator++() {
    *this += 1;
    return *this;
}

Rational& Rational::operator--() {
    *this -= 1;
    return *this;
}

Rational Rational::operator++(int) {
    Rational temp(*this);
    ++(*this);
    return temp;
}

Rational Rational::operator--(int) {
    Rational temp(*this);
    --(*this);
    return temp;
}

bool operator==(const Rational& a, const Rational& b) {
    return a.numerator() == b.numerator() && a.denominator() == b.denominator();
}

bool operator!=(const Rational& a, const Rational& b) {
    return !(a == b);
}


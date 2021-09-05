#include <math.h>

class Complex {
private:
    double re, im;
public:
    explicit Complex(double re);
    Complex(double re, double im);

    double Re() const;
    double Im() const;

    Complex operator+() const;
    Complex operator-() const;
};

Complex::Complex(double _re): re(_re), im(0) {}

Complex::Complex(double _re, double _im): re(_re), im(_im) {}

double Complex::Re() const {
    return re;
}

double Complex::Im() const {
    return im;
}

Complex Complex::operator+() const {
    return *this;
}

Complex Complex::operator-() const {
    return Complex(-re, -im);
}

Complex operator+(const Complex& a, const Complex& b) {
    return {a.Re() + b.Re(), a.Im() + b.Im()};
}

Complex operator+(const Complex& a, const double b) {
    return {a.Re() + b, a.Im()};
}

Complex operator+(const double a, const Complex& b) {
    return {b.Re() + a, b.Im()};
}

Complex operator-(const Complex& a, const Complex& b) {
    return a+(-b);
}

Complex operator-(const Complex& a, const double b) {
    return {a.Re() - b, a.Im()};
}

Complex operator-(const double a, const Complex& b) {
    return -(b - a);
}

Complex operator*(const Complex& a, const Complex& b) {
    return {a.Re() * b.Re() - a.Im() * b.Im(),
            a.Re() * b.Im() + b.Re() * a.Im()};
}

Complex operator*(const Complex& a, const double b) {
    return {a.Re() * b, a.Im() * b};
}

Complex operator*(const double a, const Complex& b) {
    return b * a;
}

Complex operator/(const Complex& a, const double b) {
    return {a.Re() / b, a.Im() / b};
}

Complex operator/(const Complex& a, const Complex& b) {
    Complex b_rev = Complex(b.Re(), -b.Im());

    return (a * b_rev) / (b * b_rev).Re();
}

Complex operator/(const double a, const Complex& b) {
    return Complex(a) / b;
}

bool operator==(const Complex& a, const Complex& b) {
    return (a.Re() == b.Re() && a.Im() == b.Im());
}

bool operator!=(const Complex& a, const Complex& b) {
    return !(a == b);
}

double abs(const Complex& a) {
    return std::sqrt(a.Re() * a.Re() + a.Im() * a.Im());
}


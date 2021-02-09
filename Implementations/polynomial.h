#pragma once

//
// Created on 04/10/2020
//

#include <algorithm>
#include <iostream>
#include <vector>

template <typename ValueType>
class Polynomial {
private:
    std::vector<ValueType> coeffs;
    ValueType ZERO;

    void delete_leading_zeros();

public:
    explicit Polynomial(const std::vector<ValueType>&);
    explicit Polynomial(const ValueType& = ValueType());
    template <typename Iter>
    Polynomial(Iter first, Iter last);

    int Degree() const;

    const ValueType& operator[](size_t index) const;
    // ValueType& operator[](size_t index);

    ValueType operator()(const ValueType&) const;

    typename std::vector<ValueType>::const_iterator begin() const;
    typename std::vector<ValueType>::const_iterator end() const;

    Polynomial<ValueType>& operator+=(const Polynomial<ValueType>&);
    Polynomial<ValueType>& operator+=(const ValueType&);

    Polynomial<ValueType>& operator-=(const Polynomial<ValueType>&);
    Polynomial<ValueType>& operator-=(const ValueType&);

    Polynomial<ValueType>& operator*=(const Polynomial<ValueType>&);
    Polynomial<ValueType>& operator*=(const ValueType&);

    Polynomial<ValueType> operator-() const;
};

template<typename ValueType>
Polynomial<ValueType> operator+(const Polynomial<ValueType>& lhs,
                                const Polynomial<ValueType>& rhs);

template<typename ValueType>
Polynomial<ValueType> operator+(const Polynomial<ValueType>& lhs, const ValueType& rhs);

template<typename ValueType>
Polynomial<ValueType> operator+(const ValueType& lhs, const Polynomial<ValueType>& rhs);

template<typename ValueType>
Polynomial<ValueType> operator-(const Polynomial<ValueType>& lhs,
                                const Polynomial<ValueType>& rhs);

template<typename ValueType>
Polynomial<ValueType> operator-(const Polynomial<ValueType>& lhs, const ValueType& rhs);

template<typename ValueType>
Polynomial<ValueType> operator-(const ValueType& lhs, const Polynomial<ValueType>& rhs);

template<typename ValueType>
Polynomial<ValueType> operator*(const Polynomial<ValueType>& lhs,
                                const Polynomial<ValueType>& rhs);

template<typename ValueType>
Polynomial<ValueType> operator*(const Polynomial<ValueType>& lhs, const ValueType& rhs);

template<typename ValueType>
Polynomial<ValueType> operator*(const ValueType& lhs, const Polynomial<ValueType>& rhs);

template<typename ValueType>
bool operator==(const Polynomial<ValueType>& lhs, const Polynomial<ValueType>& rhs);

template<typename ValueType>
bool operator==(const Polynomial<ValueType>& lhs, const ValueType& rhs);

template<typename ValueType>
bool operator==(const ValueType& lhs, const Polynomial<ValueType>& rhs);

template<typename ValueType>
bool operator!=(const Polynomial<ValueType>& lhs, const Polynomial<ValueType>& rhs);

template<typename ValueType>
bool operator!=(const Polynomial<ValueType>& lhs, const ValueType& rhs);

template<typename ValueType>
bool operator!=(const ValueType& lhs, const Polynomial<ValueType>& rhs);

template<typename ValueType>
ValueType abs(const ValueType& num);

template<typename ValueType>
std::ostream& operator<<(std::ostream& out, const Polynomial<ValueType>& poly);

// returns f(g(x))
template <typename ValueType>
Polynomial<ValueType> operator&(const Polynomial<ValueType>& f, const Polynomial<ValueType>& g);

template <typename ValueType>
Polynomial<ValueType> x_pow(size_t pow);

template <typename ValueType>
Polynomial<ValueType> operator/(Polynomial<ValueType> dividend,
                                const Polynomial<ValueType>& divisor);

template <typename ValueType>
Polynomial<ValueType> operator%(const Polynomial<ValueType>& dividend,
                                const Polynomial<ValueType>& divisor);

template <typename ValueType>
Polynomial<ValueType> operator,(Polynomial<ValueType> a, Polynomial<ValueType> b);

//
// implementation:
//

template<typename ValueType>
void Polynomial<ValueType>::delete_leading_zeros() {
    while (!coeffs.empty() && coeffs.back() == ValueType(0)) {
        coeffs.pop_back();
    }
}

template<typename ValueType>
Polynomial<ValueType>::Polynomial(const std::vector<ValueType>& coeffs_vec):
        coeffs(coeffs_vec.begin(), coeffs_vec.end()), ZERO(ValueType(0)) {
    delete_leading_zeros();
}

template<typename ValueType>
Polynomial<ValueType>::Polynomial(const ValueType& coeff): coeffs{coeff}, ZERO(ValueType(0)) {
    delete_leading_zeros();
}

template<typename ValueType>
template<typename Iter>
Polynomial<ValueType>::Polynomial(Iter first, Iter last): coeffs(first, last), ZERO(ValueType(0)) {
    delete_leading_zeros();
}

template<typename ValueType>
int Polynomial<ValueType>::Degree() const {
    return static_cast<int>(coeffs.size()) - 1;
}

template<typename ValueType>
ValueType Polynomial<ValueType>::operator()(const ValueType& arg) const {
    ValueType res(0);
    ValueType cur_pow(1);

    for (int deg = 0; deg <= this->Degree(); ++deg) {
        res += coeffs[deg] * cur_pow;
        cur_pow *= arg;
    }
    return res;
}

template<typename ValueType>
const ValueType& Polynomial<ValueType>::operator[](size_t index) const {
    if (static_cast<int>(index) > Degree()) {
        return ZERO;
    } else {
        return coeffs[index];
    }
}

template<typename ValueType>
typename std::vector<ValueType>::const_iterator Polynomial<ValueType>::begin() const {
    return coeffs.begin();
}

template<typename ValueType>
typename std::vector<ValueType>::const_iterator Polynomial<ValueType>::end() const {
    return coeffs.end();
}

template<typename ValueType>
Polynomial<ValueType> &Polynomial<ValueType>::operator+=(const Polynomial<ValueType>& add) {
    return (*this = *this + add);
}

template<typename ValueType>
Polynomial<ValueType> &Polynomial<ValueType>::operator+=(const ValueType& add) {
    return (*this = *this + add);
}

template<typename ValueType>
Polynomial<ValueType> &Polynomial<ValueType>::operator-=(const Polynomial<ValueType>& add) {
    return (*this = *this - add);
}

template<typename ValueType>
Polynomial<ValueType> &Polynomial<ValueType>::operator-=(const ValueType& add) {
    return (*this = *this - add);
}

template<typename ValueType>
Polynomial<ValueType>& Polynomial<ValueType>::operator*=(const Polynomial<ValueType>& rhs) {
    return (*this = *this * rhs);
}

template<typename ValueType>
Polynomial<ValueType>& Polynomial<ValueType>::operator*=(const ValueType& rhs) {
    return (*this = *this * rhs);
}

template<typename ValueType>
Polynomial<ValueType> Polynomial<ValueType>::operator-() const {
    std::vector<ValueType> res(coeffs);
    for (auto& coeff : res) {
        coeff = -coeff;
    }
    return Polynomial(res);
}

template<typename ValueType>
Polynomial<ValueType> operator+(const Polynomial<ValueType>& lhs,
                                const Polynomial<ValueType>& rhs) {
    std::vector<ValueType> res;
    for (int deg = 0; deg <= std::max(lhs.Degree(), rhs.Degree()); ++deg) {
        res.push_back(lhs[deg] + rhs[deg]);
    }
    return Polynomial(res);
}

template<typename ValueType>
Polynomial<ValueType> operator+(const Polynomial<ValueType>& lhs, const ValueType& rhs) {
    if (lhs.Degree() == -1) {
        return Polynomial(ValueType(0) + rhs);
    }
    std::vector<ValueType> res(lhs.begin(), lhs.end());
    res[0] += rhs;
    return Polynomial(res);
}

template<typename ValueType>
Polynomial<ValueType> operator+(const ValueType& lhs, const Polynomial<ValueType>& rhs) {
    return rhs + lhs;
}

template<typename ValueType>
Polynomial<ValueType> operator-(const Polynomial<ValueType>& lhs,
                                const Polynomial<ValueType>& rhs) {
    return lhs + (-rhs);
}

template<typename ValueType>
Polynomial<ValueType> operator-(const Polynomial<ValueType>& lhs, const ValueType& rhs) {
    return lhs + (-rhs);
}

template<typename ValueType>
Polynomial<ValueType> operator-(const ValueType& lhs, const Polynomial<ValueType>& rhs) {
    return lhs + (-rhs);
}

template<typename ValueType>
Polynomial<ValueType> operator*(const Polynomial<ValueType>& lhs,
                                const Polynomial<ValueType>& rhs) {
    const int LDEG = lhs.Degree(), RDEG = rhs.Degree();
    if (LDEG == -1 || RDEG == -1) {
        return (LDEG == -1 ? lhs : rhs);
    }

    std::vector<ValueType> res(LDEG + RDEG + 1, ValueType(0));

    for (int ldeg = 0; ldeg <= LDEG; ++ldeg) {
        for (int rdeg = 0; rdeg <= RDEG; ++rdeg) {
            res[ldeg + rdeg] += lhs[ldeg] * rhs[rdeg];
        }
    }
    return Polynomial(res);
}

template<typename ValueType>
Polynomial<ValueType> operator*(const Polynomial<ValueType>& lhs, const ValueType& rhs) {
    std::vector<ValueType> res(lhs.begin(), lhs.end());
    for (auto& coeff : res) {
        coeff *= rhs;
    }
    return Polynomial(res);
}

template<typename ValueType>
Polynomial<ValueType> operator*(const ValueType& lhs, const Polynomial<ValueType>& rhs) {
    return rhs * lhs;
}

template<typename ValueType>
bool operator==(const Polynomial<ValueType>& lhs, const Polynomial<ValueType>& rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template<typename ValueType>
bool operator==(const Polynomial<ValueType>& lhs, const ValueType& rhs) {
    return (lhs.Degree() <= 0) && (lhs[0] == rhs);
}

template<typename ValueType>
bool operator==(const ValueType& lhs, const Polynomial<ValueType>& rhs) {
    return (rhs == lhs);
}

template<typename ValueType>
bool operator!=(const Polynomial<ValueType>& lhs, const Polynomial<ValueType>& rhs) {
    return !(lhs == rhs);
}

template<typename ValueType>
bool operator!=(const Polynomial<ValueType>& lhs, const ValueType& rhs) {
    return !(lhs == rhs);
}

template<typename ValueType>
bool operator!=(const ValueType& lhs, const Polynomial<ValueType>& rhs) {
    return !(lhs == rhs);
}

template<typename ValueType>
ValueType abs(const ValueType& num) {
    if (num > ValueType(0)) {
        return num;
    } else {
        return -num;
    }
}

template<typename ValueType>
std::ostream& operator<<(std::ostream& out, const Polynomial<ValueType>& poly) {
    if (poly.Degree() == -1) {
        out << ValueType(0);
    } else {
        for (int deg = poly.Degree(); deg >= 0; --deg) {
            if (poly[deg] == ValueType(0)) {
                continue;
            }

            ValueType abs_coeff = abs(poly[deg]);

            if (abs_coeff == poly[deg]) {  // +
                if (deg != poly.Degree()) {
                    out << '+';
                }
            } else {  // -
                out << '-';
            }

            if (deg == 0 || abs_coeff != ValueType(1)) {
                out << abs_coeff;
                if (deg != 0) {
                    out << '*';
                }
            }

            if (deg != 0) {
                out << 'x';
                if (deg > 1) {
                    out << '^' << deg;
                }
            }
        }
    }
    return out;
}

// returns f(g(x))
template <typename ValueType>
Polynomial<ValueType> operator&(const Polynomial<ValueType>& f, const Polynomial<ValueType>& g) {
    Polynomial<ValueType> cur_pow(ValueType(1));
    Polynomial<ValueType> res(ValueType(0));

    for (int deg = 0; deg <= f.Degree(); ++deg) {
        res += f[deg] * cur_pow;
        cur_pow *= g;
    }
    return res;
}

template <typename ValueType>
Polynomial<ValueType> x_pow(size_t pow) {
    std::vector<ValueType> vec(pow + 1, ValueType(0));
    vec[pow] = ValueType(1);
    return Polynomial(vec);
}

template <typename ValueType>
Polynomial<ValueType> operator/(Polynomial<ValueType> dividend,
                                const Polynomial<ValueType>& divisor) {
    Polynomial<ValueType> res(ValueType(0));
    if (dividend.Degree() < divisor.Degree()) {
        return res;
    }

    for (int cur_pow = dividend.Degree() - divisor.Degree(); cur_pow >= 0; ) {
        Polynomial<ValueType> cur_mult =
                (dividend[dividend.Degree()] / divisor[divisor.Degree()])
                * x_pow<ValueType>(cur_pow);

        if (cur_mult == Polynomial<ValueType>(ValueType(0))) {
            break;
        }

        dividend -= cur_mult * divisor;
        res += cur_mult;

        cur_pow = dividend.Degree() - divisor.Degree();
    }

    return res;
}

template <typename ValueType>
Polynomial<ValueType> operator%(const Polynomial<ValueType>& dividend,
                                const Polynomial<ValueType>& divisor) {
    return dividend - (dividend / divisor) * divisor;
}

template <typename ValueType>
Polynomial<ValueType> operator,(Polynomial<ValueType> a, Polynomial<ValueType> b) {
    while (a.Degree() != -1 && b.Degree() != -1) {
        if (a.Degree() > b.Degree() ||
            (a.Degree() == b.Degree() && a[a.Degree()] > b[b.Degree()])) {
            a = a % b;
        } else {
            b = b % a;
        }
    }
    Polynomial<ValueType> res = a + b;
    return res / Polynomial(res[res.Degree()]);
}

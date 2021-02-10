//
// Created on 28.10.2019
//
// Bad code, should be rewritten (at least it works)
// More functions should be added

#pragma once

/*
Supports following functions:
for both positive and negative numbers:
    Reading
    Output
    Addition
    Subtraction
    Unary minus
    Multiplication
positive numbers only:
    Maximum
*/
////////////////////
const int POW10 = 1000 * 1000 * 1000;

void check(bool e) {
    if (!e)
        throw 1;
}

void skip_sym() {
    int c = cin.peek();
    while (c == ' ' || c == '\r' || c == '\n' || c == '\t') {
        cin.get();
        c = cin.peek();
    }
}

struct UInt {
//constructor UInt(vector<int> &digits) builds UInt via vector of POW10-based nums
//read() - reads UInt and returns it
//size() - returns size of POW10-based digits
//operator[index] - returns index POW10-based num
//print() - prints UInt
//isZero() - returns true if UInt == 0
    vector<int> digits;

    UInt(const vector<int>& digits) : digits(digits) {
        check(digits.size() > 0);
        check(digits.size() == 1 || digits.back() > 0);
        for (int digit : digits)
            check(digit >= 0 || digit < POW10);
    }

    int size() const {
        return (int)digits.size();
    }

    static UInt read() {
        skip_sym();
        vector<char> source;

        int c = cin.get();
        check(c >= '0' && c <= '9');

        while (c >= '0' && c <= '9') {
            source.push_back(c);
            c = cin.get();
        }

        cin.unget();

        vector<int> digits;
        int n = 0;
        int pow10 = 1;
        for (int i = source.size() - 1; i >= 0; i--) {
            n += (source[i] - '0') * pow10;
            pow10 *= 10;
            if (pow10 == POW10) {
                digits.push_back(n);
                n = 0;
                pow10 = 1;
            }
        }

        if(pow10 > 1)
            digits.push_back(n);

        return UInt(digits);
    }

    int operator[] (const int index) const {
        check(index >= 0);
        if (index < digits.size())
            return digits[index];
        else
            return 0;
    }

    void print() const {
        cout << digits.back();

        for (int i = digits.size() - 2; i >= 0; i--)
            for (int pow = POW10/10; pow >= 1; pow /= 10)
                cout << digits[i] / pow % 10;
    }

    bool isZero() const{
        return (digits.size() == 1 && digits[0] == 0);
    }
};

const UInt UInt_ZERO = UInt(vector<int> {0});

int compare(const UInt& left, const UInt& right) {
    //returns negative num if (left < right), null if (left == right), positive num if(left > right)
    if (left.size() != right.size())
        return left.size() - right.size();

    for (int i = left.size()-1; i >= 0; i--) {
        if (left[i] != right[i]) {
            return left[i] - right[i];
        }
    }

    return 0;
}

UInt operator + (const UInt& left, const UInt& right) {
    vector<int> digits;
    int carry = 0;
    for (int i = 0; i < std::max(left.size(), right.size()) || carry != 0; i++) {
        carry += left[i] + right[i];
        digits.push_back(carry % POW10);
        carry /= POW10;
    }
    check(carry == 0);
    return UInt(digits);
}

UInt operator - (const UInt& left, const UInt& right) {
    //left > right
    vector<int> digits;
    int carry = 0;
    for (int i = 0; i < left.size(); i++) {
        carry += left[i] - right[i] + POW10;
        digits.push_back(carry % POW10);
        carry /= POW10;
        carry--;
    }


    //delete leading nulls
    while ((int) digits.size() > 1 && digits.back() == 0)
        digits.pop_back();

    return UInt(digits);
}

UInt operator * (const UInt& left, const UInt& right) {
    if (left.isZero() || right.isZero())
        return UInt_ZERO;

    vector<int> digits;
    ll carry = 0;

    for (int j = 0; j < right.size(); j++) {
        for (int i = 0; i < left.size() || carry != 0; i++) {
            if (i + j >= digits.size())
                digits.push_back(0);

            carry += 1LL * left[i] * right[j] + digits[i+j];
            digits[i+j] = carry % POW10;

            carry /= POW10;
        }
    }

    return UInt(digits);
}

UInt max(const UInt& a, const UInt& b) {
    //returns maximum UInt of two given UInt nums
    if (compare(a, b) > 0)
        return a;
    else
        return b;
}

UInt min(const UInt& a, const UInt& b) {
    if (compare(a, b) > 0)
        return b;
    else
        return a;
}

UInt max(const std::initializer_list<UInt> list) {
    //returns maximum UInt of given UInt list
    UInt cur_max = *list.begin();

    for (auto ptr = list.begin() + 1; ptr < list.end(); ptr++) {
        if (compare(*ptr, cur_max) > 0)
            cur_max = *ptr;
    }

    return cur_max;
}

struct Int {
    //negate() - returns Int with opposite sign
    UInt modulus;

    int sign; // -1/0/1

    Int(const UInt &modulus, const int sign) : modulus(modulus), sign(sign) {
        check(sign == 0 || sign == -1 || sign == 1);
        check(sign != 0 || modulus.isZero());
    }

    static Int read() {
        skip_sym();

        int c = cin.peek();
        int sign = 1;

        if (c == '-') {
            sign = -1;
            cin.get();
        }

        UInt modulus = UInt::read();
        if (modulus.isZero())
            sign = 0;



        return Int(modulus, sign);
    }

    void print() const {
        if (sign == -1)
            cout << '-';

        modulus.print();
    }

    bool isZero() const {
        return sign == 0;
    }

};

Int operator - (const Int& num)  {
    return Int(num.modulus, -num.sign);
}

Int operator - (const Int& left, const Int& right) {
    if (right.isZero())
        return left;
    if (left.isZero())
        return -right;

    //if different sign -3 - 4     3 - (-4), add them
    if (left.sign != right.sign) {
        return Int(left.modulus + right.modulus, left.sign);
    }
    //if the same sign -3 - (-4) = 1      3 - 4 = -1
    int cmp = compare(left.modulus, right.modulus);
    if (cmp == 0)
        return Int(UInt_ZERO, 0);
    else if (cmp < 0)
        return Int(right.modulus - left.modulus, -left.sign);
    else {
        check(cmp > 0);
        return Int(left.modulus - right.modulus, left.sign);
    }
}

Int operator + (const Int& left, const Int& right) {
    if (left.isZero())
        return right;
    if (right.isZero())
        return left;

    if (left.sign == right.sign)
        return Int(left.modulus + right.modulus, left.sign);

    int cmp = compare(left.modulus, right.modulus);
    if (cmp < 0)
        return Int(right.modulus - left.modulus, right.sign);
    else if (cmp == 0)
        return Int(UInt_ZERO, 0);
    else
        return Int(left.modulus - right.modulus, left.sign);
}

Int operator * (const Int& left, const Int& right) {
    if (left.sign == 0 || right.sign == 0)
        return Int(UInt_ZERO, 0);

    if (left.sign == right.sign)
        return Int(left.modulus * right.modulus, 1);
    else
        return Int(left.modulus * right.modulus, -1);
}

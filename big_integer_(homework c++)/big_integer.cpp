#include "big_integer.h"
#include <iostream>
using namespace std;

typedef big_integer const T;

int big_integer::cmpr(T &b) const {
    int sign1 = sign;
    int sign2 = b.sign;

    if (sign1 == sign2) {
        if (digits.size() < b.digits.size())
            return -sign1;
        if (digits.size() > b.digits.size())
            return sign1;

        for (int i = (int)digits.size() - 1; i >= 0; --i)
            if (digits[i] < b.digits[i])
                return -sign1;
            else if (digits[i] > b.digits[i])
                return sign1;
        return 0;
    } else
        return (sign1 < sign2 ? -1 : 1);
}

int big_integer::divShort(int b) {//only positive or nought
    unsigned int carry = 0;
    for (int i=(int)digits.size() - 1; i>=0; --i) {
        long long cur = digits[i] + carry * (1LL<<CNT_BIT_OF_BASE);
        digits[i] = (unsigned int)(cur / b);
        carry = (unsigned int)(cur % b);
    }
    while (digits.size() > 1 && digits.back() == 0)
        digits.pop_back();
    return carry;
}

big_integer::big_integer() {
    sign = 0;
    digits.push_back(0);
}

//copy
big_integer::big_integer(T& value)  {
    sign = value.sign;
    digits = value.digits;
}

big_integer::big_integer(const std::string& number) {
    string s;
    if (number == "0") {
        sign = 0;
        s = number;
    } else if (number[0] == '-') {
        sign = -1;
        s = number;
        s.erase(0, 1);
    } else {
        s = number;
        sign = 1;
    }

    for (int i = 0; i < s.size(); ++i)
        s[i] -= '0';

    while (s != "") {
        long long cur = 0;
        string res;
        for (int i = 0; i < s.size(); ++i) {
            cur = cur * 10 + s[i];
            unsigned int curDig = cur>>CNT_BIT_OF_BASE;
            if (curDig != 0 || res != "")
                res += curDig;
            cur = (unsigned int)cur;
        }
        s = res;
        digits.push_back(cur);
    }
    while (digits.size() > 1 && digits.back() == 0)
        digits.pop_back();
    //for (int i = 0; i < digits.size(); ++i) cout << digits[i] << " ";cout << endl;
}

big_integer::big_integer(int x) {
    if (x == 0) {
        sign = 0;
        digits.push_back(0);
    } else if (x > 0) {
        sign = 1;
        digits.push_back(x);
    } else {
        sign = -1;
        digits.push_back(-x);
    }
}

//assign
big_integer& big_integer::operator = (T& value) {
    digits = value.digits;
    sign = value.sign;
    return *this;
}

//binary operations

big_integer& big_integer::operator += (T& value) {
    int sign1 = sign;
    int sign2 = value.sign;
    big_integer x = *this;
    big_integer y = value;
    if (sign1 >= 0 && sign2 >= 0) {
        bool carry = 0;
        for (size_t i=0; i< max(x.digits.size(), x.digits.size()) || carry; ++i) {
            if (i == x.digits.size())
                x.digits.push_back(0);
            long long cur = 1LL * x.digits[i] + carry + (i < x.digits.size() ? y.digits[i] : 0);
            carry = x.digits[i] >> CNT_BIT_OF_BASE;
            x.digits[i] = cur;
        }

    }  else if (sign1 <= 0 && sign2 <= 0) {
        x.sign *= -1;
        y.sign *= -1;
        *this = x + y;
        this->sign = -1;
    } else if (sign1 < 0 && sign2 > 0) {
        x.sign *= -1;
        int resCmpr = x.cmpr(y);
        if (resCmpr >= 0)
            *this = x - y;
        else
            *this = y - x;
        this->sign = -resCmpr;
    } else if (sign1 > 0 && sign2 < 0) {
        y.sign *= -1;
        int resCmpr = y.cmpr(x);
        if (resCmpr >= 0)
            *this = y - x;
        else
            *this = x - y;
        this->sign = -resCmpr;
    }
    return *this;
}

big_integer& big_integer::operator -= (T& value) {
    big_integer x = *this;
    big_integer y = value;
    int sign1 = x.sign;
    int sign2 = y.sign;
    int resCmpr = x.cmpr(y);
    if (sign1 >= 0 && sign2 >= 0 && resCmpr >= 0) {
        bool carry = 0;
        for (size_t i=0; i<y.digits.size() || carry; ++i) {
            long long cur = 1LL * x.digits[i] - carry - (i < y.digits.size() ? y.digits[i] : 0);
            carry = carry < 0;
            x.digits[i] = cur;
        }

        while (x.digits.size() > 1 && x.digits.back() == 0)
            x.digits.pop_back();
        x.sign = resCmpr;
        *this = x;
    } else {
        y.sign *= -1;
        *this = x + y;
    }
    return *this;
}

big_integer& big_integer::operator *= (T& value) {}//TODO write
big_integer& big_integer::operator /= (T& value) {}//TODO write

big_integer operator + (big_integer a, T& b) {
    a += b;
    return a;
}

big_integer operator - (big_integer a, T& b) {
    a -= b;
    return a;
}

big_integer operator * (big_integer a, T& b) {
    a *= b;
    return a;
}

big_integer operator / (big_integer a, T& b) {
    a /= b;
    return a;
}

//unary operation
big_integer big_integer::operator -() const {}
big_integer big_integer::operator +() const {}

std::string to_string(const big_integer& a) {
    string res;
    bool isNegative = false;
    big_integer b = a;
    if (b.sign < 0) {
        isNegative = true;
        b.sign = 1;
    } else if (b.sign == 0) return "0";

    while (b.digits.back() != 0 || b.digits.size() > 1)
        res += char(b.divShort(10) + '0');
    for (int i = 0; 2 * i < res.size(); ++i) {
        char tmp = res[i];
        res[i] = res[res.size() - i - 1];
        res[res.size() - i - 1] = tmp;
    }
    if (isNegative)
        return "-" + res;
    return res;
}

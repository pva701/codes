#include "big_integer.h"
#include <iostream>
#include <cstdio>
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

unsigned short big_integer::divShort(unsigned short b) {//only positive or nought
    unsigned short carry = 0;
    for (int i=(int)digits.size() - 1; i>=0; --i) {
        unsigned int cur = digits[i] + carry * BASE;
        digits[i] = (unsigned short)(cur / b);
        carry = (unsigned short)(cur % b);
    }
    while (digits.size() > 1 && digits.back() == 0)
        digits.pop_back();
    return carry;
}

big_integer big_integer::divLong(const big_integer& value, big_integer& remainder) {
    big_integer c;
    big_integer val = value;
    c.digits.pop_back();
    remainder = 0;
    int sign1 = sign;
    int sign2 = val.sign;
    if (sign < 0) sign = 1;
    if (val.sign < 0) val.sign = 1;
    for (int i = (int)digits.size() - 1; i >= 0; --i) {
        remainder = remainder * BASE + digits[i];//O(n)
        int l = 0, r = BASE, mid;
        while (l + 1 < r) {//bin search by digit, O(1)
            mid = (l + r) / 2;
            if (val * mid <= remainder)
                l = mid;
            else
                r = mid;
        }
        c.digits.push_back(l);
        remainder -= l * val;
    }
    for (int i = 0; 2 * i < c.digits.size(); ++i) {
        short int tmp = c.digits[i];
        c.digits[i] = c.digits[c.digits.size() - i - 1];
        c.digits[c.digits.size() - i - 1] = tmp;
    }
    while (c.digits.size() > 1 && c.digits.back() == 0)
        c.digits.pop_back();

    if (c.digits.back() == 0 && c.digits.size() == 1)
        c.sign = 0;
    else
        c.sign = sign1 * sign2;

    if (remainder.digits.back() == 0 && remainder.digits.size() == 1)
        remainder.sign = 0;
    else
        remainder.sign = sign1;
    return c;
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

    for (size_t i = 0; i < s.size(); ++i)
        s[i] -= '0';

    while (s != "") {
        unsigned int cur = 0;
        string res;
        for (size_t i = 0; i < s.size(); ++i) {
            cur = cur * 10 + s[i];
            unsigned short curDig = cur>>CNT_BIT_OF_BASE;
            if (curDig != 0 || res != "")
                res += curDig;
            cur = (unsigned short)cur;
        }
        s = res;
        digits.push_back(cur);
    }
    while (digits.size() > 1 && digits.back() == 0)
        digits.pop_back();
//    for (int i = 0; i < digits.size(); ++i) cout << digits[i] << " ";cout << endl;
}

big_integer::big_integer(int x) {
    char ss[30];
    sprintf(ss, "%d", x);
    *this = big_integer(string(ss));
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
        for (size_t i=0; i < max(x.digits.size(), y.digits.size()) || carry; ++i) {
            if (i == x.digits.size())
                x.digits.push_back(0);
            unsigned int cur = (unsigned int)x.digits[i] + carry + (i < x.digits.size() ? y.digits[i] : 0);
            carry = (unsigned short)(cur >> CNT_BIT_OF_BASE);
            x.digits[i] = (unsigned short)cur;
        }
        x.sign = sign1 != 0 || sign2 != 0;
        *this = x;
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
            int cur = (int)x.digits[i] - (int)carry - (int)(i < y.digits.size() ? y.digits[i] : 0);//here int
            carry = cur < 0;
            if (carry) cur += BASE;
            x.digits[i] = (unsigned short)cur;
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

big_integer& big_integer::operator *= (T& value) {
    big_integer c;
    c.digits.resize(digits.size() + value.digits.size());
    for (size_t i=0; i<digits.size(); ++i) {
        unsigned short carry = 0;
        for (size_t j=0; j<value.digits.size() || carry; ++j) {
            unsigned int cur = (unsigned int)c.digits[i+j] + (unsigned int)digits[i] * (j < value.digits.size() ? value.digits[j] : 0) + carry;
            c.digits[i + j] = (unsigned short)cur;
            carry = (unsigned short)(cur>>CNT_BIT_OF_BASE);
        }
    }
    while (c.digits.size() > 1 && c.digits.back() == 0)
        c.digits.pop_back();
    c.sign = sign * value.sign;
    return *this = c;
}

big_integer& big_integer::operator /= (T& value) {
    big_integer rem;
    big_integer res = this->divLong(value, rem);
    return *this = res;
}

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

bool operator < (const big_integer& a, const big_integer& b) {
    return a.cmpr(b) < 0;
}

bool operator <= (const big_integer& a, const big_integer& b) {
    return a.cmpr(b) <= 0;
}

bool operator > (const big_integer& a, const big_integer& b) {
    return a.cmpr(b) > 0;
}

bool operator >= (const big_integer& a, const big_integer& b) {
    return a.cmpr(b) >= 0;
}

bool operator == (const big_integer& a, const big_integer& b) {
    return a.cmpr(b) == 0;
}

bool operator != (const big_integer& a, const big_integer& b) {
    return a.cmpr(b) != 0;
}

//unary operation
big_integer big_integer::operator -() const {}
big_integer big_integer::operator +() const {}

//bit operation
big_integer& big_integer::operator >>=(int b) {
}

std::string to_string(const big_integer& a) {
    string res;
    bool isNegative = false;
    big_integer b = a;
    if (b.sign < 0) {
        isNegative = true;
        b.sign = 1;
    } else if (b.sign == 0) return "0";
    while (b.digits.size() >  1 || b.digits.back() != 0)
        res += char(b.divShort(10) + '0');
    for (size_t i = 0; 2 * i < res.size(); ++i) {
        char tmp = res[i];
        res[i] = res[res.size() - i - 1];
        res[res.size() - i - 1] = tmp;
    }

    if (isNegative)
        return "-" + res;
    return res;
}

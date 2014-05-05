#include "big_integer.h"
#include <cstdio>
#include <algorithm>

int big_integer::cmpr(const big_integer &b) const {
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
    if (digits.size() == 1 && digits.back() == 0)
        sign = 0;
    return carry;
}

big_integer big_integer::mulShort(unsigned short b) {
    big_integer a = *this;
    unsigned int carry = 0;
    for (size_t i=0; i<a.digits.size() || carry; ++i) {
        if (i == a.digits.size())
            a.digits.push_back (0);
        unsigned int cur = carry + (unsigned int)a.digits[i] * b;
        a.digits[i] = (unsigned short)cur;
        carry = (unsigned short)(cur>>CNT_BIT_OF_BASE);
    }
    while (a.digits.size() > 1 && a.digits.back() == 0)
        a.digits.pop_back();
    if (a.digits.size() == 1 && a.digits.back() == 0)
        a.sign = 0;
    return a;
}

void big_integer::inverse(int lenbit = -1) {
    additionalCode();
    if (lenbit != -1)
        while ((int)digits.size() < lenbit)
            digits.push_back(0);
    for (size_t i = 0; i < digits.size(); ++i)
        digits[i] = ~digits[i];
}

big_integer big_integer::divLong(const big_integer& value, big_integer& remainder) {
    big_integer a = *this;
    big_integer b = value;

    int sign1 = a.sign;
    int sign2 = b.sign;
    if (a.sign < 0) a.sign *= -1;
    if (b.sign < 0) b.sign *= -1;

    if (a < b) {
        remainder = *this;
        return big_integer();
    }

    if ((int)2 * b.digits.back() < BASE) {
        a = a.mulShort(BASE / (b.digits.back() + 1));
        b = b.mulShort(BASE / (b.digits.back() + 1));
    }

    int n = b.digits.size();
    int m = a.digits.size() - n;
    big_integer q;
    q.digits.resize(m + 1);
    big_integer saveB = b;
    big_integer pw = b;

    reverse(pw.digits.begin(), pw.digits.end());
    for (int i = 0; i < m; ++i)
        pw.digits.push_back(0);
    reverse(pw.digits.begin(), pw.digits.end());

    if (a >= pw) {
        q.digits[m] = 1;
        a -= pw;
    }

    for (int j = m - 1; j >= 0; --j) {
        unsigned short dig1 = (n + j < (int)a.digits.size() ? a.digits[n + j] : 0);
        unsigned short dig2 = (n + j - 1 < (int)a.digits.size() ? a.digits[n + j - 1] : 0);
        unsigned int curq = ((unsigned int)dig1 * BASE + dig2) / b.digits[n - 1];
        curq = std::min(curq, (unsigned int)(BASE - 1));
        curq++;

        while (true) {//almost two steps
            curq--;
            pw = saveB;
            unsigned int carry = 0;
            for (size_t i = 0; i < pw.digits.size() || carry; ++i) {
                if (i == pw.digits.size())
                    pw.digits.push_back(0);
                unsigned int cur = carry + (unsigned int)pw.digits[i] * curq;
                pw.digits[i] = cur;
                carry = (unsigned short)(cur>>CNT_BIT_OF_BASE);
            }

            while (pw.digits.size() > 1 && pw.digits.back() == 0)
                pw.digits.pop_back();
            if (pw.digits.size() == 1 && pw.digits.back() == 0)
                pw.sign = 0;
            int power = (pw.sign == 0 ? 1 : j + pw.digits.size());
            if (power > (int)a.digits.size())
                continue;
            carry = 0;
            std::vector <unsigned short> saveDigits(a.digits.begin() + j, a.digits.begin() + j + pw.digits.size());
            for (size_t i=0; i + j < a.digits.size() && (i < pw.digits.size() || carry); ++i) {
                int cur = (int)a.digits[i + j] - (int)carry - (int)(i < pw.digits.size() ? pw.digits[i] : 0);//here int
                carry = cur < 0;
                if (carry) cur += BASE;
                a.digits[i + j] = (unsigned short)cur;
            }

            if (carry != 0) {
                for (size_t i = 0; i < saveDigits.size(); ++i)
                    a.digits[i + j] = saveDigits[i];
            } else break;
        }

        while (a.digits.size() > 1 && a.digits.back() == 0)
            a.digits.pop_back();
        if (a.digits.size() == 1 && a.digits.back() == 0)
            a.sign = 0;
        q.digits[j] = curq;
    }

    while (q.digits.size() > 1 && q.digits.back() == 0) q.digits.pop_back();
    q.sign = sign1 * sign2;
    remainder = *this - value * q;
    return q;
}


void big_integer::additionalCode() {
    sign *= -1;
    *this -= 1;
}

int big_integer::myOr(int x, int y) {return x | y;}
int big_integer::myAnd(int x, int y) {return x & y;}
int big_integer::myXor(int x, int y) {return x ^ y;}

big_integer& big_integer::executeBit(big_integer curb, int (*f)(int, int)) {
   big_integer cura = *this;
   big_integer res;
   res.digits.pop_back();
   res.digits.resize(std::max(digits.size(), curb.digits.size()));
   bool neg1 = sign < 0;
   bool neg2 = curb.sign < 0;
   while (cura.digits.size() < res.digits.size()) cura.digits.push_back(0);
   while (curb.digits.size() < res.digits.size()) curb.digits.push_back(0);

   if (neg1)
       cura.inverse(res.digits.size());
   if (neg2)
       curb.inverse(res.digits.size());

   for (size_t i = 0; i < res.digits.size(); ++i)
       res.digits[i] = f(cura.digits[i], curb.digits[i]);

   if (f(neg1, neg2)) res.sign = -1;
   else res.sign = 1;
   int sv = res.sign;
   if (res.sign < 0)
       res.inverse();
   res.sign = sv;
   while (res.digits.size() > 1 && res.digits.back() == 0)
       res.digits.pop_back();
   if (res.digits.size() == 1 && res.digits.back() == 0)
       res.sign = 0;
   return *this = res;
}

//public
big_integer::big_integer() {
    sign = 0;
    digits.push_back(0);
}

//copy
big_integer::big_integer(const big_integer& value)  {
    sign = value.sign;
    digits = value.digits;
}

big_integer::big_integer(const std::string& number) {
    std::string s;
    if (number == "0" || number == "-0" || number == "+0") {
        sign = 0;
        s = number;
        if (s == "-0" || s == "+0")
            s.erase(0, 1);
    } else if (number[0] == '-') {
        sign = -1;
        s = number;
        s.erase(0, 1);
    } else {
        s = number;
        if (s[0] == '+')
            s.erase(0, 1);
        sign = 1;
    }

    for (size_t i = 0; i < s.size(); ++i)
        s[i] -= '0';

    while (s != "") {
        unsigned int cur = 0;
        std::string res;
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
}

big_integer::big_integer(int x) {
    char ss[30];
    sprintf(ss, "%d", x);
    *this = big_integer(std::string(ss));
}

//assign
big_integer& big_integer::operator = (const big_integer& value) {
    digits = value.digits;
    sign = value.sign;
    return *this;
}

//binary operations

big_integer& big_integer::operator += (const big_integer& value) {
    int sign1 = sign;
    int sign2 = value.sign;
    big_integer x = *this;
    big_integer y = value;

    if (sign1 >= 0 && sign2 >= 0) {
        bool carry = 0;
        for (size_t i=0; i < std::max(x.digits.size(), y.digits.size()) || carry; ++i) {
            if (i == x.digits.size())
                x.digits.push_back(0);
            unsigned int cur = (unsigned int)x.digits[i] + carry + (i < y.digits.size() ? y.digits[i] : 0);
            carry = (unsigned short)(cur >> CNT_BIT_OF_BASE);
            x.digits[i] = (unsigned short)cur;
        }
        x.sign = sign1 != 0 || sign2 != 0;
        *this = x;
    }  else if (sign1 <= 0 && sign2 <= 0) {
        x.sign *= -1;
        y.sign *= -1;
        x += y;
        *this = x;
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

big_integer& big_integer::operator -= (const big_integer& value) {
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

big_integer& big_integer::operator *= (const big_integer& value) {
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

big_integer& big_integer::operator /= (const big_integer& value) {
    big_integer rem;
    big_integer res = this->divLong(value, rem);
    return *this = res;
}

big_integer& big_integer::operator %= (const big_integer& value) {
    big_integer rem;
    this->divLong(value, rem);
    return *this = rem;
}

big_integer operator + (big_integer a, const big_integer& b) {
    a += b;
    return a;
}

big_integer operator - (big_integer a, const big_integer& b) {
    a -= b;
    return a;
}

big_integer operator * (big_integer a, const big_integer& b) {
    a *= b;
    return a;
}

big_integer operator / (big_integer a, const big_integer& b) {
    a /= b;
    return a;
}

big_integer operator % (big_integer a, const big_integer& b) {
    a %= b;
    return a;
}

//unary operation
big_integer big_integer::operator -() const {
    big_integer b = *this;
    b.sign *= -1;
    return b;
}

big_integer big_integer::operator +() const {
    return *this;
}

big_integer& big_integer::operator++() {
    *this += 1;
    return *this;
}

big_integer big_integer::operator++(int) {
    big_integer b = *this;
    *this += 1;
    return b;
}

big_integer& big_integer::operator--() {
    *this -= 1;
    return *this;
}

big_integer big_integer::operator--(int) {
    big_integer b = *this;
    *this -= 1;
    return b;
}

//compare
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

//bit operation
big_integer& big_integer::operator |= (const big_integer& value) {
    return executeBit(value, myOr);

}

big_integer& big_integer::operator &= (const big_integer& value) {
    return executeBit(value, myAnd);
}

big_integer& big_integer::operator ^= (const big_integer& value) {
    return executeBit(value, myXor);
}

big_integer operator | (big_integer a, const big_integer& b) {
    return a |= b;
}

big_integer operator & (big_integer a, const big_integer& b) {
    return a &= b;
}

big_integer operator ^ (big_integer a, const big_integer& b) {
    return a ^= b;
}

big_integer& big_integer::operator >>=(int b) {
    if (sign >= 0) {
        int leng = ((int)digits.size() - 1) * CNT_BIT_OF_BASE;
        unsigned short x = digits.back();
        for (; x; leng++, x >>= 1);

        if (b >= leng) {
            while (digits.size() > 0) digits.pop_back();
            sign = 0;
            digits.push_back(0);
        } else {
            reverse(digits.begin(), digits.end());
            for (int i = 0; i < b / CNT_BIT_OF_BASE; ++i) digits.pop_back();
            b %= CNT_BIT_OF_BASE;
            reverse(digits.begin(), digits.end());
            if (b != 0) {
                digits[0] >>= b;
                for (int i = 1; i < digits.size(); ++i) {
                    unsigned short rem = digits[i] & ((1<<b) - 1);
                    digits[i - 1] |= rem<<(CNT_BIT_OF_BASE - b);
                    digits[i] >>= b;
                }
                while (digits.size() > 1 && digits.back() == 0) digits.pop_back();
            }
        }
    } else {
        this->additionalCode();
        *this >>= b;
        this->additionalCode();
    }
    return *this;
}

big_integer& big_integer::operator <<=(int b) {
    reverse(digits.begin(), digits.end());
    for (int i = 0; i < b / CNT_BIT_OF_BASE; ++i)
        digits.push_back(0);
    reverse(digits.begin(), digits.end());

    b %= CNT_BIT_OF_BASE;
    if (b != 0) {
        unsigned short carry = digits[0] >>(CNT_BIT_OF_BASE - b);
        digits[0] <<= b;
        for (int i = 1; i < digits.size(); ++i) {
            unsigned short newcarry = digits[i]>>(CNT_BIT_OF_BASE - b);
            digits[i] <<= b;
            digits[i] |= carry;
            carry = newcarry;
        }
        if (carry) digits.push_back(carry);
        while (digits.size() > 1 && digits.back() == 0) digits.pop_back();
    }
    return *this;
}

big_integer operator <<(big_integer a, int b) {
    a <<= b;
    return a;
}

big_integer operator >>(big_integer a, int b) {
    a >>= b;
    return a;
}


big_integer big_integer::operator ~() const {
    return -(*this + 1);
}

std::string to_string(const big_integer& a) {
    std::string res;
    bool isNegative = false;
    big_integer b = a;
    if (b.sign < 0) {
        isNegative = true;
        b.sign = 1;
    } else if (b.sign == 0) return "0";
    while (b.digits.size() >  1 || b.digits.back() != 0)
        res += char(b.divShort(10) + '0');
    reverse(res.begin(), res.end());
    if (isNegative)
        return "-" + res;
    return res;
}

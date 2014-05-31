#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H
#include <vector>
#include <string>
#include <cstring>
#include "optimize_vector.h"


struct big_integer {
private:
    static const int CNT_BIT_OF_BASE = 32;
    static const long long BASE = 1LL<<CNT_BIT_OF_BASE;
    OptimizeVector digits;
    int sign;

    int cmpr(const big_integer& b) const;
    unsigned int divShort(unsigned int b);
    void mulShort(unsigned int b);
    big_integer divLong(const big_integer& value, big_integer& remainder);
    void additionalCode();
    void inverse(int lenbit);
    big_integer& executeBit(big_integer b, unsigned int (*f)(unsigned int, unsigned int));
    static unsigned int myOr(unsigned int x, unsigned int y);
    static unsigned int myAnd(unsigned int x, unsigned int y);
    static unsigned int myXor(unsigned int x, unsigned int y);
/*
  a.back() < 0 - negative
  a.back() > 0 - positive
  a.size() == 1 && a.back() == 0 - zero
*/

public:
    //costructors
    big_integer();
    big_integer(const big_integer& value);
    explicit big_integer(const std::string& s);
    big_integer(int x);

    //assign
    big_integer& operator = (const big_integer& value);

    //binary operations
    big_integer& operator += (const big_integer& value);
    big_integer& operator -= (const big_integer& value);
    big_integer& operator *= (const big_integer& value);
    big_integer& operator /= (const big_integer& value);
    big_integer& operator %= (const big_integer& value);

    big_integer& operator |= (const big_integer& value);
    big_integer& operator &= (const big_integer& value);
    big_integer& operator ^= (const big_integer& value);

    //compare
    friend bool operator < (const big_integer& a, const big_integer& b);
    friend bool operator <= (const big_integer& a, const big_integer& b);
    friend bool operator > (const big_integer& a, const big_integer& b);
    friend bool operator >= (const big_integer& a, const big_integer& b);
    friend bool operator == (const big_integer& a, const big_integer& b);
    friend bool operator != (const big_integer& a, const big_integer& b);

    //unary operation
    big_integer operator -() const;
    big_integer operator +() const;

    big_integer& operator++();
    big_integer operator++(int);

    big_integer& operator--();
    big_integer operator--(int);

    //bits operations
    big_integer& operator >>=(int b);
    big_integer& operator <<=(int b);
    big_integer operator ~() const;

    friend std::string to_string(const big_integer& a);
};

//binary operations
big_integer operator + (big_integer a, const big_integer& b);
big_integer operator - (big_integer a, const big_integer& b);
big_integer operator * (big_integer a, const big_integer& b);
big_integer operator / (big_integer a, const big_integer& b);
big_integer operator % (big_integer a, const big_integer& b);

big_integer operator | (big_integer a, const big_integer& b);
big_integer operator & (big_integer a, const big_integer& b);
big_integer operator ^ (big_integer a, const big_integer& b);
big_integer operator <<(big_integer a, int b);
big_integer operator >>(big_integer a, int b);

std::string to_string(const big_integer& a);
#endif // BIG_INTEGER_H

#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H
#include <vector>
#include <string>

struct big_integer;

struct big_integer {
private:
    static const int CNT_BIT_OF_BASE = 16;
    static const int BASE = 1<<CNT_BIT_OF_BASE;
    std::vector <unsigned short> digits;
    int sign;

    int cmpr(const big_integer& b) const;
    unsigned short divShort(unsigned short b);
    big_integer mulShort(unsigned short b);
    big_integer divLong(const big_integer& value, big_integer& remainder);
    void additionalCode();
    void inverse(int lenbit);
    big_integer& executeBit(big_integer b, int (*f)(int, int));
    static int myOr(int x, int y);
    static int myAnd(int x, int y);
    static int myXor(int x, int y);
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

#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H
#include <vector>
#include <cstring>
#include <string>
using namespace std;

struct big_integer;

struct big_integer {
public:
    static const int CNT_BIT_OF_BASE = 16;
    static const int BASE = 1<<CNT_BIT_OF_BASE;
    vector <unsigned short> digits;
    int sign;
    int cmpr(const big_integer& b) const;
    unsigned short divShort(unsigned short b);
    big_integer divLong(const big_integer& value, big_integer& remainder);
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
    //destructor
    //~big_integer();//delete[] digits

    //assign
    big_integer& operator = (const big_integer& value);

    //binary operations
    big_integer& operator += (const big_integer& value);
    big_integer& operator -= (const big_integer& value);
    big_integer& operator *= (const big_integer& value);
    big_integer& operator /= (const big_integer& value);
    big_integer& operator %= (const big_integer& value);


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

    //bits operations
    big_integer& operator >>=(int b);
    big_integer& operator <<=(int b);
    friend std::string to_string(const big_integer& a);

    //friend Tester::testCompare();
};
//binary operations
big_integer operator + (big_integer a, const big_integer& b);
big_integer operator - (big_integer a, const big_integer& b);
big_integer operator * (big_integer a, const big_integer& b);
big_integer operator / (big_integer a, const big_integer& b);

std::string to_string(const big_integer& a);
#endif // BIG_INTEGER_H

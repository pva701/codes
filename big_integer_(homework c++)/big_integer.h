#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H
#include <vector>
#include <cstring>
#include <string>
using namespace std;

struct big_integer;
typedef big_integer const T;
typedef unsigned int usi;

struct big_integer {
private:
    static const int CNT_BIT_OF_BASE = 32;
    vector <usi> digits;
    int sign;
    int cmpr(T& b) const;
    int divShort(int b);
/*
  a.back() < 0 - negative
  a.back() > 0 - positive
  a.size() == 1 && a.back() == 0 - zero
*/

public:
    //costructors
    big_integer();
    big_integer(T& value);
    explicit big_integer(const std::string& s);
    big_integer(int x);
    //destructor
    //~big_integer();//delete[] digits

    //assign
    big_integer& operator = (T& value);

    //binary operations
    big_integer& operator += (T& value);
    big_integer& operator -= (T& value);
    big_integer& operator *= (T& value);
    big_integer& operator /= (T& value);

    //unary operation
    big_integer operator -() const;
    big_integer operator +() const;

    friend std::string to_string(const big_integer& a);
};
//compare
bool operator <  (T& a, T& b);
bool operator <= (T& a, T& b);
bool operator >  (T& a, T& b);
bool operator >= (T& a, T& b);
bool operator == (T& a, T& b);

//binary operations
big_integer operator + (T a, T& b);
big_integer operator - (T a, T& b);
big_integer operator * (T a, T& b);
big_integer operator / (T a, T& b);

std::string to_string(const big_integer& a);
#endif // BIG_INTEGER_H

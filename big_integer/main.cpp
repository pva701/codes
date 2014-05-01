#include "big_integer.h"
#include "tester.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {
//    Tester::testStringConstructorAndToString();
//    Tester::testCompare();
    //big_integer c = 5;
    //a = b + c + big_integer(10);
    //(a+b) = c;
    //int x = 10, y = 100, z;
    //a = big_integer(10) = 10;
    //(x+y) = z;
/*    cout << ((1LL<<31)*2147483647) << endl;
    big_integer a = big_integer(1) * 2;
    big_integer c = big_integer(-2147483648) * big_integer(2147483647);
    big_integer t(-2147483648);
    cout << "t = " << to_string(t) << endl;
    cout << "to str = " << to_string(a) << endl;
    cout << "to str = " << to_string(c) << endl;*/
    big_integer s1("1111111111111111111111111111111111");
    big_integer s2(2);
    s1 /= s2;
    cout << to_string(s1) << endl;
    //a = (b + c) + (a + c);
    /*big_integer a("10");
    cout << to_string(a) << endl;
    big_integer b("214748364748");
    cout << to_string(b) << endl;
    big_integer c("21999222222222222222222222222222222222220");
    cout << to_string(c) << endl;
    big_integer d("-202930202939903030");
    cout << to_string(d) << endl;*/
    return 0;
}

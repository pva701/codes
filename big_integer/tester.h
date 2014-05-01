#ifndef TESTER_H
#define TESTER_H

#include <iostream>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "big_integer.h"
using namespace std;

class Tester {
private:
    Tester() {}
    static const int ITERATION = 100;
    int cmpr(string s1, string s2) {

    }
public:
//    friend int big_integer::cmpr(const big_integer& a) const;

    static string randomBigInteger(int len = 10000) {
        string res;
        int l = rand() % len;
        if (rand()%2) res = "-";
        int c = rand() % 10;
        while (c == 0) c = rand() % 10;
        res += char(c + '0');
        for (int i = 1; i < l; ++i) {
            c = rand() % 10 + '0';
            res += c;
        }
        return res;
    }

    static void testStringConstructorAndToString() {
        for (int i = 0; i < ITERATION; ++i) {
            string s = randomBigInteger();
            big_integer a(s);
            if (to_string(a) != s) {
                cout << "wrong " << endl;
                cout << "s = " << s << endl;
                cout << to_string(a) << endl;
                return;
            }
        }
        cout << "finished!";
    }

    static void testCompare() {
        for (int i = 0; i < ITERATION; ++i) {
            string s1 = randomBigInteger(1);
            string s2 = randomBigInteger(1);
            big_integer a(s1);
            big_integer b(s2);
            cout << s1 << " " << s2 << endl;
            if ((a == b) != (s1 == s2)) {
                cout << "suprise mothefucker";
                return;
            }

            if ((a != b) != (s1 != s2)) {
                cout << "surprise mothefucker";
                return;
            }
        }
    }

};

#endif // TESTER_H

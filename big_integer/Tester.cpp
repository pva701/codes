#ifdef __TESTER__
#define __TESTER__

#include <iostream>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "big_integer.h"
using namespace std;

class Tester {
public:
    static string randomString() {
        string res;
        int l = rand() % 10000;
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

    static void test() {
        for (int i = 0; i < 10000; ++i) {
            string s = randomString();
            big_integer a(s);
            if (to_string(a) != s) {
                cout << "wrong " << endl;
                cout << "s = " << s << endl;
                cout << to_string(a) << endl;
                return 0;
            }
        }
    }
}

#undef __TESTER__

#ifndef TESTER_H
#define TESTER_H

#include <iostream>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include "big_integer.h"
#include <algorithm>
using namespace std;

class Tester {
private:
    Tester() {}
    static const int ITERATION = 100;
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

    static string toString(long long x) {
        char ss[30];
        sprintf(ss, "%ld", x);
        return string(ss);
    }

    static void testBitShift() {
        int BOUND = 1<<18;
        for (int i = -BOUND; i <= BOUND; ++i) {
            for (int j = 0; j <= 20; ++j) {
                big_integer resl(i);
                if (toString(i>>j) != to_string(resl)) {
                    cout << toString(i>>j) << " " << to_string(resl) << endl;
                    cout << "error = " << i << " " << j << endl;
                    return;
                }
            }
        }
    }

    static void testOrAndXor() {
        int BOUND = 1<<20;
        for (int t = 0; t < BOUND; ++t) {
            long long i = rand() | (1LL * rand()<<30);
            long long j = rand() | (1LL * rand()<<30);
            if (rand()&1) i = -i;
            if (rand()&1) j = -j;
            big_integer a(toString(i));
            big_integer b(toString(j));
            if (to_string(a | b) != toString(i | j)) {
                cout << to_string(a | b) << " " << toString(i | j) << endl;
                cout << "error or " << i << " " << j << endl;
                return;
            }

            if (to_string(a & b) != toString(i & j)) {
                cout << to_string(a & b) << " " << toString(i & j) << endl;
                cout << "error and " << i << " " << j << endl;
                return;
            }

            if (to_string(a ^ b) != toString(i ^ j)) {
                cout << to_string(a ^ b) << " " << toString(i ^ j) << endl;
                cout << "error xor " << i << " " << j << endl;
                return;
            }
        }
    }

    static int myrand()
    {
        int val = rand() - RAND_MAX / 2;
        if (val != 0)
            return val;
        else
            return 1;
    }

    static void testDiv() {
        unsigned const number_of_iterations = 1;
        size_t const number_of_multipliers = 1000;

        for (unsigned itn = 0; itn != number_of_iterations; ++itn)
        {
            std::vector<int> multipliers;

            for (size_t i = 0; i != number_of_multipliers; ++i)
                multipliers.push_back(myrand());

            big_integer accumulator = 1;

            for (size_t i = 0; i != number_of_multipliers; ++i)
                accumulator *= multipliers[i];

            std::random_shuffle(multipliers.begin(), multipliers.end());

            for (size_t i = 1; i != number_of_multipliers; ++i)
                accumulator /= multipliers[i];

        }
    }

    static void testMul() {
        for (int i = 0; i < ITERATION; ++i) {
            int x = rand();
            int y = rand();
            if (rand()&1) x *= -1;
            if (rand()&1) y *= -1;
            big_integer a(x);
            big_integer b(y);
            if (to_string(a * b) != toString(1LL * x * y)) {
                cout << "surpise mothefucker " << x << " " << y;
                return;
            }
        }
    }
};

#endif // TESTER_H

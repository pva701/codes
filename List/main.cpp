#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "list.h"
using namespace std;
bool eq(vector <int> x, list y) {
    list::iterator j = y.begin();
    for (int i = 0; i < x.size(); ++i, ++j)
        if (x[i] != (*j))
            return false;
    return true;
}

void testCopyAssign() {
    const int N = 100;
    const int IT = 10000;
    list a[N];
    vector <int> ar[N];
    for (int i = 0; i < IT; ++i) {
        int x = rand() % N;
        int y = rand() % N;
        int cmd = rand() % 6;
        if (cmd == 0) {
            a[x] = a[y];
            ar[x] = ar[y];
        } else if (cmd == 1) {
            a[x] = list(a[y]);
            ar[x] = ar[y];
        } else if (cmd == 2) {
            a[x].push_back(y);
            ar[x].push_back(y);
        } else if (cmd == 3 && !ar[x].empty()) {
            a[x].pop_back();
            ar[x].pop_back();
        } else if (cmd == 4) {
            ar[x].insert(ar[x].begin(), y);
            a[x].push_front(y);
        }
        else if (cmd == 5 && !ar[x].empty()) {
            a[x].pop_front();
            ar[x].erase(ar[x].begin());
        }

        assert(eq(ar[x], a[x]));
    }
}

void testSplice() {
    const int IT = 100;
    const int N = 100;
    for (int it = 0; it < IT; ++it) {
        vector <int> a, b;
        list al, bl;
        int n = rand() % N;
        int m = rand() % N;
        for (int i = 0; i < n; ++i) {
            al.push_back(rand());
            a.push_back(al.back());
        }

        for (int j = 0; j < m; ++j) {
            bl.push_back(rand());
            b.push_back(bl.back());
        }

        int pos = rand() % (n + 1);
        list::iterator it = al.begin();
        for (int i = 0; i < pos; ++i)
            ++it;
        al.splice(it, bl, bl.begin(), bl.end());

        for (int i = (int)b.size() - 1; i >= 0; --i)
            a.insert(a.begin() + pos, b[i]);
        assert(eq(a, al));
    }
}

void testRandomInsertErase() {
    const int IT = 100;
    const int N = 100;
    for (int it = 0; it < IT; ++it) {
        vector <int> a;
        list al;
        int n = rand() % N;
        for (int i = 0; i < n; ++i) {
            al.push_back(rand());
            a.push_back(al.back());
        }

        int pos = rand() % (n + 1);
        list::iterator it = al.begin();
        for (int i = 0; i < pos; ++i)
            ++it;
        int valins = rand();
        al.insert(it, valins);

        a.insert(a.begin() + pos, valins);
        assert(eq(a, al));

        it = al.begin();
        for (int i = 0; i < pos; ++i)
            ++it;
        al.erase(it);
        a.erase(a.begin() + pos);
        assert(eq(a, al));
    }
}

int main() {
    testCopyAssign();
    testSplice();
    testRandomInsertErase();
    return 0;
}

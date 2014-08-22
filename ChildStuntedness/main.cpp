#include <iostream>
#include "checker.h"
using namespace std;

vector <int> t;
const int N = 8;
int a[N], dx[N];
vector <int> perm, difbit;
int toInt() {
    int res = 0;
    for (int i = 0; i < N; ++i)
        res = res * 3 + a[i];
    return res;
}

void run(int i, int n) {
    if (i == n) {
        perm.push_back(toInt());
        return;
    }
    run(i + 1, n);
    while (a[i] + dx[i] >= 0 && a[i] + dx[i] < 2) {
        a[i] += dx[i];
        if (dx[i] == 1)
            difbit.push_back(i * 2);
        else
            difbit.push_back(i * 2 + 1);
        run(i + 1, n);
    }
    dx[i] *= -1;
}

int diff(int x, int y) {
    for (int i = 0; i < N; ++i, x /= 3, y /= 3)
        if (x % 3 != y % 3)
            return 7 - i;
}

vector <string> v;
vector <vector <string> > v2;
int getId(const string& s) {
    int i = 0, ret = 0;
    while (s[i] != ',') {
        ret = ret * 10 + s[i] - '0';
        ++i;
    }
    return ret;
}

bool cmp(double x, double y) {
    return x < y;
}

vector <vector <double> > ar;
int main(int argc, char *argv[])
{
    /*for (int i = 0; i < N; ++i)
        dx[i] = 1;
    run(0, N);
    cout << perm.size() << " " << pow(2, N) << endl;
    sort(perm.begin(), perm.end());*/
    //cout << difbit.size() << endl;*/


    //cout << 2800 * 64 * pow(3, 8) << endl;
    freopen("out.txt", "w", stdout);
    //Checker c("exampleData.csv", "samples_traning_example");
    Checker c("train1.csv", "test1.csv");
    cout << c.run() << endl;

//    freopen("exampleData.csv", "r", stdin);
//    string s;
//    while (getline(cin, s))
//        v.push_back(s);
//    for (int i = 0; i < v.size(); ) {
//        int id = getId(v[i]);
//        int j = i;
//        while (i < v.size() && getId(v[i]) == id) ++i;
//        v2.push_back(vector <string>());
//        for (; j < i; ++j)
//            v2.back().push_back(v[j]);
//    }
//    cout << v2.size() << endl;
//    for (int i = 1; i <= 10; ++i) {
//        char s1[20], s2[20];
//        sprintf(s1, "test%d.csv", i);
//        sprintf(s2, "train%d.csv", i);
//        ofstream o1(s1);
//        ofstream o2(s2);
//        for (int j = 0; j < v2.size(); ++j)
//            if (rand() % 3 == 0) {
//                for (int p = 0; p < v2[j].size(); ++p)
//                    o1 << v2[j][p] << endl;
//            } else {
//                for (int p = 0; p < v2[j].size(); ++p)
//                    o2 << v2[j][p] << endl;
//            }
//        o1.close();
//        o2.close();
//    }
    return 0;
}

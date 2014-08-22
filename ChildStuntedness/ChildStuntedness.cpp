#include "ChildStuntedness.h"

namespace MatrixOperators {
    const int INF = 1e9;
    const db EPS = 1e-18;

    TMatrix mul(const TMatrix &a, const TMatrix &b);
    TMatrix pow(TMatrix a, int power);
    TMatrix one(int n);
    TMatrix inverseMatrix(TMatrix a);
    TMatrix trans(const TMatrix &a);

    TMatrix mul(const TMatrix& a, const TMatrix& b) {
        size_t n = a.size();
        size_t m = a[0].size();
        assert(m == b.size());
        size_t k = b[0].size();
        TMatrix c(n, TVector(k));

        if (m > k) {
            TMatrix d = trans(b);
            for (size_t i = 0; i < n; ++i)
                for (size_t j = 0; j < k; ++j) {
                    db tmp = 0;
                    for (size_t p = 0; p < m; ++p)
                        tmp += a[i][p] * d[j][p];
                    c[i][j] = tmp;
                }

        } else  {
            for (size_t i = 0; i < n; ++i)
                for (size_t j = 0; j < k; ++j)
                    for (size_t p = 0; p < m; ++p)
                        c[i][j] += a[i][p] * b[p][j];
        }
        return c;
    }

    TMatrix pow(TMatrix a, int power) {
        if (power < 0) {
            a = inverseMatrix(a);
            power *= -1;
        }

        TMatrix f = one(a.size());
        while (power) {
            if (power&1)
                f = mul(f, a);
            a = mul(a, a);
            power >>= 1;
        }
        return f;
    }

    TMatrix inverseMatrix(TMatrix a) {
        cout << "MATRIX\n";
        for (size_t i = 0; i < a.size(); ++i, cout << endl)
            for (size_t j = 0; j < a[0].size(); ++j)
                cout << a[i][j] << " ";
        cout << endl;

        int n = (int) a.size();
        int m = 2 * n;
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                a[i].push_back(i == j);

        for (int col=0, row=0; col<n && row<n; ++col) {
            int sel = row;
            for (int i = row; i<n; ++i)
                if (fabs (a[i][col]) > fabs (a[sel][col]))
                    sel = i;
            if (fabs (a[sel][col]) < EPS)
                assert(0);

            for (int i=col; i< m; ++i)
                swap(a[sel][i], a[row][i]);

            for (int i=0; i<n; ++i)
                if (i != row) {
                    db c = a[i][col] / a[row][col];
                    for (int j=col; j< m; ++j)
                        a[i][j] -= a[row][j] * c;
                }
            ++row;
        }

        TMatrix b(n, TVector(n));
        for (int i = 0; i < n; ++i)
            for (int j = n; j < 2 * n; ++j)
                b[i][j - n] = a[i][j] / a[i][i];
        return b;
    }

    int solveSystem(TMatrix a, TVector res, TVector & ans) {
        for (int i = 0; i < a.size(); ++i)
            a[i].push_back(res[i]);

        int n = (int) a.size();
        int m = (int) a[0].size() - 1;

        vector<int> where (m, -1);
        for (int col=0, row=0; col<m && row<n; ++col) {
            int sel = row;
            for (int i=row; i<n; ++i)
                if (abs (a[i][col]) > abs (a[sel][col]))
                    sel = i;
            if (abs (a[sel][col]) < EPS)
                continue;
            for (int i=col; i<=m; ++i)
                swap (a[sel][i], a[row][i]);
            where[col] = row;

            for (int i=0; i<n; ++i)
                if (i != row) {
                    db c = a[i][col] / a[row][col];
                    for (int j=col; j<=m; ++j)
                        a[i][j] -= a[row][j] * c;
                }
            ++row;
        }

        ans.assign(m, 0);
        for (int i=0; i<m; ++i)
            if (where[i] != -1)
                ans[i] = a[where[i]][m] / a[where[i]][i];

        for (int i=0; i<n; ++i) {
            double sum = 0;
            for (int j=0; j<m; ++j)
                sum += ans[j] * a[i][j];
            if (abs (sum - a[i][m]) > EPS)
                return 0;
        }

        for (int i=0; i<m; ++i)
            if (where[i] == -1)
                return INF;
        return 1;
    }

    TMatrix one(int n) {
        TMatrix res(n, TVector(n));
        for (int i = 0; i < n; ++i)
            res[i][i] = 1;
        return res;
    }


    TMatrix trans(const TMatrix& a) {
        TMatrix b(a[0].size(), TVector(a.size()));
        for (size_t i = 0; i < b.size(); ++i)
            for (size_t j = 0; j < b[0].size(); ++j)
                b[i][j] = a[j][i];
        return b;
    }

    TVector castToVector(const TMatrix& a) {
        if (a.size() == 1)
            return a[0];
        TVector res(a.size());
        for (size_t i = 0; i < a.size(); ++i)
            if (a[i].size() == 1)
                res[i] = a[i][0];
            else
                assert(0);
        return res;
    }

    TMatrix castToMatrix(const TVector& a, bool Transform = true) {
        TMatrix res;
        if (Transform)
            for (size_t i = 0; i < a.size(); ++i)
                res.push_back(TVector(1, a[i]));
        else
            res.push_back(a);
        return res;
    }
}

namespace Geometry {
    struct Point {
        db x, y;
        Point(db xx, db yy) {
            x = xx;
            y = yy;
        }
        Point() {x = y = 0;}
        bool operator == (const Point& t) const {
            const db EPS = 1e-7;
            return fabs(x - t.x) < EPS && fabs(y - t.y) < EPS;
        }

        bool operator < (const Point& b) const {
            return x < b.x;
        }
    };

    db vect(const Point& a, const Point& b) {
        return a.x * b.y - a.y * b.x;
    }

    Point operator - (const Point& a, const Point& b) {
        return Point(a.x - b.x, a.y - b.y);
    }

}

using namespace MatrixOperators;
using namespace Geometry;

inline db sqr(db x) {
    return x * x;
}

inline db powOdd(db x, int y) {
    return sqr(pow(x, y>>1))*x;
}

namespace Distances {
    inline db integralPhi(db k, db m, db l, db r) {
        db er = exp(r);
        db el = exp(l);
        return k * (r * er - er) + m * er -
                k * (l * el - el) - m * el;

        return k * sqr(sqr(r)) / 4 + m * r * r * r / 3 - k * sqr(sqr(l)) / 4 - m * l * l * l / 3;
    }

    bool up(db k1, db m1, db k2, db m2, db l, db r) {
        db midx = (l + r) / 2;
        return k1 * midx + m1 > k2 * midx + m2;
    }

    db distVec(vector <Point> a, vector <Point> b) {
        const db EPS = 1e-5;
        for (int i = a.size() - 1; i >= 0; --i)
            if (fabs(a[i].y) < EPS)
                a.erase(a.begin() + i);
        for (int i = b.size() - 1; i >= 0; --i)
            if (fabs(b[i].y) < EPS)
                b.erase(b.begin() + i);

        if (a.size() <= 1 || b.size() <= 1)
            return 1;

        assert(a.size() > 1 && b.size() > 1);
        db lef = min(a[0].x, b[0].x);
        db rig = max(a.back().x, b.back().x);

        db k[2], m[2];
        k[0] = (a[1].y - a[0].y) / (a[1].x - a[0].x);
        m[0] = a[0].y - k[0] * a[0].x;

        k[1] = (b[1].y - b[0].y) / (b[1].x - b[0].x);
        m[1] = b[0].y - k[1] * b[0].x;

        db ka = (a[a.size() - 1].y - a[a.size() - 2].y) / (a[a.size() - 1].x - a[a.size() - 2].x);
        db ma = a.back().y - ka * a.back().x;

        db kb = (b[b.size() - 1].y - b[b.size() - 2].y) / (b[b.size() - 1].x - b[b.size() - 2].x);
        db mb = b.back().y - kb * b.back().x;

        a.back() = Point(rig, ka * rig + ma);
        b.back() = Point(rig, kb * rig + mb);
        a[0] = Point(lef, k[0] * lef + m[0]);
        b[0] = Point(lef, k[1] * lef + m[1]);

        db res = 0, cx = lef;
        for (int i = 0, j = 0; i + 1 < a.size() || j + 1 < b.size(); ) {
            db ix;
            if (fabs(k[1] - k[0]) < EPS) {
                if (fabs(m[1] - m[0]) < EPS)
                    ix = max(a[i + 1].x, b[j + 1].x);
                else
                    ix = -1;
            } else
                ix = (m[0] - m[1]) / (k[1] - k[0]);

            if (ix > cx && a[i].x < ix && ix < a[i + 1].x && b[j].x < ix && ix < b[j + 1].x) {
                if (up(k[0], m[0], k[1], m[1], cx, ix))
                    res += integralPhi(k[0] - k[1], m[0] - m[1], cx, ix);
                else
                    res += integralPhi(k[1] - k[0], m[1] - m[0], cx, ix);

                cx = ix;
            } else if (j + 1 == b.size() || i + 1 < a.size() && j + 1 < b.size() && a[i + 1].x <= b[j + 1].x + EPS) {
                if (up(k[0], m[0], k[1], m[1], cx, a[i + 1].x))
                    res += integralPhi(k[0] - k[1], m[0] - m[1], cx, a[i + 1].x);
                else
                    res += integralPhi(k[1] - k[0], m[1] - m[0], cx, a[i + 1].x);
                if (i + 2 < a.size()) {
                    k[0] = (a[i + 2].y - a[i + 1].y) / (a[i + 2].x - a[i + 1].x);
                    m[0] = a[i + 1].y - k[0] * a[i + 1].x;
                }
                cx = a[++i].x;
            } else {
                if (up(k[0], m[0], k[1], m[1], cx, b[j + 1].x))
                    res += integralPhi(k[0] - k[1], m[0] - m[1], cx, b[j + 1].x);
                else
                    res += integralPhi(k[1] - k[0], m[1] - m[0], cx, b[j + 1].x);

                if (j + 2 < b.size()) {
                    k[1] = (b[j + 2].y - b[j + 1].y) / (b[j + 2].x - b[j + 1].x);
                    m[1] = b[j + 1].y - k[1] * b[j + 1].x;
                }
                cx = b[++j].x;
            }
        }
        return res / integralPhi(0, 1, lef, rig);
    }

    TVector vecChild(ChildStuntedness::Child a, ChildStuntedness::Child b) {
        const db EPS = 1e-7;
        TVector res;
        for (int p = 1; p < 7; ++p) {
            vector <Point> va, vb;
            for (int j = 1; j < a.size(); ++j)
                va.push_back(Point(a.tUlts[j], a[j].measurements[p]));
            for (int j = 1; j < b.size(); ++j)
                vb.push_back(Point(b.tUlts[j], b[j].measurements[p]));
            /*for (int i = 0; i < va.size(); ++i)
                cout << va[i].x << " " << va[i].y << endl; cout << endl;
            for (int i = 0; i < vb.size(); ++i)
                cout << vb[i].x << " " << vb[i].y << endl; cout << endl;*/

            db e = distVec(va, vb);
            assert(-EPS <= e && e <= 1 + EPS);
            e = max(e, (db)0.0);
            e = min(e, (db)1.0);
            res.push_back(e);
        }
        return res;
    }

    db distChild(ChildStuntedness::Child a, ChildStuntedness::Child b) {
        TVector va = a.back().parametersFormat();
        TVector vb = b.back().parametersFormat();
        db ret = 0;//sqr(va[0] - vb[0]);//sqr(a.tUlts[a.size() - 2] - b.tUlts[b.size() - 2]);
        //TVector v = vecChild(a, b);
        for (int i = 1; i + 1 < va.size(); ++i)
            ret += sqr(va[i] - vb[i]);
        return sqrt(ret);
    }

    db distChild(ChildStuntedness::Child a, ChildStuntedness::Child b, const vector <db>& coefs) {
        TVector va = a.back().parametersFormat();
        TVector vb = b.back().parametersFormat();
        //db ret = sqr(va[0] - vb[0]);// + sqr(a.tUlts[a.size() - 2] - b.tUlts[b.size() - 2]);
        db ret = 0;
        for (int i = 1; i < va.size(); ++i)
            ret += sqr(va[i] - vb[i]) * coefs[i];
        return sqrt(ret);
    }

    db distChild(ChildStuntedness::Child a, ChildStuntedness::Child b, const vector <int>& coefs) {
        TVector va = a.back().parametersFormat();
        TVector vb = b.back().parametersFormat();
        //db ret = sqr(va[0] - vb[0]);// + sqr(a.tUlts[a.size() - 2] - b.tUlts[b.size() - 2]);
        db ret = 0;
        for (int i = 1; i < va.size(); ++i)
            ret += pow(fabs(va[i] - vb[i]), coefs[i]);
        return sqrt(ret);
    }
}

using namespace Distances;

class Distancer {
    TVector coefs;
public:
    db calc(const TVector& a, const TVector& b) const {
        db res = 0;
        assert(a.size() == b.size());
        assert(a.size() == coefs.size());
        for (size_t i = 0; i < a.size(); ++i)
            res += coefs[i] * sqr(a[i] - b[i]);
        assert(res >= -(1e-8));
        return sqrt(res);
    }

    void learn(const vector <TVector>& x, const TVector& w) {
        size_t M = x.size();
        size_t n = x[0].size();
        assert(M == w.size());
        TMatrix a2(M, TVector(n));
        for (size_t i = 0; i < M; ++i)
            for (size_t j = 0; j < n; ++j)
                a2[i][j] = x[i][j];
        coefs = castToVector(mul(mul(inverseMatrix(mul(trans(a2), a2)), trans(a2)), castToMatrix(w)));
    }

    void stupidLearn(int n) {
        for (size_t i = 0; i < n; ++i)
            coefs.push_back(1);
    }
};

class IFunctor {
public:
    virtual db operator () (const TVector& v) const = 0;
};

class FunctorPowers:public IFunctor {
public:
    vector <int> pwr;
    TVector coefs;
    int n;
    FunctorPowers() {}
    FunctorPowers(int sz, const vector <int>& pww, const TVector& cf) {
        n = sz;
        pwr = pww;
        coefs = cf;
    }

    db operator () (const TVector& v) const {

        assert(v.size() == n);
        db res = coefs[0];
        for (size_t i = 0; i < n; ++i)
            res += sqr(pow(v[i], pwr[i]>>1)) * v[i] * coefs[i + 1];
        return res;
    }

    /*vector <vector <int> > indexes;
    vector <int> pwr;
    TVector coefs;
    int n;
    FunctorPowers() {}
    FunctorPowers(int sz, const vector <vector <int> >& id, const TVector& cf, const vector <int>& pw) {
        n = sz;
        indexes = id;
        coefs = cf;
        pwr = pw;
    }

    db operator () (const TVector& v) {
        assert(v.size() == n);
        assert(v.size() == pwr.size());
        db res = coefs[0];
        for (size_t i = 0; i < indexes.size(); ++i) {
            db prod = 1;
            for (size_t j = 0; j < indexes[i].size(); ++j)
                prod *= pow(v[indexes[i][j]], pwr[indexes[i][j]]);
            res += coefs[i + 1] * prod;
        }
        return res;
    }*/
};

class FunctorLagr:public IFunctor {
    vector <Point> f;
    vector <vector <db> > dif;
    static bool byAbs(db x, db y) {
        return fabs(x) > fabs(y);
    }
public:
    FunctorLagr() {}
    FunctorLagr(vector <Point> p) {
        vector <Point> tmp;
        const db EPS = 1e-7;
        sort(p.begin(), p.end());
        tmp.push_back(p[0]);
        for (int i = 1; i < p.size(); ++i)
            if (fabs(p[i].x - p[i - 1].x) > EPS)
                tmp.push_back(p[i]);
        f = tmp;
        /*cout << "size = " << f.size() << endl;
        for (int i = 0; i < f.size(); ++i) {
            dif.push_back(vector <db>());
            for (int j = 0; j < f.size(); ++j)
                if (i != j)
                    dif[i].push_back(f[i].x - f[j].x);
            sort(dif[i].begin(), dif[i].end(), byAbs);
            cout << "dif = " << dif[i].back() << endl;
        }*/
    }

    db operator ()(const TVector& v) const {
        assert(0);
    }

    db operator ()(db x) const {
        db ret = 0;
        vector <db> neg, pos;
        for (int i = 0; i < f.size(); ++i) {
            db prod = f[i].y;
            for (int p1 = 0, p2 = 0; p1 < f.size() || p2 < f.size();) {
                while ((fabs(prod) > 1.0 || p2 == f.size()) && p1 < f.size())
                    if (p1 == i) ++p1;
                    else prod *= x - f[p1++].x;

                while ((fabs(prod) <= 1.0 || p1 == f.size()) && p2 < f.size())
                    if (p2 == i) ++p2;
                    else prod /= f[i].x - f[p2++].x;
            }
            ret += prod;
            //ret += prod;
        }
        /*for (int i = 0, j = 0; j < neg.size() || i < pos.size(); ) {
            while ((ret <= 0 || j == neg.size()) && i < pos.size())
                ret += pos[i++];
            while ((ret > 0 || i == pos.size()) && j < neg.size())
                ret += neg[j++];
        }*/
        return ret;
    }
};

class ComposeFunctorPowers:public IFunctor {
    vector <db> coefs;
    vector <FunctorPowers> funs;
public:
    ComposeFunctorPowers() {}
    ComposeFunctorPowers(const TVector& cf, const vector <FunctorPowers>& fs) {
        coefs = cf;
        funs = fs;
    }

    void add(db cf, const FunctorPowers& f) {
        coefs.push_back(cf);
        funs.push_back(f);
    }

    db operator() (const TVector& v) const {
        db res = 0;
        for (int i = 0; i < coefs.size(); ++i)
            res += coefs[i] * funs[i](v);
        return res;
    }
};

class RegressionsCollection {
public:
    static FunctorPowers buildLinearRegression(const vector <TVector>& x, const TVector& w, int pow = 3) {
        db mn;
        return buildLinearRegression(x, w, mn, pow);
    }

    static FunctorPowers buildLinearRegression(const vector <TVector>& x, const TVector& w, db& er, int pow = 3) {
        size_t M = x.size();
        assert(M > 0);
        size_t n = x[0].size();
        FunctorPowers ret;
        db mnEr = 1e18;
        vector <int> pw(n);
        TMatrix a(n + 1, TVector(n + 1));
        for (int i = 0; i < n; ++i)
            pw[i] = 1;

        vector <int> diffbit;
        int *ar = new int[n];
        int *dx = new int[n];
        for (int i = 0; i < n; ++i) {
            ar[i] = 0;
            dx[i] = 1;
        }
        sequences(0, n, ar, dx, diffbit, pow);
        TVector f(n + 1);
        TMatrix trX = trans(x);
        for (int t = 0; t < diffbit.size(); ++t) {
            //TVector coefs = castToVector(mul(mul(inverseMatrix(mul(tra, a)), tra), mw));
            f[0] = 0;
            for (size_t j = 0; j < M; ++j)
                f[0] += w[j];
            for (size_t i = 1; i <= n; ++i) {
                f[i] = 0;
                for (size_t j = 0; j < M; ++j)
                    f[i] += w[j] * trX[i - 1][j];//powOdd(x[j][i - 1], pw[i - 1]);
            }


            a[0][0] = M;
            for (size_t i = 1; i <= n; ++i) {
                a[i][0] = a[0][i] = 0;
                for (size_t j = 0; j < M; ++j) {
                    a[i][0] += trX[i - 1][j];
                    a[0][i] += trX[i - 1][j];
                }
            }

            for (size_t i = 1; i <= n; ++i)
                for (size_t k = 1; k <= n; ++k) {
                    a[i][k] = 0;
                    for (size_t j = 0; j < M; ++j)
                        a[i][k] += trX[i - 1][j] * trX[k - 1][j];
                }

            /*TMatrix invA = inverseMatrix(a);
            TVector coefs(n + 1);
            for (size_t i = 0; i <= n; ++i)
                for (size_t j = 0; j <= n; ++j)
                    coefs[i] += invA[i][j] * f[j];*/
            TVector coefs;
            solveSystem(a, f, coefs);
            FunctorPowers fun(n, pw, coefs);
            db crEr = 0;
            for (size_t j = 0; j < M; ++j) {
                db fb = fabs(fun(x[j]) - w[j]);
                crEr += sqr(fb);
            }
            if (crEr < mnEr) {
                mnEr = crEr;
                ret = fun;
            }

            int j = diffbit[t] / 2;
            int dx = (diffbit[t] % 2 == 0 ? 1 : -1);
            pw[j] += 2 * dx;
            for (size_t p = 0; p < M; ++p)
                trX[j][p] = powOdd(x[p][j], pw[j]);

        }
        //cout << "cost of regr = " << mnEr << endl;
        er = sqrt(mnEr / M);
        delete[] ar;
        delete[] dx;
        //minregr = min(minregr, mnEr);
        return ret;

        /*size_t M = x.size();
        size_t n = x[0].size();
        FunctorPowers ret;
        vector <int> pww;
        for (size_t i = 0; i < n; ++i)
            pww.push_back(powers[i]);
        db mnEr = 1e18;
        vector <vector <int> > idx;
        brute(0, idx, x, w, mnEr, ret);

        db error = 0;
        for (size_t i = 0; i < M; ++i)
            error += sqr(ret(x[i]) - w[i]);
        cout << "error of regression = " << error << endl;
        return ret;*/
    }

    static FunctorPowers buildSimpleRegression(const vector <TVector>& x, const vector <db>& w) {
        size_t M = x.size();
        size_t n = x[0].size();
        TVector f(n + 1);
        TMatrix a(n + 1, TVector(n+1));
        assert(w.size() == M);
        //f[i]
        for (size_t j = 0; j < M; ++j)
            f[0] += w[j];
        for (size_t i = 1; i <= n; ++i)
            for (size_t j = 0; j < M; ++j)
                f[i] += w[j] * x[j][i - 1];
        //a[i][j]
        a[0][0] = M;
        for (size_t i = 1; i <= n; ++i)
            for (size_t j = 0; j < M; ++j) {
                a[i][0] += x[j][i - 1];
                a[0][i] += x[j][i - 1];
            }
        for (size_t i = 1; i <= n; ++i)
            for (size_t k = 1; k <= n; ++k)
                for (size_t j = 0; j < M; ++j)
                    a[i][k] += x[j][i - 1] * x[j][k - 1];
        TVector coefs;
        solveSystem(a, f, coefs);
        vector <int> pwr;
        for (int i = 0; i < n; ++i)
            pwr.push_back(1);
        return FunctorPowers(n, pwr, coefs);
    }

    static FunctorPowers buildSheetRegression(const vector <TVector>& x, const TVector& w) {
        size_t M = x.size();
        size_t n = x[0].size();
        FunctorPowers ret;
        db mnEr = 1e18;
        vector <int> pw(n);
        TMatrix a(n + 1, TVector(n + 1));
        for (int i = 0; i < n; ++i)
            pw[i] = 1;

        vector <int> diffbit;
        int *ar = new int[n];
        int *dx = new int[n];
        for (int i = 0; i < n; ++i) {
            ar[i] = 0;
            dx[i] = 1;
        }
        sequences(0, n, ar, dx, diffbit, 2);
        TVector f(n + 1);
        TMatrix trX = trans(x);

        for (int t = 0; t < diffbit.size(); ++t) {
            f[0] = 0;
            for (size_t j = 0; j < M; ++j)
                f[0] += w[j];
            for (size_t i = 1; i <= n; ++i) {
                f[i] = 0;
                for (size_t j = 0; j < M; ++j)
                    f[i] += w[j] * trX[i - 1][j];//powOdd(x[j][i - 1], pw[i - 1]);
            }


            a[0][0] = M;
            for (size_t i = 1; i <= n; ++i) {
                a[i][0] = a[0][i] = 0;
                for (size_t j = 0; j < M; ++j) {
                    a[i][0] += trX[i - 1][j];
                    a[0][i] += trX[i - 1][j];
                }
            }

            for (size_t i = 1; i <= n; ++i)
                for (size_t k = 1; k <= n; ++k) {
                    a[i][k] = 0;
                    for (size_t j = 0; j < M; ++j)
                        a[i][k] += trX[i - 1][j] * trX[k - 1][j];
                }
            TVector coefs;
            solveSystem(a, f, coefs);
            FunctorPowers fun(n, pw, coefs);
            db crEr = 0;
            for (size_t j = 0; j < M; ++j)
                crEr += sqr(fun(x[j]) - w[j]);
            if (crEr < mnEr) {
                mnEr = crEr;
                ret = fun;
            }

            int j = diffbit[t] / 2;
            int dx = (diffbit[t] % 2 == 0 ? 1 : -1);
            pw[j] += 2 * dx;
            for (size_t p = 0; p < M; ++p)
                trX[j][p] = powOdd(x[p][j], pw[j]);

        }
        cout << "cost of regr = " << mnEr << endl;
        //er = sqrt(mnEr / M);
        delete[] ar;
        delete[] dx;
        return ret;
    }

    static ComposeFunctorPowers buildComposeRegressions(const vector <TVector>& x, const TVector& w) {
        vector <FunctorPowers> funs;
        vector <db> bm;
        //funs.push_back(buildLinearRegression(x, w));
        //bm.push_back(1);
        int n = x[0].size();
        int M = x.size();
        TVector qm(M), fm(M), hm(M);
        TVector f(n + 1);
        TMatrix trX = trans(x);

        db av = 0;
        for (int i = 0; i < w.size(); ++i)
            av += w[i];
        av /= w.size();

        vector <db> cf;
        vector <int> pp;
        cf.push_back(av);
        for (int i = 0; i < n; ++i) {
            cf.push_back(0);
            pp.push_back(1);
        }
        bm.push_back(1);
        funs.push_back(RegressionsCollection::buildLinearRegression(x, w));
        //funs.push_back(FunctorPowers(n, pp, cf));
        for (int i = 0; i < M; ++i) fm[i] = funs[0](x[i]);

        vector <int> pw(n);
        TMatrix a(n + 1, TVector(n + 1));
        for (int i = 0; i < n; ++i)
            pw[i] = 1;

        int pow = 3;
        vector <int> diffbit;
        int *ar = new int[n];
        int *dx = new int[n];
        for (int i = 0; i < n; ++i) {
            ar[i] = 0;
            dx[i] = 1;
        }
        sequences(0, n, ar, dx, diffbit, pow);

        const db V = 1;
        for (int t = 0; t < diffbit.size(); ++t)  {
            //MAKE Am

            for (int i = 0;  i < M; ++i)
                qm[i] = 2 * (w[i] - fm[i]);

            f[0] = 0;
            for (size_t j = 0; j < M; ++j)
                f[0] += qm[j];
            for (size_t i = 1; i <= n; ++i) {
                f[i] = 0;
                for (size_t j = 0; j < M; ++j)
                    f[i] += qm[j] * trX[i - 1][j];//powOdd(x[j][i - 1], pw[i - 1]);
            }

            a[0][0] = M;
            for (size_t i = 1; i <= n; ++i) {
                a[i][0] = a[0][i] = 0;
                for (size_t j = 0; j < M; ++j) {
                    a[i][0] += trX[i - 1][j];
                    a[0][i] += trX[i - 1][j];
                }
            }

            for (size_t i = 1; i <= n; ++i)
                for (size_t k = 1; k <= n; ++k) {
                    a[i][k] = 0;
                    for (size_t j = 0; j < M; ++j)
                        a[i][k] += trX[i - 1][j] * trX[k - 1][j];
                }

            TVector coefs;
            solveSystem(a, f, coefs);
            FunctorPowers funm(n, pw, coefs);

            int j = diffbit[t] / 2;
            int dx = (diffbit[t] % 2 == 0 ? 1 : -1);
            pw[j] += 2 * dx;
            for (size_t p = 0; p < M; ++p)
                trX[j][p] = powOdd(x[p][j], pw[j]);


            db b = 0, s1 = 0, s2 = 0;
            for (int i = 0; i < M; ++i)
                hm[i] = funm(x[i]);

            for (int i = 0; i < M; ++i) {
                if (rand() % 2)
                    continue;
                s1 += hm[i] * (w[i] - fm[i]);
                s2 += hm[i] * hm[i];
            }

            b = s1 / s2;
            b = max(b, 0.0);

            for (int i = 0; i < M; ++i)
                fm[i] += V * b * hm[i];

            bm.push_back(b * V);
            funs.push_back(funm);
        }
        delete[] ar;
        delete[] dx;
        //for (int i = 0; i < bm.size(); ++i) cout << bm[i] << " ";cout << endl;

        ComposeFunctorPowers ret(bm, funs);
        db error = 0;
        for (int i = 0; i < M; ++i)
            error += sqr(ret(x[i]) - w[i]);
        cerr << "error = " << error << endl;
        return ret;
    }

private:
    static void sequences(int i, int n, int* a, int* dx, vector <int>& dbit, int pow = 3) {
        if (i == n)
            return;
        sequences(i + 1, n, a, dx, dbit, pow);
        while (a[i] + dx[i] >= 0 && a[i] + dx[i] < pow) {
            a[i] += dx[i];
            if (dx[i] == 1)
                dbit.push_back(i * 2);
            else
                dbit.push_back(i * 2 + 1);

            sequences(i + 1, n, a, dx, dbit, pow);
        }
        dx[i] *= -1;
    }
};

class RegressionPredictor {
    FunctorPowers fun;
    db er;
public:
    void learn(const TMatrix& x, const TVector& w) {
        fun = RegressionsCollection::buildLinearRegression(x, w, er);
    }
    db operator() (const TVector& v) {
        return fun(v);
    }
    db error() {
        return er;
    }
};

class DistancerPredictor {
private:
    vector <vector <db> > d, dstsKth;
    TVector gamma;
    int kthFriend;
    vector <ChildStuntedness::Child> v;
    db EPS;
    vector <int> cf;

    db Ker(db ds, db h) {
        ds /= h;
        if (fabs(ds) > 1.0)
            return 0;
        return 0.75 * (1 - sqr(ds));
    }

    db approxValue(int id, int k) {
        db chAi = 0, znAi = 0;
        if (k >= dstsKth[id].size())
            return 0;
        db kth = dstsKth[id][k];
        if (fabs(kth) < EPS)
            return 0;
        for (size_t j = 0; j < v.size(); ++j) {
            if (id == j)
                continue;
            db ker = Ker(d[id][j], kth);
            chAi += gamma[j] * v[j].x * ker;
            znAi += gamma[j] * ker;
        }

        if (fabs(znAi) < EPS)
            return 0;
        return chAi / znAi;
    }

    void lowess() {
        int n = v.size();
        const int ITS = 300;
        for (int it = 0; it < ITS; it++) {
            TVector eps(n);
            for(size_t i = 0; i < n; ++i)
                eps[i] = fabs(approxValue(i, kthFriend) - v[i].b);
            nth_element(eps.begin(), eps.begin() + (eps.size() - 1) / 2, eps.end());
            db tmp = eps[(eps.size() - 1) / 2];
            for (size_t i = 0; i < gamma.size(); ++i) {
                db cr = eps[i] / (6 * tmp);
                if (fabs(cr) > 1) gamma[i] = 0;
                else gamma[i] = 15.0 / 16 * sqr(1 - sqr(cr));
            }
            cerr << "LOWESS " << it << endl;
        }
        cout << "gamma = " << endl;
        for (size_t i = 0; i < gamma.size(); ++i)
            cout << gamma[i] << " ";cout << endl;
    }
public:
    DistancerPredictor(): EPS(1e-4) {}

    void learn(const vector <ChildStuntedness::Child>& trChild, const vector <int>& coef) {
        int n = trChild.size();
        v = trChild;
        cf = coef;
        kthFriend = 0;
        const int MAX_K = n - 1;
        for (int i = 0; i < n; ++i) {
            d.push_back(TVector());
            dstsKth.push_back(TVector());
            for (int j = 0; j < n; ++j) {
                db r = distChild(trChild[i], trChild[j], cf);
                d[i].push_back(r);
                if (i != j)
                    dstsKth[i].push_back(r);
            }
            sort(dstsKth[i].begin(), dstsKth[i].end());
            int j = 0;
            while (j < dstsKth[i].size() && fabs(dstsKth[i][j]) < 0.001) ++j;
            dstsKth[i].erase(dstsKth[i].begin(), dstsKth[i].begin() + j);
            while (dstsKth[i].size() > MAX_K)
                dstsKth[i].pop_back();
        }

        db mnEr = 1e18;
        gamma = TVector(n);
        for (size_t i = 0; i < n; ++i)
            gamma[i] = 1;
        //for (db r = 0; r < rig; r += 0.01) {
        for (size_t k = 0; k < MAX_K; ++k) {
            db error = 0;
            for (size_t i = 0; i < n; ++i) {
                db value = approxValue(i, k);
                error += sqr(value - trChild[i].x);
                if (error > mnEr)
                    break;
            }
            if (error < mnEr) {
                mnEr = error;
                kthFriend = k;
            }
        }
        //cout << "k = " << kthFriend << endl;
        //lowess();
    }

    db operator() (const ChildStuntedness::Child& coord) {//, const vector <ChildStuntedness::Child>& myv) {
        db znAi = 0, chAi = 0;
        vector <db> dstsKth(v.size()), dsts(v.size());
        for (size_t j = 0; j < v.size(); ++j) {
            db r = distChild(coord, v[j], cf);
            dstsKth[j] = r;
            dsts[j] = r;
        }

        nth_element(dstsKth.begin(), dstsKth.begin() + kthFriend, dstsKth.end());
        db kth = dstsKth[kthFriend];
        if (fabs(kth) < EPS)
            return 0.0;


        for (size_t j = 0; j < v.size(); ++j) {
            db ker = Ker(dsts[j], kth);
            chAi += gamma[j] * v[j].x * ker;
            znAi += gamma[j] * ker;
        }
        if (fabs(znAi) < EPS)
            return 0;
        return chAi / znAi;
    }
};

class ClassicicationTreePredictor {
    class Predicate {
        int id;
        db val;
    public:
        Predicate() {}
        Predicate(int i, db v) {
            id = i;
            val = v;
        }

        bool operator ()(const TVector& v) {
            return v[id] < val;
        }
    };

    class Node {
    public:
        db er;
        Node *l, *r;
        int h, leafs;
        Predicate predicate;
        Node(const Predicate& pred, Node* lef, Node* rig, int hh) {
            predicate = pred;
            h = hh;
            l = lef;
            r = rig;
            leafs = l->leafs + r->leafs;
            er = l->er + r->er;
        }

        Node(const Node& t) {
            er = t.er;
            w = t.w;
            x = t.x;
            wtest = t.wtest;
            xtest = t.xtest;
            leafs = t.leafs;
            h = t.h;
            predicate = t.predicate;
            if (t.l != NULL)
                l = new Node(*t.l);
            else
                l = NULL;

            if (t.r != NULL)
                r = new Node(*t.r);
            else
                r = NULL;
        }

       Node(const TMatrix& xx, const TVector& ww, int hh) {
            h = hh;
            x = xx;
            w = ww;
            leafs = 1;
            l = r = NULL;
            er = 0;
        }

        TMatrix x, xtest;
        TVector w, wtest;

        void put(const TMatrix& x, const TVector& w) {
            if (l == NULL && r == NULL) {
                er = massCalculateError(x, w, this->w, this->x);
                xtest = x;
                wtest = w;
                return;
            }

            TMatrix xl, xr;
            TVector wl, wr;
            for (int i = 0; i < x.size(); ++i)
                if (predicate(x[i])) {
                    xl.push_back(x[i]);
                    wl.push_back(w[i]);
                } else {
                    xr.push_back(x[i]);
                    wr.push_back(w[i]);
                }
            l->put(xl, wl);
            r->put(xr, wr);
            er = l->er + r->er;
        }

        db calcInVertex(const TVector& v) {
            db sm = 0;
            for (int i = 0; i < w.size(); ++i)
                sm += w[i];
            return sm / w.size();
        }

        Node* leaf(const TVector& v) {
            Node *vert = this;
            while (vert->l != NULL || vert->r != NULL)
                if (vert->predicate(v))
                    vert = vert->l;
                else
                    vert = vert->r;
            return vert;
        }

        db operator() (const TVector& v) {
            return leaf(v)->calcInVertex(v);
        }

        db massCalculateError(const TMatrix& x, const TVector& wt, const TVector& winv, const TMatrix& xinv) {
            /*FunctorPowers fun(RegressionsCollection::buildLinearRegression(xinv, winv));
            db ret = 0;
            for (int i = 0; i < wt.size(); ++i)
                ret += sqr(fun(x[i]) - wt[i]);
            return ret;*/

            db av = 0;
            for (int i = 0; i < winv.size(); ++i)
                av += winv[i];
            av /= winv.size();
            db ret = 0;
            for (int j = 0; j < wt.size(); ++j)
                ret += sqr(wt[j] - av);
            return ret;//sqrt(ret / wt.size());*/
        }

        pair <db, Node*> dfs(db crerror, int crleafs, db Ter, int Tleafs, TMatrix& trx, TVector& trw,
                             TMatrix& tex, TVector tew, int szTest) {
            if (l == NULL && r == NULL) {
                trx = x;
                trw = w;
                tex = xtest;
                tew = wtest;
                return make_pair(1e8, this);
            }
            TVector trLw, trRw, teLw, teRw;
            TMatrix trLx, trRx, teLx, teRx;
            pair <db, Node*> lefinfo = l->dfs(crerror + r->er, crleafs + r->leafs, Ter, Tleafs, trLx, trLw, teLx, teLw, szTest);
            pair <db, Node*> riginfo = r->dfs(crerror + l->er, crleafs + l->leafs, Ter, Tleafs, trRx, trRw, teRx, teRw, szTest);
            trw = mergeVectors(trLw, trRw);
            tew = mergeVectors(teLw, teRw);
            tex = mergeMatrix(teLx, teRx);
            trx = mergeMatrix(trLx, trRx);
            crerror += massCalculateError(tex, tew, trw, trx);
            //cerr << "Tlef = " << Tleafs << " " << crleafs << endl;
            db curverror = (sqrt(crerror / szTest) - sqrt(Ter / szTest) ) / (Tleafs - (crleafs + 1));
            //cerr << "curverror " << curverror << " " << lefinfo.first << " " << riginfo.first << endl;
            if (curverror <= lefinfo.first && curverror <= riginfo.first)
                return make_pair(curverror, this);
            if (lefinfo.first < riginfo.first)
                return lefinfo;
            return riginfo;
        }

        TVector mergeVectors(const TVector& lv, const TVector& rv) {
            TVector v;
            for (int i = 0; i < lv.size(); ++i)
                v.push_back(lv[i]);
            for (int i = 0; i < rv.size(); ++i)
                v.push_back(rv[i]);
            return v;
        }

        TMatrix mergeMatrix(const TMatrix& lm, const TMatrix& rm) {
            TMatrix res;
            for (int i = 0; i < lm.size(); ++i)
                res.push_back(lm[i]);
            for (int i = 0; i < rm.size(); ++i)
                res.push_back(rm[i]);
            return res;
        }

        void merge(Node *mergeby, bool wasv) {
            if (l == NULL && r == NULL)
                return;
            wasv |= this == mergeby;
            l->merge(mergeby, wasv);
            r->merge(mergeby, wasv);
            if (!wasv) {
                leafs = l->leafs + r->leafs;
                er = l->er + r->er;
            } else {
                w = mergeVectors(l->w, r->w);
                wtest = mergeVectors(l->wtest, r->wtest);
                x = mergeMatrix(l->x, r->x);
                xtest = mergeMatrix(l->xtest, r->xtest);
                er = massCalculateError(xtest, wtest, w, x);
                leafs = 1;
                delete l;
                delete r;
                l = NULL;
                r = NULL;
            }
        }

        ~Node() {
            delete l;
            delete r;
        }
    };
    //END NODE

    vector <int> candidatesPredicateColumn(int n) {
        vector <int> res, perm;
        for (int i = 0; i < n; ++i)
            perm.push_back(i);
        next_permutation(perm.begin(), perm.end());
        int m = int(sqrt(n)) + 1;
        for (int i = 0; i < m; ++i)
            res.push_back(perm[i]);
        return res;
    }

    db costOfSet(const TMatrix& x, const TVector& w) {
        /*FunctorPowers fun(RegressionsCollection::buildSimpleRegression(x, w));
        for (int i = 0; i < w.size(); ++i)
            ret += sqr(fun(x[i]) - w[i]);*/
        db ret = 0;
        db av = 0;
        for (int i = 0; i < w.size(); ++i)
            av += w[i];
        av /= w.size();
        for (int i = 0; i < w.size(); ++i)
            ret += sqr(av - w[i]);

        return sqrt(ret / w.size());
    }

    Predicate splitter(const TMatrix& x, const TVector& w, TMatrix& xleft, TVector& wleft, TMatrix& xright, TVector& wright) {
        int n = x[0].size();
        int M = x.size();
        vector <int> predictCols = candidatesPredicateColumn(n);
        vector <pair <db, int> > vls(M);
        int resCol = -1;
        db resValue;
        db croot = costOfSet(x, w);//costOfSet(w, 0, w.size());
        db mn = 1e9;
        for (int i = 0; i < predictCols.size(); ++i) {
            int col = predictCols[i];
            for (int j = 0; j < M; ++j)
                vls[j] = make_pair(x[j][col], j);
            sort(vls.begin(), vls.end());
            TMatrix xl, xr;
            TVector wl, wr;
            for (int j = vls.size() - 1; j >= 1; --j) {
                xr.push_back(x[vls[j].second]);
                wr.push_back(w[vls[j].second]);
            }
            xl.push_back(x[vls[0].second]);
            wl.push_back(w[vls[0].second]);

            for (int splitId = 1; splitId < vls.size(); splitId++) {
                for (; splitId < vls.size() && fabs(vls[splitId].first - vls[splitId - 1].first) < 0.001; ++splitId) {
                    wl.push_back(wr.back());
                    xl.push_back(xr.back());
                    xr.pop_back();
                    wr.pop_back();
                }

                if (splitId == vls.size())
                    break;

                db clef = costOfSet(xl, wl);//costOfSet(vls, 0, splitId);
                db crig = costOfSet(xr, wr); //costOfSet(vls, splitId, vls.size());
                db crcst = 1.0 * splitId / M * clef + 1.0 * (M - splitId) / M * crig;
                //db crcst = clef + crig;
                if (crcst < mn) {
                    mn = crcst;
                    resValue = vls[splitId].first;
                    resCol = col;
                }
            }

        }
        if (mn > croot) {
            //cout << "resid = " << resCol << endl;
            //cout << "X mn > croot " << x.size() << endl; //assert(mn > croot);
            return Predicate();
        }

        for (int i = 0; i < M; ++i)
            if (x[i][resCol] >= resValue) {
                xright.push_back(x[i]);
                wright.push_back(w[i]);
            } else {
                xleft.push_back(x[i]);
                wleft.push_back(w[i]);
            }
        return Predicate(resCol, resValue);
    }

    Node* buildTree(const TMatrix& x, const TVector& w, int h) {
        if (x.size() == 1)
            return new Node(x, w, h);
        TMatrix xleft, xright;
        TVector wleft, wright;
        Predicate pr = splitter(x, w, xleft, wleft, xright, wright);
        if (xleft.size() == 0 || xright.size() == 0)
            return new Node(x, w, h);
        Node *lef = buildTree(xleft, wleft, h + 1);
        Node *rig = buildTree(xright, wright, h + 1);
        return new Node(pr, lef, rig, h);
    }

    Node *root;
    bool learnd;
    int size;
    db err;
public:
    void learn(const TMatrix& x, const TVector& w) {
        root = buildTree(x, w, 0);
        learnd = true;
        size = x[0].size();
        for (int i = 0; i < x.size(); ++i)
            err += sqr((*root)(x[i]) - w[i]);
        err = sqrt(err / x.size());
    }

    void pruning(const TMatrix& x, const TVector& w) {
        vector <Node*> T;
        T.push_back(root);
        root->put(x, w);
        int it = 0;
        cerr << root->leafs << endl;
        while (T.back()->w.size() == 0 && it < 500) {
            Node *cp = new Node(*T.back());
            TVector tr, te;
            TMatrix xtr, xte;
            pair <db, Node*> mergeby = cp->dfs(0, 0, cp->er, cp->leafs, xtr, tr, xte, te, x.size());
            //cerr << "cs = " << mergeby.first << " " << mergeby.second->l << endl;
            cp->merge(mergeby.second, false);
            T.push_back(cp);
            ++it;
        }
        for (int i = 1; i < T.size(); ++i)
            if (T[i]->er < root->er)
                root = T[i];
        for (int i = 0; i < T.size(); ++i)
            if (T[i] != root)
                delete T[i];
    }

    db operator() (const TVector& v) {
        assert(learnd);
        assert(size == v.size());
        return (*root)(v);
    }

    db error() {
        return err;
    }

    db error(const TMatrix& x, const TVector& w) {
        db ret = 0;
        for (int i = 0; i < x.size(); ++i)
            ret += sqr(operator ()(x[i]) - w[i]);
        return sqrt(ret / x.size());
    }
};


class RandomForest {
    //vector <vector <int> > columns;
    //vector <T*> instances;
    const int TREES;
    int n;
    bool learnd;
    vector <ClassicicationTreePredictor* > trees;
    vector <vector <int> > columns;
    static bool bysec(pair <db, int> x, pair <db, int> y) {
        return x.second < y.second;
    }
    vector <pair <db, int> > tr;
public:
    RandomForest(int trees):TREES(trees), learnd(false) {}
    void learn(const TMatrix& x, const TVector& w) {
        int M = x.size();
        n = x[0].size();
        bool use[M];
        for (int i = 0; i < 2 * TREES; ++i) {
            trees.push_back(new ClassicicationTreePredictor());
            TMatrix crx;
            TVector crw;
            for (int j = 0; j < M; ++j)
                use[j] = 0;
            vector <int> perm;
            for (int j = 0; j < n; ++j)
                perm.push_back(j);
            random_shuffle(perm.begin(), perm.end());
            int cn = rand() % (n - 1) + 2;
            while (perm.size() > cn) perm.pop_back();

            for (int j = 0; j < M; ++j) {
                int id = rand() % M;
                crx.push_back(elect(x[id], perm));
                crw.push_back(w[id]);
                use[id] = true;
            }
            TMatrix lrx;
            TVector lrw;
            for (int j = 0; j < M; ++j)
                if (!use[j]) {
                    lrx.push_back(elect(x[j], perm));
                    lrw.push_back(w[j]);
                }
            trees[i]->learn(crx, crw);
            //trees[i]->pruning(lrx, lrw);
            columns.push_back(perm);
            tr.push_back(make_pair(trees[i]->error(lrx, lrw), i));
            //trees[i].first = tr[i].second->error(lrx, lrw);
            //cerr << "built " << i << endl;
        }
        sort(tr.begin(), tr.end());
        learnd = true;
    }

    db operator() (const TVector& v) {
        assert(v.size() == n);
        assert(learnd);
        db res = 0;
        for (int i = 0; i < TREES; ++i) {
            ClassicicationTreePredictor *crtr = trees[tr[i].second];
            res += (*crtr)(elect(v, tr[i].second));
        }
        return res / TREES;
    }
private:
    TVector elect(const TVector& v, int i) {
        TVector vib;
        for (int p = 0; p < columns[i].size(); ++p)
            vib.push_back(v[columns[i][p]]);
        return vib;
    }

    TVector elect(const TVector& v, const vector <int>& perm) {
        TVector vib;
        for (int p = 0; p < perm.size(); ++p)
            vib.push_back(v[perm[p]]);
        return vib;
    }
};

typedef db (*FunDistance)(ChildStuntedness::Child, ChildStuntedness::Child);



db fuckDist(vector <ChildStuntedness::Child>& trChild, ChildStuntedness::Child& ch, const FunctorPowers& prevFunB, db r, db df, int id) {
    db mnUp = 1e8, mnDown = 1e8, bUp = 0, bDown = 0;
    TVector pr = ch.back().parametersFormat();
    db prd = prevFunB(pr);
    //db df = ch.b - prd;
    //if (id == -1)
        //df = (rand() % 2 == 0 ? -1 : 1);
    int cntUp = 0, cntDown = 0;
    vector <pair <db, db> > cand;
    for (int j = 0; j < trChild.size(); ++j) {
        if (id == j)
            continue;
        TVector pr2 = trChild[j].back().parametersFormat();
        db d = 100 *  sqr(trChild[j].b - prd);
        for (int j = 0; j + 1 < pr.size(); ++j)
            d += prevFunB.pwr[j] * sqr(pr[j] - pr2[j]);
        d = sqrt(d);
        //TVector v = vecChild(trChild[j]
        db f = trChild[j].b - prevFunB(pr2);
        if (f >= 0 && d <= r)
            cntUp++;
        else if (f < 0 && d <= r)
            cntDown++;

        if (d <= r)  {
            if (f >= 0)
                cand.push_back(make_pair(d, trChild[j].b));
            else
                cand.push_back(make_pair(-d, trChild[j].b));
        }

        if (f >= 0 && d < mnUp) {
            mnUp = d;
            bUp = trChild[j].b;
        } else if (f < 0 && d < mnDown) {
            mnDown = d;
            bDown = trChild[j].b;
        }
    }

    if (cntUp > cntDown)
        df = 1;
    else
        df = -1;

    /*if (df >= 0 && mnUp <= r)
        return bUp;
    else if (df < 0 && mnDown <= r)
        return bDown;*/

    //if (mnUp <= r || mnDown <= r)
        //return bUp * cntUp / (cntUp + cntDown) + bDown * cntDown / (cntUp + cntDown);
    //upB * mnDown / (mnDown + mnUp) + downB * mnUp / (mnDown + mnUp);

    if (mnUp <= mnDown && mnUp <= r)
        return bUp;
    if (mnDown <= mnUp && mnDown <= r)
        return bDown;
    return prd;
}

bool cmpx(const Point& a, const Point& b) {
    return a.x < b.x;
}

TVector parameters(const ChildStuntedness::Child& ch, int sz) {
    TVector v;
    for (int j = 0; j < sz; ++j)
        if (j >= ch.size())
            v.push_back(0);
        else
            v.push_back(ch[ch.size() - j - 1].tUlt);//ch.tUlts[ch.size() - j - 1]);
    v.push_back(v.back());
    return v;
}

bool byT(ChildStuntedness::Child a, ChildStuntedness::Child b) {
    return a.tUlts.back() < b.tUlts.back() || fabs(a.tUlts.back() - b.tUlts.back()) < 0.0001 && a.b < b.b;
}


class Comparator {

public:
    vector <vector <int> > idxs;
    Comparator() {}
    Comparator(const vector <vector <int> >& id) {
        idxs = id;
    }

    db calc(const ChildStuntedness::Child& a) {
        db res = 0;
        TVector pr = a.back().parametersFormat();
        for (int i = 0; i < idxs.size(); ++i) {
            db prod = 1;
            for (int j = 0; j < idxs[i].size(); ++j)
                prod *= pr[idxs[i][j]];
            res += prod;
        }
        return res;
    }

    bool operator () (const ChildStuntedness::Child& a, const ChildStuntedness::Child& b) {
        return calc(a) < calc(b);
    }
};

bool byB(ChildStuntedness::Child a, ChildStuntedness::Child b) {
    return a.b > b.b;
}

void brute(int p, int n, vector <vector <int> >& idx, vector <int>& ress, vector <Comparator>& cmprs,
                             vector <vector <ChildStuntedness::Child> >& groups) {
    if (p == n) {
        if (idx.size() == 0)
            return;
        Comparator comp(idx);
        for (int i = 0; i < groups.size(); ++i) {
            int res = 0;
            sort(groups[i].begin(), groups[i].end(), byB);
            sort(groups[i].begin(), groups[i].end(), comp);
            for (int j = 0; j + 1 < groups[i].size(); ++j)
                res += groups[i][j].b > groups[i][j + 1].b;
            if (res < ress[i]) {
                ress[i]  = res;
                cmprs[i] = comp;
            }
        }
    } else {
        idx.push_back(vector<int>(1, p));
        brute(p + 1, n, idx, ress, cmprs, groups);
        idx.pop_back();

        for (size_t i = 0; i < idx.size(); ++i) {
            idx[i].push_back(p);
            brute(p + 1, n, idx, ress, cmprs, groups);
            idx[i].pop_back();
        }

        //brute(p + 1, n, idx, mnEr, rescmp, groups);
    }
}

vector <double> ChildStuntedness::predict(vector <string> train, vector <string> testing) {
    const int SZB = 2;

    vector <Ultrasound> trUlt, teUlt;
    vector <Child> trChild, teChild;

    for (size_t i = 0; i < train.size(); ++i)
        trUlt.push_back(Ultrasound(train[i]));

    for (size_t i = 0; i < testing.size(); ++i) {
        teUlt.push_back(Ultrasound(testing[i]));
        //teUlt[i].w = teUlt[i].b = 0;
    }


    vector <TVector> xw, xbt, xb;
    vector <db> w, bt, b;
    vector <pair <db, db> > pt;

    for (size_t i = 0; i < teUlt.size(); ) {
        size_t j = i;
        teChild.push_back(Child());
        for (; i < teUlt.size() && teUlt[i].id == teUlt[j].id; ++i)
            teChild.back().add(teUlt[i]);
    }


    TMatrix xba[2];
    TVector wba[2], t1;
    for (size_t i = 0; i < trUlt.size(); ) {
        size_t j = i;
        trChild.push_back(Child());
        for (; i < trUlt.size() && trUlt[i].id == trUlt[j].id; ++i)
            trChild.back().add(trUlt[i]);
        TVector pr = trChild.back().back().parametersFormat();

        /*if (trChild.back().size() >= SZB){
            TVector v = parameters(trChild.back(), SZB);
            xbt.push_back(v);
            bt.push_back(trChild.back().b);
        }*/

        TVector v = parameters(trChild.back(), SZB);
        t1.push_back(v[0]);

        xbt.push_back(v);
        bt.push_back(trChild.back().b);

        xb.push_back(pr);
        b.push_back(trChild.back().b);

        xw.push_back(pr);
        w.push_back(trChild.back().w);
    }
    cout << trChild.size() << " " << teChild.size() << endl;
    sort(t1.begin(), t1.end());
    db t1mid = t1[t1.size() / 2];
    for (int i = 0; i < trChild.size(); ++i) {
        TVector v = parameters(trChild[i], SZB);
        bool c1 = v[0] > t1mid;
        xba[c1].push_back(v);
        wba[c1].push_back(trChild[i].b);
    }

   /* FunctorPowers prevFunBt[2];
    for (int i = 0; i < 2; ++i)
            prevFunBt[i]= RegressionsCollection::buildLinearRegression(xba[i], wba[i]);*/

    FunctorPowers prevFunB(RegressionsCollection::buildLinearRegression(xb, b));
    FunctorPowers prevFunBt(RegressionsCollection::buildLinearRegression(xbt, bt));
    FunctorPowers prevFunW(RegressionsCollection::buildLinearRegression(xw, w));
    //ComposeFunctorPowers compb(RegressionsCollection::buildComposeRegressions(xbt, bt));

    //for (int i = 0; i < prevFunBt.coefs.size(); ++i) cout << prevFunBt.coefs[i] << " ";cout << endl;


    /*for (int i = 0; i < trChild.size(); ++i) {
        vector <pair <db, db> > ds;
        db prd = prevFunB(trChild[i].back().parametersFormat());
        for (int j = 0; j < trChild.size(); ++j)
            if (i != j) {
                db d = sqr(trChild[i].b - trChild[j].b) + sqr(prd - trChild[j].tUlts.back());
                ds.push_back(make_pair(d, fabs(trChild[i].b - trChild[j].b)));
            }
        sort(ds.begin(), ds.end());
        for (int j = 0; j < 50; ++j)
            cout << "(" << ds[j].first << ", " << ds[j].second << ") ";
        cout << endl;
    }*/

    /*vector <pair <db, int> > badvals;
    db av = 0;
    for (int i = 0; i < xb.size(); ++i) {
        av += b[i];
        badvals.push_back(make_pair(fabs(prevFunB(xb[i]) - b[i]), i));
    }
    av /= b.size();
    sort(badvals.begin(), badvals.end());
    const int BAD = 300;
    bool shit[500] = {0};
    for (int i = 0; i < BAD; ++i)
        shit[badvals[badvals.size() - i - 1].second] = true;
    for (int i = xb.size() - 1; i >= 0; --i)
        if (shit[i]) {
            swap(xb[i], xb.back());
            swap(b[i], b.back());
            xb.pop_back();
            b.pop_back();
        }
    cout << "size of b = " << xb.size() << endl;
    prevFunB = RegressionsCollection::buildLinearRegression(xb, b);*/

    /*db ansr = 0;
    db mn = 1e8;
    const db STEP = 0.01;
    db rig = 0;
    for (int i = 0; i < trChild.size(); ++i)
    for (int j = i + 1; j < trChild.size(); ++j)
        rig = max(rig, distChild(trChild[i], trChild[j]));
    rig = 0.7;///////////SHIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIT
    for (db r = 0; r < rig; r += STEP) {
        db crer = 0;
        for (int i = 0; i < trChild.size(); ++i) {
            db df = trChild[i].b - prevFunB(trChild[i].back().parametersFormat());
            db d = fuckDist(trChild, trChild[i], prevFunB, r, df, i);
            crer += sqr(d - trChild[i].b);
        }

        if (crer < mn) {
            mn = crer;
            ansr = r;
        }
        cerr << "finish " << int(r / STEP) << " from " << int(rig / STEP) << endl;
    }*/


    /*TVector ansr;
    db er = 0;
    for (int i = 0; i < trChild.size(); ++i) {
        vector <pair <db, db> > ds;
        db prd = prevFunB(trChild[i].back().parametersFormat());
        for (int j = 0; j < trChild.size(); ++j)
            if (i != j) {
                db d = sqr(prd - trChild[j].b);// + sqr(trChild[i].tUlts.back() - trChild[j].tUlts.back());
                ds.push_back(make_pair(d, trChild[j].b));
            }
        sort(ds.begin(), ds.end());
        db s = 0;
        int k = 0;
        db mn = 1e8;
        ansr.push_back(0);
        int it;
        for (int j = 0; j < ds.size(); ++j) {
            s += ds[j].second;
            ++k;
            if (fabs(trChild[i].b - s / k) < mn) {
                mn = fabs(trChild[i].b - s / k);
                ansr.back() = ds[j].first;
                it = j;
            }
        }
        er += sqr(mn);
        printf("%d %.7f %d\n", i, ansr.back(), it);
    }
    cerr << "er = " << er << endl;*/

        //ComposeFunctorPowers reg(RegressionsCollection::buildComposeRegressions(dsts, v));//buildRegression(dsts, v));
    //cout << "ansr = " << ansr << endl;
    //cout << "err = " << mner << endl;

    //RandomForest frst(30);
    //frst.learn(xb, b);

    cout << "sizes " << xba[0].size() << " " << xba[1].size() << " " << t1mid << endl;

    for (int i = 0; i < trChild.size(); ++i)
        trChild[i].x = trChild[i].b;

    sort(trChild.begin(), trChild.end(), byT);
    vector <vector <Child> > groups;
    TVector times;
    vector <vector <int> > ee;
    vector <FunctorPowers> regs;

    for (int i = 0; i < trChild.size(); ) {
        int j = i;
        for (; i < trChild.size() && fabs(trChild[i].tUlts.back() - trChild[j].tUlts.back()) < 0.001; ++i);
        times.push_back(trChild[j].tUlts.back());
        groups.push_back(vector <Child>());
        ee.push_back(vector <int>());
        for (int p = j; p < i; ++p) {
            //trChild[p].b = prevFunB(trChild[p].back().parametersFormat());
            if (groups.back().size() == 0 ||
                    fabs(groups.back().back().b - trChild[p].b) > 0.001)
                groups.back().push_back(trChild[p]);
            printf("7 %.5f %.5f ", trChild[p].tUlts.back(), trChild[p].b);
            TVector pr = trChild[p].back().parametersFormat();
            db ar = 0;
            for (int t = 1; t + 1 < pr.size(); ++t)
                ar += (pr[t] + pr[t + 1]) / 2;
            printf("ar = %.3f\n", ar);
            /*for (int e = 1; e < pr.size(); ++e)
                printf("%.3f %.5f ", 1.0 * e / 8, pr[e]);
            printf("\n");*/
        }
        cout << endl;
    }


    /*int ss = trChild.size() / 3;
    for (int i = 30; i < groups.size(); ++i) {
        vector <Child> ch;
        int p1 = i, p2 = i + 1;
        while (ch.size() < ss && (p1 != -1 || p2 < groups.size())) {
            if (p2 == groups.size() || p1 != -1 && times[i] - times[p1] < times[p2] - times[i]) {
                for (int j = 0; j < groups[p1].size(); ++j)
                    ch.push_back(groups[p1][j]);
                --p1;
            } else {
                for (int j = 0; j < groups[p2].size(); ++j)
                    ch.push_back(groups[p2][j]);
                ++p2;
            }
        }

        TMatrix crx;
        TVector crw;
        for (int j = 0; j < ch.size(); ++j) {
            crx.push_back(ch[j].back().parametersFormat());
            crw.push_back(ch[j].b);
        }
        regs.push_back(RegressionsCollection::buildLinearRegression(crx, crw));
        //dprs.push_back(DistancerPredictor());
        //dprs.back().learn(ch, prevFunB.pwr);
        cerr << "i = " << i << endl;
    }*/

    /*TVector rad;
    for (int g = 0; g < groups.size(); ++g) {
        db mner = 1e8, ansr;
        for (db r = 0; r <= 0.8; r += 0.01) {
            db cr = 0;
            for (int i = 0;  i < groups[g].size(); ++i) {
                TVector pr = groups[g][i].back().parametersFormat();
                db prd = prevFunB(pr);
                int k = 0;
                db sm = 0;
                for (int j = 0; j < groups[g].size(); ++j)
                    if (i != j) {
                        TVector pr2 = groups[g][j].back().parametersFormat();
                        db d = 100 *  sqr(groups[g][j].b - prd);
                        for (int e = 0; e + 1 < pr.size(); ++e)
                            d += prevFunB.pwr[e] * sqr(pr[e] - pr2[e]);
                        d = sqrt(d);

                        if (d <= r) {
                            sm += groups[g][j].b;
                            ++k;
                        }
                    }
                db b;
                if (k == 0) b = prd;
                else b = sm / k;
                cr += sqr(b - groups[g][i].b);
            }

            if (cr < mner) {
                mner = cr;
                ansr = r;
            }
        }
        rad.push_back(ansr);
    }

    for (int i = 0; i < rad.size(); ++i)
        cout << i << " " << rad[i] << endl;*/


    int mnRes = 0;
    vector <Comparator> cmprs(groups.size());
    vector <vector <int> > idxs;
    vector <int> ress(groups.size());
    for (int i = 0; i < ress.size(); ++i)
        ress[i] = groups[i].size();

    brute(0, 8, idxs, ress, cmprs, groups);
    for (int i = 0; i < ress.size(); ++i) {
        mnRes += ress[i];
        cout << "cmprs = " << ress[i] << endl;
    }

    for (int i = 0; i < groups.size(); ++i) {
        sort(groups[i].begin(), groups[i].end(), cmprs[i]);
        for (int j = groups[i].size() - 1; j >= 1; --j)
            if (groups[i][j].b < groups[i][j - 1].b)
                groups[i].erase(groups[i].begin() + j);
    }

    vector <Child> resChilds;
    int se = 0;
    for (size_t i = 0; i < teChild.size(); ++i) {
        TVector pr = teChild[i].back().parametersFormat();
        //db w = funW(pr);//approxValue(trChild, teChild[i].back().parametersFormat(), kthFriend, dstW, gamma);
        db w, b;

        TVector v = parameters(teChild[i], SZB);
        db prd = prevFunBt(v);

        db mn = 1e8;
        db t = teChild[i].tUlts.back();
        int gr = 0;
        for (int j = 0; j < times.size(); ++j) {
            db d = fabs(times[j] - t);
            if (d < mn) {
                mn = d;
                gr = j;
            }
        }

        b = groups[gr].back().b;
        Comparator mnComp = cmprs[gr];
        for (int j = 0; j < groups[gr].size(); ++j)
            cout << mnComp.calc(groups[gr][j]) << " ";cout << endl;
        for (int j = 0; j + 1 < groups[gr].size(); ++j)
            if (mnComp(groups[gr][j], teChild[i]) && mnComp(teChild[i], groups[gr][j + 1])) {
                db c1 = mnComp.calc(groups[gr][j]);
                db c2 = mnComp.calc(teChild[i]);
                db c3 = mnComp.calc(groups[gr][j + 1]);
                if (c2 - c1 < c3 - c2) b = groups[gr][j].b;
                else b = groups[gr][j + 1].b;
                b = groups[gr][j].b;
                ++se;
            }
        w = prevFunW(pr);
        resChilds.push_back(Child(teChild[i].id, w, b));
    }
    //cout << "okte = " << okte << " from " << teChild.size() << endl;
    /*for (int i = 0; i < ee.size(); ++i) {
        cout << "gr = ";
        for (int j = 0; j < groups[i].size(); ++j)
            cout << prevFunB(groups[i][j].back().parametersFormat()) << " ";
        cout << endl;
        for (int j = 0; j < ee[i].size(); ++j)
            cout << ee[i][j] << " ";
        cout << endl;
    }*/
    cout << "set = " << se << endl;

    //cout << "cnt good = " << no << endl;
    sort(resChilds.begin(), resChilds.end());
    vector <double> res;
    for (size_t i = 0; i < resChilds.size(); ++i) {
        res.push_back(resChilds[i].b);
        res.push_back(resChilds[i].w);
    }
    return res;
}

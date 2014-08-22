#ifndef CHILDSTUNTEDNESS_H
#define CHILDSTUNTEDNESS_H
#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <cassert>
#include <cmath>
#include <set>
#include <sstream>
#include <cstdio>
#include <algorithm>
#include <fstream>
#include <map>
using namespace std;

typedef double db;
typedef vector <db> TVector;
typedef vector <vector <db> > TMatrix;
class FunctorPowers;
class FunctorGauss;

namespace MatrixOperators {
    TMatrix mul(const TMatrix &a, const TMatrix &b);
    TMatrix pow(TMatrix a, int power);
    TMatrix one(int n);
    TMatrix inverseMatrix(TMatrix a);
    int solveSystem(TMatrix a, TVector & ans);
    TMatrix trans(const TMatrix& a);
    TVector castToVector(const TMatrix& a);
    TMatrix castToMatrix(const TVector& a, bool Transform);
}

class ChildStuntedness {
public:
    vector <double> predict(vector <string> train, vector <string> testing);

    class Child {
    public:
        int id;
        mutable int lastTouch;
        int sex;
        int stat;
        db w, b, x;
        vector <vector <db> > measurements;
        vector <db> tUlts;
        struct Ultrasound {
            const Child *par;
            int id;
            db tUlt;
            int sex;
            int stat;
            vector <db> measurements;
            db w, b;
            Ultrasound() {}
            Ultrasound(const Child* pr): par(pr) {}

            Ultrasound(int idd, db ww, db bb) {
                id = idd;
                w = ww;
                b = bb;
            }

            Ultrasound(string s) {
                stringstream in(s);
                char comma;
                in >> id >> comma >> tUlt >> comma >> sex >> comma >> stat >> comma;
                ++sex;
                for (int i = 0; i < 8; ++i) {
                    db x;
                    in >> x >> comma;
                    measurements.push_back(x);
                }
                in >> w >> comma >> b;
            }

            bool operator < (const Ultrasound& t) const {
                return id < t.id;
            }

            TVector parameters() const {
                TVector res;
                res.push_back(tUlt);
                //res.push_back(sex);
                //res.push_back(stat);
                for (size_t i = 0; i < measurements.size(); ++i)
                    res.push_back(measurements[i]);
                return res;
            }

            TVector parametersFormat() const {
                TVector res;
                res.push_back(tUlt);
                //res.push_back(sex);
                //res.push_back(stat);
                assert(par != NULL);
                for (size_t i = 1; i < measurements.size(); ++i)
                    if (fabs(measurements[i]) < 0.001)
                        res.push_back(par->notNull(i));
                    else
                        res.push_back(measurements[i]);
                return res;
            }
        };

        Child() {}

        Child(int idd, db ww, db bb) {
            id = idd;
            w = ww;
            b = bb;
        }

        Child(int idd, db ww, db bb, int sexx, int statt, vector <vector <db> >  meas) {
            id = idd;
            w = ww;
            b = bb;
            sex = sexx;
            stat = statt;
            measurements = meas;
        }

        void add(const string& s) {
            stringstream in(s);
            char comma;
            db t;
            in >> id >> comma >> t >> comma >> sex >> comma >> stat >> comma;
            ++sex;
            vector <db> cr;
            for (int i = 0; i < 8; ++i) {
                db x;
                in >> x >> comma;
                cr.push_back(x);
            }
            measurements.push_back(cr);
            tUlts.push_back(t);
            in >> w >> comma >> b;
        }

        void add(const Ultrasound& s) {
            id = s.id;
            sex = s.sex;
            stat = s.stat;
            measurements.push_back(s.measurements);
            tUlts.push_back(s.tUlt);
            w = s.w;
            b = s.b;
        }

        Ultrasound operator[] (int i) {
            Ultrasound ret(this);
            ret.id = id;
            ret.tUlt = tUlts.back();////tUlts[i]
            ret.sex = sex;
            ret.stat = stat;
            ret.measurements = measurements[i];
            lastTouch = i;
            return ret;
        }

        Ultrasound back() {
            return this->operator [](measurements.size() - 1);
        }

        Ultrasound operator[] (int i) const {
            //cerr << "[] " << i << " " << tUlts.size() << " " << measurements.size() << endl;
            Ultrasound ret(this);
            ret.id = id;
            ret.tUlt = tUlts.back();////tUlts[i]
            ret.sex = sex;
            ret.stat = stat;
            ret.measurements = measurements[i];
            lastTouch = i;
            return ret;
        }

        Ultrasound back() const {
            return this->operator [](measurements.size() - 1);
        }

        int size() const {
            return measurements.size();
        }

        bool operator < (const Child& t) const {
            return id < t.id;
        }

        db notNull(int j) const {
            int i = lastTouch;
            //cerr << "i = " << lastTouch << endl;
            assert(lastTouch >= 0);
            while (i > 0 && fabs(measurements[i][j]) < 0.001) --i;
            return measurements[i][j];
        }
    };
    typedef Child::Ultrasound Ultrasound;
private:

    TVector featuresParams(Child chld);
    //void sequences(int i, int n, int* a, int* dx, vector <int>& dbit);
    //FunctorPowers buildLinerRegression(const vector <TVector>& x, const TVector& w,
      //                                 const vector <TVector>& trX, const TVector& wTr);
    //FunctorPowers buildLinerRegression(const vector <TVector>& x, const TVector& w);
    //void brute(int p, vector <vector <int> >& idx, const vector<TVector>& x, const TVector &w, db& mnEr, FunctorPowers &ret);
    //FunctorGauss buildGaussRegression(const vector <TVector>& x, const TVector& w, int *powers);
    //FunctorPowers buildBetterRegression(const vector<TVector> &x, const TVector &w, int *powers);
};

#endif // CHILDSTUNTEDNESS_H

#include "checker.h"

using namespace MatrixOperators;

Checker::Checker(const char* trainf, const char* testf) {
    train = trainf;
    test = testf;
}

bool cmprB(Checker::Ultrasound x, Checker::Ultrasound y) {
    return x.b < y.b;
}

bool cmprW(Checker::Child x, Checker::Child y) {
    return x.w < y.w;
}

bool cmprT(Checker::Ultrasound x, Checker::Ultrasound y) {
    return x.tUlt < y.tUlt;
}

double sqr(double x) {
    return x * x;
}

double corr(vector <double> x, vector <double> y) {
    int n = x.size();
    double ex = 0, ey = 0, disx = 0, disy = 0;
    for (int i = 0; i < n; ++i) {
        ex += x[i];
        ey += y[i];
    }
    ex /= n;
    ey /= n;
    for (int i = 0; i < n; ++i) {
        disx += sqr(x[i] - ex);
        disy += sqr(y[i] - ey);
    }
    disx /= n;
    disy /= n;
    disx = sqrt(disx);
    disy = sqrt(disy);
    double cov = 0;
    for (int i = 0; i < n; ++i)
        cov += (x[i] - ex) * (y[i] - ey);
    cov /= n;
    return cov / (disx * disy);
}

double Checker::run() {
    vector <string> trV, teV;
    ifstream inTrain(train);
    string s;
    while (getline(inTrain, s))
        trV.push_back(s);
    ifstream inTest(test);
    while (getline(inTest, s))
        teV.push_back(s);

    ChildStuntedness tst;
    int l = clock();
    vector <double> myRes = tst.predict(trV, teV);
    cerr << "run time: " << (clock() - l) / 1000000.0 << endl;
    vector <Child> correctRes;

    for (size_t i = 0; i < teV.size();) {
        int j = i;
        correctRes.push_back(Child());
        correctRes.back().add(Ultrasound(teV[i]));
        ++i;
        for (; i < teV.size() && Ultrasound(teV[i]).id == correctRes.back().id; ++i)
            correctRes.back().add(Ultrasound(teV[i]));
    }

    //cout << myRes.size() << " " << correctRes.size() << endl;
    assert(myRes.size() == 2 * correctRes.size());

    sort(correctRes.begin(), correctRes.end());

    double avb = 0, avw = 0;
    vector <Child> srt;
    vector <double> x, y;
    for (size_t i = 0; i < trV.size(); ) {
        srt.push_back(Child());
        srt.back().add(Ultrasound(trV[i]));
        ++i;
        for (; i < trV.size() && Ultrasound(trV[i]).id == srt.back().id; ++i)
            srt.back().add(Ultrasound(trV[i]));
        avb += srt.back().b;
        avw += srt.back().w;
    }
    for (int i = 0; i < srt.size(); ++i) {
        x.push_back(srt[i].w);
        y.push_back(srt[i].b);
    }
    set <int> se;
    for (int i = 0; i < srt.size(); ++i)
        se.insert(srt[i].id);
    int cnt = 0;
    for (int i = 0; i < correctRes.size(); ++i)
        cnt += se.find(correctRes[i].id) != se.end();
    //cout << "cnt eq = " << cnt << " from " << correctRes.size() << endl;
    /*cout << x.size() << endl;
    for (int i = 0; i < x.size(); ++i)
        cout << y[i] << " " << x[i] << endl;
    cout << "correltion = " << corr(x, y) << endl;*/

    ofstream ow("W.txt");
    ow << srt.size() << endl;
    for (int i = 0; i < srt.size(); ++i)
        ow << srt[i].back().tUlt << " " << srt[i].w << endl;
    ow.close();


    ofstream ob("B.txt");
    ob << srt.size() << endl;
    for (int i = 0; i < srt.size(); ++i)
        ob << srt[i].back().tUlt << " " << srt[i].b << endl;
    ob.close();
    /*sort(srt.begin(), srt.end(), cmprW);
    reverse(srt.begin(), srt.end());
    db ww = 0;
    ofstream *of[8];
    for (int i = 1; i < 8; ++i) {
        char ss[20];
        sprintf(ss, "anal%d.txt", i);
        of[i] = new ofstream(ss);
    }
    for (size_t i = 0; i < srt.size(); ++i) {
        for (int p = 1; p < 8; ++p) {
            *of[p] << srt[i].w << " " << srt[i].b << endl;
            *of[p] << srt[i].size() << endl;
            for (int j = 0; j < srt[i].size(); ++j)
                *of[p] << srt[i].tUlts[j] << " " << srt[i][j].measurements[p] << endl;
        }

    }
    for (int i = 1; i < 8; ++i)
        of[i]->close();*/

    avb = avb / srt.size();
    avw = avw / srt.size();
    cout << "avw avv " << avw << " " << avb << endl;
    double sse = 0, sse0 = 0;
    double oterw = 0, oterb = 0;
    int pos = 0, neg = 0;
    db tt = 0;
    int ccc = 0;
    for (size_t i = 0; i < correctRes.size(); ++i) {
        //if (correctRes[i].size() != 6)
            //continue;
        ++ccc;
        TVector v1;
        v1.push_back(myRes[2 * i] - correctRes[i].b);
        v1.push_back(myRes[2 * i + 1] - correctRes[i].w);
        TMatrix invs(2, TVector(2));
        invs[0][0] = 3554.42;
        invs[0][1] = -328.119;
        invs[1][0] = -328.119;
        invs[1][1] = 133.511;
        double ei = mul(mul(castToMatrix(v1, false), invs), castToMatrix(v1, true))[0][0];
        TVector v2;
        v2.push_back(avb - correctRes[i].b);
        v2.push_back(avw - correctRes[i].w);
        double e0i = mul(mul(castToMatrix(v2, false), invs), castToMatrix(v2, true))[0][0];
        sse += ei;
        sse0 += e0i;

        oterb += fabs(myRes[2 * i] - correctRes[i].b) / correctRes[i].b;
        oterw += fabs(myRes[2 * i + 1] - correctRes[i].w) / correctRes[i].w;
        //cout << fabs(myRes[2 * i] - correctRes[i].b)
             //<< " " << fabs(myRes[2 * i + 1] - correctRes[i].w) << endl;

        //cout << "cor = " << correctRes[i].b - myRes[2 * i] << endl;
        //printf("cor = %.6f %.8f\n", correctRes[i].b, myRes[2 * i]);
        pos += correctRes[i].w - myRes[2 * i + 1] > 0;
        neg += correctRes[i].w - myRes[2 * i + 1] < 0;
        tt += fabs(correctRes[i].b - myRes[2 * i]);
    }
    cout << "pos && neg && err " << pos << " " << neg << " " << tt << endl;
    cout << "erb = " << oterb / ccc << " "
         << "erw = " << oterw / ccc << endl;
    return max(0.0, 1 - sse / sse0) * 1000000;
}


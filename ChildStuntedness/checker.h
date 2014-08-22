#ifndef CHECKER_H
#define CHECKER_H
#include "ChildStuntedness.h"

class Checker {
    const char* train, *test;
public:
    typedef ChildStuntedness::Child::Ultrasound Ultrasound;
    typedef ChildStuntedness::Child Child;
    Checker(const char* trainf, const char* testf);
    double run();
};

#endif // CHECKER_H

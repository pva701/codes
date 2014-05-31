#ifndef OPTIMIZE_VECTOR_H
#define OPTIMIZE_VECTOR_H
#include <cstdlib>
#include <cstring>
#include <iostream>

enum State {IsEmpty, IsShort, IsLong};

struct OptimizeVector {
private:
    struct StorageVector;
    StorageVector* store;
    unsigned int shortNumber;
public:
    State curState;
    OptimizeVector();
    OptimizeVector(const OptimizeVector& rhs);
    OptimizeVector& operator = (const OptimizeVector& rhs);

    unsigned int& operator [] (int i);
    unsigned int operator  [] (int i) const;
    void push_back(unsigned int x);
    void pop_back();
    size_t size() const;
    unsigned int back() const;
    void resize(int sz);
    void reverse();
    OptimizeVector subVector(int i, int j) const;
    void getOwnCopy();
    ~OptimizeVector();
};

#endif // OPTIMIZE_VECTOR_H

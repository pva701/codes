#include "optimize_vector.h"

struct OptimizeVector::StorageVector {
private:
    int ptr;
    unsigned int *a;
    int sizeA;

    StorageVector(const StorageVector& rhs);//copy protection
    StorageVector& operator = (const StorageVector& rhs);//assign protection

    void incCapacity() {
        if (sizeA == 0)
            sizeA = 1;
        unsigned int *newA = new unsigned int[2 * sizeA];
        for (int i = 0; i < ptr; ++i)
            newA[i] = a[i];
        delete[] a;
        sizeA *= 2;
        a = newA;
    }

    void decCapacity() {
        unsigned int *newA = new unsigned int[sizeA / 2];
        for (int i = 0; i < ptr; ++i)
            newA[i] = a[i];
        delete[] a;
        sizeA /= 2;
        a = newA;
    }

public:
    int countRefs;
    StorageVector() {
        a = NULL;
        countRefs = 1;
        ptr = 0;
        sizeA = 0;
    }

    StorageVector(unsigned int* cpa, int sz) {
        countRefs = 1;
        sizeA = 1;
        while (sizeA < sz) sizeA *= 2;
        ptr = sz;
        a = new unsigned int[sizeA];
        for (int i = 0; i < sz; ++i)
            a[i] = cpa[i];
    }

    StorageVector* getOwnCopy() {
        if (countRefs == 1)
            return this;
        --countRefs;
        return new StorageVector(a, ptr);
    }

    inline int size() const {
        return ptr;
    }

    inline void push_back(unsigned int x) {
        if (sizeA == ptr)
            incCapacity();
        a[ptr++] = x;
    }

    inline void pop_back() {
        if (sizeA / 4 == ptr)
            decCapacity();
        ptr--;
    }

    void resize(int sz) {
        delete[] a;
        ptr = sz;
        sizeA = 1;
        while (sizeA < sz) sizeA *= 2;
        countRefs = 1;
        a = new unsigned int[sizeA];
        memset(a, 0, sizeof (unsigned int) * sz);
    }

    void reverse() {
        for (int i = 0; 2 * i < ptr; ++i)
            std::swap(a[i], a[ptr - i - 1]);
    }

    inline unsigned int& operator [] (int i) {
        return a[i];
    }

    ~StorageVector() {
        delete[] a;
    }
};

//OptimizeVector
OptimizeVector::OptimizeVector() {
    curState = IsEmpty;
    store = NULL;
}

OptimizeVector::OptimizeVector(const OptimizeVector& rhs) {
    if (rhs.curState == IsLong) {
        rhs.store->countRefs++;
        store = rhs.store;
        curState = IsLong;
    } else {
        curState = rhs.curState;
        shortNumber = rhs.shortNumber;
    }
}

OptimizeVector& OptimizeVector::operator = (const OptimizeVector& rhs) {
    if (rhs.curState == IsLong)
        rhs.store->countRefs++;

    if (curState == IsLong)
        if (--store->countRefs == 0)
            delete store;

    if (rhs.curState != IsLong) {
        curState = rhs.curState;
        shortNumber = rhs.shortNumber;
    } else {
        curState = IsLong;
        store = rhs.store;
    }
    return *this;
}

unsigned int& OptimizeVector::operator [] (int i) {//need ensure own copy for fast exec
    //assert(store->countRefs == 1);
    if (curState == IsShort)
        return shortNumber;
    if (store->countRefs != 1)
        store = store->getOwnCopy();
    return (*store)[i];
}

unsigned int OptimizeVector::operator [] (int i) const {
    if (curState == IsShort)
        return shortNumber;
    return (*store)[i];
}

void OptimizeVector::push_back(unsigned int x) {//own copy ensure for fast exec
    //assert(store->countRefs == 1);
    if (curState == IsEmpty) {
        shortNumber = x;
        curState = IsShort;
    } else {
        if (curState == IsShort) {
            store = new StorageVector();
            store->push_back(shortNumber);
        }
        if (store->countRefs != 1)
            store = store->getOwnCopy();
        curState = IsLong;
        store->push_back(x);
    }
}

void OptimizeVector::pop_back() {//own copy ensure for fast exec
    //assert(store->countRefs == 1);
    if (curState == IsShort)
        curState = IsEmpty;
    else {
        if (store->countRefs != 1)
            store = store->getOwnCopy();

        store->pop_back();
        if (store->size() == 1) {
            curState = IsShort;
            shortNumber = (*store)[0];
            delete store;
        }
    }
}

size_t OptimizeVector::size() const {
    if (curState == IsEmpty)
        return 0;
    if (curState == IsShort)
        return 1;
    return store->size();
}

unsigned int OptimizeVector::back() const {
    if (curState == IsShort)
        return shortNumber;
    return (*store)[size() - 1];
}

void OptimizeVector::resize(int sz) {//own copy ensure for fast exec
    //assert(store->countRefs == 1);
    if (sz == 0)
        curState = IsEmpty;
    else if (sz == 1) {
        curState = IsShort;
        shortNumber = 0;
    } else {
        if (curState != IsLong)
            store = new StorageVector();
        if (store->countRefs != 1)
            store = store->getOwnCopy();
        curState = IsLong;
        store->resize(sz);
    }
}

void OptimizeVector::reverse() {//own copy ensure for fast exec
    //assert(store->countRefs == 1);
    if (store != NULL && store->countRefs != 1)
        store = store->getOwnCopy();
    if (curState == IsLong)
        store->reverse();
}

OptimizeVector OptimizeVector::subVector(int i, int j) const {
    OptimizeVector ret;
    if (curState == IsEmpty || j - i == 0)
        return ret;
    else if (curState == IsShort) {
        ret.push_back(shortNumber);
        return ret;
    }

    if (j - i == 1) {
        ret.push_back((*store)[i]);
        return ret;
    }

    ret.resize(j - i);
    int len = j - i;
    for (int t = 0; t < len; ++t)
        ret[t] = (*store)[i + t];
    return ret;
}

void OptimizeVector::getOwnCopy() {
    if (curState == IsLong)
        store = store->getOwnCopy();
}

OptimizeVector::~OptimizeVector() {
    if (curState == IsLong) {
        if (--store->countRefs == 0)
            delete store;
        curState = IsEmpty;
    }
}

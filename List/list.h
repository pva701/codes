#ifndef LIST_H
#define LIST_H
#include <iostream>
#include <cassert>
using namespace std;

struct list {
    struct const_iterator;
    struct iterator;

    list();
    list(list const&);
    list& operator = (list const&);
    bool empty() const;
    void push_back(int);
    int& back();
    int const& back() const;
    void pop_back();
    void push_front(int);
    int& front();
    int const& front() const;
    void pop_front();

    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;

    void insert(iterator, int);
    void erase(iterator);
    void splice(iterator pos, list& other, iterator first, iterator last);
    ~list();
    friend ostream& operator << (ostream& out, list const& a);
private:
    struct node {
        node *next, *prev;
        int val;
        node() {}
        node(int value) {
            next = 0;
            prev = 0;
            val = value;
        }
    };
    node *head;
};

struct list::iterator {
    iterator();
    int& operator *() const;
    iterator& operator ++ ();
    iterator operator ++ (int);
    iterator& operator --();
    iterator operator -- (int);
    friend class list;
    friend bool operator == (iterator const& a, iterator const& b);
    friend bool operator != (iterator const& a, iterator const& b);
private:
    iterator(node*);
    list::node *cur;
};

struct list::const_iterator {
    const_iterator();
    int const& operator * () const;
    friend class list;
private:
    const_iterator(node*);
    node *cur;
};

bool operator == (list::iterator const& a, list::iterator const& b);
bool operator != (list::iterator const& a, list::iterator const& b);

ostream& operator << (ostream& out, list const& a);
#endif // LIST_H

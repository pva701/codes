#include "list.h"

//begin of dangerous zone
list::list() {
    head = new node();
    head->next = head;
    head->prev = head;
}

list::~list() {
    node *cur = head;
    cur = cur->next;
    while (cur != head) {
        node *nx = cur->next;
        delete cur;
        cur = nx;
    }
    delete cur;
}

list::list(list const& oth)
{
    node *cur = oth.head->next;
    head = new node();
    try
    {
        node *tmpPrev = head;
        while (cur != oth.head) {
            node *curNode = new node(cur->val);
            curNode->next = 0;//end of list if exc
            tmpPrev->next = curNode;
            curNode->prev = tmpPrev;
            tmpPrev = curNode;
            cur = cur->next;
        }
        tmpPrev->next = head;
        head->prev = tmpPrev;
    } catch (...) {
        node *cur = head->next;
        while (cur != 0) {
            node *nx = cur->next;
            delete cur;
            cur = nx;
        }
        head->next = head;
        head->prev = head;
        throw;
    }
}

list& list::operator = (list const& oth) {
    if (this == &oth)
        return *this;
    list tmp(oth);
    swap(tmp.head, this->head);
    return *this;
}
//end of dangerous zone


bool list::empty() const {
    return head->next == head;
}

void list::push_back(int value) {
    node *newNode = new node(value);
    newNode->prev = head->prev;
    head->prev->next = newNode;

    newNode->next = head;
    head->prev = newNode;
}

int& list::back() {
    assert(!empty());
    return head->prev->val;
}

const int& list::back() const {
    assert(!empty());
    return head->prev->val;
}

void list::pop_back() {
    assert(!empty());
    node *last = head->prev;
    last->prev->next = head;
    head->prev = last->prev;
    delete last;
}


void list::push_front(int value) {
    node *newNode = new node(value);
    node *first = head->next;
    newNode->next = first;
    first->prev = newNode;

    head->next = newNode;
    newNode->prev = head;
}

int& list::front() {
    assert(!empty());
    return head->next->val;
}

const int& list::front() const {
    assert(!empty());
    return head->next->val;
}

void list::pop_front() {
    assert(!empty());
    node *first = head->next;
    head->next = first->next;
    first->next->prev = head;
    delete first;
}

list::iterator list::begin() {
    return iterator(head->next);
}

list::const_iterator list::begin() const {
    return const_iterator(head->next);
}

list::iterator list::end() {
    return iterator(head);
}

list::const_iterator list::end() const {
    return const_iterator(head);
}

void list::insert(iterator pos, int value) {
    node *newNode = new node(value);
    node *v = pos.cur;
    newNode->prev = v->prev;
    v->prev->next = newNode;

    newNode->next = v;
    v->prev = newNode;
}

void list::erase(iterator pos) {
    node *v = pos.cur;
    v->prev->next = v->next;
    v->next->prev = v->prev;
    delete v;
}

void list::splice(iterator pos, list &other, iterator first, iterator last) {
    if (first == last)
        return;
    node *fV = first.cur;
    node *lV = last.cur->prev;
    fV->prev->next = lV->next;
    //fv->prev = 0;

    lV->next->prev = fV;
    //lv->next = 0;

    node *v = pos.cur;
    v->prev->next = fV;
    fV->prev = v->prev;

    lV->next = v;
    v->prev = lV;
}

//iterators
list::iterator::iterator() {
    cur = 0;
}

list::iterator::iterator(node *oth) {
    cur = oth;
}

int& list::iterator::operator * () const {
    return cur->val;
}

list::iterator& list::iterator::operator ++ () {
    cur = cur->next;
    return *this;
}

list::iterator list::iterator::operator ++ (int) {
    iterator tmp = *this;
    cur = cur->next;
    return tmp;
}

list::iterator& list::iterator::operator --() {
    cur = cur->prev;
    return *this;
}

list::iterator list::iterator::operator -- (int) {
    iterator tmp = *this;
    cur = cur->prev;
    return tmp;
}

//const iterator
list::const_iterator::const_iterator(node *oth) {
    cur = oth;
}

const int& list::const_iterator::operator * () const {
    return cur->val;
}


bool operator == (list::iterator const& a, list::iterator const& b) {
    return a.cur == b.cur;
}

bool operator != (list::iterator const& a, list::iterator const& b) {
    return a.cur != b.cur;
}

ostream& operator << (ostream& out, list const& a) {
    list::node *cur = a.head->next;
    while (cur != a.head) {
        out << cur->val << " ";
        cur = cur->next;
    }
    return out;
}

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <iostream>
#include <memory>
#include <vector>

class QueueP {
public:
    QueueP();
    QueueP(const QueueP& lhs);
    QueueP(QueueP&& rhs);

    QueueP& operator=(const QueueP& lhs);
    QueueP& operator=(QueueP&& rhs);

    void push(int val);
    int pop();
    size_t get_size();
    void clear();
    void print() const;
    std::vector<int> make_arr();
    ~QueueP();

private:
    struct Node {
        int val;
		std::unique_ptr<Node> next;
    };

    size_t size;
    std::unique_ptr<Node> head;
};

//--------------------------------------------------

QueueP::QueueP(): size(0), head(nullptr) {}

QueueP::QueueP(const QueueP& lhs): size(0) {
    Node* t = lhs.head.get();

    while (t != nullptr) {
        push(t->val);
        t = t->next.get();
    }
}

QueueP::QueueP(QueueP&& rhs): size(0) {
    Node* t = rhs.head.get();

    while (t != nullptr) {
        push(t->val);
        t = t->next.get();
    }

    rhs.clear();
}

QueueP& QueueP::operator=(const QueueP& lhs) {
    if (this == &lhs) {
        return *this;
    }

    QueueP T(lhs);
    std::swap(head, T.head);
    std::swap(size, T.size);

    return *this;
}

QueueP& QueueP::operator=(QueueP&& rhs) {
    if (this == &rhs) {
        return *this;
    }

    QueueP T(std::move(rhs));
    std::swap(head, T.head);
    std::swap(size, T.size);

    return *this;
}

void QueueP::push(int val) {
    if (head == nullptr || val < head.get()->val) {
        head = std::unique_ptr<Node>(new Node{val, std::move(head)});
    } else {
        Node* t = head.get();
        while (t->next != nullptr && val >= (t->next).get()->val) {
            t = t->next.get();
        }
        t->next = std::unique_ptr<Node>(new Node{val, std::move(t->next)});
    }
    size += 1;
}

int QueueP::pop() {
    if (head != nullptr) {
        int val = head.get()->val;
        head = std::move(head.get()->next);
        size -= 1;
        return val;
    } else {
        return 0;
    }
}

size_t QueueP::get_size() {
    return size;
}

void QueueP::clear() {
    head = nullptr;
    size = 0;
}

void QueueP::print() const {
    Node* t = head.get();
    while (t != nullptr) {
        std::cout << t->val << " ";
        t = t->next.get();
    }
    std::cout << "| size: " << size << "\n";
}

std::vector<int> QueueP::make_arr() {
    std::vector<int> arr;
    Node* t = head.get();

    while (t != nullptr) {
        arr.push_back(t->val);
        t = t->next.get();
    }

    return arr;
}

QueueP::~QueueP() {
    while (head != nullptr) {
        head = std::move(head.get()->next);
    }
}

void random(std::vector<int>& arr, QueueP& q, size_t size) {
    int r;
    for (size_t i = 0; i < size; i++) {
        r = rand() % 1000;
        arr.push_back(r);
        q.push(r);
    }

    std::sort(arr.begin(), arr.end());
}

void random(QueueP& q, size_t size) {
    int r;
    for (size_t i = 0; i < size; i++) {
        r = rand() % 1000;
        q.push(r);
    }
}

// int main() {
//     QueueP q1, q2;

//     q1.push(8);
//     q1.push(4);
//     q1.push(3);

//     q2.push(1);
//     q2.push(2);
//     q2.push(7);
//     q2.push(0);

//     q1.print();
//     q2.print();

//     QueueP q3(std::move(q1));
//     q2.clear();

//     q1.print();
//     q2.print();
//     q3.print();

//     return 0;
// }

TEST_CASE("Push_random_0") {
    size_t size = 0;
    QueueP q;
    std::vector<int> arr;

    random(arr, q, size);

    CHECK(arr == q.make_arr());
}

TEST_CASE("Push_random_1") {
    size_t size = 10;
    QueueP q;
    std::vector<int> arr;

    random(arr, q, size);

    CHECK(arr == q.make_arr());
}

TEST_CASE("Push_random_2") {
    size_t size = 100;
    QueueP q;
    std::vector<int> arr;

    random(arr, q, size);

    CHECK(arr == q.make_arr());
}

TEST_CASE("Pop_random_0") {
    size_t size = 0;
    QueueP q;

    CHECK(q.pop() == 0);
}

TEST_CASE("Pop_random_1") {
    size_t size = 10;
    QueueP q;
    std::vector<int> arr;

    random(arr, q, size);

    CHECK(arr[0] == q.pop());
}

TEST_CASE("Pop_random_2") {
    size_t size = 100;
    QueueP q;
    std::vector<int> arr;

    random(arr, q, size);

    CHECK(arr[0] == q.pop());
}

TEST_CASE("Copy_lhs_1") {
    size_t size = 10;
    QueueP q1;

    random(q1, size);

    QueueP q2(q1);

    CHECK(q1.make_arr() == q2.make_arr());
}

TEST_CASE("Copy_lhs_2") {
    size_t size = 10;
    QueueP q1, q2;

    random(q1, size);

    q2 = q1;

    CHECK(q1.make_arr() == q2.make_arr());
}

TEST_CASE("Copy_rhs_1") {
    size_t size = 10;
    QueueP q1;
    std::vector<int> arr;

    random(arr, q1, size);

    QueueP q2(std::move(q1));

    CHECK(arr == q2.make_arr());
    CHECK(q1.get_size() == 0);
}

TEST_CASE("Copy_rhs_2") {
    size_t size = 10;
    QueueP q1,  q2;
    std::vector<int> arr;

    random(arr, q1, size);
    q2 = std::move(q1);

    CHECK(arr == q2.make_arr());
    CHECK(q1.get_size() == 0);
}

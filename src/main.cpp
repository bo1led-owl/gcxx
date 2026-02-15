#include <iostream>

#include "alloc.hpp"

struct Example {
    int a;
    int b;
    int c;

    void print() {
        std::cout << a << b << c << std::endl;
    }
};

int main(void) {
    auto gpa = GC::Alloc(4096);
    Example* a = (Example*)gpa.allocate(sizeof(Example));
    a->a = 1;
    a->b = 2;
    a->c = 3;
    Example* b = (Example*)gpa.allocate(sizeof(Example));
    b->a = 4;
    b->b = 5;
    b->c = 6;
    Example* c = (Example*)gpa.allocate(sizeof(Example));
    c->a = 7;
    c->b = 8;
    c->c = 9;

    std::cout << "all allocated\n";

    a->print();
    b->print();
    c->print();

    gpa.deallocate(c);
    gpa.deallocate(a);
    gpa.deallocate(b);

    return 0;
}

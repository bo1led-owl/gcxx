#include <iostream>
#include "gc.hpp"

struct Example {
    int a;
    int b;
    int c;

    void print() {
        std::cout << a << b << c << std::endl;
    }
};

void memory_leak() {
    void* bebebebebbeeb = GC::instance.allocate(sizeof(int));
}

int main(void) {
    void* a = GC::instance.allocate(502);
    memory_leak();
    void* b = GC::instance.allocate(1022);

    return 0;
}

#include <iostream>
#include "gc_new.hpp"

struct Example {
    int* a;
    int b;
    int c;
};

void memory_leak() {
    void* bebebebebbeeb = GC::instance.allocate(502);
}

void another_memory_leak() {
    Example* a = (Example*)GC::instance.allocate(100);
}

int main(void) {
    void* a = GC::instance.allocate(4);
    memory_leak();
    void* b = GC::instance.allocate(1022);
    another_memory_leak();
    void* c = GC::instance.allocate(1000);

    return 0;
}

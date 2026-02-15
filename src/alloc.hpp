#pragma once

#include <cstddef>
#include <list>
#include <utility>

#include "mallocator.hpp"

class Alloc {
    using Header = std::pair<void*, size_t>;
    using ObjectList = std::list<Header, MLC::Alloc<Header>>;
    ObjectList free;
    ObjectList allocated;

public:
    [[nodiscard]] void* allocate(size_t size);
    void deallocate(void* p);

    auto allocated_begin() {
        return allocated.begin();
    }

    auto allocated_end() {
        return allocated.end();
    }
};

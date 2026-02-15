#pragma once

#include <cstddef>
#include <list>
#include <map>

#include "mallocator.hpp"

namespace GC {

class Alloc {
public:
    Alloc(size_t size_bytes);
    ~Alloc();

    std::map<void*, size_t, std::less<void*>, MLC::Alloc<std::pair<void* const, size_t>>> allocated;
    void* heap;
    static constexpr size_t MIN_SIZE = sizeof(void*) * 2;

    [[nodiscard]] void* allocate(size_t size);
    void deallocate(void* p);

private:
    struct Header {
        void* addr;
        size_t size;
    };
    using ObjectList = std::list<Header, MLC::Alloc<Header>>;
    ObjectList free;
    size_t heap_size;

    void split_if_possible(ObjectList::iterator node, size_t needed_size);
    void coalesce_with_next(GC::Alloc::ObjectList::iterator nd);
};
}  // namespace GC

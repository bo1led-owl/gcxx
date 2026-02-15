#include "alloc.hpp"

#include <algorithm>
#include <cstdio>
#include <stdexcept>
#include <utility>

void GC::Alloc::print_state() {
    std::puts("\tfree:");
    for (auto block : free) {
        std::printf("\t\taddr: %p\n\t\tsize: %lu", block.addr, block.size);
    }
    std::puts("\tallocated:");
    for (auto e : allocated) {
        std::printf("\t\taddr: %p\n\t\tsize: %lu", e.first, e.second);
    }
}

GC::Alloc::Alloc(size_t size_bytes) : heap_size(size_bytes) {
    heap = std::malloc(heap_size);
    free.emplace_back(Header{heap, heap_size});
}

GC::Alloc::~Alloc() {
    std::free(heap);
}

/**
 * If possible, shrinks node and adds the remainder to free as a free block.
 *
 * @param node **allocated** node.
 * @param needed_size the minimal size node needs.
 */
void GC::Alloc::split_if_possible(GC::Alloc::ObjectList::iterator node, size_t needed_size) {
    if (node != free.end() && node->size > needed_size) {
        free.push_back(Header{(char*)node->addr + needed_size, node->size - needed_size});
        node->size = needed_size;
        allocated.insert_or_assign(node->addr, node->size);
    }
}

void* GC::Alloc::allocate(size_t sz) {
    size_t size = sz + (MIN_SIZE - (sz % MIN_SIZE)) % MIN_SIZE;
    auto block = std::find_if(free.begin(), free.end(), [size](auto& x) { return x.size >= size; });

    print_state();
    if (block == free.end()) {
        return nullptr;
    }

    auto addr = block->addr;

    allocated.emplace(block->addr, block->size);
    split_if_possible(block, size);
    free.erase(block);

    print_state();
    return addr;
}

void GC::Alloc::coalesce_with_next(GC::Alloc::ObjectList::iterator nd) {
    if (nd == free.end()) {
        return;
    }
    auto node = nd;
    auto next = ++nd;

    if (node != free.end() && next != free.end()) {
        if ((char*)node->addr + node->size == (char*)next->addr) {
            node->size += next->size;
            free.erase(next);
        }
    }
}

void GC::Alloc::deallocate(void* p) {
    print_state();

    auto block_it = allocated.find(p);

    if (block_it == allocated.end()) {
        throw std::invalid_argument("Trying to deallocate never-allocated pointer!");
    }

    auto next = std::find_if(
        free.begin(), free.end(), [block_it](auto& block) { return block.addr > block_it->first; });
    free.insert(next, Header{block_it->first, block_it->second});
    allocated.erase(p);

    coalesce_with_next(--next);

    print_state();
}

#include "alloc.hpp"

#include <cstdio>
#include <stdexcept>
#include <utility>

void GC::Alloc::print_state() {
    printf("\tfree:\n");
    for (auto block : free) {
        printf("\t\taddr: %p\n\t\tsize: %zu\n", block.addr, block.size);
    }
    printf("\tallocated:\n");
    for (auto e : allocated) {
        printf("\t\taddr: %p\n\t\tsize: %zu\n", e.first, e.second);
    }
}

GC::Alloc::Alloc(size_t size_bytes) : heap_size(size_bytes) {
    heap = std::malloc(heap_size);
    printf("[ALLOC DBG] heap at %p\n", heap);
    free.emplace_back(Header{heap, heap_size});
}

GC::Alloc::~Alloc() {
    std::free(heap);
}

GC::Alloc::ObjectList::iterator GC::Alloc::find(size_t size) {
    auto it = free.begin();
    while (it != free.end()) {
        if (it->size >= size) {
            return it;
        }
        ++it;
    }

    std::unreachable();
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
    auto block = find(size);

    printf(
        "[ALLOC DBG] allocating:\n\trequired size %zu\n\taligned size %zu\n\tblock address: %p\n",
        sz,
        size,
        block->addr);

    print_state();
    if (block == free.end()) {
        return nullptr;
    }

    auto addr = block->addr;

    allocated.emplace(block->addr, block->size);
    split_if_possible(block, size);
    free.erase(block);

    print_state();
    printf("\n\n\n");
    return addr;
}

void GC::Alloc::deallocate(void* p) {
    printf("[ALLOC DBG] deallocating %p\n", p);
    print_state();
    auto block_it = allocated.find(p);
    printf("\tblock_it:\n\t\taddr: %p\n\t\tsize: %zu\n", block_it->first, block_it->second);

    if (block_it == allocated.end()) {
        throw std::invalid_argument("Trying to deallocate never-allocated pointer!");
    }

    free.push_back(Header{block_it->first, block_it->second});
    allocated.erase(p);
    print_state();
}

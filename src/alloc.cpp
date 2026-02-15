#include "alloc.hpp"

#include <algorithm>
#include <print>
#include <stdexcept>
#include <utility>

void GC::Alloc::print_state() {
    std::println("\tfree:");
    for (auto block : free) {
        std::println("\t\taddr: {}\n\t\tsize: {}", block.addr, block.size);
    }
    std::println("\tallocated:");
    for (auto e : allocated) {
        std::println("\t\taddr: {}\n\t\tsize: {}", e.first, e.second);
    }
}

GC::Alloc::Alloc(size_t size_bytes) : heap_size(size_bytes) {
    heap = std::malloc(heap_size);
    std::println("[ALLOC DBG] heap at {}", heap);
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

    std::println(
        "[ALLOC DBG] allocating:\n\trequired size {}\n\taligned size {}\n\tblock address: {}",
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
    std::println("\n\n");
    return addr;
}

void GC::Alloc::deallocate(void* p) {
    std::println("[ALLOC DBG] deallocating {}", p);
    print_state();

    auto block_it = allocated.find(p);

    std::println("\tblock_it:\n\t\taddr: {}\n\t\tsize: {}", block_it->first, block_it->second);

    if (block_it == allocated.end()) {
        throw std::invalid_argument("Trying to deallocate never-allocated pointer!");
    }

    free.push_back(Header{block_it->first, block_it->second});
    allocated.erase(p);

    print_state();
}

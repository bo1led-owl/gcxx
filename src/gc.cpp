#include "gc.hpp"

#include <cstdio>

#include "src/alloc.hpp"

bool GC::initialized = false;
GC::GCollector GC::instance{};

namespace {
void ensure_initialized() {
    if (!GC::initialized) [[unlikely]] {
        std::construct_at<GC::GCollector>(&GC::instance);
    }
}
}  // namespace

void* operator new(std::size_t size) {
    if (size == 0)
        size = 1;
    ensure_initialized();
    void* ptr = GC::instance.allocate(size);
    if (!ptr)
        throw std::bad_alloc{};

    return ptr;
}

void operator delete(void* ptr) noexcept {
    ensure_initialized();
    GC::instance.deallocate(ptr);
}

void* operator new[](std::size_t size) {
    if (size == 0)
        size = 1;
    ensure_initialized();
    void* ptr = GC::instance.allocate(size);
    if (!ptr)
        throw std::bad_alloc{};

    return ptr;
}

void operator delete[](void* ptr) noexcept {
    ensure_initialized();
    GC::instance.deallocate(ptr);
}

void* operator new(std::size_t size, [[maybe_unused]] const std::nothrow_t& nothrw) noexcept {
    if (size == 0)
        size = 1;
    ensure_initialized();
    void* ptr = GC::instance.allocate(size);
    if (!ptr)
        return nullptr;

    return ptr;
}

void* operator new[](std::size_t size, [[maybe_unused]] const std::nothrow_t& nothrw) noexcept {
    if (size == 0)
        size = 1;
    ensure_initialized();
    void* ptr = GC::instance.allocate(size);
    if (!ptr)
        return nullptr;

    return ptr;
}

#define GET_REG(name)                                   \
    ({                                                  \
        size_t v;                                       \
        asm volatile("movq %%" #name ", %0" : "=r"(v)); \
        v;                                              \
    })

template <typename ItOut>
ItOut GC::GCollector::GC_scan_registers(ItOut stack) {
    uintptr RBP = GET_REG(rbp);
    uintptr RBX = GET_REG(rbx);
    uintptr RSP = GET_REG(rsp);
    uintptr R12 = GET_REG(r12);
    uintptr R13 = GET_REG(r13);
    uintptr R14 = GET_REG(r14);
    uintptr R15 = GET_REG(r15);

    uintptr RAX = GET_REG(rax);
    uintptr RCX = GET_REG(rcx);
    uintptr RDX = GET_REG(rdx);
    uintptr RSI = GET_REG(rsi);
    uintptr RDI = GET_REG(rdi);
    uintptr R8 = GET_REG(r8);
    uintptr R9 = GET_REG(r9);
    uintptr R10 = GET_REG(r10);
    uintptr R11 = GET_REG(r11);

    *(stack++) = (void*)RBP;
    *(stack++) = (void*)RBX;
    *(stack++) = (void*)RSP;
    *(stack++) = (void*)R12;
    *(stack++) = (void*)R13;
    *(stack++) = (void*)R14;
    *(stack++) = (void*)R15;

    // *(stack++) = (void*)RAX;
    // *(stack++) = (void*)RCX;
    // *(stack++) = (void*)RDX;
    // *(stack++) = (void*)RSI;
    // *(stack++) = (void*)RDI;
    // *(stack++) = (void*)R8;
    // *(stack++) = (void*)R9;
    // *(stack++) = (void*)R10;
    // *(stack++) = (void*)R11;
    return stack;
}

template <typename ItOut>
ItOut GC::GCollector::GC_scan_stack(ItOut stack) {
    // void* stack_end = (void*)GET_REG(rsp);
    void* stack_end = __builtin_frame_address(0);
    void** array = static_cast<void**>(stack_begin);

    for (; array != stack_end; --array) {
        *(stack++) = *array;
    }

    return stack;
}

void GC::GCollector::memory_dfs(m_stack& stack) {
    while (!stack.empty()) {
        void* current = stack.back();
        stack.pop_back();

        if (allocator.allocated.find(current) == allocator.allocated.end()) {
            continue;
        }
        printf("found %p\n", current);
        reachable[((char*)current - (char*)allocator.heap) / GC::Alloc::MIN_SIZE] = true;
        size_t block_size = allocator.allocated[current] / sizeof(void*);
        void** block = (void**)(*(void**)current);
        for (size_t i = 0; i < block_size; ++i) {
            stack.push_back(block + i);
        }
    }
}

void GC::GCollector::GC_sweep() {
    auto it = allocator.allocated.begin();

    while (it != allocator.allocated.end()) {
        if (!reachable[((char*)it->first - (char*)allocator.heap) / GC::Alloc::MIN_SIZE]) {
            it = allocator.deallocate(it->first);
        } else {
            it++;
        }
    }
}

void GC::GCollector::GC_scan() {
    m_stack to_mark;
    auto out = GC_scan_registers(std::back_inserter(to_mark));
    GC_scan_stack(out);
    memory_dfs(to_mark);
    GC_sweep();
    reachable.reset();
}

void* GC::GCollector::allocate(size_t size) {
    allocations_count++;
    if (allocations_count) {
        printf("scan start\n");
        GC_scan();
    }
    return allocator.allocate(size);
}

void GC::GCollector::deallocate(void* ptr) {
    allocator.deallocate(ptr);
}

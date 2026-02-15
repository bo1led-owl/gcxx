#include <setjmp.h>
#include <bitset>
#include <stack>
#include <deque>
#include <concepts>


#include "alloc.hpp"
#include "mallocator.hpp"

#define HEAP_SIZE 1 << 30;
#define BITSET_SIZE static_cast<size_t>(HEAP_SIZE / GC::Alloc::MIN_SIZE);

#define GET_REG(name) \
    ({ uint64_t v; asm volatile ("movq %%" #name ", %0" : "=r"(v)); v; })

namespace GC {
  class GCollector {
    using uintptr = unsigned long;
    using m_stack = std::stack<void*, std::deque<void*, MLC::Alloc<void*>>>;

    GC::Alloc allocator;
    std::bitset<BITSET_SIZE> reachable;
    void* stack_begin;
    size_t allocations_count;

    template<std::output_iterator ItOut>
    ItOut GC_scan_registers(ItOut stack) {
      uintptr RBP = GET_REG(rbp);
      uintptr RBX = GET_REG(rbx);
      uintptr RSP = GET_REG(rsp);
      uintptr R12 = GET_REG(r12);
      uintptr R13 = GET_REG(r13);
      uintptr R14 = GET_REG(r14);
      uintptr R15 = GET_REG(r15);


      *(stack++) = RBP;
      *(stack++) = RBX;
      *(stack++) = RSP;
      *(stack++) = R12;
      *(stack++) = R13;
      *(stack++) = R14;
      *(stack++) = R15;
      return stack;
    }

    template <std::output_iterator ItOut>
    ItOut GC_scan_stack(ItOut stack) {
      void* stack_end = __builtin_frame_address(0);
      void** array = static_cast<void**>(stack_begin);

      for(; array != stack_end; ++array) {
        *(stack++) = array;
      }

      return stack;
    }

    void memory_dfs(m_stack& stack) {
      while(!stack.empty()) {
        void* current = stack.top();
        stack.pop();

        if (!allocator.allocated.find(current)) { continue; }
        reachable[(current - allocator.heap) / GC::Alloc::MIN_SIZE] = true;
        size_t block_size = allocator.allocated[current];
        void** block = (void**) (*(void**)current);
        for (size_t i = 0; i < block_size; ++i) {
          stack.push(block + i);
        }
      }
    }

    void GC_sweep() {
      for (auto&& [ptr, _] : allocator.allocated) {
        if (!reachable[(ptr - allocator.heap) / GC::Alloc::MIN_SIZE]) { allocator.deallocate(ptr); } 
      }
    }

    void GC_scan() {
      m_stack to_mark;
      auto out = GC_scan_registers(std::back_inserter(to_mark));
      GC_scan_stack(out);
      memory_dfs(to_mark);

    }

    public:

    GC() : allocator(HEAP_SIZE), allocations_count(0) {
      stack_begin = __builtin_frame_address(0);
    }


    void* allocate(size_t size) {
      return allocator.allocate(size);
    }

    void deallocate(void* ptr) {
      allocator.deallocate(ptr);
    }

  }


  static GC instance;

} // namespace GarbageCollector

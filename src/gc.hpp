#include <vector>
#include <bitset>

#include "mallocator.hpp"
#include "alloc.hpp"

#define HEAP_SIZE 1 << 30
#define BITSET_SIZE static_cast<size_t>(HEAP_SIZE / GC::Alloc::MIN_SIZE)

namespace GC {
  class GCollector {
    using uintptr = unsigned long;
    using uint = unsigned int;
    using m_stack = std::vector<void*, MLC::Alloc<void*>>;

    GC::Alloc allocator;
    std::bitset<BITSET_SIZE> reachable;
    void* stack_begin;
    uint allocations_count;

    template<typename ItOut>
    ItOut GC_scan_registers(ItOut stack);

    template <typename ItOut>
    ItOut GC_scan_stack(ItOut stack);

    void memory_dfs(m_stack& stack);

    void GC_sweep();

    void GC_scan();

    public:

    GCollector() : allocator(HEAP_SIZE), allocations_count(0) {
      stack_begin = __builtin_frame_address(0);
    }


    void* allocate(size_t size);

    void deallocate(void* ptr);

  };


  static GCollector instance;

} // namespace GarbageCollector
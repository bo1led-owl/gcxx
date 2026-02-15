#include "gc.hpp"

#include <cstdio>

#define GET_REG(name) \
    ({ size_t v; asm volatile ("movq %%" #name ", %0" : "=r"(v)); v; })

template<typename ItOut>
ItOut GC::GCollector::GC_scan_registers(ItOut stack) {
  uintptr RBP = GET_REG(rbp);
  uintptr RBX = GET_REG(rbx);
  uintptr RSP = GET_REG(rsp);
  uintptr R12 = GET_REG(r12);
  uintptr R13 = GET_REG(r13);
  uintptr R14 = GET_REG(r14);
  uintptr R15 = GET_REG(r15);


  *(stack++) = (void*)RBP;
  *(stack++) = (void*)RBX;
  *(stack++) = (void*)RSP;
  *(stack++) = (void*)R12;
  *(stack++) = (void*)R13;
  *(stack++) = (void*)R14;
  *(stack++) = (void*)R15;
  return stack;
}

template <typename ItOut>
ItOut GC::GCollector::GC_scan_stack(ItOut stack) {
  void* stack_end = __builtin_frame_address(0);
  void** array = static_cast<void**>(stack_begin);

  for(; array != stack_end; --array) {
    *(stack++) = *array;
  }

  return stack;
}

void GC::GCollector::memory_dfs(m_stack& stack) {
  while(!stack.empty()) {
    void* current = stack.back();
    stack.pop_back();

    if (allocator.allocated.find(current) == allocator.allocated.end()) { continue; }
    printf("found %p", current);
    reachable[((char*)current - (char*)allocator.heap) / GC::Alloc::MIN_SIZE] = true;
    size_t block_size = allocator.allocated[current];
    void** block = (void**) (*(void**)current);
    for (size_t i = 0; i < block_size; ++i) {
      stack.push_back(block + i);
    }
  }
}

void GC::GCollector::GC_sweep() {
  for (auto&& [ptr, _] : allocator.allocated) {
    if (!reachable[((char*)ptr - (char*)allocator.heap) / GC::Alloc::MIN_SIZE]) { allocator.deallocate(ptr); } 
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

void* GC::GCollector::allocate(size_t size)  {
  allocations_count++;
  if (allocations_count) { 
    printf("scan start");
    GC_scan(); 
  }
  return allocator.allocate(size);
}

void GC::GCollector::deallocate(void* ptr) {
  allocator.deallocate(ptr);
}
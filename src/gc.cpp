#include <setjmp.h>
#include <string_view>
#include <array>

#define GET_REG(name) \
    ({ uint64_t v; asm volatile ("movq %%" #name ", %0" : "=r"(v)); v; })

namespace GarbageCollector {

class GC {
  using uintptr = unsigned long;
  void* stack_begin;


  bool GC_our_pointer(void* ptr) {}

  void GC_scan_registers() {
    uintptr RBP = GET_REG(rbp);
    uintptr RBX = GET_REG(rbx);
    uintptr RSP = GET_REG(rsp);
    uintptr R12 = GET_REG(r12);
    uintptr R13 = GET_REG(r13);
    uintptr R14 = GET_REG(r14);
    uintptr R15 = GET_REG(r15);


  }

  void GC_scan_stack() {
    void* stack_end = __builtin_frame_address(0);
    void** array = static_cast<void**>(stack_begin);

    for(; *array != stack_end; ++array) {
      if (GC_our_pointer(*array)) { /*WIP*/ }
    }
  }

  public:

  GC() {
    stack_begin = __builtin_frame_address(0);
  }


}


static GC instance;

} // namespace GarbageCollector

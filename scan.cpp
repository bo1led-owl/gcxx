static void* stack_begin = nullptr;
__attribute__((constructor)) void init() {
  stack_begin = __builtin_frame_address(0);
}
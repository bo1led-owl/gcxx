#include "gc.hpp"

#include <stdio.h>

void* operator new(std::size_t size) {
  if (size == 0) size = 1;
  void* ptr = GC::instance.allocate(size);
  if (!ptr) throw std::bad_alloc{};

  return ptr;
}

void operator delete(void* ptr) {
  GC::instance.deallocate(ptr);
}

void* operator new[](std::size_t size) {
  if (size == 0) size = 1;
  void* ptr = GC::instance.allocate(size);
  if (!ptr) throw std::bad_alloc{};

  return ptr;
}

void operator delete[](void* ptr) {
  GC::instance.deallocate(ptr);
}

void* operator new(std::size_t size, const std::nothrow_t& nothrw) noexcept {
  if (size == 0) size = 1;
  void* ptr = GC::instance.allocate(size);
  if (!ptr) return nullptr;

  return ptr;
}

void* operator new[](std::size_t size, const std::nothrow_t& nothrw) noexcept {
  if (size == 0) size = 1;
  void* ptr = GC::instance.allocate(size);
  if (!ptr) return nullptr;

  return ptr;
}
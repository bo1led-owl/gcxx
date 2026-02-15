#include "gc.hpp"

void ensure_initialized();
void* operator new(std::size_t size);
void operator delete(void* ptr) noexcept;
void* operator new[](std::size_t size);
void operator delete[](void* ptr) noexcept;
void* operator new(std::size_t size, const std::nothrow_t& nothrw) noexcept;
void* operator new[](std::size_t size, const std::nothrow_t& nothrw) noexcept;

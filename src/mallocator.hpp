#pragma once

#include <cstddef>
#include <cstdlib>

namespace MLC {

template <typename T>
struct Alloc {
    using pointer = T*;
    using const_pointer = const T*;
    using void_pointer = void*;
    using const_void_pointer = const void*;
    using value_type = T;
    using difference_type = ptrdiff_t;
    using size_type = size_t;

    Alloc() noexcept = default;

    [[nodiscard]] pointer allocate(size_type n) {
        return std::malloc(n * sizeof(value_type));
    }

    void deallocate(pointer p, [[maybe_unused]] size_type n) {
        std::free(p);
    }

    template <class U>
    bool operator==(Alloc<U>&) {
        return true;
    }
};

}  // namespace MLC

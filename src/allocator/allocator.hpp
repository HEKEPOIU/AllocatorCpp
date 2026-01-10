#pragma once

#include "Define.hpp"
#include "base/slice.hpp"

#define DEFAULT_ALIGNMENT (2 * sizeof(void *))

enum class Allocator_Mode : u8 {
  Alloc,
  Free,
  Free_All,
  Resize,
  Alloc_Non_Zeroed,
  Resize_Non_Zeroed,
};

// enum class Allocator_Error : u8 {
//     None = 0,
//     Out_Of_Memory = 1,
//     Invalid_Pointer = 2,
//     Invalid_Argument = 3,
//     Mode_Not_Implemented = 4,
// };

// INFO:
// The Function don't return the allocator Error for now.
// Problem 1: We nothing to do with memory allocate fail.
// Problem 2: The Error value have multiple way to return
//            1. std::except<T, E> -> This depened on std.
//            2. use golang like multiple return value -> bad for c++ some RAII
//            3. implement std::except<T, E> for my own version -> Currently
//            this project are for the allocator pratice so not now.
using Alloc_Func = Slice<u8> (*)(void *allocator_data, Allocator_Mode mode,
                                 size_t size, void *old_memory, size_t old_size,
                                 Source_Location location, size_t alignment);

struct Allocator {
  Alloc_Func alloc_func;
  void *data;
};

constexpr inline bool is_power_of_two(uintptr_t x) {
  return (x & (x - 1)) == 0;
}

constexpr inline uintptr_t align_forward(uintptr_t ptr, size_t align) {
  uintptr_t p, a, modulo;
  assert(is_power_of_two(align));

  p = ptr;
  a = static_cast<uintptr_t>(align);

  // (p % a)
  modulo = p & (a - 1);

  if (modulo != 0) {
    p += (a - modulo);
  }

  return p;
}

constexpr inline bool is_aligned(uintptr_t ptr, uintptr_t align) {
  assert(is_power_of_two(align));
  return (ptr & (align - 1)) == 0;
}

#define alloc_without_type(allocator, size)                                    \
  allocator.alloc_func(allocator.data, Allocator_Mode::Alloc, size, nullptr,   \
                       0, CALLER_LOC, DEFAULT_ALIGNMENT)
#define alloc_with_type(allocator, size, type)                                 \
  alloc_without_type(allocator, size * sizeof(type)).as<type>()

#define alloc(...)                                                             \
  SELECTER3(__VA_ARGS__, alloc_with_type, alloc_without_type)(__VA_ARGS__)

#define alloc_none_zero_without_type(allocator, size)                          \
  allocator.alloc_func(allocator.data, Allocator_Mode::Alloc_Non_Zeroed, size, \
                       nullptr, 0, CALLER_LOC, DEFAULT_ALIGNMENT)

#define alloc_none_zero_with_type(allocator, size, type)                       \
  alloc_none_zero_without_type(allocator, size * sizeof(type)).as<type>()

#define alloc_none_zero(...)                                                   \
  SELECTER3(__VA_ARGS__, alloc_none_zero_with_type,                            \
            alloc_none_zero_without_type)                                      \
  (__VA_ARGS__)

#define free_all(allocator)                                                    \
  allocator.alloc_func(allocator.data, Allocator_Mode::Free_All, 0, nullptr,   \
                       0, CALLER_LOC, DEFAULT_ALIGNMENT)

#define free(allocator, ptr)                                                   \
  allocator.alloc_func(allocator.data, Allocator_Mode::Free, 0, ptr, 0,        \
                       CALLER_LOC, DEFAULT_ALIGNMENT)

#define resize(allocator, old_mem, old_size, new_size, type)                   \
  allocator                                                                    \
      .alloc_func(allocator.data, Allocator_Mode::Resize, new_size * sizeof(type), old_mem,   \
                  old_size * sizeof(type), CALLER_LOC, DEFAULT_ALIGNMENT)                     \
      .as<type>()

#define resize_none_zero(allocator, old_mem, old_size, new_size, type)         \
  allocator                                                                    \
      .alloc_func(allocator.data, Allocator_Mode::Resize_Non_Zeroed, new_size * sizeof(type), \
                  old_mem * sizeof(type), old_size, CALLER_LOC, DEFAULT_ALIGNMENT)            \
      .as<type>()
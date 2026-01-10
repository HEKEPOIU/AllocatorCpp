#pragma once

#include "Define.hpp"

// Only used of array
template <typename T> class Slice {
public:
  Slice() : start(nullptr), last(nullptr) {};
  Slice(T *base, size_t size) : start(base), last(base + size) {}
  Slice(T *_start, T *_end) : start(_start), last(_end) {}
  Slice(T *base, size_t start_index, size_t end_index)
      : start(base + start_index), last(base + end_index) {}

  template <typename T2> Slice<T2> as() const {
    auto byte_len = len() * sizeof(T);
    return Slice<T2>(reinterpret_cast<T2 *>(start), byte_len / sizeof(T2));
  }

  T &operator[](size_t index) {
	  ensure(index < len(), "Slice out of bound");
	  return start[index];
  }
  const T &operator[](size_t index) const {
	  ensure(index < len(), "Slice out of bound");
	  return start[index]; 
  }

  operator T *() { return start; }
  operator const T *() const { return start; }

  explicit operator bool() const { return !empty(); }
  inline size_t len() const { return last - start; };

  bool empty() { return start == last; }
  T *begin() { return start; }
  T *end() { return last; }

  const T *begin() const { return start; }
  const T *end() const { return last; }

private:
  T *start;
  T *last;
};

inline Slice<u8> byte_slice(void *ptr, size_t len) {
  return Slice<u8>{reinterpret_cast<u8 *>(ptr), len};
}
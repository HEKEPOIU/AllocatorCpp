#pragma once
#include <format>
#include <print>
#include <stdint.h>

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using b32 = i32;
using b8 = i8;

#define KiB(n) ((u64)(n) << 10)
#define MiB(n) ((u64)(n) << 20)
#define GiB(n) ((u64)(n) << 30)

#define SELECTER(_1, _2, NAME, ...) NAME

#if defined(NDEBUG)
#define assert_with_msg(exp, message)
#define assert_without_msg(exp, message)
#else
#define assert_with_msg(exp, message)                                          \
  do {                                                                         \
    if (!(exp)) {                                                              \
      std::print("{} assert faild, {} \n", CALLER_LOC, message);               \
      __builtin_trap();                                                        \
    }                                                                          \
  } while (0)
#define assert_without_msg(exp)                                                \
  do {                                                                         \
    if (!(exp)) {                                                              \
      std::print("{} assert faild\n", CALLER_LOC);                             \
      __builtin_trap();                                                        \
    }                                                                          \
  } while (0)
#endif

#define assert(...)                                                            \
  SELECTER(__VA_ARGS__, assert_with_msg, assert_without_msg)(__VA_ARGS__)

#define panic_with_msg(exp, msg)                                               \
  do {                                                                         \
    if (!(exp)) {                                                              \
      std::print("{} panic, {}\n", CALLER_LOC, msg);                           \
      __builtin_trap();                                                        \
    }                                                                          \
  } while (0)

#define panic_without_msg(exp)                                                 \
  do {                                                                         \
    if (!(exp)) {                                                              \
      std::print("{} panic\n", CALLER_LOC);                                    \
      __builtin_trap();                                                        \
    }                                                                          \
  } while (0)

#define panic(...)                                                             \
  SELECTER(__VA_ARGS__, panic_with_msg, panic_without_msg)(__VA_ARGS__)

template <typename T, typename N> auto min(T a, N b) -> decltype(a + b) {
  return (a > b) ? b : a;
}

template <typename T, typename N> auto max(T a, N b) -> decltype(a + b) {
  return (a < b) ? b : a;
}

struct Source_Location {
  const char *file;
  const char *func;
  u32 line;
};

template <> struct std::formatter<Source_Location> {
  constexpr auto parse(std::format_parse_context &ctx) { return ctx.begin(); }

  auto format(const Source_Location &v, std::format_context &ctx) const {
    return std::format_to(ctx.out(), "[{}:{}:{}]", v.file, v.line, v.func);
  }
};

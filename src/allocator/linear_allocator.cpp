#include "allocator/linear_allocator.hpp"
#include <string.h>

Allocator create_arena_allocator(Arena *arena) {
  return {
      .alloc_func = arena_allocator_func,
      .data = static_cast<void *>(arena),
  };
};

Arena init_arena(u8 *data, size_t size) {
  return {
      .buf = data,
      .buf_len = size,
  };
};

Arena_Snapshot take_snapshot(Arena& arena){
	return {
		.arena = arena,
		.curr_offset = arena.curr_offset,
		.prev_offset = arena.prev_offset,
	};
};
void restore_snapshot(Arena_Snapshot& snapshot) {
	snapshot.arena.curr_offset = snapshot.curr_offset;
	snapshot.arena.prev_offset = snapshot.prev_offset;
};

void *arena_alloc(Arena *arena, size_t size, size_t alignment,
                  Source_Location loc, b32 zero_mem = true) {
  auto curr_ptr = reinterpret_cast<uintptr_t>(arena->buf) +
                  reinterpret_cast<uintptr_t>(arena->curr_offset);
  auto offset = align_forward(curr_ptr, alignment);
  offset -= reinterpret_cast<uintptr_t>(arena->buf);

  // Out of arena.
  if (offset + size > arena->buf_len) {
    assert(false, "Allocation error arena out of memory");
    return nullptr;
  }
  auto ptr = &arena->buf[offset];
  arena->prev_offset = offset;
  arena->curr_offset = offset + size;

  if (zero_mem)
    memset(ptr, 0, size);
  return ptr;
}

void *arena_resize(Arena *arena, size_t size, size_t old_size, void *old_memory,
                   size_t alignment, Source_Location location,
                   bool zero_mem = true) {
  auto old_mem = reinterpret_cast<u8 *>(old_memory);
  if (old_mem == nullptr || old_size == 0) {
    return arena_alloc(arena, size, alignment, location, zero_mem);
  } else if (old_mem >= arena->buf && old_mem < arena->buf + arena->buf_len) {
    if (old_mem == arena->buf + arena->prev_offset &&
        is_aligned(reinterpret_cast<uintptr_t>(old_mem), alignment)) {
      arena->curr_offset = arena->prev_offset + size;
      if (arena->curr_offset > arena->buf_len) {
        assert(false, "Resize error arena out of memory");
        return nullptr;
      }
      if (old_size < size) {
        if (zero_mem)
          memset(&arena->buf[arena->prev_offset + old_size], 0,
                 size - old_size);
      }
      return old_memory;
    } else {
      void *new_memory =
          arena_alloc(arena, size, alignment, location, zero_mem);
      if (!new_memory)
        return nullptr;
      size_t copy_size = old_size > size ? size : old_size;
      memmove(new_memory, old_memory, copy_size);
      return new_memory;
    }
  } else {
    assert(false, "Old Memory not belong to this arena!");
    return nullptr;
  }

  return nullptr;
}

void *arena_allocator_func(void *allocator_data, Allocator_Mode mode,
                           size_t size, void *old_memory, size_t old_size,
                           Source_Location location,
                           size_t alignment = DEFAULT_ALIGNMENT) {
  Arena *arena = static_cast<Arena *>(allocator_data);
  switch (mode) {
  case Allocator_Mode::Alloc: {
    return arena_alloc(arena, size, alignment, location);
  } break;
  case Allocator_Mode::Free: {
    panic(!"Arena allocator not support free singal memory block");
  } break;
  case Allocator_Mode::Free_All: {
    arena->curr_offset = 0;
    arena->prev_offset = 0;
  } break;
  case Allocator_Mode::Resize: {
    return arena_resize(arena, size, old_size, old_memory, alignment, location);
  } break;
  case Allocator_Mode::Alloc_Non_Zeroed: {
    return arena_alloc(arena, size, alignment, location, false);
  } break;
  case Allocator_Mode::Resize_Non_Zeroed: {
    return arena_resize(arena, size, old_size, old_memory, alignment, location,
                        false);
  } break;
  }
  return nullptr;
};
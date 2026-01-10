#pragma once

#include "allocator/allocator.hpp"

struct Arena {
  u8 *buf;
  size_t buf_len;
  size_t curr_offset;
  size_t prev_offset;
};

struct Arena_Snapshot {
	Arena& arena;
  	size_t curr_offset;
  	size_t prev_offset;
};

Arena_Snapshot take_snapshot(Arena& arena);
void restore_snapshot(Arena_Snapshot& snapshot);

Allocator create_arena_allocator(Arena* arena);

Arena init_arena(Slice<u8>);

Slice<u8> arena_allocator_func(void *allocator_data, Allocator_Mode mode,
                           size_t size, void *old_memory, size_t old_size,
                           Source_Location location, size_t alignment);

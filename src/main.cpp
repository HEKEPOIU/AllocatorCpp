#include "Define.hpp"
#include "allocator/allocator.hpp"
#include "allocator/linear_allocator.cpp"
#include <print>

#define BUFFER_SIZE 1024

int main() {
	std::print("start testing the Arena allocator.");
	//u8 buffer[BUFFER_SIZE];
	// For small program, we should not free the malloc thing.
	u8* buffer = reinterpret_cast<u8*>(malloc(BUFFER_SIZE));
	auto arena = init_arena(buffer, BUFFER_SIZE);
	auto allocator = create_arena_allocator(&arena);

	auto ptr1 = alloc(allocator, 64);
	auto ptr2 = alloc(allocator, 100);

	assert(ptr1, "Ptr1 not exist");
	assert(ptr2, "Ptr2 not exist");

	std::print("Current arena memory used case: {}\n", arena.curr_offset);

	ptr1 = resize(allocator, ptr1, 64, 200);


	std::print("Current arena memory used case: {}\n", arena.curr_offset);
	assert(arena.curr_offset == (364 + (364 % DEFAULT_ALIGNMENT)), "Not last alloc arena memory resize should push in back");

	ptr1 = resize(allocator, ptr1, 200, 100);

	std::print("Current arena memory used case: {}\n", arena.curr_offset);
	assert(arena.curr_offset == (364 + (364 % DEFAULT_ALIGNMENT) - 100),
		   "Resize last alloc mem should not move the pointer position");


	ptr1 = resize(allocator, ptr1, 100, 500);

	std::print("Current arena memory used case: {}\n", arena.curr_offset);
	assert(arena.curr_offset == (364 + (364 % DEFAULT_ALIGNMENT) - 100+400),
		   "Resize last alloc mem should not move the pointer position");


	free_all(allocator);
	assert(arena.curr_offset == 0 && arena.prev_offset == 0, "free all should reset all alloc memory");

	auto ptr_all = reinterpret_cast<u8*>(alloc_none_zero(allocator, BUFFER_SIZE));

	for (int i = 0; i < BUFFER_SIZE; i++) {
		std::print("{}", ptr_all[i]);
	}
	std::print("\n\n");

	free_all(allocator);

	auto snapshot = take_snapshot(arena);

	alloc(allocator, BUFFER_SIZE);

	for (int i = 0; i < BUFFER_SIZE; i++) {
		std::print("{}", ptr_all[i]);
	}
	std::print("\n\n");

	assert(arena.curr_offset != 0);
	restore_snapshot(snapshot);

	assert(arena.curr_offset == 0);

	std::print("arena test pass\n");
	
}
#include <catch2/catch_test_macros.hpp>
#include "Define.hpp"
#include "allocator/linear_allocator.hpp"
#include "base/slice.hpp"

#define BUFFER_SIZE 1024

TEST_CASE("linear_allocator test", "linear_allocator") {
	//u8 buffer[BUFFER_SIZE];
	// For small program, we should not free the malloc thing.
	auto buffer = reinterpret_cast<u8*>(malloc(BUFFER_SIZE));
	auto arena = init_arena(Slice{buffer, BUFFER_SIZE});
	auto allocator = create_arena_allocator(&arena);

	auto ptr1 = alloc(allocator, 64, u8);
	auto ptr2 = alloc(allocator, 100);

	REQUIRE(ptr1);
	REQUIRE(ptr2);


	ptr1 = resize(allocator, ptr1, 64, 200, u8);


	REQUIRE((arena.curr_offset == (364 + (364 % DEFAULT_ALIGNMENT))));

	ptr1 = resize(allocator, ptr1, 200, 100, u8);

	REQUIRE((arena.curr_offset == (364 + (364 % DEFAULT_ALIGNMENT) - 100)));


	ptr1 = resize(allocator, ptr1, 100, 500, u8);

	REQUIRE((arena.curr_offset == (364 + (364 % DEFAULT_ALIGNMENT) - 100+400)));


	free_all(allocator);
	REQUIRE((arena.curr_offset == 0 && arena.prev_offset == 0));

	auto snapshot = take_snapshot(arena);

	alloc(allocator, BUFFER_SIZE);

	REQUIRE(arena.curr_offset != 0);
	restore_snapshot(snapshot);

	REQUIRE(arena.curr_offset == 0);

};

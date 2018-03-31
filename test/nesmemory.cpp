#include "catch.hpp"

#include "../src/nesmemory.hpp"

TEST_CASE("NES Memory Read/Write", "[NESMemory]")
{
	NESMemory mem;
	mem.memWrite(0x0001, 10);
	REQUIRE(mem.memRead(0x0001) == 10);
	mem.memWrite(0x0810, 21);
	REQUIRE(mem.memRead(0x0010) == 21);
}

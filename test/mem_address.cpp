#include "catch.hpp"

#include "../src/memaddress.hpp"

TEST_CASE("MemAddress initializations", "[MemAddress]")
{
	MemAddress addr;
	REQUIRE(addr.value == 0);

	MemAddress addr2{5, 11};
	REQUIRE(addr2.low() == 5);
	REQUIRE(addr2.high() == 11);
	REQUIRE(addr2.value == 2821);

	MemAddress addr3{2821};
	REQUIRE(addr3.low() == 5);
	REQUIRE(addr3.high() == 11);
	REQUIRE(addr3.value == 2821);

	MemAddress addr4 = 258;
	REQUIRE(addr4.high() == 1);
	REQUIRE(addr4.low() == 2);
	REQUIRE(addr4.value == 258);
}

TEST_CASE("MemAddress assignments", "[MemAddress]")
{
	MemAddress addr;
	addr.low() = 5;
	addr.high() = 11;
	REQUIRE(addr.low() == 5);
	REQUIRE(addr.high() == 11);
	REQUIRE(addr.value == 2821);
}

TEST_CASE("MemAddress math", "[MemAddress]")
{
	MemAddress addr{250, 0};
	addr.low() += 10;
	REQUIRE(addr.low() == 4);
	REQUIRE(addr.value == 4);
	REQUIRE(addr.high() == 0);

	addr.value += 255;
	REQUIRE(addr.low() == 3);
	REQUIRE(addr.high() == 1);

	addr = 1;
	MemAddress addr2{4, 0};
	MemAddress addr3 = addr + addr2;
	REQUIRE(addr3.value == 5);
	MemAddress addr4 = addr3 + 90;
	REQUIRE(addr4.value == 95);
	REQUIRE(addr4.addLow(161) == 0);

	addr4.high() += 1;
	REQUIRE(addr4.value == 256);
	addr4.low() += 1;
	REQUIRE(addr4.value == 257);
}

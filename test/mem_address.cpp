#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "../src/mem_address.hpp"

TEST_CASE("mem_address initializations", "[mem_address]")
{
	mem_address addr;
	REQUIRE(addr.value == 0);

	mem_address addr2{5, 11};
	REQUIRE(addr2.low() == 5);
	REQUIRE(addr2.high() == 11);
	REQUIRE(addr2.value == 2821);

	mem_address addr3{2821};
	REQUIRE(addr3.low() == 5);
	REQUIRE(addr3.high() == 11);
	REQUIRE(addr3.value == 2821);
}

TEST_CASE("mem_address assignments", "[mem_address]")
{
	mem_address addr;
	addr.low() = 5;
	addr.high() = 11;
	REQUIRE(addr.low() == 5);
	REQUIRE(addr.high() == 11);
	REQUIRE(addr.value == 2821);
}

TEST_CASE("mem_address math", "[mem_address]")
{
	mem_address addr{250, 0};
	addr.low() += 10;
	REQUIRE(addr.low() == 4);
	REQUIRE(addr.value == 4);
	REQUIRE(addr.high() == 0);

	addr.value += 255;
	REQUIRE(addr.low() == 3);
	REQUIRE(addr.high() == 1);

	addr = 1;
	mem_address addr2{4, 0};
	mem_address addr3 = addr + addr2;
	REQUIRE(addr3.value == 5);
	mem_address addr4 = addr3 + 90;
	REQUIRE(addr4.value == 95);

	addr4.low() += 161;
	REQUIRE(addr4.value == 0);

	addr4.high() += 1;
	REQUIRE(addr4.value == 256);
	addr4.low() += 1;
	REQUIRE(addr4.value == 257);
}

#ifndef MEMADDRESS_H
#define MEMADDRESS_H

#include "common.hpp"

union mem_address
{
	byte bytes[2];
	uint16_t value;

	mem_address(byte low, byte high)
	{
		bytes[0] = low;
		bytes[1] = high;
	}
	mem_address(uint16_t value) : value(value) { }
	mem_address() : value(0) { }

	byte low() const { return bytes[0]; }
	byte high() const { return bytes[1]; }

	mem_address &operator=(uint16_t value)
	{
		this->value = value;
		return *this;
	}

	bool operator<=(const uint16_t &value) const
	{
		return this->value <= value;
	}

	mem_address operator+(int) const
	{
		mem_address addr = *this;
		addr.value += value;
		return addr;
	}

	mem_address operator+(byte value) const
	{
		mem_address addr = *this;
		addr.value += value;
		return addr;
	}

	mem_address &operator+=(byte value)
	{
		this->bytes[0] += value;
		return *this;
	}

	mem_address &operator+=(uint16_t value)
	{
		this->value += value;
		return *this;
	}

	bool operator<(const mem_address &rhs) const
	{
		return this->value < rhs.value;
	}
};

#endif /* MEMADDRESS_H */

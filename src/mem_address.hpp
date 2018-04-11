#ifndef MEMADDRESS_H
#define MEMADDRESS_H

#include "common.hpp"

union mem_address
{
	uint16_t value;
	byte bytes[2];

	mem_address(byte low, byte high)
	{
		bytes[0] = low;
		bytes[1] = high;
	}
	mem_address(uint16_t value) : value(value) { }
	mem_address() : value(0) { }

	byte &low() { return bytes[0]; }
	byte &high() { return bytes[1]; }


	mem_address &operator++()
	{
		++value;
		return *this;
	}
	mem_address &operator=(const mem_address &rhs)
	{
		value = rhs.value;
		return *this;
	}
	mem_address &operator=(uint16_t value)
	{
		this->value = value;
		return *this;
	}
	bool operator<=(const uint16_t &value) const
	{
		return this->value <= value;
	}
	bool operator>=(const uint16_t &value) const
	{
		return this->value >= value;
	}
	bool operator<(const mem_address &rhs) const
	{
		return this->value < rhs.value;
	}

	mem_address operator+(const mem_address &rhs) const
	{
		mem_address addr = *this;
		addr.value += rhs.value;
		return addr;
	}
	mem_address operator+(uint16_t value) const
	{
		mem_address addr = *this;
		addr.value += value;
		return addr;
	}
	mem_address operator-(uint16_t value) const
	{
		mem_address addr = *this;
		addr.value -= value;
		return addr;
	}
	mem_address &operator+=(uint16_t value)
	{
		this->value += value;
		return *this;
	}
	mem_address operator&(uint16_t value) const
	{
		mem_address addr = *this;
		addr.value &= value;
		return addr;
	}
};

#endif /* MEMADDRESS_H */

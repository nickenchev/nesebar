#ifndef MEMADDRESS_H
#define MEMADDRESS_H

#include "common.hpp"

union MemAddress
{
	uint16_t value;
	byte bytes[2];

	MemAddress() : value(0) { }
	MemAddress(byte low, byte high)
	{
		bytes[0] = low;
		bytes[1] = high;
	}
	MemAddress(uint16_t value)
	{
		this->value = value;
	}
	MemAddress(const MemAddress &addr) : MemAddress(addr.value) { }

	byte &low() { return bytes[0]; }
	byte &high() { return bytes[1]; }

	MemAddress &addLow(byte lowInc)
	{
		bytes[0] += lowInc;
		return *this;
	}
	MemAddress &addHigh(byte highInc)
	{
		bytes[1] += highInc;
		return *this;
	}

	bool add(byte inc)
	{
		byte page = high();
		value += inc;
		return high() != page;
	}
	bool addSigned(signed_byte inc)
	{
		byte page = high();
		value += inc;
		return high() != page;
	}

	MemAddress operator%(const uint16_t &value) const
	{
		return MemAddress(this->value % value);
	}
	
	MemAddress &operator++()
	{
		++value;
		return *this;
	}
	MemAddress &operator=(const MemAddress &rhs)
	{
		value = rhs.value;
		return *this;
	}
	MemAddress &operator=(uint16_t value)
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
	bool operator<(const MemAddress &rhs) const
	{
		return this->value < rhs.value;
	}

	MemAddress operator+(const MemAddress &rhs) const
	{
		MemAddress addr = *this;
		addr.value += rhs.value;
		return addr;
	}
	MemAddress operator+(uint16_t value) const
	{
		MemAddress addr = *this;
		addr.value += value;
		return addr;
	}
	MemAddress operator-(uint16_t value) const
	{
		MemAddress addr = *this;
		addr.value -= value;
		return addr;
	}
	MemAddress &operator+=(uint16_t value)
	{
		this->value += value;
		return *this;
	}
	MemAddress operator&(uint16_t value) const
	{
		MemAddress addr = *this;
		addr.value &= value;
		return addr;
	}
	bool operator==(int value) const
	{
		return this->value == value;
	}
};

#endif /* MEMADDRESS_H */

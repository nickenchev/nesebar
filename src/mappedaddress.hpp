#ifndef MAPPEDADDRESS_H
#define MAPPEDADDRESS_H

#include "memaddress.hpp"

struct MappedAddress
{
	MemAddress address;
	bool readOnly;

	MappedAddress()
	{
		address = 0;
		readOnly = false;
	}
	MappedAddress(const MemAddress &address, bool readOnly = false)
	{
		this->address = address;
		this->readOnly = readOnly;
	}
};

#endif /* MAPPEDADDRESS_H */

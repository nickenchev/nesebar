#ifndef MEMCHUNK_H
#define MEMCHUNK_H

#include <array>
#include "common.hpp"
#include "memaddress.hpp"

template <typename T, unsigned int N>
class MemChunk
{
	std::array<T, N> memory;

public:
	T &operator[](const MemAddress &address)
	{
		return memory[address.value];
	}

	auto &data()
	{
		return memory;
	}
};


#endif /* MEMCHUNK_H */

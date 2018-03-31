#ifndef MEMCHUNK_H
#define MEMCHUNK_H

#include <array>
#include "common.hpp"
#include "mem_address.hpp"

template <typename T, unsigned int N>
class MemChunk
{
	std::array<T, N> memory;

public:
	T &operator[](const mem_address &address)
	{
		return memory[address.value];
	}

	auto &data()
	{
		return memory;
	}
};


#endif /* MEMCHUNK_H */

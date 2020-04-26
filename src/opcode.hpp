#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <string>
#include "common.hpp"

namespace mos6502
{

	using Asm = const std::string;

	template<short Value, short ByteSize, short Cycles, byte FlagsAffected>
	struct Opcode
	{
		static constexpr short value = Value;
		static constexpr short byteSize = ByteSize;
		static constexpr short cycles = Cycles;
		static constexpr byte flagsAffected = FlagsAffected;
	};
};

#endif /* INSTRUCTION_H */

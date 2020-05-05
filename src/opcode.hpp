#ifndef OPCODE_H
#define OPCODE_H

#include <string>
#include "common.hpp"
#include "addressingmode.hpp"

namespace mos6502
{
	using Asm = const std::string;

	template<short Value, Addressing::Mode AddressingMode, short ByteSize, short Cycles, byte AutoFlags, byte ManualFlags=0>
	struct Opcode
	{
		static constexpr Addressing::Mode addressingMode = AddressingMode;
		static constexpr short value = Value;
		static constexpr short byteSize = ByteSize;
		static constexpr short cycles = Cycles;
		static constexpr byte autoFlags = AutoFlags;
		static constexpr byte manualFlags = ManualFlags;
	};
};

#endif /* OPCODE_H */

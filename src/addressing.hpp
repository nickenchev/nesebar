#ifndef ADDRESSMODE_H
#define ADDRESSMODE_H

#include "opcode.hpp"
#include "addressingmode.hpp"

namespace mos6502
{
	namespace Addressing		
	{
		template<short Value, short Size, short Cycles, byte Flags, byte ManualFlags=0>
		struct Implicit : Opcode<Value, Addressing::Mode::Implicit, Size, Cycles, Flags, ManualFlags> { };

		template<short Value, short Size, short Cycles, byte Flags, byte ManualFlags=0>
		struct Accumulator : Opcode<Value, Addressing::Mode::Accumulator, Size, Cycles, Flags, ManualFlags> { };

		template<short Value, short Size, short Cycles, byte Flags, byte ManualFlags=0>
		struct Immediate : Opcode<Value, Addressing::Mode::Immediate, Size, Cycles, Flags, ManualFlags> { };

		template<short Value, short Size, short Cycles, byte Flags, byte ManualFlags=0>
		struct ZeroPage : Opcode<Value, Addressing::Mode::ZeroPage, Size, Cycles, Flags, ManualFlags> { };

		template<short Value, short Size, short Cycles, byte Flags, byte ManualFlags=0>
		struct ZeroPageX : Opcode<Value, Addressing::Mode::ZeroPageX, Size, Cycles, Flags, ManualFlags> { };

		template<short Value, short Size, short Cycles, byte Flags, byte ManualFlags=0>
		struct ZeroPageY : Opcode<Value, Addressing::Mode::ZeroPageY, Size, Cycles, Flags, ManualFlags> { };

		template<short Value, short Size, short Cycles, byte Flags, byte ManualFlags=0>
		struct Relative : Opcode<Value, Addressing::Mode::Relative, Size, Cycles, Flags, ManualFlags> { };

		template<short Value, short Size, short Cycles, byte Flags, byte ManualFlags=0>
		struct Absolute : Opcode<Value, Addressing::Mode::Absolute, Size, Cycles, Flags, ManualFlags> { };

		template<short Value, short Size, short Cycles, byte Flags, byte ManualFlags=0>
		struct AbsoluteX : Opcode<Value, Addressing::Mode::AbsoluteX, Size, Cycles, Flags, ManualFlags> { };

		template<short Value, short Size, short Cycles, byte Flags, byte ManualFlags=0>
		struct AbsoluteY : Opcode<Value, Addressing::Mode::AbsoluteY, Size, Cycles, Flags, ManualFlags> { };

		template<short Value, short Size, short Cycles, byte Flags, byte ManualFlags=0>
		struct Indirect : Opcode<Value, Addressing::Mode::Indirect, Size, Cycles, Flags, ManualFlags> { };

		template<short Value, short Size, short Cycles, byte Flags, byte ManualFlags=0>
		struct IndexedIndirect : Opcode<Value, Addressing::Mode::IndexedIndirect, Size, Cycles, Flags, ManualFlags> { };

		template<short Value, short Size, short Cycles, byte Flags, byte ManualFlags=0>
		struct IndirectIndexed : Opcode<Value, Addressing::Mode::IndirectIndexed, Size, Cycles, Flags, ManualFlags> { };
	}
}

#endif /* ADDRESSMODE_H */

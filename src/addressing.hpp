#ifndef ADDRESSMODE_H
#define ADDRESSMODE_H

#include "opcode.hpp"
#include "addressingmode.hpp"

namespace mos6502
{
	namespace Addressing		
	{
		template<short Value, short Cycles, byte Flags, byte ManualFlags=0>
		struct Implicit : Opcode<Value, Addressing::Mode::Implicit, 1, Cycles, Flags, ManualFlags> { };

		template<short Value, short Cycles, byte Flags, byte ManualFlags=0>
		struct Accumulator : Opcode<Value, Addressing::Mode::Accumulator, 1, Cycles, Flags, ManualFlags> { };

		template<short Value, short Cycles, byte Flags, byte ManualFlags=0>
		struct Immediate : Opcode<Value, Addressing::Mode::Immediate, 2, Cycles, Flags, ManualFlags> { };

		template<short Value, short Cycles, byte Flags, byte ManualFlags=0>
		struct ZeroPage : Opcode<Value, Addressing::Mode::ZeroPage, 2, Cycles, Flags, ManualFlags> { };

		template<short Value, short Cycles, byte Flags, byte ManualFlags=0>
		struct ZeroPageX : Opcode<Value, Addressing::Mode::ZeroPageX, 2, Cycles, Flags, ManualFlags> { };

		template<short Value, short Cycles, byte Flags, byte ManualFlags=0>
		struct ZeroPageY : Opcode<Value, Addressing::Mode::ZeroPageY, 2, Cycles, Flags, ManualFlags> { };

		template<short Value, short Cycles, byte Flags, byte ManualFlags=0>
		struct Relative : Opcode<Value, Addressing::Mode::Relative, 2, Cycles, Flags, ManualFlags> { };

		template<short Value, short Cycles, byte Flags, byte ManualFlags=0>
		struct Absolute : Opcode<Value, Addressing::Mode::Absolute, 3, Cycles, Flags, ManualFlags> { };

		template<short Value, short Cycles, byte Flags, byte ManualFlags=0>
		struct AbsoluteX : Opcode<Value, Addressing::Mode::AbsoluteX, 3, Cycles, Flags, ManualFlags> { };

		template<short Value, short Cycles, byte Flags, byte ManualFlags=0>
		struct AbsoluteY : Opcode<Value, Addressing::Mode::AbsoluteY, 3, Cycles, Flags, ManualFlags> { };

		template<short Value, short Cycles, byte Flags, byte ManualFlags=0>
		struct Indirect : Opcode<Value, Addressing::Mode::Indirect, 3, Cycles, Flags, ManualFlags> { };

		template<short Value, short Cycles, byte Flags, byte ManualFlags=0>
		struct IndexedIndirect : Opcode<Value, Addressing::Mode::IndexedIndirect, 2, Cycles, Flags, ManualFlags> { };

		template<short Value, short Cycles, byte Flags, byte ManualFlags=0>
		struct IndirectIndexed : Opcode<Value, Addressing::Mode::IndirectIndexed, 2, Cycles, Flags, ManualFlags> { };
	}
}

#endif /* ADDRESSMODE_H */

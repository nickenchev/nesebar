#ifndef ADDRESSINGMODE_H
#define ADDRESSINGMODE_H

namespace mos6502
{
	namespace Addressing
	{
		enum class Mode
		{
			Implicit = 0,
			Accumulator = 1,
			Immediate = 2,
			ZeroPage = 3,
			ZeroPageX = 4,
			ZeroPageY = 5,
			Relative = 6,
			Absolute = 7,
			AbsoluteX = 8,
			AbsoluteY = 9,
			Indirect = 10,
			IndexedIndirect = 11,
			IndirectIndexed = 12
		};
	}
}

#endif /* ADDRESSINGMODE_H */

#ifndef STATE6502_H
#define STATE6502_H

#include "common.hpp"

namespace mos6502
{
	struct State
	{
		byte a, x, y, sp, p;
		MemAddress pc;
		short cycles, totalCycles;
		short byteStep;
		byte opcodeResult;

		State()
		{
			p = 0x34;
			a = x = y = 0;
			sp = 0;
			pc = 0;

			cycles = totalCycles = 0;
			byteStep = 0;
		}

		inline void setA(byte value)
		{
			a = value;
			opcodeResult = value;
		}
		inline void setSP(byte value)
		{
			sp = value;
			opcodeResult = value;
		}
		inline void setX(byte value)
		{
			x = value;
			opcodeResult = value;
		}
		inline void setY(byte value)
		{
			y = value;
			opcodeResult = value;
		}
		inline void setP(byte value)
		{
			namespace sb = status_bits;
			constexpr byte retain = sb::E|sb::B; // bits we don't want to affect (5, 4)
			value &= ~retain; // transfer all bits except 5, 4
			p = (p & retain) | value; // set p, keep bits 5, 4 if currently set
		}
	};
}

#endif /* STATE6502_H */

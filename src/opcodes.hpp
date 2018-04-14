#ifndef OPCODES_H
#define OPCODES_H

namespace mos6502
{
	namespace OpCodes
	{
		constexpr byte BRK = 0x00;
		constexpr byte ORA_IMMED = 0x09;
		constexpr byte ORA_ZERO = 0x05;
		constexpr byte ORA_ZERO_X = 0x15;
		constexpr byte ORA_ABS = 0x0d;
		constexpr byte ORA_ABS_X = 0x1d;
		constexpr byte ORA_ABS_Y = 0x19;
		constexpr byte ORA_IND_X = 0x01;
		constexpr byte ORA_IND_Y = 0x11;
		constexpr byte CLD = 0xd8;
		constexpr byte SEI = 0x78;
	};
};

#endif /* OPCODES_H */

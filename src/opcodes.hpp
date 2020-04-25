#ifndef OPCODES_H
#define OPCODES_H

#include <tuple>
#include <map>
#include "instruction.hpp"

namespace mos6502 { namespace OpCodes
{

constexpr auto pair(const Instruction &inst)
{
	return std::make_pair(inst, &inst);
}
// Bit No.       7   6   5   4   3   2   1   0
//               S   V       B   D   I   Z   C
/*
	static constexpr Instruction ADC_IMMED(0x69, "ADC", 2, 2);
	static constexpr Instruction ADC_ZERO(0x65, "ADC", 2, 3);
	static constexpr Instruction ADC_ZERO_X(0x75, "ADC", 2, 4);
	static constexpr Instruction ADC_ABS(0x6d, "ADC", 3, 4);
	static constexpr Instruction ADC_ABS_X(0x7d, "ADC", 3, 4);
	static constexpr Instruction ADC_ABS_Y(0x79, "ADC", 3, 4);
	static constexpr Instruction ADC_IND_X(0x61, "ADC", 2, 6);
	static constexpr Instruction ADC_IND_Y(0x71, "ADC", 2, 5);

	static constexpr Instruction AND_IMMED(0x29, "AND", 2, 2);
	static constexpr Instruction AND_ZERO(0x25, "AND", 2, 3);
	static constexpr Instruction AND_ZERO_X(0x35, "AND", 2, 4);
	static constexpr Instruction AND_ABS(0x2d, "AND", 3, 4);
	static constexpr Instruction AND_ABS_X(0x3d, "AND", 3, 4);
	static constexpr Instruction AND_ABS_Y(0x39, "AND", 3, 4);
	static constexpr Instruction AND_IND_X(0x21, "AND", 2, 6);
	static constexpr Instruction AND_IND_Y(0x31, "AND", 2, 5);

	static constexpr Instruction BRK(0x00, "BRK", 1, 7);

	static constexpr Instruction ORA_IMMED(0x09, "ORA", 2, 2);
	static constexpr Instruction ORA_ZERO(0x05, "ORA", 2, 3);
	static constexpr Instruction ORA_ZERO_X(0x15, "ORA", 2, 4);
	static constexpr Instruction ORA_ABS(0x0d, "ORA", 3, 4);
	static constexpr Instruction ORA_ABS_X(0x1d, "ORA", 3, 4);
	static constexpr Instruction ORA_ABS_Y(0x19, "ORA", 3, 4);
	static constexpr Instruction ORA_IND_X(0x01, "ORA", 2, 6);
	static constexpr Instruction ORA_IND_Y(0x11, "ORA", 2, 5);

	static constexpr Instruction TXS(0x9a, "TXS", 1, 2); // Done

	static constexpr Instruction LDA_ABS(0xad, "LDA", 3, 4); // Done

	static constexpr Instruction SEI(0x78, "SEI", 1, 2);
*/

static constexpr Instruction LDX_IMMED(0xa2, "LDX", 2, 2, 0b10000010);
static constexpr Instruction CLD(0xd8, "CLD", 1, 1, 0b00001000);

};
};

#endif /* OPCODES_H */

#ifndef OPCODES_H
#define OPCODES_H

#include <tuple>

namespace mos6502
{
	namespace OpCodes
	{
		using namespace std;

		struct Instruction {
			const byte opcode;
			const char *name;
			const short byteSize;
			const short cycles;

			constexpr Instruction(const byte opcode, const char *name, short byteSize, short cycles) : opcode(opcode),
				name(name), byteSize(byteSize), cycles(cycles)
			{
			}

			constexpr auto pair() const
			{
				return make_pair(opcode, this);
			}

			constexpr operator byte() const
			{
				return opcode;
			}
		};

		static constexpr Instruction ADC_IMMED(0x69, "ADC", 2, 2);
		static constexpr Instruction ADC_ZERO(0x65, "ADC", 2, 3);
		static constexpr Instruction ADC_ZERO_X(0x75, "ADC", 2, 4);
		static constexpr Instruction ADC_ABS(0x6d, "ADC", 3, 4);
		static constexpr Instruction ADC_ABS_X(0x7d, "ADC", 3, 4);
		static constexpr Instruction ADC_ABS_Y(0x79, "ADC", 3, 4);
		static constexpr Instruction ADC_IND_X(0x61, "ADC", 2, 6);
		static constexpr Instruction ADC_IND_Y(0x71, "ADC", 2, 5);

		static constexpr Instruction BRK(0x00, "BRK", 1, 7);

		static constexpr Instruction ORA_IMMED(0x09, "ORA", 2, 2);
		static constexpr Instruction ORA_ZERO(0x05, "ORA", 2, 3);
		static constexpr Instruction ORA_ZERO_X(0x15, "ORA", 2, 4);
		static constexpr Instruction ORA_ABS(0x0d, "ORA", 3, 4);
		static constexpr Instruction ORA_ABS_X(0x1d, "ORA", 3, 4);
		static constexpr Instruction ORA_ABS_Y(0x19, "ORA", 3, 4);
		static constexpr Instruction ORA_IND_X(0x01, "ORA", 2, 6);
		static constexpr Instruction ORA_IND_Y(0x11, "ORA", 2, 5);

		static constexpr Instruction CLD(0xd8, "CLD", 1, 1);
		static constexpr Instruction SEI(0x78, "SEI", 1, 2);

		static const map<const byte, const Instruction *> opCodeMap = {
			BRK.pair(),
			CLD.pair(),
			SEI.pair(),
		};
	};
};

#endif /* OPCODES_H */

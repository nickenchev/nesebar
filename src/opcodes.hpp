#ifndef OPCODES_H
#define OPCODES_H

#include <tuple>

namespace mos6502
{
	namespace OpCodes
	{
		using namespace std;
		static constexpr auto BRK = make_pair(0x00, "BRK");
		static constexpr auto ORA_IMMED = make_pair(0x09, "ORA");
		static constexpr auto ORA_ZERO = make_pair(0x05, "ORA");
		static constexpr auto ORA_ZERO_X = make_pair(0x15, "ORA");
		static constexpr auto ORA_ABS = make_pair(0x0d, "ORA");
		static constexpr auto ORA_ABS_X = make_pair(0x1d, "ORA");
		static constexpr auto ORA_ABS_Y = make_pair(0x19, "ORA");
		static constexpr auto ORA_IND_X = make_pair(0x01, "ORA");
		static constexpr auto ORA_IND_Y = make_pair(0x11, "ORA");
		static constexpr auto CLD = make_pair(0xd8, "CLD");
		static constexpr auto SEI = make_pair(0x78, "SEI");

		static map<byte, std::string> opCodeMap = {
			BRK,
			CLD,
			SEI,
		};
	};
};

#endif /* OPCODES_H */

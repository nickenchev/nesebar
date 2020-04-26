#ifndef OPCODES_H
#define OPCODES_H

#include <tuple>
#include <map>
#include "opcode.hpp"

namespace mos6502 { namespace opcodes
{

	/*
	  7   6   5   4   3   2   1   0
	  N   V       B   D   I   Z   C
	*/
	static constexpr byte None{0};
	static constexpr byte NZ{0b10000010};
	static constexpr byte I{0b00000100};

	struct BPL : Opcode<0x10, 2, 2, None> { static inline Asm name{"BPL"}; };
	struct JMP : Opcode<0x4c, 3, 3, None> { static inline Asm name{"JMP"}; };
	struct SEI : Opcode<0x78, 1, 2, I> { static inline Asm name{"SEI"}; };

	namespace STA
	{
		static constexpr const char *groupName = "STA";
		struct Absolute: Opcode<0x8d, 3, 4, None> { static inline Asm name{groupName}; };
	}

	namespace STX
	{
		static constexpr const char *groupName = "STX";
		struct ZeroPage : Opcode<0x86, 2, 3, None> { static inline Asm name{groupName}; };
	}

	struct TXS : Opcode<0x9a, 1, 2, None> { static inline Asm name{"TXS"}; };

	namespace LDX
	{
		static constexpr const char *groupName = "LDX";
		struct Immediate: Opcode<0xa2, 2, 2, NZ> { static inline Asm name{groupName}; };
	}

	namespace LDA
	{
		static constexpr const char *groupName = "LDA";
		struct Immediate : Opcode<0xa9, 2, 2, NZ> { static inline Asm name{groupName}; };
		struct Absolute : Opcode<0xad, 3, 2, NZ> { static inline Asm name{groupName}; };
	}

	struct CLD : Opcode<0xd8, 1, 1, None> { static inline Asm name{"CLD"}; };
};
};

#endif /* OPCODES_H */

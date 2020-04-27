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
	static constexpr byte Manual{0};
	static constexpr byte N{0b10000000};
	static constexpr byte V{0b00100000};
	static constexpr byte B{0b00010000};
	static constexpr byte D{0b00001000};
	static constexpr byte I{0b00000100};
	static constexpr byte Z{0b00000010};
	static constexpr byte C{0b00000001};

	struct BPL : Opcode<0x10, 2, 2, None> { static inline Asm name{"BPL"}; };
	struct CLC : Opcode<0x18, 1, 2, C> { static inline Asm name{"CLC"}; };
	struct JSR : Opcode<0x20, 3, 6, None> { static inline Asm name{"JSR"}; };

	namespace BIT
	{
		static constexpr const char *groupName = "BIT";
		struct ZeroPage: Opcode<0x24, 2, 3, Z> { static inline Asm name{groupName}; };
		struct Absolute: Opcode<0x2c, 3, 4, Z> { static inline Asm name{groupName}; };
	}

	struct SEC : Opcode<0x38, 1, 2, C> { static inline Asm name{"SEC"}; };
	struct JMP : Opcode<0x4c, 3, 3, None> { static inline Asm name{"JMP"}; };
	struct BVS : Opcode<0x70, 2, 2, None> { static inline Asm name{"BVS"}; };
	struct SEI : Opcode<0x78, 1, 2, I> { static inline Asm name{"SEI"}; };

	namespace STA
	{
		static constexpr const char *groupName = "STA";
		struct ZeroPage: Opcode<0x85, 2, 3, None> { static inline Asm name{groupName}; };
		struct Absolute: Opcode<0x8d, 3, 4, None> { static inline Asm name{groupName}; };
	}

	namespace STX
	{
		static constexpr const char *groupName = "STX";
		struct ZeroPage : Opcode<0x86, 2, 3, None> { static inline Asm name{groupName}; };
	}

	struct BCC : Opcode<0x90, 2, 2, None> { static inline Asm name{"BCC"}; };
	struct TXS : Opcode<0x9a, 1, 2, None> { static inline Asm name{"TXS"}; };

	namespace LDX
	{
		static constexpr const char *groupName = "LDX";
		struct Immediate: Opcode<0xa2, 2, 2, N|Z> { static inline Asm name{groupName}; };
	}

	namespace LDA
	{
		static constexpr const char *groupName = "LDA";
		struct Immediate : Opcode<0xa9, 2, 2, N|Z> { static inline Asm name{groupName}; };
		struct Absolute : Opcode<0xad, 3, 2, N|Z> { static inline Asm name{groupName}; };
	}

	struct BCS : Opcode<0xb0, 2, 2, None> { static inline Asm name{"BCS"}; };
	struct BNE : Opcode<0xd0, 2, 2, None> { static inline Asm name{"BNE"}; };
	struct CLD : Opcode<0xd8, 1, 1, None> { static inline Asm name{"CLD"}; };
	struct NOP : Opcode<0xea, 1, 2, None> { static inline Asm name{"NOP"}; };
	struct BEQ : Opcode<0xf0, 2, 2, None> { static inline Asm name{"BEQ"}; };
};
};

#endif /* OPCODES_H */

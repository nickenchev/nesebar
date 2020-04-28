#ifndef OPCODES_H
#define OPCODES_H

#include <tuple>
#include <map>
#include "opcode.hpp"
#include "flags.hpp"

namespace mos6502 { namespace opcodes
{
	using namespace status_bits;

	namespace ADC
	{
		static constexpr const char *group = "ADC";
		static constexpr byte flags = N|Z|C|V;
		static constexpr byte manual = C;
		struct Immediate : Opcode<0x69, 2, 2, flags, manual> { static inline Asm name{group}; };
	}
	namespace ORA
	{
		static constexpr const char *groupName = "ORA";
		struct Immediate : Opcode<0x09, 2, 2, N|Z> { static inline Asm name{groupName}; };
	}

	namespace EOR
	{
		static constexpr const char *groupName = "EOR";
		struct Immediate : Opcode<0x49, 2, 2, N|Z> { static inline Asm name{groupName}; };
	}

	struct PHP : Opcode<0x08, 1, 3, None> { static inline Asm name{"PHP"}; };
	struct PHA : Opcode<0x48, 1, 3, None> { static inline Asm name{"PHA"}; };
	struct PLP : Opcode<0x28, 1, 4, None> { static inline Asm name{"PLP"}; };

	// branching
	struct BCS : Opcode<0xb0, 2, 2, None> { static inline Asm name{"BCS"}; };
	struct BCC : Opcode<0x90, 2, 2, None> { static inline Asm name{"BCC"}; };
	struct BEQ : Opcode<0xf0, 2, 2, None> { static inline Asm name{"BEQ"}; };
	struct BMI : Opcode<0x30, 2, 2, None> { static inline Asm name{"BMI"}; };
	struct BNE : Opcode<0xd0, 2, 2, None> { static inline Asm name{"BNE"}; };
	struct BPL : Opcode<0x10, 2, 2, None> { static inline Asm name{"BPL"}; };
	struct BVC : Opcode<0x50, 2, 2, None> { static inline Asm name{"BVC"}; };
	struct BVS : Opcode<0x70, 2, 2, None> { static inline Asm name{"BVS"}; };

	// cpu flags
	struct CLC : Opcode<0x18, 1, 2, C, C> { static inline Asm name{"CLC"}; };
	struct CLD : Opcode<0xd8, 1, 2, None> { static inline Asm name{"CLD"}; };
	struct SEC : Opcode<0x38, 1, 2, C, C> { static inline Asm name{"SEC"}; };
	struct SED : Opcode<0xf8, 1, 2, D> { static inline Asm name{"SED"}; };
	struct SEI : Opcode<0x78, 1, 2, I, I> { static inline Asm name{"SEI"}; };
	struct CLV : Opcode<0xb8, 1, 2, V, V> { static inline Asm name{"CLV"}; };
	
	// jumping
	struct JSR : Opcode<0x20, 3, 6, None> { static inline Asm name{"JSR"}; };
	struct JMP : Opcode<0x4c, 3, 3, None> { static inline Asm name{"JMP"}; };
	struct RTS : Opcode<0x60, 1, 6, None> { static inline Asm name{"RTS"}; };

	namespace BIT
	{
		static constexpr const char *groupName = "BIT";
		static constexpr byte allFlags = N|V|Z;
		static constexpr byte manualFlags = N|V;
		struct ZeroPage: Opcode<0x24, 2, 3, allFlags, manualFlags> { static inline Asm name{groupName}; };
	}

	namespace AND
	{
		static constexpr const char *groupName = "AND";
		struct Immediate: Opcode<0x29, 2, 2, N|Z> { static inline Asm name{groupName}; };
	}

	struct PLA : Opcode<0x68, 1, 4, N|Z> { static inline Asm name{"PLA"}; };

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

	struct TXS : Opcode<0x9a, 1, 2, None> { static inline Asm name{"TXS"}; };

	namespace LDX
	{
		static constexpr const char *groupName = "LDX";
		struct Immediate: Opcode<0xa2, 2, 2, N|Z> { static inline Asm name{groupName}; };
	}

	namespace LDY
	{
		static constexpr const char *groupName = "LDY";
		struct Immediate: Opcode<0xa0, 2, 2, N|Z> { static inline Asm name{groupName}; };
	}

	namespace LDA
	{
		static constexpr const char *groupName = "LDA";
		struct Immediate : Opcode<0xa9, 2, 2, N|Z> { static inline Asm name{groupName}; };
		struct Absolute : Opcode<0xad, 3, 2, N|Z> { static inline Asm name{groupName}; };
	}

	namespace CMP
	{
		static constexpr const char *group = "CMP";
		static constexpr byte allFlags = N|Z|C;
		static constexpr byte manualFlags = C;
		struct Immediate : Opcode<0xc9, 2, 2, allFlags, manualFlags> { static inline Asm name{group}; };
	}

	namespace CPX
	{
		static constexpr const char *group = "CPX";
		static constexpr byte flags = N|Z|C;
		static constexpr byte manualFlags = C;
		struct Immediate : Opcode<0xe0, 2, 2, flags, manualFlags> { static inline Asm name{group}; };
		struct ZeroPage : Opcode<0xe4, 2, 3, flags, manualFlags> { static inline Asm name{group}; };
		struct Absolute : Opcode<0xec, 3, 4, flags, manualFlags> { static inline Asm name{group}; };
	}

	namespace CPY
	{
		static constexpr const char *group = "CPY";
		static constexpr byte flags = N|Z|C;
		static constexpr byte manualFlags = C;
		struct Immediate : Opcode<0xc0, 2, 2, flags, manualFlags> { static inline Asm name{group}; };
		struct ZeroPage : Opcode<0xc4, 2, 3, flags, manualFlags> { static inline Asm name{group}; };
		struct Absolute : Opcode<0xcc, 3, 4, flags, manualFlags> { static inline Asm name{group}; };
	}

	struct NOP : Opcode<0xea, 1, 2, None> { static inline Asm name{"NOP"}; };
};
};

#endif /* OPCODES_H */

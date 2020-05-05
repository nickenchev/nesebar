#ifndef OPCODES_H
#define OPCODES_H

#include <tuple>
#include <map>
#include "opcode.hpp"
#include "flags.hpp"

namespace mos6502 { namespace opcodes
{
	using namespace status_bits;
	struct BRK : Opcode<0x00, 1, 7, I, I> { static inline Asm name{"BRK"}; };

	// interrupt related
	
	// arithmetic 
	namespace ADC
	{
		static constexpr const char *group = "ADC";
		static constexpr byte flags = N|Z|C|V;
		static constexpr byte manual = C;
		struct Immediate : Opcode<0x69, 2, 2, flags, manual> { static inline Asm name{group}; };
		struct ZeroPage: Opcode<0x65, 2, 3, flags, manual> { static inline Asm name{group}; };
		struct ZeroPageX: Opcode<0x75, 2, 4, flags, manual> { static inline Asm name{group}; };
		struct Absolute: Opcode<0x6d, 3, 4, flags, manual> { static inline Asm name{group}; };
		struct AbsoluteX: Opcode<0x7d, 3, 4, flags, manual> { static inline Asm name{group}; };
		struct AbsoluteY: Opcode<0x79, 3, 4, flags, manual> { static inline Asm name{group}; };
		struct IndexedIndirect : Opcode<0x61, 2, 6, flags, manual> { static inline Asm name{group}; };
		struct IndirectIndexed : Opcode<0x71, 2, 5, flags, manual> { static inline Asm name{group}; };
	}

	namespace SBC
	{
		static constexpr const char *group = "SBC";
		static constexpr byte flags = N|Z|C|V;
		static constexpr byte manual = C|V;
		struct Immediate : Opcode<0xe9, 2, 2, flags, manual> { static inline Asm name{group}; };
		struct ZeroPage: Opcode<0xe5, 2, 3, flags, manual> { static inline Asm name{group}; };
		struct ZeroPageX: Opcode<0xf5, 2, 4, flags, manual> { static inline Asm name{group}; };
		struct Absolute: Opcode<0xed, 3, 4, flags, manual> { static inline Asm name{group}; };
		struct AbsoluteX: Opcode<0xfd, 3, 4, flags, manual> { static inline Asm name{group}; };
		struct AbsoluteY: Opcode<0xf9, 3, 4, flags, manual> { static inline Asm name{group}; };
		struct IndexedIndirect : Opcode<0xe1, 2, 6, flags, manual> { static inline Asm name{group}; };
		struct IndirectIndexed : Opcode<0xf1, 2, 5, flags, manual> { static inline Asm name{group}; };
	}

	namespace INC
	{
		static constexpr const char *group = "INC";
		static constexpr byte flags = N|Z;
		struct ZeroPage: Opcode<0xe6, 2, 5, flags> { static inline Asm name{group}; };
		struct ZeroPageX: Opcode<0xf6, 2, 6, flags> { static inline Asm name{group}; };
		struct Absolute: Opcode<0xee, 3, 6, flags> { static inline Asm name{group}; };
		struct AbsoluteX: Opcode<0xfe, 3, 7, flags> { static inline Asm name{group}; };
	}

	namespace DEC
	{
		static constexpr const char *group = "DEC";
		static constexpr byte flags = N|Z;
		struct ZeroPage: Opcode<0xc6, 2, 5, flags> { static inline Asm name{group}; };
		struct ZeroPageX: Opcode<0xd6, 2, 6, flags> { static inline Asm name{group}; };
		struct Absolute: Opcode<0xce, 3, 6, flags> { static inline Asm name{group}; };
		struct AbsoluteX: Opcode<0xde, 3, 7, flags> { static inline Asm name{group}; };
	}

	// register related
	struct INX : Opcode<0xe8, 1, 2, N|Z> { static inline Asm name{"INX"}; };
	struct INY : Opcode<0xc8, 1, 2, N|Z> { static inline Asm name{"INY"}; };
	struct DEX : Opcode<0xca, 1, 2, N|Z> { static inline Asm name{"DEX"}; };
	struct DEY : Opcode<0x88, 1, 2, N|Z> { static inline Asm name{"DEY"}; };
	struct TAX : Opcode<0xaa, 1, 2, N|Z> { static inline Asm name{"TAX"}; };
	struct TAY : Opcode<0xa8, 1, 2, N|Z> { static inline Asm name{"TAY"}; };
	struct TXA : Opcode<0x8a, 1, 2, N|Z> { static inline Asm name{"TXA"}; };
	struct TYA : Opcode<0x98, 1, 2, N|Z> { static inline Asm name{"TYA"}; };
	struct TSX : Opcode<0xba, 1, 2, N|Z> { static inline Asm name{"TSX"}; };

	// binary operators
	namespace ORA
	{
		static constexpr const char *group = "ORA";
		static constexpr byte flags = N|Z;
		struct Immediate : Opcode<0x09, 2, 2, flags> { static inline Asm name{group}; };
		struct ZeroPage: Opcode<0x05, 2, 3, flags> { static inline Asm name{group}; };
		struct ZeroPageX: Opcode<0x15, 2, 4, flags> { static inline Asm name{group}; };
		struct Absolute: Opcode<0x0d, 3, 4, flags> { static inline Asm name{group}; };
		struct AbsoluteX: Opcode<0x1d, 3, 4, flags> { static inline Asm name{group}; };
		struct AbsoluteY: Opcode<0x19, 3, 4, flags> { static inline Asm name{group}; };
		struct IndexedIndirect : Opcode<0x01, 2, 6, flags> { static inline Asm name{group}; };
		struct IndirectIndexed : Opcode<0x11, 2, 5, flags> { static inline Asm name{group}; };
	}

	namespace EOR
	{
		static constexpr const char *group = "EOR";
		static constexpr byte flags = N|Z;
		struct Immediate : Opcode<0x49, 2, 2, flags> { static inline Asm name{group}; };
		struct ZeroPage: Opcode<0x45, 2, 3, flags> { static inline Asm name{group}; };
		struct ZeroPageX: Opcode<0x55, 2, 4, flags> { static inline Asm name{group}; };
		struct Absolute: Opcode<0x4d, 3, 4, flags> { static inline Asm name{group}; };
		struct AbsoluteX: Opcode<0x5d, 3, 4, flags> { static inline Asm name{group}; };
		struct AbsoluteY: Opcode<0x59, 3, 4, flags> { static inline Asm name{group}; };
		struct IndexedIndirect : Opcode<0x41, 2, 6, flags> { static inline Asm name{group}; };
		struct IndirectIndexed : Opcode<0x51, 2, 5, flags> { static inline Asm name{group}; };
	}

	namespace ASL
	{
		static constexpr const char *group = "ASL";
		static constexpr byte flags = N|Z|C;
		static constexpr byte manual = N|C;
		struct Accumulator : Opcode<0x0a, 1, 2, flags, manual> { static inline Asm name{group}; };
		struct ZeroPage : Opcode<0x06, 2, 5, flags, manual> { static inline Asm name{group}; };
		struct ZeroPageX : Opcode<0x16, 2, 6, flags, manual> { static inline Asm name{group}; };
		struct Absolute : Opcode<0x0e, 3, 6, flags, manual> { static inline Asm name{group}; };
		struct AbsoluteX : Opcode<0x1e, 3, 7, flags, manual> { static inline Asm name{group}; };
	}
	namespace LSR
	{
		static constexpr const char *group = "LSR";
		static constexpr byte flags = N|Z|C;
		static constexpr byte manual = N|C;
		struct Accumulator : Opcode<0x4a, 1, 2, flags, manual> { static inline Asm name{group}; };
		struct ZeroPage : Opcode<0x46, 2, 5, flags, manual> { static inline Asm name{group}; };
		struct ZeroPageX : Opcode<0x56, 2, 6, flags, manual> { static inline Asm name{group}; };
		struct Absolute : Opcode<0x4e, 3, 6, flags, manual> { static inline Asm name{group}; };
		struct AbsoluteX : Opcode<0x5e, 3, 7, flags, manual> { static inline Asm name{group}; };
	}
	namespace ROL
	{
		static constexpr const char *group = "ROL";
		static constexpr byte flags = N|Z|C;
		static constexpr byte manual = N|C;
		struct Accumulator : Opcode<0x2a, 1, 2, flags, manual> { static inline Asm name{group}; };
		struct ZeroPage : Opcode<0x26, 2, 5, flags, manual> { static inline Asm name{group}; };
		struct ZeroPageX : Opcode<0x36, 2, 6, flags, manual> { static inline Asm name{group}; };
		struct Absolute : Opcode<0x2e, 3, 6, flags, manual> { static inline Asm name{group}; };
		struct AbsoluteX : Opcode<0x3e, 3, 7, flags, manual> { static inline Asm name{group}; };
	}
	namespace ROR
	{
		static constexpr const char *group = "ROR";
		static constexpr byte flags = N|Z|C;
		static constexpr byte manual = N|C;
		struct Accumulator : Opcode<0x6a, 1, 2, flags, manual> { static inline Asm name{group}; };
		struct ZeroPage : Opcode<0x66, 2, 5, flags, manual> { static inline Asm name{group}; };
		struct ZeroPageX : Opcode<0x76, 2, 6, flags, manual> { static inline Asm name{group}; };
		struct Absolute : Opcode<0x6e, 3, 6, flags, manual> { static inline Asm name{group}; };
		struct AbsoluteX : Opcode<0x7e, 3, 7, flags, manual> { static inline Asm name{group}; };
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
	struct CLD : Opcode<0xd8, 1, 2, D, D> { static inline Asm name{"CLD"}; };
	struct SEC : Opcode<0x38, 1, 2, C, C> { static inline Asm name{"SEC"}; };
	struct SED : Opcode<0xf8, 1, 2, D, D> { static inline Asm name{"SED"}; };
	struct SEI : Opcode<0x78, 1, 2, I, I> { static inline Asm name{"SEI"}; };
	struct CLV : Opcode<0xb8, 1, 2, V, V> { static inline Asm name{"CLV"}; };
	
	// jumping
	struct JSR : Opcode<0x20, 3, 6, None> { static inline Asm name{"JSR"}; };

	namespace JMP
	{
		static constexpr const char *group = "JMP";
		struct Absolute : Opcode<0x4c, 3, 3, None> { static inline Asm name{group}; };
		struct Indirect : Opcode<0x6c, 3, 5, None> { static inline Asm name{group}; };
	}

	struct RTS : Opcode<0x60, 1, 6, None> { static inline Asm name{"RTS"}; };
	struct RTI : Opcode<0x40, 1, 6, None> { static inline Asm name{"RTI"}; };

	namespace BIT
	{
		static constexpr const char *group = "BIT";
		static constexpr byte all = N|V|Z;
		static constexpr byte flags = N|V;
		struct ZeroPage: Opcode<0x24, 2, 3, all, flags> { static inline Asm name{group}; };
		struct Absolute: Opcode<0x2c, 3, 4, all, flags> { static inline Asm name{group}; };
	}

	namespace AND
	{
		static constexpr const char *group = "AND";
		static constexpr byte all = N|Z;
		struct Immediate: Opcode<0x29, 2, 2, all> { static inline Asm name{group}; };
		struct ZeroPage: Opcode<0x25, 2, 3, all> { static inline Asm name{group}; };
		struct ZeroPageX: Opcode<0x35, 2, 4, all> { static inline Asm name{group}; };
		struct Absolute: Opcode<0x2d, 3, 4, all> { static inline Asm name{group}; };
		struct AbsoluteX: Opcode<0x3d, 3, 4, all> { static inline Asm name{group}; };
		struct AbsoluteY: Opcode<0x39, 3, 4, all> { static inline Asm name{group}; };
		struct IndexedIndirect : Opcode<0x21, 2, 6, all> { static inline Asm name{group}; };
		struct IndirectIndexed : Opcode<0x31, 2, 5, all> { static inline Asm name{group}; };
	}

	struct PLA : Opcode<0x68, 1, 4, N|Z> { static inline Asm name{"PLA"}; };

	namespace STY
	{
		static constexpr const char *group = "STY";
		struct ZeroPage: Opcode<0x84, 2, 3, None> { static inline Asm name{group}; };
		struct ZeroPageX: Opcode<0x94, 2, 4, None> { static inline Asm name{group}; };
		struct Absolute: Opcode<0x8c, 3, 4, None> { static inline Asm name{group}; };
	}

	namespace STA
	{
		static constexpr const char *group = "STA";
		struct ZeroPage: Opcode<0x85, 2, 3, None> { static inline Asm name{group}; };
		struct ZeroPageX: Opcode<0x95, 2, 4, None> { static inline Asm name{group}; };
		struct Absolute: Opcode<0x8d, 3, 4, None> { static inline Asm name{group}; };
		struct AbsoluteX: Opcode<0x9d, 3, 5, None> { static inline Asm name{group}; };
		struct AbsoluteY: Opcode<0x99, 3, 5, None> { static inline Asm name{group}; };
		struct IndexedIndirect : Opcode<0x81, 2, 6, None> { static inline Asm name{group}; };
		struct IndirectIndexed : Opcode<0x91, 2, 6, None> { static inline Asm name{group}; };
	}

	namespace STX
	{
		static constexpr const char *group = "STX";
		struct ZeroPage : Opcode<0x86, 2, 3, None> { static inline Asm name{group}; };
		struct ZeroPageY : Opcode<0x96, 2, 4, None> { static inline Asm name{group}; };
		struct Absolute : Opcode<0x8e, 3, 4, None> { static inline Asm name{group}; };
	}

	struct TXS : Opcode<0x9a, 1, 2, None> { static inline Asm name{"TXS"}; };

	namespace LDX
	{
		static constexpr const char *group = "LDX";
		struct Immediate: Opcode<0xa2, 2, 2, N|Z> { static inline Asm name{group}; };
		struct ZeroPage: Opcode<0xa6, 2, 3, N|Z> { static inline Asm name{group}; };
		struct ZeroPageY: Opcode<0xb6, 2, 4, N|Z> { static inline Asm name{group}; };
		struct Absolute: Opcode<0xae, 3, 4, N|Z> { static inline Asm name{group}; };
		struct AbsoluteY: Opcode<0xbe, 3, 4, N|Z> { static inline Asm name{group}; };
	}

	namespace LDY
	{
		static constexpr const char *group = "LDY";
		static constexpr byte flags = N|Z;
		struct Immediate: Opcode<0xa0, 2, 2, flags> { static inline Asm name{group}; };
		struct ZeroPage: Opcode<0xa4, 2, 3, flags> { static inline Asm name{group}; };
		struct ZeroPageX: Opcode<0xb4, 2, 4, flags> { static inline Asm name{group}; };
		struct Absolute: Opcode<0xac, 3, 4, flags> { static inline Asm name{group}; };
		struct AbsoluteX: Opcode<0xbc, 3, 4, flags> { static inline Asm name{group}; };
	}

	namespace LDA
	{
		static constexpr const char *group = "LDA";
		static constexpr byte flags = N|Z;
		struct Immediate : Opcode<0xa9, 2, 2, flags> { static inline Asm name{group}; };
		struct ZeroPage : Opcode<0xa5, 2, 3, flags> { static inline Asm name{group}; };
		struct ZeroPageX : Opcode<0xb5, 2, 4, flags> { static inline Asm name{group}; };
		struct Absolute : Opcode<0xad, 3, 4, flags> { static inline Asm name{group}; };
		struct AbsoluteX : Opcode<0xbd, 3, 4, flags> { static inline Asm name{group}; };
		struct AbsoluteY : Opcode<0xb9, 3, 4, flags> { static inline Asm name{group}; };
		struct IndexedIndirect : Opcode<0xa1, 2, 6, flags> { static inline Asm name{group}; };
		struct IndirectIndexed : Opcode<0xb1, 2, 5, flags> { static inline Asm name{group}; };
	}

	namespace CMP
	{
		static constexpr const char *group = "CMP";
		static constexpr byte flags = N|Z|C;
		static constexpr byte manual = C;
		struct Immediate : Opcode<0xc9, 2, 2, flags, manual> { static inline Asm name{group}; };
		struct ZeroPage : Opcode<0xc5, 2, 3, flags, manual> { static inline Asm name{group}; };
		struct ZeroPageX : Opcode<0xd5, 2, 4, flags, manual> { static inline Asm name{group}; };
		struct Absolute : Opcode<0xcd, 3, 4, flags, manual> { static inline Asm name{group}; };
		struct AbsoluteX : Opcode<0xdd, 3, 4, flags, manual> { static inline Asm name{group}; };
		struct AbsoluteY : Opcode<0xd9, 3, 4, flags, manual> { static inline Asm name{group}; };
		struct IndexedIndirect : Opcode<0xc1, 2, 6, flags, manual> { static inline Asm name{group}; };
		struct IndirectIndexed : Opcode<0xd1, 2, 5, flags, manual> { static inline Asm name{group}; };
	}

	namespace CPX
	{
		static constexpr const char *group = "CPX";
		static constexpr byte flags = N|Z|C;
		static constexpr byte manual = C;
		struct Immediate : Opcode<0xe0, 2, 2, flags, manual> { static inline Asm name{group}; };
		struct ZeroPage : Opcode<0xe4, 2, 3, flags, manual> { static inline Asm name{group}; };
		struct Absolute : Opcode<0xec, 3, 4, flags, manual> { static inline Asm name{group}; };
	}

	namespace CPY
	{
		static constexpr const char *group = "CPY";
		static constexpr byte flags = N|Z|C;
		static constexpr byte manual = C;
		struct Immediate : Opcode<0xc0, 2, 2, flags, manual> { static inline Asm name{group}; };
		struct ZeroPage : Opcode<0xc4, 2, 3, flags, manual> { static inline Asm name{group}; };
		struct Absolute : Opcode<0xcc, 3, 4, flags, manual> { static inline Asm name{group}; };
	}

	namespace NOP
	{
		static constexpr const char *group = "*NOP";

		namespace ZeroPage
		{
			struct _1 : Opcode<0x04, 2, 3, None> { static inline Asm name{group}; };
			struct _2 : Opcode<0x44, 2, 3, None> { static inline Asm name{group}; };
			struct _3 : Opcode<0x64, 2, 3, None> { static inline Asm name{group}; };
		}
		namespace Absolute
		{
			struct _1 : Opcode<0x0c, 3, 4, None> { static inline Asm name{group}; };
		}
		namespace ZeroPageX
		{
			struct _1 : Opcode<0x14, 2, 4, None> { static inline Asm name{group}; };
			struct _2 : Opcode<0x34, 2, 4, None> { static inline Asm name{group}; };
			struct _3 : Opcode<0x54, 2, 4, None> { static inline Asm name{group}; };
			struct _4 : Opcode<0x74, 2, 4, None> { static inline Asm name{group}; };
			struct _5 : Opcode<0xd4, 2, 4, None> { static inline Asm name{group}; };
			struct _6 : Opcode<0xf4, 2, 4, None> { static inline Asm name{group}; };
		}
		namespace Implied
		{
			struct Official : Opcode<0xea, 1, 2, None> { static inline Asm name{"NOP"}; };
			struct _1 : Opcode<0x1a, 1, 2, None> { static inline Asm name{group}; };
			struct _2 : Opcode<0x3a, 1, 2, None> { static inline Asm name{group}; };
			struct _3 : Opcode<0x5a, 1, 2, None> { static inline Asm name{group}; };
			struct _4 : Opcode<0x7a, 1, 2, None> { static inline Asm name{group}; };
			struct _5 : Opcode<0xda, 1, 2, None> { static inline Asm name{group}; };
			struct _6 : Opcode<0xfa, 1, 2, None> { static inline Asm name{group}; };
		}
		struct Immediate : Opcode<0x80, 2, 2, None> { static inline Asm name{group}; };

		namespace AbsoluteX
		{
			struct _1 : Opcode<0x1c, 3, 4, None> { static inline Asm name{group}; };
			struct _2 : Opcode<0x3c, 3, 4, None> { static inline Asm name{group}; };
			struct _3 : Opcode<0x5c, 3, 4, None> { static inline Asm name{group}; };
			struct _4 : Opcode<0x7c, 3, 4, None> { static inline Asm name{group}; };
			struct _5 : Opcode<0xdc, 3, 4, None> { static inline Asm name{group}; };
			struct _6 : Opcode<0xfc, 3, 4, None> { static inline Asm name{group}; };
		}
	}
};
};

#endif /* OPCODES_H */

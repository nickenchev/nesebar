#ifndef OPCODES_H
#define OPCODES_H

#include <tuple>
#include <map>
#include "opcode.hpp"
#include "flags.hpp"
#include "addressing.hpp"

namespace mos6502 { namespace opcodes
{
	using namespace status_bits;
	namespace Addr = mos6502::Addressing;

	// interrupt related
	struct BRK : Addr::Implicit<0x00, 7, I, I> { static inline Asm name{"BRK"}; };
	
	// arithmetic 
	namespace ADC
	{
		static constexpr const char *group = "ADC";
		static constexpr byte flags = N|Z|C|V;
		static constexpr byte manual = C;
		struct Immediate : Addr::Immediate<0x69, 2, flags, manual> { static inline Asm name{group}; };
		struct ZeroPage: Addr::ZeroPage<0x65, 3, flags, manual> { static inline Asm name{group}; };
		struct ZeroPageX: Addr::ZeroPageX<0x75, 4, flags, manual> { static inline Asm name{group}; };
		struct Absolute: Addr::Absolute<0x6d, 4, flags, manual> { static inline Asm name{group}; };
		struct AbsoluteX: Addr::AbsoluteX<0x7d, 4, flags, manual> { static inline Asm name{group}; };
		struct AbsoluteY: Addr::AbsoluteY<0x79, 4, flags, manual> { static inline Asm name{group}; };
		struct IndexedIndirect : Addr::IndexedIndirect<0x61, 6, flags, manual> { static inline Asm name{group}; };
		struct IndirectIndexed : Addr::IndirectIndexed<0x71, 5, flags, manual> { static inline Asm name{group}; };
	}

	namespace SBC
	{
		static constexpr const char *group = "SBC";
		static constexpr byte flags = N|Z|C|V;
		static constexpr byte manual = C|V;
		struct Immediate : Addr::Immediate<0xe9, 2, flags, manual> { static inline Asm name{group}; };
		struct Immediate_2 : Addr::Immediate<0xeb, 2, flags, manual> { static inline Asm name{group}; };
		struct ZeroPage: Addr::ZeroPage<0xe5, 3, flags, manual> { static inline Asm name{group}; };
		struct ZeroPageX: Addr::ZeroPageX<0xf5, 4, flags, manual> { static inline Asm name{group}; };
		struct Absolute: Addr::Absolute<0xed, 4, flags, manual> { static inline Asm name{group}; };
		struct AbsoluteX: Addr::AbsoluteX<0xfd, 4, flags, manual> { static inline Asm name{group}; };
		struct AbsoluteY: Addr::AbsoluteY<0xf9, 4, flags, manual> { static inline Asm name{group}; };
		struct IndexedIndirect : Addr::IndexedIndirect<0xe1, 6, flags, manual> { static inline Asm name{group}; };
		struct IndirectIndexed : Addr::IndirectIndexed<0xf1, 5, flags, manual> { static inline Asm name{group}; };
	}

	namespace INC
	{
		static constexpr const char *group = "INC";
		static constexpr byte flags = N|Z;
		struct ZeroPage: Addr::ZeroPage<0xe6, 5, flags> { static inline Asm name{group}; };
		struct ZeroPageX: Addr::ZeroPageX<0xf6, 6, flags> { static inline Asm name{group}; };
		struct Absolute: Addr::Absolute<0xee, 6, flags> { static inline Asm name{group}; };
		struct AbsoluteX: Addr::AbsoluteX<0xfe, 7, flags> { static inline Asm name{group}; };
	}

	namespace DEC
	{
		static constexpr const char *group = "DEC";
		static constexpr byte flags = N|Z;
		struct ZeroPage: Addr::ZeroPage<0xc6, 5, flags> { static inline Asm name{group}; };
		struct ZeroPageX: Addr::ZeroPageX<0xd6, 6, flags> { static inline Asm name{group}; };
		struct Absolute: Addr::Absolute<0xce, 6, flags> { static inline Asm name{group}; };
		struct AbsoluteX: Addr::AbsoluteX<0xde, 7, flags> { static inline Asm name{group}; };
	}

	// register related
	struct INX : Addr::Implicit<0xe8, 2, N|Z> { static inline Asm name{"INX"}; };
	struct INY : Addr::Implicit<0xc8, 2, N|Z> { static inline Asm name{"INY"}; };
	struct DEX : Addr::Implicit<0xca, 2, N|Z> { static inline Asm name{"DEX"}; };
	struct DEY : Addr::Implicit<0x88, 2, N|Z> { static inline Asm name{"DEY"}; };
	struct TAX : Addr::Implicit<0xaa, 2, N|Z> { static inline Asm name{"TAX"}; };
	struct TAY : Addr::Implicit<0xa8, 2, N|Z> { static inline Asm name{"TAY"}; };
	struct TXA : Addr::Implicit<0x8a, 2, N|Z> { static inline Asm name{"TXA"}; };
	struct TYA : Addr::Implicit<0x98, 2, N|Z> { static inline Asm name{"TYA"}; };
	struct TSX : Addr::Implicit<0xba, 2, N|Z> { static inline Asm name{"TSX"}; };

	// binary operators
	namespace ORA
	{
		static constexpr const char *group = "ORA";
		static constexpr byte flags = N|Z;
		struct Immediate : Addr::Immediate<0x09, 2, flags> { static inline Asm name{group}; };
		struct ZeroPage : Addr::ZeroPage<0x05, 3, flags> { static inline Asm name{group}; };
		struct ZeroPageX : Addr::ZeroPageX<0x15, 4, flags> { static inline Asm name{group}; };
		struct Absolute : Addr::Absolute<0x0d, 4, flags> { static inline Asm name{group}; };
		struct AbsoluteX : Addr::AbsoluteX<0x1d, 4, flags> { static inline Asm name{group}; };
		struct AbsoluteY : Addr::AbsoluteY<0x19, 4, flags> { static inline Asm name{group}; };
		struct IndexedIndirect : Addr::IndexedIndirect<0x01, 6, flags> { static inline Asm name{group}; };
		struct IndirectIndexed : Addr::IndirectIndexed<0x11, 5, flags> { static inline Asm name{group}; };
	}

	namespace EOR
	{
		static constexpr const char *group = "EOR";
		static constexpr byte flags = N|Z;
		struct Immediate : Addr::Immediate<0x49, 2, flags> { static inline Asm name{group}; };
		struct ZeroPage : Addr::ZeroPage<0x45, 3, flags> { static inline Asm name{group}; };
		struct ZeroPageX : Addr::ZeroPageX<0x55, 4, flags> { static inline Asm name{group}; };
		struct Absolute : Addr::Absolute<0x4d, 4, flags> { static inline Asm name{group}; };
		struct AbsoluteX : Addr::AbsoluteX<0x5d, 4, flags> { static inline Asm name{group}; };
		struct AbsoluteY : Addr::AbsoluteY<0x59, 4, flags> { static inline Asm name{group}; };
		struct IndexedIndirect : Addr::IndexedIndirect<0x41, 6, flags> { static inline Asm name{group}; };
		struct IndirectIndexed : Addr::IndirectIndexed<0x51, 5, flags> { static inline Asm name{group}; };
	}

	namespace ASL
	{
		static constexpr const char *group = "ASL";
		static constexpr byte flags = N|Z|C;
		static constexpr byte manual = N|C;
		struct Accumulator : Addr::Accumulator<0x0a, 2, flags, manual> { static inline Asm name{group}; };
		struct ZeroPage : Addr::ZeroPage<0x06, 5, flags, manual> { static inline Asm name{group}; };
		struct ZeroPageX : Addr::ZeroPageX<0x16, 6, flags, manual> { static inline Asm name{group}; };
		struct Absolute : Addr::Absolute<0x0e, 6, flags, manual> { static inline Asm name{group}; };
		struct AbsoluteX : Addr::AbsoluteX<0x1e, 7, flags, manual> { static inline Asm name{group}; };
	}
	namespace LSR
	{
		static constexpr const char *group = "LSR";
		static constexpr byte flags = N|Z|C;
		static constexpr byte manual = N|C;
		struct Accumulator : Addr::Accumulator<0x4a, 2, flags, manual> { static inline Asm name{group}; };
		struct ZeroPage : Addr::ZeroPage<0x46, 5, flags, manual> { static inline Asm name{group}; };
		struct ZeroPageX : Addr::ZeroPageX<0x56, 6, flags, manual> { static inline Asm name{group}; };
		struct Absolute : Addr::Absolute<0x4e, 6, flags, manual> { static inline Asm name{group}; };
		struct AbsoluteX : Addr::AbsoluteX<0x5e, 7, flags, manual> { static inline Asm name{group}; };
	}
	namespace ROL
	{
		static constexpr const char *group = "ROL";
		static constexpr byte flags = N|Z|C;
		static constexpr byte manual = N|C;
		struct Accumulator : Addr::Accumulator<0x2a, 2, flags, manual> { static inline Asm name{group}; };
		struct ZeroPage : Addr::ZeroPage<0x26, 5, flags, manual> { static inline Asm name{group}; };
		struct ZeroPageX : Addr::ZeroPageX<0x36, 6, flags, manual> { static inline Asm name{group}; };
		struct Absolute : Addr::Absolute<0x2e, 6, flags, manual> { static inline Asm name{group}; };
		struct AbsoluteX : Addr::AbsoluteX<0x3e, 7, flags, manual> { static inline Asm name{group}; };
	}
	namespace ROR
	{
		static constexpr const char *group = "ROR";
		static constexpr byte flags = N|Z|C;
		static constexpr byte manual = N|C;
		struct Accumulator : Addr::Accumulator<0x6a, 2, flags, manual> { static inline Asm name{group}; };
		struct ZeroPage : Addr::ZeroPage<0x66, 5, flags, manual> { static inline Asm name{group}; };
		struct ZeroPageX : Addr::ZeroPageX<0x76, 6, flags, manual> { static inline Asm name{group}; };
		struct Absolute : Addr::Absolute<0x6e, 6, flags, manual> { static inline Asm name{group}; };
		struct AbsoluteX : Addr::AbsoluteX<0x7e, 7, flags, manual> { static inline Asm name{group}; };
	}

	struct PHP : Addr::Implicit<0x08, 3, None> { static inline Asm name{"PHP"}; };
	struct PHA : Addr::Implicit<0x48, 3, None> { static inline Asm name{"PHA"}; };
	struct PLP : Addr::Implicit<0x28, 4, None> { static inline Asm name{"PLP"}; };

	// branching
	struct BCS : Addr::Relative<0xb0, 2, None> { static inline Asm name{"BCS"}; };
	struct BCC : Addr::Relative<0x90, 2, None> { static inline Asm name{"BCC"}; };
	struct BEQ : Addr::Relative<0xf0, 2, None> { static inline Asm name{"BEQ"}; };
	struct BMI : Addr::Relative<0x30, 2, None> { static inline Asm name{"BMI"}; };
	struct BNE : Addr::Relative<0xd0, 2, None> { static inline Asm name{"BNE"}; };
	struct BPL : Addr::Relative<0x10, 2, None> { static inline Asm name{"BPL"}; };
	struct BVC : Addr::Relative<0x50, 2, None> { static inline Asm name{"BVC"}; };
	struct BVS : Addr::Relative<0x70, 2, None> { static inline Asm name{"BVS"}; };

	// cpu flags
	struct CLC : Addr::Implicit<0x18, 2, C, C> { static inline Asm name{"CLC"}; };
	struct CLD : Addr::Implicit<0xd8, 2, D, D> { static inline Asm name{"CLD"}; };
	struct SEC : Addr::Implicit<0x38, 2, C, C> { static inline Asm name{"SEC"}; };
	struct SED : Addr::Implicit<0xf8, 2, D, D> { static inline Asm name{"SED"}; };
	struct SEI : Addr::Implicit<0x78, 2, I, I> { static inline Asm name{"SEI"}; };
	struct CLV : Addr::Implicit<0xb8, 2, V, V> { static inline Asm name{"CLV"}; };
	
	// jumping
	struct JSR : Addr::Absolute<0x20, 6, None> { static inline Asm name{"JSR"}; };

	namespace JMP
	{
		static constexpr const char *group = "JMP";
		struct Absolute : Addr::Absolute<0x4c, 3, None> { static inline Asm name{group}; };
		struct Indirect : Addr::Indirect<0x6c, 5, None> { static inline Asm name{group}; };
	}

	struct RTS : Addr::Implicit<0x60, 6, None> { static inline Asm name{"RTS"}; };
	struct RTI : Addr::Implicit<0x40, 6, None> { static inline Asm name{"RTI"}; };

	namespace BIT
	{
		static constexpr const char *group = "BIT";
		static constexpr byte all = N|V|Z;
		static constexpr byte flags = N|V;
		struct ZeroPage : Addr::ZeroPage<0x24, 3, all, flags> { static inline Asm name{group}; };
		struct Absolute : Addr::Absolute<0x2c, 4, all, flags> { static inline Asm name{group}; };
	}

	namespace AND
	{
		static constexpr const char *group = "AND";
		static constexpr byte all = N|Z;
		struct Immediate : Addr::Immediate<0x29, 2, all> { static inline Asm name{group}; };
		struct ZeroPage : Addr::ZeroPage<0x25, 3, all> { static inline Asm name{group}; };
		struct ZeroPageX : Addr::ZeroPageX<0x35, 4, all> { static inline Asm name{group}; };
		struct Absolute : Addr::Absolute<0x2d, 4, all> { static inline Asm name{group}; };
		struct AbsoluteX : Addr::AbsoluteX<0x3d, 4, all> { static inline Asm name{group}; };
		struct AbsoluteY : Addr::AbsoluteY<0x39, 4, all> { static inline Asm name{group}; };
		struct IndexedIndirect : Addr::IndexedIndirect<0x21, 6, all> { static inline Asm name{group}; };
		struct IndirectIndexed : Addr::IndirectIndexed<0x31, 5, all> { static inline Asm name{group}; };
	}

	struct PLA : Addr::Implicit<0x68, 4, N|Z> { static inline Asm name{"PLA"}; };

	namespace STY
	{
		static constexpr const char *group = "STY";
		struct ZeroPage: Addr::ZeroPage<0x84, 3, None> { static inline Asm name{group}; };
		struct ZeroPageX: Addr::ZeroPageX<0x94, 4, None> { static inline Asm name{group}; };
		struct Absolute: Addr::Absolute<0x8c, 4, None> { static inline Asm name{group}; };
	}

	namespace STA
	{
		static constexpr const char *group = "STA";
		struct ZeroPage: Addr::ZeroPage<0x85, 3, None> { static inline Asm name{group}; };
		struct ZeroPageX: Addr::ZeroPageX<0x95, 4, None> { static inline Asm name{group}; };
		struct Absolute: Addr::Absolute<0x8d, 4, None> { static inline Asm name{group}; };
		struct AbsoluteX: Addr::AbsoluteX<0x9d, 5, None> { static inline Asm name{group}; };
		struct AbsoluteY: Addr::AbsoluteY<0x99, 5, None> { static inline Asm name{group}; };
		struct IndexedIndirect : Addr::IndexedIndirect<0x81, 6, None> { static inline Asm name{group}; };
		struct IndirectIndexed : Addr::IndirectIndexed<0x91, 6, None> { static inline Asm name{group}; };
	}

	namespace STX
	{
		static constexpr const char *group = "STX";
		struct ZeroPage : Addr::ZeroPage<0x86, 3, None> { static inline Asm name{group}; };
		struct ZeroPageY : Addr::ZeroPageY<0x96, 4, None> { static inline Asm name{group}; };
		struct Absolute : Addr::Absolute<0x8e, 4, None> { static inline Asm name{group}; };
	}

	struct TXS : Addr::Implicit<0x9a, 2, None> { static inline Asm name{"TXS"}; };

	namespace LDX
	{
		static constexpr const char *group = "LDX";
		struct Immediate: Addr::Immediate<0xa2, 2, N|Z> { static inline Asm name{group}; };
		struct ZeroPage: Addr::ZeroPage<0xa6, 3, N|Z> { static inline Asm name{group}; };
		struct ZeroPageY: Addr::ZeroPageY<0xb6, 4, N|Z> { static inline Asm name{group}; };
		struct Absolute: Addr::Absolute<0xae, 4, N|Z> { static inline Asm name{group}; };
		struct AbsoluteY: Addr::AbsoluteY<0xbe, 4, N|Z> { static inline Asm name{group}; };
	}

	namespace LDY
	{
		static constexpr const char *group = "LDY";
		static constexpr byte flags = N|Z;
		struct Immediate: Addr::Immediate<0xa0, 2, flags> { static inline Asm name{group}; };
		struct ZeroPage: Addr::ZeroPage<0xa4, 3, flags> { static inline Asm name{group}; };
		struct ZeroPageX: Addr::ZeroPageX<0xb4, 4, flags> { static inline Asm name{group}; };
		struct Absolute: Addr::Absolute<0xac, 4, flags> { static inline Asm name{group}; };
		struct AbsoluteX: Addr::AbsoluteX<0xbc, 4, flags> { static inline Asm name{group}; };
	}

	namespace LDA
	{
		static constexpr const char *group = "LDA";
		static constexpr byte flags = N|Z;
		struct Immediate : Addr::Immediate<0xa9, 2, flags> { static inline Asm name{group}; };
		struct ZeroPage : Addr::ZeroPage<0xa5, 3, flags> { static inline Asm name{group}; };
		struct ZeroPageX : Addr::ZeroPageX<0xb5, 4, flags> { static inline Asm name{group}; };
		struct Absolute : Addr::Absolute<0xad, 4, flags> { static inline Asm name{group}; };
		struct AbsoluteX : Addr::AbsoluteX<0xbd, 4, flags> { static inline Asm name{group}; };
		struct AbsoluteY : Addr::AbsoluteY<0xb9, 4, flags> { static inline Asm name{group}; };
		struct IndexedIndirect : Addr::IndexedIndirect<0xa1, 6, flags> { static inline Asm name{group}; };
		struct IndirectIndexed : Addr::IndirectIndexed<0xb1, 5, flags> { static inline Asm name{group}; };
	}

	namespace CMP
	{
		static constexpr const char *group = "CMP";
		static constexpr byte flags = N|Z|C;
		static constexpr byte manual = C;
		struct Immediate : Addr::Immediate<0xc9, 2, flags, manual> { static inline Asm name{group}; };
		struct ZeroPage : Addr::ZeroPage<0xc5, 3, flags, manual> { static inline Asm name{group}; };
		struct ZeroPageX : Addr::ZeroPageX<0xd5, 4, flags, manual> { static inline Asm name{group}; };
		struct Absolute : Addr::Absolute<0xcd, 4, flags, manual> { static inline Asm name{group}; };
		struct AbsoluteX : Addr::AbsoluteX<0xdd, 4, flags, manual> { static inline Asm name{group}; };
		struct AbsoluteY : Addr::AbsoluteY<0xd9, 4, flags, manual> { static inline Asm name{group}; };
		struct IndexedIndirect : Addr::IndexedIndirect<0xc1, 6, flags, manual> { static inline Asm name{group}; };
		struct IndirectIndexed : Addr::IndirectIndexed<0xd1, 5, flags, manual> { static inline Asm name{group}; };
	}

	namespace CPX
	{
		static constexpr const char *group = "CPX";
		static constexpr byte flags = N|Z|C;
		static constexpr byte manual = C;
		struct Immediate : Addr::Immediate<0xe0, 2, flags, manual> { static inline Asm name{group}; };
		struct ZeroPage : Addr::ZeroPage<0xe4, 3, flags, manual> { static inline Asm name{group}; };
		struct Absolute : Addr::Absolute<0xec, 4, flags, manual> { static inline Asm name{group}; };
	}

	namespace CPY
	{
		static constexpr const char *group = "CPY";
		static constexpr byte flags = N|Z|C;
		static constexpr byte manual = C;
		struct Immediate : Addr::Immediate<0xc0, 2, flags, manual> { static inline Asm name{group}; };
		struct ZeroPage : Addr::ZeroPage<0xc4, 3, flags, manual> { static inline Asm name{group}; };
		struct Absolute : Addr::Absolute<0xcc, 4, flags, manual> { static inline Asm name{group}; };
	}

	namespace NOP
	{
		static constexpr const char *group = "NOP";

		namespace ZeroPage
		{
			struct _1 : Addr::ZeroPage<0x04, 3, None> { static inline Asm name{group}; };
			struct _2 : Addr::ZeroPage<0x44, 3, None> { static inline Asm name{group}; };
			struct _3 : Addr::ZeroPage<0x64, 3, None> { static inline Asm name{group}; };
		}
		namespace Absolute
		{
			struct _1 : Addr::Absolute<0x0c, 4, None> { static inline Asm name{group}; };
		}
		namespace ZeroPageX
		{
			struct _1 : Addr::ZeroPageX<0x14, 4, None> { static inline Asm name{group}; };
			struct _2 : Addr::ZeroPageX<0x34, 4, None> { static inline Asm name{group}; };
			struct _3 : Addr::ZeroPageX<0x54, 4, None> { static inline Asm name{group}; };
			struct _4 : Addr::ZeroPageX<0x74, 4, None> { static inline Asm name{group}; };
			struct _5 : Addr::ZeroPageX<0xd4, 4, None> { static inline Asm name{group}; };
			struct _6 : Addr::ZeroPageX<0xf4, 4, None> { static inline Asm name{group}; };
		}
		namespace Implicit
		{
			struct Official : Addr::Implicit<0xea, 2, None> { static inline Asm name{"NOP"}; };
			struct _1 : Addr::Implicit<0x1a, 2, None> { static inline Asm name{group}; };
			struct _2 : Addr::Implicit<0x3a, 2, None> { static inline Asm name{group}; };
			struct _3 : Addr::Implicit<0x5a, 2, None> { static inline Asm name{group}; };
			struct _4 : Addr::Implicit<0x7a, 2, None> { static inline Asm name{group}; };
			struct _5 : Addr::Implicit<0xda, 2, None> { static inline Asm name{group}; };
			struct _6 : Addr::Implicit<0xfa, 2, None> { static inline Asm name{group}; };
		}
		struct Immediate : Addr::Immediate<0x80, 2, None> { static inline Asm name{group}; };

		namespace AbsoluteX
		{
			struct _1 : Addr::AbsoluteX<0x1c, 4, None> { static inline Asm name{group}; };
			struct _2 : Addr::AbsoluteX<0x3c, 4, None> { static inline Asm name{group}; };
			struct _3 : Addr::AbsoluteX<0x5c, 4, None> { static inline Asm name{group}; };
			struct _4 : Addr::AbsoluteX<0x7c, 4, None> { static inline Asm name{group}; };
			struct _5 : Addr::AbsoluteX<0xdc, 4, None> { static inline Asm name{group}; };
			struct _6 : Addr::AbsoluteX<0xfc, 4, None> { static inline Asm name{group}; };
		}
	}

	namespace LAX
	{
		static constexpr const char *group = "LAX";
		static constexpr byte flags = N|Z;
		struct ZeroPage : Addr::ZeroPage<0xa7, 3, flags> { static inline Asm name{group}; };
		struct ZeroPageY : Addr::ZeroPageY<0xb7, 4, flags> { static inline Asm name{group}; };
		struct Absolute : Addr::Absolute<0xaf, 4, flags> { static inline Asm name{group}; };
		struct AbsoluteY : Addr::AbsoluteY<0xbf, 4, flags> { static inline Asm name{group}; };
		struct IndexedIndirect : Addr::IndexedIndirect<0xa3, 6, flags> { static inline Asm name{group}; };
		struct IndirectIndexed : Addr::IndirectIndexed<0xb3, 5, flags> { static inline Asm name{group}; };
	}

	namespace SAX
	{
		static constexpr const char *group = "SAX";
		static constexpr byte flags = None;
		struct ZeroPage : Addr::ZeroPage<0x87, 3, flags> { static inline Asm name{group}; };
		struct ZeroPageY : Addr::ZeroPageY<0x97, 4, flags> { static inline Asm name{group}; };
		struct Absolute : Addr::Absolute<0x8f, 4, flags> { static inline Asm name{group}; };
		struct IndexedIndirect : Addr::IndexedIndirect<0x83, 6, flags> { static inline Asm name{group}; };
	}

	namespace DCP
	{
		static constexpr const char *group = "DCP";
		static constexpr byte flags = N|Z|C;
		static constexpr byte manual = C;
		struct ZeroPage : Addr::ZeroPage<0xc7, 5, flags, manual> { static inline Asm name{group}; };
		struct ZeroPageX : Addr::ZeroPageX<0xd7, 6, flags, manual> { static inline Asm name{group}; };
		struct Absolute : Addr::Absolute<0xcf, 6, flags, manual> { static inline Asm name{group}; };
		struct AbsoluteX : Addr::AbsoluteX<0xdf, 7, flags, manual> { static inline Asm name{group}; };
		struct AbsoluteY : Addr::AbsoluteY<0xdb, 7, flags, manual> { static inline Asm name{group}; };
		struct IndexedIndirect : Addr::IndexedIndirect<0xc3, 8, flags, manual> { static inline Asm name{group}; };
		struct IndirectIndexed : Addr::IndirectIndexed<0xd3, 8, flags, manual> { static inline Asm name{group}; };
	}

	namespace ISC
	{
		static constexpr const char *group = "ISC";
		static constexpr byte flags = N|V|Z|C;
		static constexpr byte manual = V|C;
		struct ZeroPage : Addr::ZeroPage<0xe7, 5, flags, manual> { static inline Asm name{group}; };
		struct ZeroPageX : Addr::ZeroPageX<0xf7, 6, flags, manual> { static inline Asm name{group}; };
		struct Absolute : Addr::Absolute<0xef, 6, flags, manual> { static inline Asm name{group}; };
		struct AbsoluteX : Addr::AbsoluteX<0xff, 7, flags, manual> { static inline Asm name{group}; };
		struct AbsoluteY : Addr::AbsoluteY<0xfb, 7, flags, manual> { static inline Asm name{group}; };
		struct IndexedIndirect : Addr::IndexedIndirect<0xe3, 8, flags, manual> { static inline Asm name{group}; };
		struct IndirectIndexed : Addr::IndirectIndexed<0xf3, 8, flags, manual> { static inline Asm name{group}; };
	}

	namespace SLO
	{
		static constexpr const char *group = "SLO";
		static constexpr byte flags = N|Z|C;
		static constexpr byte manual = N|C;
		struct ZeroPage : Addr::ZeroPage<0x07, 5, flags, manual> { static inline Asm name{group}; };
		struct ZeroPageX : Addr::ZeroPageX<0x17, 6, flags, manual> { static inline Asm name{group}; };
		struct Absolute : Addr::Absolute<0x0f, 6, flags, manual> { static inline Asm name{group}; };
		struct AbsoluteX : Addr::AbsoluteX<0x1f, 7, flags, manual> { static inline Asm name{group}; };
		struct AbsoluteY : Addr::AbsoluteY<0x1b, 7, flags, manual> { static inline Asm name{group}; };
		struct IndexedIndirect : Addr::IndexedIndirect<0x03, 8, flags, manual> { static inline Asm name{group}; };
		struct IndirectIndexed : Addr::IndirectIndexed<0x13, 8, flags, manual> { static inline Asm name{group}; };
	}

	namespace RLA
	{
		static constexpr const char *group = "RLA";
		static constexpr byte flags = N|Z|C;
		static constexpr byte manual = N|C;
		struct ZeroPage : Addr::ZeroPage<0x27, 5, flags, manual> { static inline Asm name{group}; };
		struct ZeroPageX : Addr::ZeroPageX<0x37, 6, flags, manual> { static inline Asm name{group}; };
		struct Absolute : Addr::Absolute<0x2f, 6, flags, manual> { static inline Asm name{group}; };
		struct AbsoluteX : Addr::AbsoluteX<0x3f, 7, flags, manual> { static inline Asm name{group}; };
		struct AbsoluteY : Addr::AbsoluteY<0x3b, 7, flags, manual> { static inline Asm name{group}; };
		struct IndexedIndirect : Addr::IndexedIndirect<0x23, 8, flags, manual> { static inline Asm name{group}; };
		struct IndirectIndexed : Addr::IndirectIndexed<0x33, 8, flags, manual> { static inline Asm name{group}; };
	}

	namespace SRE
	{
		static constexpr const char *group = "SRE";
		static constexpr byte flags = N|Z|C;
		static constexpr byte manual = N|C;
		struct ZeroPage : Addr::ZeroPage<0x47, 5, flags, manual> { static inline Asm name{group}; };
		struct ZeroPageX : Addr::ZeroPageX<0x57, 6, flags, manual> { static inline Asm name{group}; };
		struct Absolute : Addr::Absolute<0x4f, 6, flags, manual> { static inline Asm name{group}; };
		struct AbsoluteX : Addr::AbsoluteX<0x5f, 7, flags, manual> { static inline Asm name{group}; };
		struct AbsoluteY : Addr::AbsoluteY<0x5b, 7, flags, manual> { static inline Asm name{group}; };
		struct IndexedIndirect : Addr::IndexedIndirect<0x43, 8, flags, manual> { static inline Asm name{group}; };
		struct IndirectIndexed : Addr::IndirectIndexed<0x53, 8, flags, manual> { static inline Asm name{group}; };
	}

	namespace RRA
	{
		static constexpr const char *group = "RRA";
		static constexpr byte flags = N|V|Z|C;
		static constexpr byte manual = C;
		struct ZeroPage : Addr::ZeroPage<0x67, 5, flags, manual> { static inline Asm name{group}; };
		struct ZeroPageX : Addr::ZeroPageX<0x77, 6, flags, manual> { static inline Asm name{group}; };
		struct Absolute : Addr::Absolute<0x6f, 6, flags, manual> { static inline Asm name{group}; };
		struct AbsoluteX : Addr::AbsoluteX<0x7f, 7, flags, manual> { static inline Asm name{group}; };
		struct AbsoluteY : Addr::AbsoluteY<0x7b, 7, flags, manual> { static inline Asm name{group}; };
		struct IndexedIndirect : Addr::IndexedIndirect<0x63, 8, flags, manual> { static inline Asm name{group}; };
		struct IndirectIndexed : Addr::IndirectIndexed<0x73, 8, flags, manual> { static inline Asm name{group}; };
	}
};
};

#endif /* OPCODES_H */

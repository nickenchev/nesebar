#include <iostream>
#include <string>
#include <vector>
#include "core6502.hpp"
#include "nesmemory.hpp"
#include "opcodes.hpp"

using namespace mos6502;

template<typename Memory, typename Mapping, bool DecimalMode>
Core<Memory, Mapping, DecimalMode>::Core(const Mapping &mapping) : memory(state, mapping), mapping(mapping)
{
	memory.write(0x4017, 0); // frame IRQ enable
	memory.write(0x4015, 0); // disable all channels

	for (MemAddress addr = 0x4000; addr <= 0x400f; ++addr)
	{
		memory.write(addr, 0);
	}
	for (MemAddress addr = 0x4010; addr <= 0x4013; ++addr)
	{
		memory.write(addr, 0);
	}
}

template<typename Memory, typename Mapping, bool DecimalMode>
void Core<Memory, Mapping, DecimalMode>::step()
{
	using namespace mos6502::opcodes;

	logInfo();
	std::cout << '$' << std::hex << std::setfill('0')
			  << std::setw(4) << state.pc.value << ": ";

	const byte opcode = nextOpcode();
	switch (opcode)
	{
		case ADC::Immediate::value:
		{
			beginInstruction<ADC::Immediate>();
			byte operand = state.a;
			byte data = memory.memImmediate();
			byte carry = static_cast<int>(isStatus(Status::Carry));
			state.setA(operand + data + carry);

			// figure out if there is carry from addition
			uint16_t sum = operand + data + isStatus(Status::Carry);
			updateStatus(Status::Carry, sum > 0xff);

			endInstruction<ADC::Immediate>(operand, data);
			break;
		}
		case SBC::Immediate::value:
		{
			beginInstruction<SBC::Immediate>();
			byte minuhend = state.a;
			byte subtrahend = memory.memImmediate();
			byte carry = static_cast<int>(isStatus(Status::Carry));

			state.setA(minuhend - subtrahend - (1 - carry));
			int16_t diff = minuhend - subtrahend - (1 - carry);

			// figure out if there is carry from subtraction
			updateStatus(Status::Carry, static_cast<signed_byte>(state.a) >= 0);

			constexpr byte signBit = 0b10000000;
			const bool isOverflow = (minuhend ^ subtrahend) & (minuhend ^ state.opcodeResult) & signBit;
			updateStatus(Status::Overflow, isOverflow);

			endInstruction<SBC::Immediate>(minuhend, subtrahend);
			break;
		}
		case INX::value:
		{
			beginInstruction<INX>();
			state.setX(state.x + 1);
			endInstruction<INX>();
			break;
		}
		case INY::value:
		{
			beginInstruction<INY>();
			state.setY(state.y + 1);
			endInstruction<INY>();
			break;
		}
		case DEX::value:
		{
			beginInstruction<DEX>();
			state.setX(state.x - 1);
			endInstruction<DEX>();
			break;
		}
		case DEY::value:
		{
			beginInstruction<DEY>();
			state.setY(state.y - 1);
			endInstruction<DEY>();
			break;
		}
		case TAX::value:
		{
			beginInstruction<TAX>();
			state.setX(state.a);
			endInstruction<TAX>();
			break;
		}
		case TAY::value:
		{
			beginInstruction<TAY>();
			state.setY(state.a);
			endInstruction<TAY>();
			break;
		}
		case TXA::value:
		{
			beginInstruction<TXA>();
			state.setA(state.x);
			endInstruction<TXA>();
			break;
		}
		case TYA::value:
		{
			beginInstruction<TYA>();
			state.setA(state.y);
			endInstruction<TYA>();
			break;
		}
		case TSX::value:
		{
			beginInstruction<TSX>();
			state.setX(state.sp);
			endInstruction<TSX>();
			break;
		}
		case ORA::Immediate::value:
		{
			beginInstruction<ORA::Immediate>();
			state.setA(state.a | memory.memImmediate());
			endInstruction<ORA::Immediate>();
			break;
		}
		case EOR::Immediate::value:
		{
			beginInstruction<EOR::Immediate>();
			state.setA(state.a ^ memory.memImmediate());
			endInstruction<EOR::Immediate>();
			break;
		}
		case PHP::value:
		{
			beginInstruction<PHP>();
			stackPush(state.p);
			endInstruction<PHP>();
			break;
		}
		case PHA::value:
		{
			beginInstruction<PHA>();
			stackPush(state.a);
			endInstruction<PHA>();
			break;
		}
		case PLP::value:
		{
			beginInstruction<PLP>();
			state.setP(stackPop());
			endInstruction<PLP>();
			break;
		}
		case BPL::value:
		{
			beginInstruction<BPL>();
			branchIf(Status::NegativeResult, false);
			endInstruction<BPL>();
			break;
		}
		case CLC::value:
		{
			beginInstruction<CLC>();
			updateStatus(Status::Carry, false);
			endInstruction<CLC>();
			break;
		}
		case JSR::value:
		{
			beginInstruction<JSR>();
			MemAddress jumpAddr = memory.addressAbsolute();
			stackPushAddress(state.pc);
			state.pc = jumpAddr;
			endInstruction<JSR>();
			break;
		}
		case BIT::ZeroPage::value:
		{
			beginInstruction<BIT::ZeroPage>();
			byte operand = memory.memZeroPage();
			updateStatus(Status::NegativeResult,
						 checkBit(operand, status_int(Status::NegativeResult)));
			updateStatus(Status::Overflow,
						 checkBit(operand, status_int(Status::Overflow)));
			state.opcodeResult = operand & state.a;
			endInstruction<BIT::ZeroPage>();
			break;
		}
		case AND::Immediate::value:
		{
			beginInstruction<AND::Immediate>();
			state.setA(state.a & memory.memImmediate());
			endInstruction<AND::Immediate>();
			break;
		}
		case SEC::value:
		{
			beginInstruction<SEC>();
			updateStatus(Status::Carry, true);
			endInstruction<SEC>();
			break;
		}
		case JMP::value:
		{
			beginInstruction<JMP>();
			state.pc = memory.addressAbsolute();
			endInstruction<JMP>();
			break;
		}
		case BVC::value:
		{
			beginInstruction<BVC>();
			branchIf(Status::Overflow, false);
			endInstruction<BVC>();
			break;
		}
		case RTS::value:
		{
			beginInstruction<RTS>();
			state.pc = stackPopAddress();
			endInstruction<RTS>();
			break;
		}
		case PLA::value:
		{
			beginInstruction<PLA>();
			state.setA(stackPop());
			endInstruction<PLA>();
			break;
		}
		case BVS::value:
		{
			beginInstruction<BVS>();
			branchIf(Status::Overflow, true);
			endInstruction<BVS>();
			break;
		}
		case SEI::value:
		{
			beginInstruction<SEI>();
			updateStatus(Status::InterruptDisable, true);
			endInstruction<SEI>();
			break;
		}
		case STY::ZeroPage::value:
		{
			beginInstruction<STY::ZeroPage>();
			memory.writeZeroPage(memory.fetchByte(), state.y);
			endInstruction<STY::ZeroPage>();
			break;
		}
		case STA::ZeroPage::value:
		{
			beginInstruction<STA::ZeroPage>();
			memory.writeZeroPage(memory.fetchByte(), state.a);
			endInstruction<STA::ZeroPage>();
			break;
		}
		case STA::Absolute::value:
		{
			beginInstruction<STA::Absolute>();
			exit(1);
			endInstruction<STA::Absolute>();
			break;
		}
		case STX::ZeroPage::value:
		{
			beginInstruction<STX::ZeroPage>();
			memory.writeZeroPage(memory.fetchByte(), state.x);
			endInstruction<STX::ZeroPage>();
			break;
		}
		case STX::Absolute::value:
		{
			beginInstruction<STX::Absolute>();
			memory.writeAbsolute(state.x);
			endInstruction<STX::Absolute>();
			break;
		}
		case BCC::value:
		{
			beginInstruction<BCC>();
			branchIf(Status::Carry, false);
			endInstruction<BCC>();
			break;
		}
		case TXS::value:
		{
			beginInstruction<TXS>();
			state.setSP(state.x);
			endInstruction<TXS>();
			break;
		}
		case LDX::Immediate::value:
		{
			beginInstruction<LDX::Immediate>();
			state.setX(memory.memImmediate());
			endInstruction<LDX::Immediate>();
			break;
		}
		case LDY::Immediate::value:
		{
			beginInstruction<LDY::Immediate>();
			state.setY(memory.memImmediate());
			endInstruction<LDY::Immediate>();
			break;
		}
		case LDA::Immediate::value:
		{
			beginInstruction<LDA::Immediate>();
			state.setA(memory.memImmediate());
			endInstruction<LDA::Immediate>();
			break;
		}
		case LDA::Absolute::value:
		{
			beginInstruction<LDA::Absolute>();
			state.setA(memory.memAbsolute());
			endInstruction<LDA::Absolute>();
			break;
		}
		case BCS::value:
		{
			beginInstruction<BCS>();
			branchIf(Status::Carry, true);
			endInstruction<BCS>();
			break;
		}
		case CMP::Immediate::value:
		{
			using OP = CMP::Immediate;
			beginInstruction<OP>();
			byte data =memory. memImmediate();
			compare(state.a, data);
			endInstruction<OP>(state.a, data);
			break;
		}
		case CPX::Immediate::value:
		{
			using OP = CPX::Immediate;
			beginInstruction<OP>();
			byte data = memory.memImmediate();
			compare(state.x, data);
			endInstruction<OP>(state.x, data);
			break;
		}
		case CPX::ZeroPage::value:
		{
			using OP = CPX::ZeroPage;
			beginInstruction<OP>();
			byte data = memory.memZeroPage();
			compare(state.x, data);
			endInstruction<OP>(state.x, data);
			break;
		}
		case CPX::Absolute::value:
		{
			using OP = CPX::Absolute;
			beginInstruction<OP>();
			byte data = memory.memAbsolute();
			compare(state.x, data);
			endInstruction<OP>(state.x, data);
			break;
		}
		case CPY::Immediate::value:
		{
			using OP = CPX::Immediate;
			beginInstruction<OP>();
			byte data = memory.memImmediate();
			compare(state.y, data);
			endInstruction<OP>(state.y, data);
			break;
		}
		case CPY::ZeroPage::value:
		{
			using OP = CPY::ZeroPage;
			beginInstruction<OP>();
			byte data = memory.memZeroPage();
			compare(state.y, data);
			endInstruction<OP>(state.y, data);
			break;
		}
		case CPY::Absolute::value:
		{
			using OP = CPY::Absolute;
			beginInstruction<OP>();
			byte data = memory.memAbsolute();
			compare(state.y, data);
			endInstruction<OP>(state.y, data);
			break;
		}
		case BMI::value:
		{
			beginInstruction<BMI>();
			branchIf(Status::NegativeResult, true);
			endInstruction<BMI>();
			break;
		}
		case BNE::value:
		{
			beginInstruction<BNE>();
			branchIf(Status::ZeroResult, false);
			endInstruction<BNE>();
			break;
		}
		case CLD::value:
		{
			beginInstruction<CLD>();
			updateStatus(Status::DecimalMode, false);
			endInstruction<CLD>();
			break;
		}
		case CLV::value:
		{
			beginInstruction<CLV>();
			updateStatus(Status::Overflow, false);
			endInstruction<CLV>();
			break;
		}
		case NOP::value:
		{
			beginInstruction<NOP>();
			endInstruction<NOP>();
			break;
		}
		case BEQ::value:
		{
			beginInstruction<BEQ>();
			branchIf(Status::ZeroResult, true);
			endInstruction<BEQ>();
			break;
		}
		case SED::value:
		{
			beginInstruction<SED>();
			updateStatus(Status::DecimalMode, true);
			endInstruction<SED>();
			break;
		}
		default:
		{
			std::cout << std::endl << std::hex << "Unsupported opcode \""
					  << std::setw(2) << static_cast<int>(opcode)
					  << "\", stopping." << std::endl;
			exit(1);
			break;
		}
	}
	std::cout << std::endl;
}

template<typename Memory, typename Mapping, bool DecimalMode>
void Core<Memory, Mapping, DecimalMode>::interruptReset()
{
	state.opcodeResult = 0;
	state.sp = -3; // cycle 0: sp = 0, then gets decremented 3 times, look more into this
	state.p = 0x24; // TODO: Properly configure the status flags
	state.pc = memory.readMemAddress(0xfffc);
	state.pc = 0xc000; // TODO: This is only for nestest.nes "auto mode"

	state.totalCycles = 7;
}

template class mos6502::Core<Mem6502<NESMemory>, NESMemory, false>;

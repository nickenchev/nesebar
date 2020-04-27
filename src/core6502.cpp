#include <iostream>
#include <string>
#include <vector>
#include "core6502.hpp"
#include "nesmemory.hpp"
#include "opcodes.hpp"

using namespace mos6502;

template<typename MemType, bool DecimalMode>
Core<MemType, DecimalMode>::Core(MemType &memory) : memory(memory)
{
	p = 0x34;
	a = x = y = 0;
	sp = 0;
	pc = 0;

	memory.memWrite(0x4017, 0); // frame IRQ enable
	memory.memWrite(0x4015, 0); // disable all channels
	for (MemAddress addr = 0x4000; addr <= 0x400f; ++addr)
	{
		memory.memWrite(addr, 0);
	}
	for (MemAddress addr = 0x4010; addr <= 0x4013; ++addr)
	{
		memory.memWrite(addr, 0);
	}
}

template<typename MemType, bool DecimalMode>
bool Core<MemType, DecimalMode>::step()
{
	using namespace mos6502::opcodes;

	bool keepGoing = true;

	logInfo();

	std::cout << '$' << std::hex << std::setfill('0')
			  << std::setw(4) << pc.value << ": ";
	byte opcode = fetchByte();

	switch (opcode)
	{
		case PHP::value:
		{
			beginInstruction<PHP>();
			stackPush(p);
			endInstruction<PHP>();
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
			MemAddress jumpAddr = addressAbsolute();
			stackPushAddress(pc);
			pc = jumpAddr;
			endInstruction<JSR>();
			break;
		}
		case BIT::ZeroPage::value:
		{
			beginInstruction<BIT::ZeroPage>();
			byte operand = readZeroPage();
			updateStatus(Status::NegativeResult,
						 checkBit(operand, status_int(Status::NegativeResult)));
			updateStatus(Status::Overflow,
						 checkBit(operand, status_int(Status::Overflow)));
			opcodeResult = operand & a;
			endInstruction<BIT::ZeroPage>();
			break;
		}
		case AND::Immediate::value:
		{
			beginInstruction<AND::Immediate>();
			setA(a & memImmediate());
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
			pc = addressAbsolute();
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
			pc = stackPopAddress();
			endInstruction<RTS>();
			break;
		}
		case PLA::value:
		{
			beginInstruction<PLA>();
			setA(stackPop());
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
		case STA::ZeroPage::value:
		{
			beginInstruction<STA::ZeroPage>();
			writeZeroPage(fetchByte(), a);
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
			writeZeroPage(fetchByte(), x);
			endInstruction<STX::ZeroPage>();
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
			setSP(x);
			endInstruction<TXS>();
			break;
		}
		case LDX::Immediate::value:
		{
			beginInstruction<LDX::Immediate>();
			x = memImmediate();
			endInstruction<LDX::Immediate>();
			break;
		}
		case LDA::Immediate::value:
		{
			beginInstruction<LDA::Immediate>();
			setA(memImmediate());
			endInstruction<LDA::Immediate>();
			break;
		}
		case LDA::Absolute::value:
		{
			beginInstruction<LDA::Absolute>();
			setA(memAbsolute());
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
			beginInstruction<CMP::Immediate>();
			opcodeResult = a - memImmediate();
			endInstruction<CMP::Immediate>();
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
			keepGoing = false;
			break;
		}
	}
	std::cout << std::endl;
	return keepGoing;
}

template<typename MemType, bool DecimalMode>
void Core<MemType, DecimalMode>::interruptReset()
{
	opcodeResult = 0;
	sp = -3; // cycle 0: sp = 0, then gets decremented 3 times, look more into this
	p = 0x24; // TODO: Properly configure the status flags
	pc = readMemAddress(0xfffc);
	pc = 0xc000; // TODO: This is only for nestest.nes "auto mode"

	totalCycles = 7;
}

template class mos6502::Core<NESMemory, false>;

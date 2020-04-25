#include <string>
#include "core6502.hpp"
#include "nesmemory.hpp"

using namespace mos6502;

template<typename MemType, bool DecimalMode>
Core<MemType, DecimalMode>::Core(MemType &memory) : memory(memory)
{
	x = y = a = 0;
	cycles = 0;
	byteStep = 0;
	flagsAffected = 0;
	instructionResult = 0;

	for (MemAddress addr = 0x4000; addr <= 0x400F; ++addr)
	{
		memory.memWrite(addr, 0);
	}
	memory.memWrite(0x4015, 0); // disable all channels
	memory.memWrite(0x4017, 0); // frame IRQ enable
}

template<typename MemType, bool DecimalMode>
bool Core<MemType, DecimalMode>::step()
{
	using namespace mos6502::OpCodes;

	bool keepGoing = true;
	byte opcode = fetchByte();

	switch (opcode)
	{
		/*
		case ADC_IMMED:
		{
			setInstruction(ADC_IMMED);
			addWithCarry(a, memAbsolute());
			break;
		}
		case ADC_ZERO:
		{
			setInstruction(ADC_ZERO);
			addWithCarry(a, memZeroPage());
			break;
		}
		case ADC_ZERO_X:
		{
			setInstruction(ADC_ZERO_X);
			addWithCarry(a, memZeroPageX());
			break;
		}
		case ADC_ABS:
		{
			setInstruction(ADC_ABS);
			addWithCarry(a, memAbsolute());
			break;
		}
		case ADC_ABS_X:
		{
			setInstruction(ADC_ABS_X);
			addWithCarry(a, memAbsoluteX());
			break;
		}
		case ADC_ABS_Y:
		{
			setInstruction(ADC_ABS_Y);
			addWithCarry(a, memAbsoluteY());
			break;
		}
		case ADC_IND_X:
		{
			setInstruction(ADC_IND_X);
			addWithCarry(a, memIndexedIndirect());
			break;
		}
		case ADC_IND_Y:
		{
			setInstruction(ADC_IND_Y);
			addWithCarry(a, memIndirectIndexed());
			break;
		}
		case BRK:
		{
			setInstruction(BRK);
			setStatus(CPUStatus::BreakCommand);
			break;
		}
		case ORA_IND_X:
		{
			setInstruction(ORA_IND_X);
			a = a | memIndexedIndirect();
			break;
		}
		case ORA_ZERO:
		{
			setInstruction(ORA_ZERO);
			a = a | memZeroPage();
			break;
		}
		case ORA_IMMED:
		{
			setInstruction(ORA_IMMED);
			a = a | memImmediate();
			break;
		}
		case ORA_ABS:
		{
			setInstruction(ORA_ABS);
			a = a | memAbsolute();
			break;
		}
		case ORA_IND_Y:
		{
			setInstruction(ORA_IND_Y);
			a = a | memIndirectIndexed();
			break;
		}
		case ORA_ZERO_X:
		{
			setInstruction(ORA_ZERO_X);
			a = a | memZeroPageX();
			break;
		}
		case ORA_ABS_Y:
		{
			setInstruction(ORA_ABS_Y);
			a = a | memAbsoluteY();
			break;
		}
		case ORA_ABS_X:
		{
			setInstruction(ORA_ABS_X);
			a = a | memAbsoluteX();
			break;
		}
		case TXS:
		{
			setInstruction(TXS);
			sp = x;
			break;
		}
		case LDA_ABS:
		{
			setInstruction(LDA_ABS);
			a = memAbsolute();
			break;
		}
		case SEI:
		{
			setInstruction(SEI);
			setStatus(CPUStatus::InterruptDisable);
			break;
		}
		*/
		case LDX_IMMED:
		{
			setInstruction(LDX_IMMED);
			x = memImmediate();

			handleFlags<LDX_IMMED.flagsAffected>();
			break;
		}
		case CLD:
		{
			setInstruction(CLD);
			status[status_int(Status::DecimalMode)] = false;
			break;
		}
		default:
		{
			std::cout << std::hex << "Invalid opcode \"" << std::setw(2)
					  << (int)opcode << "\", stopping." << std::endl;
			keepGoing = false;
			break;
		}
	}
	pc += byteStep;

	return keepGoing;
}

template<typename MemType, bool DecimalMode>
void Core<MemType, DecimalMode>::interruptReset()
{
	sp = 0xfd;
	pc = readMemAddress(0xfffc);
}

template class mos6502::Core<NESMemory, false>;

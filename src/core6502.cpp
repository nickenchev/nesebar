#include <string>
#include <iostream>
#include <iomanip>
#include "core6502.hpp"
#include "nesmemory.hpp"

using namespace mos6502;

template<typename MemType, bool DecimalMode>
Core<MemType, DecimalMode>::Core(MemType &memory) : memory(memory)
{
	x = y = a = 0;

	for (mem_address addr = 0x4000; addr <= 0x400F; ++addr)
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
	byte opcode = readByte(pc);
	logInstruction(pc, opcode);

	switch (opcode)
	{
		case ADC_IMMED:
		{
			setInstruction(ADC_IMMED);
			addWithCarry(memAbsolute());
			break;
		}
		case ADC_ZERO:
		{
			setInstruction(ADC_ZERO);
			addWithCarry(memZeroPage());
			break;
		}
		case ADC_ZERO_X:
		{
			setInstruction(ADC_ZERO_X);
			addWithCarry(memZeroPageX());
			break;
		}
		case ADC_ABS:
		{
			setInstruction(ADC_ABS);
			addWithCarry(ADC_ABS);
			break;
		}
		case ADC_ABS_X:
		{
			setInstruction(ADC_ABS_X);
			addWithCarry(ADC_ABS_X);
			break;
		}
		case ADC_ABS_Y:
		{
			setInstruction(ADC_ABS_Y);
			addWithCarry(ADC_ABS_Y);
			break;
		}
		case ADC_IND_X:
		{
			setInstruction(ADC_IND_X);
			addWithCarry(memIndexedIndirect());
			break;
		}
		case ADC_IND_Y:
		{
			setInstruction(ADC_IND_Y);
			addWithCarry(memIndirectIndexed());
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
		case SEI:
		{
			setInstruction(SEI);
			setStatus(CPUStatus::InterruptDisable);
			break;
		}
		case CLD:
		{
			setInstruction(CLD);
			clearStatus(CPUStatus::DecimalMode);
			break;
		}
		default:
		{
			keepGoing = false;
			break;
		}
	}
	updateStatusFlags();
	pc += byteStep;

	return keepGoing;
}

template<typename MemType, bool DecimalMode>
void Core<MemType, DecimalMode>::updateStatusFlags()
{
	if (a == 0) setStatus(CPUStatus::ZeroResult);
	if (checkBit(a, 7)) setStatus(CPUStatus::NegativeResult);
}

template<typename MemType, bool DecimalMode>
void Core<MemType, DecimalMode>::interruptReset()
{
	sp = 0xfd;
	pc = readMemAddress(0xfffc);
}

void logInstruction(const mem_address &pc, const byte &opcode)
{
	auto &inst = mos6502::OpCodes::opCodeMap.at(opcode);

	std::cout << std::hex << std::setfill('0') << std::setw(4) << pc.value;
	std::cout << ' ' << std::setw(2) << (int)opcode << ' ';
	std::cout << inst->name << std::endl;
}

template class mos6502::Core<NESMemory, false>;

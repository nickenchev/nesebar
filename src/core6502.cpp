#include <string>
#include <iostream>
#include <iomanip>
#include "core6502.hpp"
#include "nesmemory.hpp"

using namespace mos6502;

template<typename MemType>
Core<MemType>::Core(MemType &memory) : memory(memory)
{
	x = y = a = 0;

	for (mem_address addr = 0x4000; addr <= 0x400F; ++addr)
	{
		memory.memWrite(addr, 0);
	}
	memory.memWrite(0x4015, 0); // disable all channels
	memory.memWrite(0x4017, 0); // frame IRQ enable
}

template<typename MemType>
bool Core<MemType>::step()
{
	using namespace mos6502::OpCodes;
	using namespace std;

	bool keepGoing = true;
	byte opcode = readByte(pc);
	logInstruction(pc, opcode);

	switch (opcode)
	{
		case ADC_IMMED:
		{
			setInstruction(ADC_IMMED);
			a += memImmediate();
			if (getStatus(CPUStatus::Carry)) ++a;
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
			updateStatusFlags();
			break;
		}
		case ORA_ZERO:
		{
			setInstruction(ORA_ZERO);
			a = a | memZeroPage();
			updateStatusFlags();
			break;
		}
		case ORA_IMMED:
		{
			setInstruction(ORA_IMMED);
			a = a | memImmediate();
			updateStatusFlags();
			break;
		}
		case ORA_ABS:
		{
			setInstruction(ORA_ABS);
			a = a | memAbsolute();
			updateStatusFlags();
			break;
		}
		case ORA_IND_Y:
		{
			setInstruction(ORA_IND_Y);
			a = a | memIndirectIndexed();
			updateStatusFlags();
			break;
		}
		case ORA_ZERO_X:
		{
			setInstruction(ORA_ZERO_X);
			a = a | memZeroPageX();
			updateStatusFlags();
			break;
		}
		case ORA_ABS_Y:
		{
			setInstruction(ORA_ABS_Y);
			a = a | memAbsoluteY();
			updateStatusFlags();
			break;
		}
		case ORA_ABS_X:
		{
			setInstruction(ORA_ABS_X);
			a = a | memAbsoluteX();
			updateStatusFlags();
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
	pc += byteStep;

	return keepGoing;
}

void logInstruction(const mem_address &pc, const byte &opcode)
{
	auto name = mos6502::OpCodes::opCodeMap.at(opcode)->name;

	std::cout << std::hex << std::setfill('0') << std::setw(4) << pc.value;
	std::cout << ' ' << std::setw(2) << (int)opcode << ' ';
	std::cout << name << std::endl;
}

template<typename MemType>
void Core<MemType>::updateStatusFlags()
{
	if (a == 0) setStatus(CPUStatus::ZeroResult);
	if (checkBit(a, 7)) setStatus(CPUStatus::NegativeResult);
}

template <typename MemType>
void Core<MemType>::interruptReset()
{
	sp = 0xfd;
	pc = readMemAddress(0xfffc);
}

template class mos6502::Core<NESMemory>;

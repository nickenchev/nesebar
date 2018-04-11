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
	bool keepGoing = true;
	byte opCode = readByte(pc);
	switch (opCode)
	{
		case 0x00:
		{
			instruction.begin("BRK", 1, 7);
			setStatus(CPUStatus::BreakCommand);
			break;
		}
		case 0x01:
		{
			instruction.begin("ORA", 2, 6);
			a = a | memAbsolute() + x;
			updateStatusFlags();
			break;
		}
		case 0x05:
		{
			instruction.begin("ORA", 2, 3);
			a = a | memZeroPage();
			updateStatusFlags();
			break;
		}
		case 0x09:
		{
			instruction.begin("ORA", 2, 2);
			a = a | memImmediate();
			updateStatusFlags();
			break;
		}
		case 0x0d:
		{
			instruction.begin("ORA", 3, 4);
			a = a | memAbsolute();
			updateStatusFlags();
			break;
		}
		case 0x15:
		{
			instruction.begin("ORA", 2, 4);
			a = a | memZeroPage() + x;
			updateStatusFlags();
			break;
		}
		case 0x19:
		{
			instruction.begin("ORA", 3, 4);
			a = a | memAbsoluteY();
			updateStatusFlags();
			break;
		}
		case 0x1d:
		{
			instruction.begin("ORA", 3, 4);
			a = a | memAbsoluteX();
			updateStatusFlags();
			break;
		}
		case 0x78:
		{
			instruction.begin("SEI", 1, 2);
			setStatus(CPUStatus::InterruptDisable);
			break;
		}
		case 0xa9:
		{
			instruction.begin("LDA", 2, 2);
			break;
		}
		case 0xd8:
		{
			instruction.begin("CLD", 1, 1);
			clearStatus(CPUStatus::DecimalMode);
			break;
		}
		default:
		{
			keepGoing = false;
			break;
		}
	}

	logInstruction(instruction, pc, opCode);

	pc.value += instruction.getStepSize();
	instruction.end();

	return keepGoing;
}

void logInstruction(const Instruction &inst, const mem_address &pc, const byte &opCode)
{
	std::cout << std::hex << std::setfill('0') << std::setw(4) << pc.value;
	std::cout << ' ' << std::setw(2) << (int)opCode << ' ';
	std::cout << inst.getName() << std::endl;
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

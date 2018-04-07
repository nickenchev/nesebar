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
	pc = readMemAddress(mem_address{0xfffc});
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
		default:
		{
			keepGoing = false;
			break;
		}
	}
	pc.value += instruction.getStepSize();
	instruction.end();

	std::cout << std::hex << std::setfill('0') <<
		std::setw(2) << (int)opCode << ' ';
	std::cout << instruction.getName() << std::endl;

	return keepGoing;
}

template<typename MemType>
void Core<MemType>::updateStatusFlags()
{
	if (a == 0) setStatus(CPUStatus::ZeroResult);
	if (checkBit(a, 7)) setStatus(CPUStatus::NegativeResult);
}

template class mos6502::Core<NESMemory>;

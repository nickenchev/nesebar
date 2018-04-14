#include <string>
#include <iostream>
#include <iomanip>
#include "core6502.hpp"
#include "nesmemory.hpp"
#include "opcodes.hpp"

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

	switch (opcode)
	{
		case get<0>(BRK):
		{
			cycles = 7;
			pcStep = 1;
			setStatus(CPUStatus::BreakCommand);
			break;
		}
		case get<0>(ORA_IND_X):
		{
			cycles = 6;
			pcStep = 2;
			a = a | memIndexedIndirect();
			updateStatusFlags();
			break;
		}
		case get<0>(ORA_ZERO):
		{
			cycles = 3;
			pcStep = 2;
			a = a | memZeroPage();
			updateStatusFlags();
			break;
		}
		case get<0>(ORA_IMMED):
		{
			cycles = 2;
			pcStep = 2;
			a = a | memImmediate();
			updateStatusFlags();
			break;
		}
		case get<0>(ORA_ABS):
		{
			cycles = 4;
			pcStep = 3;
			a = a | memAbsolute();
			updateStatusFlags();
			break;
		}
		case get<0>(ORA_IND_Y):
		{
			cycles = 5;
			pcStep = 2;
			a = a | memIndirectIndexed();
			updateStatusFlags();
			break;
		}
		case get<0>(ORA_ZERO_X):
		{
			cycles = 4;
			pcStep = 2;
			a = a | memZeroPageX();
			updateStatusFlags();
			break;
		}
		case get<0>(ORA_ABS_Y):
		{
			cycles = 4;
			pcStep = 3;
			a = a | memAbsoluteY();
			updateStatusFlags();
			break;
		}
		case 0x1d:
		{
			cycles = 4;
			pcStep = 3;
			a = a | memAbsoluteX();
			updateStatusFlags();
			break;
		}
		case get<0>(SEI):
		{
			cycles = 2;
			pcStep = 1;
			setStatus(CPUStatus::InterruptDisable);
			break;
		}
		case 0xa9:
		{
			//instruction.begin(opcode, "LDA", 2, 2);
			break;
		}
		case get<0>(CLD):
		{
			cycles = 1;
			pcStep = 1;
			clearStatus(CPUStatus::DecimalMode);
			break;
		}
		default:
		{
			keepGoing = false;
			break;
		}
	}
	logInstruction(pc, opcode);
	pc += pcStep;

	return keepGoing;
}

void logInstruction(const mem_address &pc, const byte &opcode)
{
	std::string name = mos6502::OpCodes::opCodeMap[opcode];

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

template <typename MemType>
typename Core<MemType>::OpMap Core<MemType>::opcodeMap = {
	{ 0x00, "BRK" },
	{ 0x01, "ORA" },
	{ 0x05, "ORA" },
	{ 0x09, "ORA" },
	{ 0x0d, "ORA" },
	{ 0x11, "ORA" },
	{ 0x15, "ORA" },
	{ 0x19, "ORA" },
	{ 0x1d, "ORA" },
};

template class mos6502::Core<NESMemory>;

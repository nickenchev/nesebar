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
	bool keepGoing = true;
	byte opcode = readByte(pc);
	using namespace mos6502;
	switch (opcode)
	{
		case OpCodes::BRK:
		{
			cycles = 7;
			pcStep = 1;
			setStatus(CPUStatus::BreakCommand);
			break;
		}
		case 0x01:
		{
			cycles = 6;
			pcStep = 2;
			a = a | memIndexedIndirect();
			updateStatusFlags();
			break;
		}
		case 0x05:
		{
			cycles = 3;
			pcStep = 2;
			a = a | memZeroPage();
			updateStatusFlags();
			break;
		}
		case 0x09:
		{
			cycles = 2;
			pcStep = 2;
			a = a | memImmediate();
			updateStatusFlags();
			break;
		}
		case 0x0d:
		{
			cycles = 4;
			pcStep = 3;
			a = a | memAbsolute();
			updateStatusFlags();
			break;
		}
		case 0x11:
		{
			cycles = 5;
			pcStep = 2;
			a = a | memIndirectIndexed();
			updateStatusFlags();
			break;
		}
		case 0x15:
		{
			cycles = 4;
			pcStep = 2;
			a = a | memZeroPageX();
			updateStatusFlags();
			break;
		}
		case 0x19:
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
		case OpCodes::SEI:
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
		case OpCodes::CLD:
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
	pc += pcStep;

	return keepGoing;
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

#include <string>
#include <iostream>
#include <iomanip>
#include "cpu.hpp"

CPUCore::CPUCore(const std::vector<byte> &program) : program(program)
{
	pc = 0;
	sp = 0;
	// set map to nullptr
	for (memAddress addr = 0; addr < cpuMemSize; ++addr)
	{
		memoryMap[addr] = nullptr;
	}

	// map RAM 4 times to emulate mirroring
	memAddress memAddr = 0;
	for (int x = 0; x < 4; ++x)
	{
		for (memAddress ramAddr = 0; ramAddr < ramSize; ++ramAddr)
		{
			memoryMap[memAddr++] = &ram[ramAddr];
		}
	}
}

bool CPUCore::step()
{
	bool keepGoing = false;
	if (pc < static_cast<size_t>(program.size()))
	{
		keepGoing = true;
		const byte *opCode = &program[pc];

		std::string instruction;
		short stepSize = 1;
		short cycles = 1;
		switch (opCode[0])
		{
			case 0x00:
			{
				instruction = "BRK";
				setStatus(CPUStatus::BreakCommand);
				break;
			}
			case 0x01:
			{
				instruction = "ORA";
				cycles = 6;
				stepSize = 2;
				break;
			}
			case 0x02:
			{
				instruction = "STP";
				break;
			}
			case 0x78:
			{
				cycles = 2;
				instruction = "SEI";
				setStatus(CPUStatus::InterruptDisable);
				break;
			}
			default:
			{
				instruction = "Unknown Instruction";
				keepGoing = false;
				break;
			}
		}
		pc += stepSize;

		std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)*opCode << ' ';
		std::cout << instruction << std::endl;
	}
	return keepGoing;
}

byte CPUCore::memRead(byte address) const
{
	return 0;
}

void CPUCore::memWrite(byte address, byte data)
{
}

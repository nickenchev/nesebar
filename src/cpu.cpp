#include <string>
#include <iostream>
#include <iomanip>
#include "cpu.hpp"

CPUCore::CPUCore(const std::vector<byte> &program) : program(program)
{
	pc = 0;
	sp = 0;
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
		switch (*opCode)
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

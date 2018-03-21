#include <string>
#include <iostream>
#include <iomanip>
#include "cpu.hpp"

CPUCore::CPUCore(std::vector<byte> &program) : program(program)
{
	pc = 0;
	sp = 0;

	// map RAM 4 times to emulate mirroring
	memAddress memAddr = 0;
	for (int x = 0; x < 4; ++x)
	{
		for (memAddress ramAddr = 0; ramAddr < ramSize; ++ramAddr)
		{
			memoryMap[memAddr++] = &ram[ramAddr];
		}
	}

	// PPU registers / PPU mirroring
	short ppuIndex = 0;
	for (memAddress x = 0x2000; x <= 0x3fff; ++x)
	{
		memoryMap[x] = &ppuRegisters[ppuIndex++];
		if (ppuIndex % 8 == 0)
		{
			ppuIndex = 0;
		}
	}

	// APU Registers
	short apuIndex = 0;
	for (memAddress x = 0x4000; x <= 0x4017; ++x)
	{
		memoryMap[x] = &apuRegisters[apuIndex++];
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
				A = A | memAbsolute() + X;
				updateStatusFlags();
				stepSize.setup(2, 6);
				break;
			}
			case 0x02:
			{
				instruction = "STP";
				break;
			}
			case 0x05:
			{
				instruction = "ORA";
				A = A | memZeroPage();
				updateStatusFlags();
				stepSize.setup(2, 3);
				break;
			}
			case 0x09:
			{
				instruction = "ORA";
				A = A | memImmediate();
				updateStatusFlags();
				stepSize.setup(2, 2);
				break;
			}
			case 0x0d:
			{
				instruction = "ORA";
				A = A | memAbsolute();
				updateStatusFlags();
				stepSize.setup(3, 4);
				break;
			}
			case 0x15:
			{
				instruction = "ORA";
				A = A | memZeroPage() + X;
				updateStatusFlags();
				stepSize.setup(2, 4);
				break;
			}
			case 0x1d:
			{
				break;
			}
			default:
			{
				instruction = "Unknown Instruction";
				keepGoing = false;
				break;
			}
		}
		pc += stepSize.getStepSize();

		std::cout << std::hex << std::setfill('0') << std::setw(2)
				  << (int)*opCode << ' ';
		std::cout << instruction << std::endl;
	}

	return keepGoing;
}

memAddress CPUCore::combine(const byte &highByte,
							const byte &lowByte) const
{
	memAddress address = 0;
	address = (address | highByte) << 8;
	address = address | lowByte;
	return address;
}

bool CPUCore::checkBit(int bitNumber) const
{
	return A & (1 << bitNumber);
}

void CPUCore::updateStatusFlags()
{
	if (A == 0) setStatus(CPUStatus::ZeroResult);
	if (checkBit(7)) setStatus(CPUStatus::NegativeResult);
}

byte CPUCore::highByte(const memAddress &addr) const
{
	return addr >> 8;
}

byte CPUCore::lowByte(const memAddress &addr) const
{
	return addr & 0b0000000011111111;
}

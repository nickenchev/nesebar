#include <string>
#include <iostream>
#include <iomanip>
#include "cpu.hpp"

CPUCore::CPUCore(std::vector<byte> &program) : program(program)
{
	pc = 0;
	sp = 0;

	// map RAM 4 times to emulate mirroring
	mem_address memAddr = 0;
	for (int x = 0; x < 4; ++x)
	{
		for (mem_address ramAddr = 0; ramAddr < ramSize; ++ramAddr.value)
		{
			memoryMap[memAddr.value++] = &ram[ramAddr.value];
		}
	}

	// PPU registers / PPU mirroring
	short ppuIndex = 0;
	for (mem_address x = 0x2000; x <= 0x3fff; ++x.value)
	{
		memoryMap[x.value] = &ppuRegisters[ppuIndex++];
		if (ppuIndex % 8 == 0)
		{
			ppuIndex = 0;
		}
	}

	// APU Registers
	short apuIndex = 0;
	for (mem_address x = 0x4000; x <= 0x4017; ++x.value)
	{
		memoryMap[x.value] = &apuRegisters[apuIndex++];
	}
}


bool CPUCore::step()
{
	bool keepGoing = false;
	if (pc < static_cast<size_t>(program.size()))
	{
		keepGoing = true;
		const byte *opCode = &program[pc.value];

		switch (opCode[0])
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

		std::cout << std::hex << std::setfill('0') << std::setw(2)
				  << (int)*opCode << ' ';
		std::cout << instruction.getName() << std::endl;
	}

	return keepGoing;
}

bool CPUCore::checkBit(int bitNumber) const
{
	return a & (1 << bitNumber);
}

void CPUCore::updateStatusFlags()
{
	if (a == 0) setStatus(CPUStatus::ZeroResult);
	if (checkBit(7)) setStatus(CPUStatus::NegativeResult);
}

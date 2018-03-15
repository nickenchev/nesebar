#include <string>
#include <iostream>
#include <iomanip>
#include "cpu.h"

CPUCore::CPUCore()
{
}

void CPUCore::execue(byte opCode)
{
	std::string instruction;
	switch (opCode)
	{
		case 0x00:
		{
			instruction = "BRK";
			setStatus(CPUStatus::Break);
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
		default:
		{
			instruction = "???";
			break;
		}
	}

	std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)opCode << ' ';
	std::cout << instruction << std::endl;
}

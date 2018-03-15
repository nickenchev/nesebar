#include <string>
#include <iostream>
#include <iomanip>
#include "cpu.hpp"

CPUCore::CPUCore()
{
}

void CPUCore::execue(byte opCode)
{
	std::string instruction;
	switch (static_cast<OpCode>(opCode))
	{
		case OpCode::BRK:
		{
			instruction = "BRK";
			setStatus(CPUStatus::BreakCommand);
			break;
		}
		case OpCode::ORA:
		{
			instruction = "ORA";
			break;
		}
		case OpCode::STP:
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

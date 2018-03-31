#include <string>
#include <iostream>
#include <iomanip>
#include "core6502.hpp"

template<typename M>
Core6502<M>::Core6502(std::vector<byte> &program) : program(program)
{
	pc = 0;
	sp = 0;
}

template<typename M>
bool Core6502<M>::step()
{
	bool keepGoing = false;
	if (pc < static_cast<size_t>(program.size()))
	{
		keepGoing = true;
		const byte *opCode = &program[pc.value];

		/*
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
		*/
		pc.value += instruction.getStepSize();
		instruction.end();

		std::cout << std::hex << std::setfill('0') << std::setw(2)
				  << (int)*opCode << ' ';
		std::cout << instruction.getName() << std::endl;
	}

	return keepGoing;
}

template<typename M>
void Core6502<M>::updateStatusFlags()
{
	if (a == 0) setStatus(CPUStatus::ZeroResult);
	if (checkBit(a, 7)) setStatus(CPUStatus::NegativeResult);
}

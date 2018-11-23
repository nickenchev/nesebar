#include "nes.hpp"

void NES::run()
{
	cpu.reset();
	while (cpu.step())
	{
	}
}

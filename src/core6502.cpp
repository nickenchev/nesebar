#include <iostream>
#include <string>
#include <vector>
#include "core6502.hpp"
#include "nesmemory.hpp"
#include "opcodes.hpp"

using namespace mos6502;

template<typename MemType, bool DecimalMode>
Core<MemType, DecimalMode>::Core(MemType &memory) : memory(memory)
{
	x = y = a = 0;
	cycles = 0;
	byteStep = 0;
	opcodeResult = 0;

	for (MemAddress addr = 0x4000; addr <= 0x400F; ++addr)
	{
		memory.memWrite(addr, 0);
	}
	memory.memWrite(0x4015, 0); // disable all channels
	memory.memWrite(0x4017, 0); // frame IRQ enable
}

template<typename MemType, bool DecimalMode>
bool Core<MemType, DecimalMode>::step()
{
	using namespace mos6502::opcodes;

	bool keepGoing = true;

	std::cout << '$' << std::hex << std::setfill('0')
				<< std::setw(4) << pc.value << ": ";
	byte opcode = fetchByte();

	switch (opcode)
	{
		case BPL::value:
		{
			beginInstruction<BPL>();
			if (!isStatus(Status::NegativeResult))
			{
				short extraCycles = 1;
				signed_byte branch = static_cast<signed_byte>(fetchByte());

				// page cross requires extra cycle
				if (pc.addSigned(branch)) extraCycles++;
				addCycles(extraCycles);
			}
			endInstruction<BPL>();
			break;
		}
		case JSR::value:
		{
			beginInstruction<JSR>();
			endInstruction<JSR>();
			break;
		}
		case JMP::value:
		{
			beginInstruction<JMP>();
			pc = addressAbsolute();
			endInstruction<JMP>();
			break;
		}
		case SEI::value:
		{
			beginInstruction<SEI>();
			status[status_int(Status::InterruptDisable)] = true;
			endInstruction<SEI>();
			break;
		}
		case STA::Absolute::value:
		{
			beginInstruction<STA::Absolute>();
			memory.memWrite(memAbsolute(), a);
			endInstruction<STA::Absolute>();
			break;
		}
		case STX::ZeroPage::value:
		{
			beginInstruction<STX::ZeroPage>();
			writeZeroPage(fetchByte(), x);
			endInstruction<STX::ZeroPage>();
			break;
		}
		case TXS::value:
		{
			beginInstruction<TXS>();
			setSP(x);
			endInstruction<TXS>();
			break;
		}
		case LDX::Immediate::value:
		{
			beginInstruction<LDX::Immediate>();
			x = memImmediate();
			endInstruction<LDX::Immediate>();
			break;
		}
		case LDA::Immediate::value:
		{
			beginInstruction<LDA::Immediate>();
			setA(memImmediate());
			endInstruction<LDA::Immediate>();
			break;
		}
		case LDA::Absolute::value:
		{
			beginInstruction<LDA::Absolute>();
			setA(memAbsolute());
			endInstruction<LDA::Absolute>();
			break;
		}
		case CLD::value:
		{
			beginInstruction<CLD>();
			status[status_int(Status::DecimalMode)] = false;
			endInstruction<CLD>();
			break;
		}
		default:
		{
			std::cout << std::endl << std::hex << "Unsupported opcode \""
					  << std::setw(2) << static_cast<int>(opcode)
					  << "\", stopping." << std::endl;
			keepGoing = false;
			break;
		}
	}
	std::cout << std::endl;
	return keepGoing;
}

template<typename MemType, bool DecimalMode>
void Core<MemType, DecimalMode>::interruptReset()
{
	sp = 0xff;
	pc = readMemAddress(0xfffc);
	pc = 0xc000;
}

template class mos6502::Core<NESMemory, false>;

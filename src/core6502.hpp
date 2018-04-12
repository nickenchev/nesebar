#ifndef CORE6502_H
#define CORE6502_H

#include <iostream>
#include <bitset>
#include <cstdint>
#include <vector>
#include "common.hpp"
#include "mem_address.hpp"
#include "instruction.hpp"

namespace mos6502
{
	enum class CPUStatus
	{
		Carry = 0,
		ZeroResult = 1,
		InterruptDisable = 2,
		DecimalMode = 3,
		BreakCommand = 4,
		Overflow = 5,
		NegativeResult = 6
	};

	template<typename MemType>
	class Core
	{
		constexpr static uint16_t stackAddress = 0x0100;
		constexpr static byte stackSize = 0xff;

		MemType &memory;
		byte a, x, y, sp;
		mem_address pc;
		std::bitset<7> status;
		Instruction instruction;

		void setStatus(CPUStatus flag)
		{
			status.set(static_cast<int>(flag));
		}
		void clearStatus(CPUStatus flag)
		{
			status.reset(static_cast<int>(flag));
		}

		void updateStatusFlags();
		bool checkBit(const byte &reg, short bitNumber) const
		{
			return reg & (1 << bitNumber);
		}

		byte readByte(const mem_address &address)
		{
			return memory.memRead(address);
		}
		byte readNextByte() { return readByte(pc + 1); }

		mem_address readMemAddress(const mem_address &address)
		{
			return mem_address(memory.memRead(address), memory.memRead(address + 1));
		}
		mem_address readNextMemAddress() { return readMemAddress(pc + 1); }

		byte memImmediate()
		{
			return readNextByte();
		}
		byte memAbsolute()
		{
			return memory.memRead(readNextMemAddress());
		}
		byte memAbsoluteX()
		{
			mem_address addr = readNextMemAddress();
			byte page = addr.high();
			addr += x;
			if (addr.high() > page) instruction.increaseCycles(1);
			return memory.memRead(addr);
		}
		byte memAbsoluteY()
		{
			mem_address addr = readNextMemAddress();
			byte page = addr.high();
			addr += y;
			if (addr.high() > page) instruction.increaseCycles(1);
			return memory.memRead(addr);
		}
		
		byte memZeroPage()
		{
			return memory.memRead(readNextByte());
		}
		byte memZeroPageX()
		{
			byte val = readNextByte();
			return memory.memRead(mem_address{val}.addLow(x));
		}

		byte memIndirect()
		{
			mem_address addr = readMemAddress(mem_address{memZeroPage()});
			return memory.memRead(addr);
		}

		byte memIndirectX()
		{
			mem_address addr = readMemAddress(mem_address{memZeroPageX()});
			return memory.memRead(addr);
		}

		void interruptReset();

	public:
		Core(MemType &memory);

		void reset() { interruptReset(); }
		bool step();
	};
};

#endif /* CORE6502_H */

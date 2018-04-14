#ifndef CORE6502_H
#define CORE6502_H

#include <iostream>
#include <bitset>
#include <cstdint>
#include <vector>
#include <map>
#include "common.hpp"
#include "mem_address.hpp"
#include "opcodes.hpp"
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

	template<typename MemType, bool DecimalMode>
	class Core
	{
		constexpr static uint16_t stackAddress = 0x0100;
		constexpr static byte stackSize = 0xff;

		using OpMap = std::map<byte, std::string>;
		static OpMap opcodeMap;
		MemType &memory;
		byte a, x, y, sp;
		mem_address pc;
		std::bitset<7> status;
		short cycles = 0;
		short byteStep = 0;

		void setInstruction(const Instruction &inst)
		{
			cycles = inst.cycles;
			byteStep = inst.byteSize;
		}
		bool getStatus(CPUStatus flag) const
		{
			return status.test(static_cast<int>(flag));
		}
		void setStatus(CPUStatus flag)
		{
			status.set(static_cast<int>(flag));
		}
		void clearStatus(CPUStatus flag)
		{
			status.reset(static_cast<int>(flag));
		}
		short getCarry() const { return getStatus(CPUStatus::Carry) ? 1 : 0; }

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
		char memRelative()
		{
			return readNextByte();
		}
		byte memIndirect()
		{
			mem_address addr = readMemAddress(mem_address{memZeroPage()});
			return memory.memRead(addr);
		}
		byte memAbsolute()
		{
			return memory.memRead(readNextMemAddress());
		}
		byte memAbsoluteX()
		{
			mem_address addr = readNextMemAddress();
			if (addr.add(x)) ++cycles;
			return memory.memRead(addr);
		}
		byte memAbsoluteY()
		{
			mem_address addr = readNextMemAddress();
			if (addr.add(y)) ++cycles;
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

		// indexed addressing
		byte memAbsuluteIndexed()
		{
			mem_address addr = readNextMemAddress();
			if (addr.add(x)) ++cycles;
			return memory.memRead(addr);
		}
		byte memIndexedIndirect()
		{
			mem_address addr = readMemAddress(mem_address{memZeroPageX()});
			return memory.memRead(addr);
		}
		byte memIndirectIndexed()
		{
			mem_address addr = readMemAddress(mem_address{readNextByte()});
			if (addr.add(y)) ++cycles;
			return memory.memRead(addr);
		}

		// interrupts
		void interruptReset();

	public:
		Core(MemType &memory);

		void reset() { interruptReset(); }
		bool step();
	};
};

#endif /* CORE6502_H */

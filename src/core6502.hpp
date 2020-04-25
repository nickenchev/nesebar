#ifndef CORE6502_H
#define CORE6502_H

#include <iostream>
#include <iomanip>
#include <bitset>
#include <cstdint>
#include <vector>

#include "common.hpp"
#include "memaddress.hpp"
#include "opcodes.hpp"
#include "instruction.hpp"

namespace mos6502
{

enum class Status
{
	Carry = 0,
	ZeroResult = 1,
	InterruptDisable = 2,
	DecimalMode = 3,
	BreakCommand = 4,
	Unused = 5,
	Overflow = 6,
	NegativeResult = 7
};

constexpr auto status_int(const Status status)
{
	return static_cast<short>(status);
}


void logInstruction(const MemAddress &pc, const Instruction &inst);

template<typename MemType, bool DecimalMode>
class Core
{
	MemType &memory;
	byte a, x, y, sp;
	MemAddress pc;
	std::bitset<7> status;
	short cycles;
	short byteStep;
	byte instructionResult;
	byte flagsAffected;

	constexpr inline void setInstruction(const Instruction &inst)
	{
		std::cout << std::hex << std::setfill('0') << std::setw(4) << pc.value;
		std::cout << ' ' << std::setw(2) << (int)inst.opcode << ' ';
		std::cout << inst.name << std::endl;

		cycles = inst.cycles;
		byteStep = inst.byteSize;
		flagsAffected = inst.flagsAffected;
	}

	bool checkBit(const byte &reg, short bitNumber) const
	{
		return reg & (1 << bitNumber);
	}

	template<byte value, Status statusFlag>
	constexpr static bool checkBit()
	{
		return value & (1 << static_cast<short>(statusFlag));
	}

	// status management
	template<byte affectedFlags>
	inline void handleFlags()
	{
		if constexpr (checkBit<affectedFlags, Status::NegativeResult>())
		{
			std::cout << "Negative Flag" << std::endl;
			status[status_int(Status::NegativeResult)] = instructionResult < 0;
		}
		if constexpr (checkBit<affectedFlags, Status::ZeroResult>())
		{
			std::cout << "Zero Flag" << std::endl;
			status[status_int(Status::NegativeResult)] = instructionResult == 0;
		}
	}

	bool isStatus(Status flag) const
	{
		return status.test(static_cast<int>(flag));
	}
	short getCarry() const { return isStatus(Status::Carry) ? 1 : 0; }

	// status flag logic
	void updateFlagZero()
	{
		status[status_int(Status::ZeroResult)] = instructionResult == 0;
	}

	void updateFlagNegative()
	{
		status[status_int(Status::NegativeResult)] = instructionResult < 0;
	}

	// Memory access
	byte fetchByte()
	{
		return memory.memRead(pc + 1);
	}
	MemAddress readMemAddress(const MemAddress &address)
	{
		return MemAddress(memory.memRead(address), memory.memRead(address + 1));
	}
	MemAddress readNextMemAddress()
	{
		return readMemAddress(pc + 1);
	}

	// Memory Addressing
	byte memImmediate()
	{
		return fetchByte();
	}
	byte memRelative()
	{
		return fetchByte();
	}
	byte memIndirect()
	{
		MemAddress addr = readMemAddress(MemAddress{memZeroPage()});
		return memory.memRead(addr);
	}
	byte memAbsolute()
	{
		return memory.memRead(readNextMemAddress());
	}
	byte memAbsoluteX()
	{
		MemAddress addr = readNextMemAddress();
		if (addr.add(x)) ++cycles;
		return memory.memRead(addr);
	}
	byte memAbsoluteY()
	{
		MemAddress addr = readNextMemAddress();
		if (addr.add(y)) ++cycles;
		return memory.memRead(addr);
	}
	byte memZeroPage()
	{
		return memory.memRead(fetchByte());
	}
	byte memZeroPageX()
	{
		byte val = fetchByte();
		return memory.memRead(MemAddress{val}.addLow(x));
	}

	// indexed addressing
	byte memAbsuluteIndexed()
	{
		MemAddress addr = readNextMemAddress();
		if (addr.add(x)) ++cycles;
		return memory.memRead(addr);
	}
	byte memIndexedIndirect()
	{
		MemAddress addr = readMemAddress(MemAddress{memZeroPageX()});
		return memory.memRead(addr);
	}
	byte memIndirectIndexed()
	{
		MemAddress addr = readMemAddress(MemAddress{fetchByte()});
		if (addr.add(y)) ++cycles;
		return memory.memRead(addr);
	}

	// Common math
	void addWithCarry(const signed_byte &a, const signed_byte &b)
	{
		if constexpr(DecimalMode)
		{
			// TODO: Decimal mode for other systems
		}
		else
		{
			const signed_byte c = a + b;
			// uint16_t sum = a + data + getCarry();
			// if (sum > 0xff)
			// {
			// 	setStatus(CPUStatus::Carry);
			// }
			// else if (sum > 127 || sum < -128)
			// {
			// 	setStatus(CPUStatus::Overflow);
			// }
			// a = sum & 0xff; // take only 8-bits of 16-bit result
		}
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

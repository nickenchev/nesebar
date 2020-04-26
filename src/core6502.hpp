#ifndef CORE6502_H
#define CORE6502_H

#include <iostream>
#include <iomanip>
#include <bitset>
#include <cstdint>
#include <vector>

#include "common.hpp"
#include "memaddress.hpp"

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


template<typename Memory, bool DecimalMode>
class Core
{
	Memory &memory;
	byte a, x, y, sp;
	MemAddress pc;
	std::bitset<7> status;
	short cycles;
	short byteStep;
	byte opcodeResult;

	inline void setA(byte value)
	{
		a = value;
		opcodeResult = value;
	}
	inline void setSP(byte value) {
		sp = value;
		opcodeResult = value;
	}
	inline void incPC(byte value)
	{
		pc += value;
	}

	template<typename T>
	constexpr inline void beginInstruction()
	{
		std::cout << std::hex << std::setfill('0') << std::setw(4) << pc.value;
		std::cout << ": " << std::setw(2) << (int)T::value << ' ';
		std::cout << T::name;
	}

	template<typename T>
	constexpr inline void endInstruction() 
	{
		cycles = T::cycles;
		byteStep = T::byteSize;

		handleFlags<T::flagsAffected>(opcodeResult);
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
	inline void handleFlags(byte instructionResult)
	{
		if constexpr (affectedFlags != 0)
		{
			if constexpr (checkBit<affectedFlags, Status::NegativeResult>())
			{
				status[status_int(Status::NegativeResult)] = instructionResult < 0;
			}
			if constexpr (checkBit<affectedFlags, Status::ZeroResult>())
			{
				status[status_int(Status::NegativeResult)] = instructionResult == 0;
			}
		}
	}

	void addCycles(short numCycles)
	{
		this->cycles += numCycles;
	}
	bool isStatus(Status flag) const
	{
		return status[status_int(flag)];
	}
	short getCarry() const { return isStatus(Status::Carry) ? 1 : 0; }

	// Memory access
	byte fetchByte()
	{
		byte val = memory.memRead(pc);
		incPC(1);
		return val;
	}
	MemAddress fetchNextMemAddress()
	{
		MemAddress addr = readMemAddress(pc);
		incPC(2);
		return addr;
	}

	MemAddress readMemAddress(const MemAddress &address)
	{
		return MemAddress(memory.memRead(address), memory.memRead(address + 1));
	}

	// Memory Addressing
	byte memImmediate()
	{
		byte data = fetchByte();
		std::cout << " #" << std::hex << static_cast<int>(data);
		return data;
	}
	byte memRelative()
	{
		byte data = fetchByte();
		std::cout << std::hex << static_cast<int>(data);
		return data;
	}
	byte memIndirect()
	{
		MemAddress addr = readMemAddress(MemAddress{memZeroPage()});
		return memory.memRead(addr);
	}
	byte memAbsolute()
	{
		byte data = memory.memRead(fetchNextMemAddress());
		std::cout << " $" << std::hex << static_cast<int>(data);
		return data;
	}
	byte memAbsoluteX()
	{
		MemAddress addr = fetchNextMemAddress();
		if (addr.add(x)) ++cycles;
		return memory.memRead(addr);
	}
	byte memAbsoluteY()
	{
		MemAddress addr = fetchNextMemAddress();
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
		MemAddress addr = fetchNextMemAddress();
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
	Core(Memory &memory);

	void reset() { interruptReset(); }
	bool step();
};

};

#endif /* CORE6502_H */

#ifndef CORE6502_H
#define CORE6502_H

#include <iostream>
#include <iomanip>
#include <bitset>
#include <cstdint>
#include <vector>

#include "common.hpp"
#include "flags.hpp"
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
	static constexpr unsigned int stackStart = 0x0100;

	Memory &memory;
	byte a, x, y, sp, p;
	MemAddress pc;
	short cycles, totalCycles;
	short byteStep;
	byte flagsChanged;
	byte opcodeResult;

	void logInfo()
	{
		std::cout << std::hex << std::uppercase
				  << "A:" << std::setw(2) << static_cast<int>(a)
				  << "\tX:" << std::setw(2) << static_cast<int>(x)
				  << "\tY:" << std::setw(2) << static_cast<int>(y)
				  << "\tSP:" << std::setw(2) << static_cast<int>(sp)
				  << "\tP:" << std::bitset<8>(p)
				  << std::hex << "=" << std::setw(2) << static_cast<int>(p)
				  << std::dec << "\tCycles:" << totalCycles << "\t";
	}

	inline void setA(byte value)
	{
		a = value;
		opcodeResult = value;
	}
	inline void setSP(byte value)
	{
		sp = value;
		opcodeResult = value;
	}
	inline void setP(byte value)
	{
		namespace sb = status_bits;
		constexpr byte retain = sb::E|sb::B; // bits we don't want to affect (5, 4)
		value &= ~retain; // transfer all bits except 5, 4
		p = (p & retain) | value; // set p, keep bits 5, 4 if currently set
	}

	template<typename T>
	constexpr inline void beginInstruction()
	{
		cycles = T::cycles;
		byteStep = T::byteSize;

		std::cout << std::setw(2) << (int)T::value << ' ' << T::name;
	}

	template<typename T>
	constexpr inline void endInstruction(byte operand1 = 0, byte operand2 = 0)
	{
		handleFlags<T::autoFlags ^ T::manualFlags>(operand1, operand2);
		totalCycles += cycles;
	}

	// stack
	void stackPush(byte data)
	{
		memory.memWrite(stackStart + sp--, data);
	}
	byte stackPop()
	{
		MemAddress addr = MemAddress(stackStart) + ++sp;
		byte data = memory.memRead(addr);
		return data;
	}
	void stackPushAddress(MemAddress address)
	{
		stackPush(pc.high());
		stackPush(pc.low());
	}
	MemAddress stackPopAddress()
	{
		byte low = stackPop();
		byte high = stackPop();
		MemAddress addr(low, high);
		return addr;
	}

	bool isStatus(Status flag) const
	{
		return checkBit(p, status_int(flag));
	}

	void updateStatus(Status flag, bool state)
	{
		if (state)
		{
			// set bit
			p |= 1 << status_int(flag);
		}
		else
		{
			// clear bit
			p &= ~(1 << status_int(flag));
		}
	}

	bool checkBit(const byte &operand, short bitNumber) const
	{
		return operand & (1 << bitNumber);
	}

	template<byte value, Status statusFlag>
	constexpr static bool checkBit()
	{
		return value & (1 << static_cast<short>(statusFlag));
	}

	// status management
	template<byte autoFlags>
	constexpr inline void handleFlags(byte operand1, byte operand2)
	{
		if constexpr (autoFlags != 0)
		{
			if constexpr (checkBit<autoFlags, Status::NegativeResult>())
			{
				updateStatus(Status::NegativeResult,
							 checkBit(opcodeResult,
									  status_int(Status::NegativeResult)));
			}
			if constexpr (checkBit<autoFlags, Status::Overflow>())
			{
				// overflow only occurs if operands have different signs
				constexpr byte signBit = 0b10000000;
				const bool isOverflow = (~(operand1 ^ operand2)) & (operand1 ^ opcodeResult) & signBit;
				updateStatus(Status::Overflow, isOverflow);
			}
			if constexpr (checkBit<autoFlags, Status::Carry>())
			{
				uint16_t sum = operand1 + operand2 + isStatus(Status::Carry);
				updateStatus(Status::Carry, sum > 0xff);
			}
			if constexpr (checkBit<autoFlags, Status::Unused>())
			{
				exit(1);
			}
			if constexpr (checkBit<autoFlags, Status::BreakCommand>())
			{
				exit(1);
			}
			if constexpr (checkBit<autoFlags, Status::ZeroResult>())
			{
				updateStatus(Status::ZeroResult, opcodeResult == 0);
			}
		}
	}

	void addCycles(short numCycles)
	{
		this->cycles += numCycles;
	}

	// Memory access
	byte fetchByte()
	{
		byte val = memory.memRead(pc);
		pc.add(1);
		return val;
	}
	MemAddress fetchNextMemAddress()
	{
		MemAddress addr = readMemAddress(pc);
		pc.add(2);
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
		std::cout << " #" << std::hex << std::setw(2)
				  << static_cast<int>(data);
		return data;
	}
	byte memRelative()
	{
		byte data = fetchByte();
		std::cout << std::hex << std::setw(2)
				  << static_cast<int>(data);
		return data;
	}
	byte memIndirect()
	{
		MemAddress addr = readMemAddress(MemAddress{readZeroPage()});
		return memory.memRead(addr);
	}
	MemAddress addressAbsolute()
	{
		MemAddress addr = fetchNextMemAddress();
		std::cout << " $" << std::hex << std::setw(2)
				  << static_cast<uint16_t>(addr.value);
		return addr;
	}
	byte memAbsolute()
	{
		MemAddress addr = fetchNextMemAddress();
		byte data = memory.memRead(addr);
		std::cout << " $" << std::hex << static_cast<uint16_t>(addr.value)
				  << " = " << static_cast<int>(data);
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
	byte readZeroPage()
	{
		byte addr = fetchByte();
		byte data = memory.memRead(addr);
		std::cout << " $" << std::hex << std::setw(2) << static_cast<int>(addr)
				  << " = " << static_cast<int>(data);
		return data;
	}
	void writeZeroPage(byte address, byte value)
	{
		std::cout << " $" << std::setw(2) << std::hex << static_cast<uint16_t>(address)
				  << " = " << static_cast<int>(value);
		memory.memWrite(MemAddress(address), value);
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
	
	// branching
	inline void branchIf(Status flag, bool checkStatus)
	{
		short extraCycles = 1;
		signed_byte branch = static_cast<signed_byte>(fetchByte());

		std::cout << std::hex << " #$" << static_cast<int>(branch)
				  << " ($" << (pc + branch).value << ") ";

		if (isStatus(flag) == checkStatus)
		{
			if (pc.addSigned(branch)) extraCycles++;
			addCycles(extraCycles);
		}
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

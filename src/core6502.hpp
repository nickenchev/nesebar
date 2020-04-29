#ifndef CORE6502_H
#define CORE6502_H

#include <iostream>
#include <iomanip>
#include <bitset>
#include <cstdint>
#include <vector>

#include "common.hpp"
#include "flags.hpp"
#include "mem6502.hpp"
#include "memaddress.hpp"
#include "state.hpp"

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

template<typename Memory, typename Mapping, bool DecimalMode>
class Core
{
	static constexpr unsigned int stackStart = 0x0100;

	State state;
	Memory memory;
	const Mapping &mapping;

	void logInfo()
	{
		std::cout << std::hex << std::uppercase
				  << "A:" << std::setw(2) << static_cast<int>(state.a)
				  << "\tX:" << std::setw(2) << static_cast<int>(state.x)
				  << "\tY:" << std::setw(2) << static_cast<int>(state.y)
				  << "\tSP:" << std::setw(2) << static_cast<int>(state.sp)
				  << "\tP:" << std::bitset<8>(state.p)
				  << std::hex << "=" << std::setw(2) << static_cast<int>(state.p)
				  << std::dec << "\tCycles:" << state.totalCycles << "\t";
	}

	inline byte nextOpcode()
	{
		return memory.fetchByte();
	}

	template<typename T>
	constexpr inline void beginInstruction()
	{
		state.cycles = T::cycles;
		state.byteStep = T::byteSize;

		std::cout << std::setw(2) << (int)T::value << ' ' << T::name;
	}

	template<typename T>
	constexpr inline void endInstruction(byte operand1 = 0, byte operand2 = 0)
	{
		handleFlags<T::autoFlags ^ T::manualFlags>(operand1, operand2);
		state.totalCycles += state.cycles;
	}

	// stack
	void stackPush(byte data)
	{
		memory.write(stackStart + state.sp--, data);
	}
	byte stackPop()
	{
		MemAddress addr = MemAddress(stackStart) + ++state.sp;
		byte data = memory.read(addr);
		return data;
	}
	void stackPushAddress(MemAddress address)
	{
		stackPush(state.pc.high());
		stackPush(state.pc.low());
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
		return checkBit(state.p, status_int(flag));
	}

	void updateStatus(Status flag, bool flagState)
	{
		if (flagState)
		{
			// set bit
			state.p |= 1 << status_int(flag);
		}
		else
		{
			// clear bit
			state.p &= ~(1 << status_int(flag));
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
							 checkBit(state.opcodeResult,
									  status_int(Status::NegativeResult)));
			}
			if constexpr (checkBit<autoFlags, Status::Overflow>())
			{
				// overflow only occurs if operands have different signs
				constexpr byte signBit = 0b10000000;
				const bool isOverflow = (~(operand1 ^ operand2)) & (operand1 ^ state.opcodeResult) & signBit;
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
				updateStatus(Status::ZeroResult, state.opcodeResult == 0);
			}
		}
	}

	void addCycles(short numCycles)
	{
		state.cycles += numCycles;
	}

	// compare
	inline void compare(byte &reg, const byte &data)
	{
		state.opcodeResult = reg - data;
		updateStatus(Status::Carry, data <= reg);
	}
	
	// branching
	inline void branchIf(Status flag, bool checkStatus)
	{
		short extraCycles = 1;
		signed_byte branch = static_cast<signed_byte>(memory.memRelative());

		if (isStatus(flag) == checkStatus)
		{
			if (state.pc.addSigned(branch)) extraCycles++;
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
	Core(const Mapping &mapping);

	Memory &getMemory() { return memory; }
	void reset() { interruptReset(); }
	void step();
};

};

#endif /* CORE6502_H */

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
				  << "\tP:" << std::bitset<8>(state.p)
				  << std::hex << "=" << std::setw(2) << static_cast<int>(state.p)
				  << "\tSP:" << std::setw(2) << static_cast<int>(state.sp)
				  << std::dec << "\tCycles:" << state.totalCycles << "\t";
	}

	inline byte nextOpcode()
	{
		return memory.fetchByte();
	}

	template<typename Opcode, typename F, bool addPageCrossCycles = true>
	constexpr inline void perform(F body)
	{
		beginInstruction<Opcode>();
		if constexpr (Opcode::addressingMode == Addressing::Mode::Implicit)
		{
			body();
		}
		else if constexpr (Opcode::addressingMode == Addressing::Mode::Immediate)
		{
			body(memory.fetchImmediate());
		}
		else if constexpr (Opcode::addressingMode == Addressing::Mode::ZeroPage)
		{
			body(memory.fetchZeroPage());
		}
		else if constexpr (Opcode::addressingMode == Addressing::Mode::ZeroPageX)
		{
			body(memory.fetchZeroPageX());
		}
		else if constexpr (Opcode::addressingMode == Addressing::Mode::ZeroPageY)
		{
			body(memory.fetchZeroPageY());
		}
		else if constexpr (Opcode::addressingMode == Addressing::Mode::Absolute)
		{
			body(memory.fetchAbsolute());
		}
		else if constexpr (Opcode::addressingMode == Addressing::Mode::AbsoluteX)
		{
			body(memory.fetchAbsoluteX());
		}
		else if constexpr (Opcode::addressingMode == Addressing::Mode::AbsoluteY)
		{
			body(memory.fetchAbsoluteY());
		}
		else if constexpr (Opcode::addressingMode == Addressing::Mode::IndexedIndirect)
		{
			body(memory.fetchIndexedIndirect());
		}
		else if constexpr (Opcode::addressingMode == Addressing::Mode::IndirectIndexed)
		{
			body(memory.fetchIndirectIndexed());
		}
		else
		{
			exit(EXIT_FAILURE); // invalid addressing mode
		}
		endInstruction<Opcode, addPageCrossCycles>();
	}

	template<typename T>
	constexpr inline void beginInstruction()
	{
		state.operand1 = state.operand2 = state.opcodeResult = 0;
		state.pageCrossCycles = 0;
		state.cycles = T::cycles;
		state.byteStep = T::byteSize;

		std::cout << std::setw(2) << (int)T::value << ' ' << T::name << std::flush;
	}

	constexpr inline void setOperands(const byte operand1, const byte operand2)
	{
		state.operand1 = operand1;
		state.operand2 = operand2;
	}

	// TODO: Consolidate these two methods
	template<typename T, bool addPageCrossCycles = true>
	constexpr inline void endInstruction()
	{
		handleFlags<T::autoFlags ^ T::manualFlags>(state.operand1, state.operand2);
		state.totalCycles += state.cycles;
		if constexpr (addPageCrossCycles) state.totalCycles += state.pageCrossCycles;
	}

	template<typename T, bool addPageCrossCycles = true>
	constexpr inline void endInstruction(byte operand1, byte operand2)
	{
		handleFlags<T::autoFlags ^ T::manualFlags>(operand1, operand2);
		state.totalCycles += state.cycles;
		if constexpr (addPageCrossCycles) state.totalCycles += state.pageCrossCycles;
	}

	template<typename T, typename F>
	constexpr inline void noOperation(F body)
	{
		beginInstruction<T>();
		body();
		endInstruction<T>();
	}
	template<typename T>
	constexpr inline void noOperation()
	{
		beginInstruction<T>();
		endInstruction<T>();
	}

	inline byte logicalShiftRight(byte value)
	{
		byte result = value >> 1;
		updateStatus(Status::NegativeResult, false);
		updateStatus(Status::Carry, checkBit(value, Status::Carry));
		state.opcodeResult = result;
		return result;
	}
	inline byte arithmeticShiftLeft(byte value)
	{
		byte result = value << 1;
		updateStatus(Status::NegativeResult, checkBit(result, Status::NegativeResult));
		updateStatus(Status::Carry, checkBit(value, Status::NegativeResult));

		state.opcodeResult = result;
		return result;
	}
	inline byte rotateLeft(byte value)
	{
		const bool carry = checkBit(state.p, Status::Carry);
		byte result = value << 1;
		if (carry) setBit(result, 0);

		updateStatus(Status::Carry, checkBit(value, 7));
		updateStatus(Status::NegativeResult, checkBit(value, 6));

		state.opcodeResult = result;
		return result;
	}
	inline byte rotateRight(byte value)
	{
		const bool carry = checkBit(state.p, Status::Carry);
		byte result = value >> 1;
		if (carry) setBit(result, 7);

		updateStatus(Status::Carry, checkBit(value, 0));
		updateStatus(Status::NegativeResult, carry);

		state.opcodeResult = result;
		return result;
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
	void stackPushAddress(MemAddress &address)
	{
		stackPush(address.high());
		stackPush(address.low());
	}
	MemAddress stackPopAddress()
	{
		byte low = stackPop();
		byte high = stackPop();
		MemAddress addr(low, high);
		return addr;
	}

	bool checkBit(const byte &operand, Status flag) const
	{
		return checkBit(operand, status_int(flag));
	}
	bool checkBit(const byte &operand, short bitNumber) const
	{
		return operand & (1 << bitNumber);
	}
	constexpr void setBit(byte &operand, short bitNumber) const
	{
		operand |= 1 << bitNumber;
	}
	constexpr void clearBit(byte &operand, short bitNumber) const
	{
		operand &= ~(1 << bitNumber);
	}

	template<byte value, Status statusFlag>
	constexpr static bool checkBit()
	{
		return value & (1 << static_cast<short>(statusFlag));
	}

	// status management
	bool isStatus(Status flag) const
	{
		return checkBit(state.p, status_int(flag));
	}

	void updateStatus(Status flag, bool flagState)
	{
		if (flagState)
		{
			setBit(state.p, status_int(flag));
		}
		else
		{
			clearBit(state.p, status_int(flag));
		}
	}

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

	// compare and bit testing
	inline void compare(byte &reg, const byte &data)
	{
		state.opcodeResult = reg - data;
		updateStatus(Status::Carry, data <= reg);
	}

	inline void bit(byte operand)
	{
		updateStatus(Status::NegativeResult, checkBit(operand, status_int(Status::NegativeResult)));
		updateStatus(Status::Overflow, checkBit(operand, status_int(Status::Overflow)));
		state.opcodeResult = operand & state.a;
	}

	// branching
	inline void branchIf(Status flag, bool checkStatus)
	{
		short extraCycles = 1;
		signed_byte branch = static_cast<signed_byte>(memory.memRelative());

		if (isStatus(flag) == checkStatus)
		{
			if (state.pc.addSigned(branch)) extraCycles++;
			state.addCycles(extraCycles);
		}
	}

	// arithmetic
	inline void adc(byte value)
	{
		if constexpr(DecimalMode)
		{
			// TODO: Decimal mode for other systems
		}
		else
		{
			byte operand = state.a;
			setOperands(operand, value);
			byte carry = static_cast<int>(isStatus(Status::Carry));
			state.setA(operand + value + carry);

			// figure out if there is carry from addition
			uint16_t sum = operand + value + isStatus(Status::Carry);
			updateStatus(Status::Carry, sum > 0xff);
		}
	}
	inline void sbc(byte value)
	{
		byte minuhend = state.a;
		byte subtrahend = value;
		byte carry = static_cast<int>(isStatus(Status::Carry));

		int16_t difference = minuhend - subtrahend - (1 - carry);
		state.setA(difference);

		// figure out if there is carry from subtraction
		updateStatus(Status::Carry, difference >= 0);

		constexpr byte signBit = 0b10000000;
		const bool isOverflow = (minuhend ^ subtrahend) & (minuhend ^ state.opcodeResult) & signBit;
		updateStatus(Status::Overflow, isOverflow);
	}
	
	inline void dcp(const MemAccess &access)
	{
		const byte newValue = access.value - 1;
		memory.write(access.address, newValue);
		compare(state.a, newValue);
		state.pageCrossCycles = 0;
	}

	inline void isc(const MemAccess &access)
	{
		const byte newValue = access.value + 1;
		memory.write(access.address, newValue);
		sbc(newValue);
		state.pageCrossCycles = 0;
	}

	inline void slo(const MemAccess &access)
	{
		const byte newValue = access.value << 1;
		memory.write(access.address, newValue);
		state.setA(state.a | newValue);

		updateStatus(Status::NegativeResult, checkBit(state.a, Status::NegativeResult));
		updateStatus(Status::Carry, checkBit(access.value, Status::NegativeResult));
		state.pageCrossCycles = 0;
	}

	inline void rla(const MemAccess &access)
	{
		const byte newValue = rotateLeft(access.value);
		memory.write(access.address, newValue);
		state.setA(state.a & newValue);

		updateStatus(Status::Carry, checkBit(access.value, 7));
		updateStatus(Status::NegativeResult, checkBit(access.value, 6));

		state.pageCrossCycles = 0;
	}

	inline void sre(const MemAccess &access)
	{
		const byte newValue = logicalShiftRight(access.value);
		memory.write(access.address, newValue);
		state.setA(state.a ^ newValue);

		updateStatus(Status::Carry, checkBit(access.value, Status::Carry));

		state.pageCrossCycles = 0;
	}
	
	inline void rra(const MemAccess &access)
	{
		const byte newValue = rotateRight(access.value);
		memory.write(access.address, newValue);
		adc(newValue);
		state.pageCrossCycles = 0;
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

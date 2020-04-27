#ifndef FLAGS_H
#define FLAGS_H

#include "common.hpp"

/*
	7   6   5   4   3   2   1   0
	N   V       B   D   I   Z   C
*/


namespace mos6502
{
	static constexpr byte None{0};
	static constexpr byte Manual{0};
	static constexpr byte N{0b10000000}; // Negative Result
	static constexpr byte V{0b01000000}; // Overflow
	static constexpr byte E{0b00100000}; // Expansion
	static constexpr byte B{0b00010000}; // Break Command
	static constexpr byte D{0b00001000}; // Decimal Mode
	static constexpr byte I{0b00000100}; // Interrupt Disable
	static constexpr byte Z{0b00000010}; // Zero Result
	static constexpr byte C{0b00000001}; // Carry
}

#endif /* FLAGS_H */

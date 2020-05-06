#ifndef MEM6502_H
#define MEM6502_H

#include <iostream>
#include <iomanip>
#include "common.hpp"
#include "memaddress.hpp"
#include "memchunk.hpp"
#include "mappedaddress.hpp"
#include "state.hpp"

namespace mos6502
{
	struct MemAccess
	{
		MemAddress address;
		byte value;

		MemAccess(MemAddress address, byte value)
		{
			this->address = address;
			this->value = value;
		}
	};

	template<typename Mapping>
	class Mem6502
	{
		static constexpr unsigned int cpuMemSize = 65535;
		
		State &cpuState;
		MemChunk<byte, cpuMemSize> memory;
		const Mapping &mapping;

	public:
		Mem6502(State &cpuState, const Mapping &mapping) : cpuState(cpuState), mapping(mapping)
		{
		}

		byte read(const MemAddress &address)
		{
			return memory[mapping.mapAddress(address).address];
		}

		void write(const MemAddress &address, byte value)
		{
			MappedAddress mapped = mapping.mapAddress(address);
			if (!mapped.readOnly)
			{
				memory[mapped.address] = value;
			}
			else
			{
				exit(EXIT_FAILURE);
			}
		}

		MemAddress readMemAddress(const MemAddress &address)
		{
			return MemAddress(read(address), read(address + 1));
		}
		
		byte &operator[](const MemAddress &address)
		{
			return memory[address];
		}
		
		// Memory access
		byte fetchByte()
		{
			byte val = read(cpuState.pc);
			cpuState.pc.add(1);
			return val;
		}
		MemAddress fetchNextMemAddress()
		{
			MemAddress addr = readMemAddress(cpuState.pc);
			cpuState.pc.add(2);
			return addr;
		}

		// Memory Addressing
		byte memRelative()
		{
			byte data = fetchByte();
			std::cout << std::hex << " #$" << static_cast<int>(data)
					  << " ($" << (cpuState.pc + data).value << ") ";
			return data;
		}

		byte fetchImmediate()
		{
			byte data = fetchByte();
			std::cout << " #" << std::hex << std::setw(2)
					<< static_cast<int>(data);
			return data;
		}
		byte memIndirect()
		{
			MemAddress addr = readMemAddress(MemAddress{fetchZeroPage()});
			return read(addr);
		}
		MemAddress addressAbsolute()
		{
			MemAddress addr = fetchNextMemAddress();
			std::cout << " $" << std::hex << std::setw(4) << static_cast<uint16_t>(addr.value);
			return addr;
		}
		MemAccess fetchAbsolute()
		{
			MemAddress addr = fetchNextMemAddress();
			byte data = read(addr);
			std::cout << " $" << std::hex << std::setw(4) << static_cast<uint16_t>(addr.value)
				<< " = " << std::setw(2) << static_cast<int>(data);
			return MemAccess(addr, data);
		}
		void writeAbsolute(byte value)
		{
			const MemAddress address = addressAbsolute();
			write(address, value);
		}
		void writeAbsolute(MemAddress address, byte value)
		{
			write(address, value);
		}
		MemAccess fetchAbsoluteX()
		{
			MemAddress addr = fetchNextMemAddress();
			if (addr.add(cpuState.x)) ++cpuState.pageCrossCycles;
			return MemAccess(addr, read(addr));
		}
		void writeAbsoluteX(byte value)
		{
			writeAbsoluteX(fetchNextMemAddress(), value);
		}
		void writeAbsoluteX(MemAddress address, byte value)
		{
			if (address.add(cpuState.x)) ++cpuState.pageCrossCycles;
			write(address, value);
		}
		MemAccess fetchAbsoluteY()
		{
			MemAddress addr = fetchNextMemAddress();
			if (addr.add(cpuState.y)) ++cpuState.pageCrossCycles;
			return MemAccess(addr, read(addr));
		}
		void writeAbsoluteY(byte value)
		{
			MemAddress addr = fetchNextMemAddress();
			if (addr.add(cpuState.y)) ++cpuState.pageCrossCycles;
			write(addr, value);
		}
		MemAccess fetchZeroPage()
		{
			byte addr = fetchByte();
			byte data = read(addr);
			std::cout << " $" << std::hex << std::setw(2) << static_cast<int>(addr)
					<< " = " << static_cast<int>(data);
			return MemAccess(addr, data);
		}
		void writeZeroPage(MemAddress address, byte value)
		{
			std::cout << " $" << std::setw(2) << std::hex << address.value
					<< " = " << static_cast<int>(value);

			write(address, value);
		}
		MemAddress addressZeroPageIndexed(byte index)
		{
			MemAddress address{static_cast<byte>((fetchByte() + index) % 256)};
			return address;
		}
		MemAccess fetchZeroPageIndexed(byte index)
		{
			MemAddress address = addressZeroPageIndexed(index);
			byte data = read(address);
			std::cout << " $" << std::hex << std::setw(2) << static_cast<int>(address.value)
					  << " = " << static_cast<int>(data);
			return MemAccess(address, data);
		}
		MemAccess fetchZeroPageX()
		{
			return fetchZeroPageIndexed(cpuState.x);
		}
		void writeZeroPageX(byte value)
		{
			MemAddress address = addressZeroPageIndexed(cpuState.x);
			write(address, value);
		}
		MemAccess fetchZeroPageY()
		{
			return fetchZeroPageIndexed(cpuState.y);
		}
		void writeZeroPageY(byte value)
		{
			MemAddress address = addressZeroPageIndexed(cpuState.y);
			write(address, value);
		}

		// indexed addressing
		MemAccess addressIndexedIndirect()
		{
			byte zeroPageAddr = (fetchByte() + cpuState.x) % 256;
			std::cout << " @ " << std::setw(2) << std::hex << static_cast<int>(zeroPageAddr);
			MemAddress indirect(read(zeroPageAddr), read((zeroPageAddr + 1) % 256));

			std::cout << " " << std::setw(4) << std::hex << static_cast<int>(indirect.value);
			byte value = read(indirect); // TODO: This read is pointless, only for debug

			std::cout << " = " << std::setw(2) << std::hex << static_cast<int>(value);
			return MemAccess(indirect, value);
		}
		MemAccess fetchIndexedIndirect()
		{
			return addressIndexedIndirect();
		}
		void writeIndexedIndirect(byte value)
		{
			write(addressIndexedIndirect().address, value);
		}

		MemAccess addressIndirectIndexed()
		{
			// get the zero page address
			const byte zeroPage = fetchByte();
			const byte zeroPageVal = read(zeroPage);

			// add Y to the zero page, and check for carry
			const uint16_t sum = zeroPageVal + cpuState.y;
			const short carry = sum > 0xFF;
			if (carry) ++cpuState.pageCrossCycles;

			// get the final address
			const byte addrLow = zeroPageVal + cpuState.y;
			const byte addrHigh = read((zeroPage + 1) % 256) + carry;
			MemAddress indirect(addrLow, addrHigh);

			std::cout << " " << std::setw(4) << std::hex << static_cast<int>(indirect.value);
			byte value = read(indirect); // TODO: This read is pointless, only for debug

			std::cout << " = " << std::setw(2) << std::hex << static_cast<int>(value);
			return MemAccess(indirect, value);
		}
		MemAccess fetchIndirectIndexed()
		{
			return addressIndirectIndexed();
		}
		void writeIndirectIndexed(byte value)
		{
			write(addressIndirectIndexed().address, value);
		}
	};
}

#endif /* MEM6502_H */

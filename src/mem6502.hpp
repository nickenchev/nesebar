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
		byte fetchAbsolute()
		{
			MemAddress addr = fetchNextMemAddress();
			byte data = read(addr);
			std::cout << " $" << std::hex << std::setw(4) << static_cast<uint16_t>(addr.value)
				<< " = " << std::setw(2) << static_cast<int>(data);
			return data;
		}
		void writeAbsolute(byte value)
		{
			const MemAddress address = addressAbsolute();
			write(address, value);
		}
		byte fetchAbsoluteX()
		{
			MemAddress addr = fetchNextMemAddress();
			if (addr.add(cpuState.x)) ++cpuState.cycles;
			return read(addr);
		}
		byte fetchAbsoluteY()
		{
			MemAddress addr = fetchNextMemAddress();
			if (addr.add(cpuState.y)) ++cpuState.cycles;
			return read(addr);
		}
		MemAccess fetchZeroPage()
		{
			byte addr = fetchByte();
			byte data = read(addr);
			std::cout << " $" << std::hex << std::setw(2) << static_cast<int>(addr)
					<< " = " << static_cast<int>(data);
			return MemAccess(addr, data);
		}
		void writeZeroPage(byte address, byte value)
		{
			std::cout << " $" << std::setw(2) << std::hex << static_cast<uint16_t>(address)
					<< " = " << static_cast<int>(value);

			write(MemAddress(address), value);
		}
		byte fetchZeroPageIndexed(byte index)
		{
			byte address{static_cast<byte>((fetchByte() + index) % 256)};
			byte data = read(MemAddress(address));
			std::cout << " $" << std::hex << std::setw(2) << address
					  << " = " << static_cast<int>(data);
			return data;
		}
		byte fetchZeroPageX()
		{
			return fetchZeroPageIndexed(cpuState.x);
		}
		byte fetchZeroPageY()
		{
			return fetchZeroPageIndexed(cpuState.y);
		}

		// indexed addressing
		byte memAbsoluteIndexed()
		{
			MemAddress addr = fetchNextMemAddress();
			if (addr.add(cpuState.x)) ++cpuState.cycles;
			return read(addr);
		}
		byte memIndexedIndirect()
		{
			MemAddress addr = readMemAddress(MemAddress{fetchZeroPageX()});
			return read(addr);
		}
		byte memIndirectIndexed()
		{
			MemAddress addr = readMemAddress(MemAddress{fetchByte()});
			if (addr.add(cpuState.y)) ++cpuState.cycles;
			return read(addr);
		}
	};
}

#endif /* MEM6502_H */

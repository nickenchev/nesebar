#include <iostream>
#include <string>
#include <vector>
#include "core6502.hpp"
#include "flags.hpp"
#include "nesmemory.hpp"
#include "opcodes.hpp"

using namespace mos6502;

template<typename Memory, typename Mapping, bool DecimalMode>
Core<Memory, Mapping, DecimalMode>::Core(const Mapping &mapping) : memory(state, mapping), mapping(mapping)
{
	memory.write(0x4017, 0); // frame IRQ enable
	memory.write(0x4015, 0); // disable all channels

	for (MemAddress addr = 0x4000; addr <= 0x400f; ++addr)
	{
		memory.write(addr, 0);
	}
	for (MemAddress addr = 0x4010; addr <= 0x4013; ++addr)
	{
		memory.write(addr, 0);
	}
}

template<typename Memory, typename Mapping, bool DecimalMode>
void Core<Memory, Mapping, DecimalMode>::step()
{
	using namespace mos6502::opcodes;

	logInfo();
	std::cout << '$' << std::hex << std::setfill('0')
			  << std::setw(4) << state.pc.value << ": ";

	const byte opcode = nextOpcode();
	switch (opcode)
	{
		case BRK::value:
		{
			exit(1);
			beginInstruction<BRK>();
			stackPush(state.p);
			memory.fetchByte();
			stackPushAddress(state.pc);
			updateStatus(Status::InterruptDisable, true);
			endInstruction<BRK>();
			break;
		}
		case ADC::Immediate::value:
		{
			perform<ADC::Immediate>([this](const byte data) {
				adc(data);
			});
			break;
		}
		case ADC::ZeroPage::value:
		{
			beginInstruction<ADC::ZeroPage>();
			byte aCopy = state.a;
			byte data = memory.fetchZeroPage().value;
			adc(data);
			endInstruction<ADC::ZeroPage>(aCopy, data);
			break;
		}
		case ADC::ZeroPageX::value:
		{
			beginInstruction<ADC::ZeroPageX>();
			byte aCopy = state.a;
			byte data = memory.fetchZeroPageX().value;
			adc(data);
			endInstruction<ADC::ZeroPageX>(aCopy, data);
			break;
		}
		case ADC::Absolute::value:
		{
			beginInstruction<ADC::Absolute>();
			byte aCopy = state.a;
			byte data = memory.fetchAbsolute().value;
			adc(data);
			endInstruction<ADC::Absolute>(aCopy, data);
			break;
		}
		case ADC::AbsoluteX::value:
		{
			beginInstruction<ADC::AbsoluteX>();
			byte aCopy = state.a;
			byte data = memory.fetchAbsoluteX().value;
			adc(data);
			endInstruction<ADC::AbsoluteX>(aCopy, data);
			break;
		}
		case ADC::AbsoluteY::value:
		{
			beginInstruction<ADC::AbsoluteY>();
			byte aCopy = state.a;
			byte data = memory.fetchAbsoluteY();
			adc(data);
			endInstruction<ADC::AbsoluteY>(aCopy, data);
			break;
		}
		case ADC::IndexedIndirect::value:
		{
			beginInstruction<ADC::IndexedIndirect>();
			byte aCopy = state.a;
			byte value = memory.fetchIndexedIndirect();
			adc(value);
			endInstruction<ADC::IndexedIndirect>(aCopy, value);
			break;
		}
		case ADC::IndirectIndexed::value:
		{
			beginInstruction<ADC::IndirectIndexed>();
			byte aCopy = state.a;
			byte value = memory.fetchIndirectIndexed();
			adc(value);
			endInstruction<ADC::IndirectIndexed>(aCopy, value);
			break;
		}
		case SBC::Immediate::value:
		{
			beginInstruction<SBC::Immediate>();
			byte aCopy = state.a;
			byte value = memory.fetchImmediate();
			sbc(value);
			endInstruction<SBC::Immediate>(aCopy, value);
			break;
		}
		case SBC::ZeroPage::value:
		{
			beginInstruction<SBC::ZeroPage>();
			byte aCopy = state.a;
			byte value = memory.fetchZeroPage().value;
			sbc(value);
			endInstruction<SBC::ZeroPage>(aCopy, value);
			break;
		}
		case SBC::ZeroPageX::value:
		{
			beginInstruction<SBC::ZeroPageX>();
			byte aCopy = state.a;
			byte value = memory.fetchZeroPageX().value;
			sbc(value);
			endInstruction<SBC::ZeroPageX>(aCopy, value);
			break;
		}
		case SBC::Absolute::value:
		{
			beginInstruction<SBC::Absolute>();
			byte aCopy = state.a;
			byte value = memory.fetchAbsolute().value;
			sbc(value);
			endInstruction<SBC::Absolute>(aCopy, value);
			break;
		}
		case SBC::AbsoluteX::value:
		{
			beginInstruction<SBC::AbsoluteX>();
			byte aCopy = state.a;
			byte value = memory.fetchAbsoluteX().value;
			sbc(value);
			endInstruction<SBC::AbsoluteX>(aCopy, value);
			break;
		}
		case SBC::AbsoluteY::value:
		{
			beginInstruction<SBC::AbsoluteY>();
			byte aCopy = state.a;
			byte value = memory.fetchAbsoluteY();
			sbc(value);
			endInstruction<SBC::AbsoluteY>(aCopy, value);
			break;
		}
		case SBC::IndexedIndirect::value:
		{
			beginInstruction<SBC::IndexedIndirect>();
			byte aCopy = state.a;
			byte value = memory.fetchIndexedIndirect();
			sbc(value);
			endInstruction<SBC::IndexedIndirect>(aCopy, value);
			break;
		}
		case SBC::IndirectIndexed::value:
		{
			beginInstruction<SBC::IndirectIndexed>();
			byte aCopy = state.a;
			byte value = memory.fetchIndirectIndexed();
			sbc(value);
			endInstruction<SBC::IndirectIndexed>(aCopy, value);
			break;
		}
		case INC::ZeroPage::value:
		{
			beginInstruction<INC::ZeroPage>();
			MemAccess access = memory.fetchZeroPage();
			memory.writeZeroPage(access.address, ++access.value);
			state.opcodeResult = access.value;
			endInstruction<INC::ZeroPage>();
			break;
		}
		case INC::ZeroPageX::value:
		{
			beginInstruction<INC::ZeroPageX>();
			MemAccess access = memory.fetchZeroPageX();
			memory.write(access.address, ++access.value);
			state.opcodeResult = access.value;
			endInstruction<INC::ZeroPageX>();
			break;
		}
		case INC::Absolute::value:
		{
			beginInstruction<INC::Absolute>();
			MemAccess access = memory.fetchAbsolute();
			memory.writeAbsolute(access.address, ++access.value);
			state.opcodeResult = access.value;
			endInstruction<INC::Absolute>();
			break;
		}
		case INC::AbsoluteX::value:
		{
			beginInstruction<INC::AbsoluteX>();
			MemAccess access = memory.fetchAbsoluteX();
			memory.write(access.address, ++access.value);
			state.opcodeResult = access.value;
			endInstruction<INC::AbsoluteX>();
			break;
		}
		case DEC::ZeroPage::value:
		{
			beginInstruction<DEC::ZeroPage>();
			MemAccess access = memory.fetchZeroPage();
			memory.writeZeroPage(access.address, --access.value);
			state.opcodeResult = access.value;
			endInstruction<DEC::ZeroPage>();
			break;
		}
		case DEC::ZeroPageX::value:
		{
			beginInstruction<DEC::ZeroPageX>();
			MemAccess access = memory.fetchZeroPageX();
			memory.write(access.address, --access.value);
			state.opcodeResult = access.value;
			endInstruction<DEC::ZeroPageX>();
			break;
		}
		case DEC::Absolute::value:
		{
			beginInstruction<DEC::Absolute>();
			MemAccess access = memory.fetchAbsolute();
			memory.writeAbsolute(access.address, --access.value);
			state.opcodeResult = access.value;
			endInstruction<DEC::Absolute>();
			break;
		}
		case DEC::AbsoluteX::value:
		{
			beginInstruction<DEC::AbsoluteX>();
			MemAccess access = memory.fetchAbsoluteX();
			memory.write(access.address, --access.value);
			state.opcodeResult = access.value;
			endInstruction<DEC::AbsoluteX>();
			break;
		}
		case INX::value:
		{
			beginInstruction<INX>();
			state.setX(state.x + 1);
			endInstruction<INX>();
			break;
		}
		case INY::value:
		{
			beginInstruction<INY>();
			state.setY(state.y + 1);
			endInstruction<INY>();
			break;
		}
		case DEX::value:
		{
			beginInstruction<DEX>();
			state.setX(state.x - 1);
			endInstruction<DEX>();
			break;
		}
		case DEY::value:
		{
			beginInstruction<DEY>();
			state.setY(state.y - 1);
			endInstruction<DEY>();
			break;
		}
		case TAX::value:
		{
			beginInstruction<TAX>();
			state.setX(state.a);
			endInstruction<TAX>();
			break;
		}
		case TAY::value:
		{
			beginInstruction<TAY>();
			state.setY(state.a);
			endInstruction<TAY>();
			break;
		}
		case TXA::value:
		{
			beginInstruction<TXA>();
			state.setA(state.x);
			endInstruction<TXA>();
			break;
		}
		case TYA::value:
		{
			beginInstruction<TYA>();
			state.setA(state.y);
			endInstruction<TYA>();
			break;
		}
		case TSX::value:
		{
			beginInstruction<TSX>();
			state.setX(state.sp);
			endInstruction<TSX>();
			break;
		}
		case ORA::Immediate::value:
		{
			beginInstruction<ORA::Immediate>();
			state.setA(state.a | memory.fetchImmediate());
			endInstruction<ORA::Immediate>();
			break;
		}
		case ORA::ZeroPage::value:
		{
			beginInstruction<ORA::ZeroPage>();
			state.setA(state.a | memory.fetchZeroPage().value);
			endInstruction<ORA::ZeroPage>();
			break;
		}
		case ORA::ZeroPageX::value:
		{
			beginInstruction<ORA::ZeroPageX>();
			state.setA(state.a | memory.fetchZeroPageX().value);
			endInstruction<ORA::ZeroPageX>();
			break;
		}
		case ORA::Absolute::value:
		{
			beginInstruction<ORA::Absolute>();
			state.setA(state.a | memory.fetchAbsolute().value);
			endInstruction<ORA::Absolute>();
			break;
		}
		case ORA::AbsoluteX::value:
		{
			beginInstruction<ORA::AbsoluteX>();
			state.setA(state.a | memory.fetchAbsoluteX().value);
			endInstruction<ORA::AbsoluteX>();
			break;
		}
		case ORA::AbsoluteY::value:
		{
			beginInstruction<ORA::AbsoluteY>();
			state.setA(state.a | memory.fetchAbsoluteY());
			endInstruction<ORA::AbsoluteY>();
			break;
		}
        case ORA::IndexedIndirect::value:
		{
			beginInstruction<ORA::IndexedIndirect>();
			state.setA(state.a | memory.fetchIndexedIndirect());
			endInstruction<ORA::IndexedIndirect>();
			break;
		}
        case ORA::IndirectIndexed::value:
		{
			beginInstruction<ORA::IndirectIndexed>();
			state.setA(state.a | memory.fetchIndirectIndexed());
			endInstruction<ORA::IndirectIndexed>();
			break;
		}
		case EOR::Immediate::value:
		{
			beginInstruction<EOR::Immediate>();
			state.setA(state.a ^ memory.fetchImmediate());
			endInstruction<EOR::Immediate>();
			break;
		}
		case EOR::ZeroPage::value:
		{
			beginInstruction<EOR::ZeroPage>();
			state.setA(state.a ^ memory.fetchZeroPage().value);
			endInstruction<EOR::ZeroPage>();
			break;
		}
		case EOR::ZeroPageX::value:
		{
			beginInstruction<EOR::ZeroPageX>();
			state.setA(state.a ^ memory.fetchZeroPageX().value);
			endInstruction<EOR::ZeroPage>();
			break;
		}
		case EOR::Absolute::value:
		{
			beginInstruction<EOR::Absolute>();
			state.setA(state.a ^ memory.fetchAbsolute().value);
			endInstruction<EOR::Absolute>();
			break;
		}
		case EOR::AbsoluteX::value:
		{
			beginInstruction<EOR::AbsoluteX>();
			state.setA(state.a ^ memory.fetchAbsoluteX().value);
			endInstruction<EOR::AbsoluteX>();
			break;
		}
		case EOR::AbsoluteY::value:
		{
			beginInstruction<EOR::AbsoluteY>();
			state.setA(state.a ^ memory.fetchAbsoluteY());
			endInstruction<EOR::AbsoluteY>();
			break;
		}
		case EOR::IndexedIndirect::value:
		{
			beginInstruction<EOR::IndexedIndirect>();
			state.setA(state.a ^ memory.fetchIndexedIndirect());
			endInstruction<EOR::IndexedIndirect>();
			break;
		}
		case EOR::IndirectIndexed::value:
		{
			beginInstruction<EOR::IndirectIndexed>();
			state.setA(state.a ^ memory.fetchIndirectIndexed());
			endInstruction<EOR::IndirectIndexed>();
			break;
		}
		case ASL::Accumulator::value:
		{
			beginInstruction<ASL::Accumulator>();
			state.a = arithmeticShiftLeft(state.a); // set opcodeResult once
			endInstruction<ASL::Accumulator>();
			break;
		}
		case ASL::ZeroPage::value:
		{
			beginInstruction<ASL::ZeroPage>();
			MemAccess access = memory.fetchZeroPage();
			memory.writeZeroPage(access.address, arithmeticShiftLeft(access.value));
			endInstruction<ASL::ZeroPage>();
			break;
		}
		case ASL::ZeroPageX::value:
		{
			beginInstruction<ASL::ZeroPageX>();
			MemAccess access = memory.fetchZeroPageX();
			memory.write(access.address, arithmeticShiftLeft(access.value));
			endInstruction<ASL::ZeroPageX>();
			break;
		}
		case ASL::Absolute::value:
		{
			beginInstruction<ASL::Absolute>();
			MemAccess access = memory.fetchAbsolute();
			memory.writeAbsolute(access.address, arithmeticShiftLeft(access.value));
			endInstruction<ASL::Absolute>();
			break;
		}
		case ASL::AbsoluteX::value:
		{
			beginInstruction<ASL::AbsoluteX>();
			MemAccess access = memory.fetchAbsoluteX();
			memory.write(access.address, arithmeticShiftLeft(access.value));
			endInstruction<ASL::AbsoluteX>();
			break;
		}
		case LSR::Accumulator::value:
		{
			beginInstruction<LSR::Accumulator>();
			state.a = logicalShiftRight(state.a); // set opcodeResult once
			endInstruction<LSR::Accumulator>();
			break;
		}
		case LSR::ZeroPage::value:
		{
			beginInstruction<LSR::ZeroPage>();
			MemAccess access = memory.fetchZeroPage();
			access.value = logicalShiftRight(access.value);
			memory.writeZeroPage(access.address, access.value);
			endInstruction<LSR::ZeroPage>();
			break;
		}
		case LSR::ZeroPageX::value:
		{
			beginInstruction<LSR::ZeroPageX>();
			MemAccess access = memory.fetchZeroPageX();
			access.value = logicalShiftRight(access.value);
			memory.write(access.address, access.value);
			endInstruction<LSR::ZeroPageX>();
			break;
		}
		case LSR::Absolute::value:
		{
			beginInstruction<LSR::Absolute>();
			MemAccess access = memory.fetchAbsolute();
			access.value = logicalShiftRight(access.value);
			memory.write(access.address, access.value);
			endInstruction<LSR::Absolute>();
			break;
		}
		case LSR::AbsoluteX::value:
		{
			beginInstruction<LSR::AbsoluteX>();
			MemAccess access = memory.fetchAbsoluteX();
			access.value = logicalShiftRight(access.value);
			memory.write(access.address, access.value);
			endInstruction<LSR::AbsoluteX>();
			break;
		}
		case ROL::Accumulator::value:
		{
			beginInstruction<ROL::Accumulator>();
			state.a = rotateLeft(state.a); // set opcodeResult once
			endInstruction<ROL::Accumulator>();
			break;
		}
		case ROL::ZeroPage::value:
		{
			beginInstruction<ROL::ZeroPage>();
			MemAccess access = memory.fetchZeroPage();
			state.opcodeResult = rotateLeft(access.value);
			memory.writeZeroPage(access.address, state.opcodeResult);
			endInstruction<ROL::ZeroPage>();
			break;
		}
		case ROL::ZeroPageX::value:
		{
			beginInstruction<ROL::ZeroPageX>();
			MemAccess access = memory.fetchZeroPageX();
			state.opcodeResult = rotateLeft(access.value);
			memory.write(access.address, state.opcodeResult);
			endInstruction<ROL::ZeroPageX>();
			break;
		}
		case ROL::Absolute::value:
		{
			beginInstruction<ROL::Absolute>();
			MemAccess access = memory.fetchAbsolute();
			state.opcodeResult = rotateLeft(access.value);
			memory.writeAbsolute(access.address, state.opcodeResult);
			endInstruction<ROL::Absolute>();
			break;
		}
		case ROL::AbsoluteX::value:
		{
			beginInstruction<ROL::AbsoluteX>();
			MemAccess access = memory.fetchAbsoluteX();
			state.opcodeResult = rotateLeft(access.value);
			memory.write(access.address, state.opcodeResult);
			endInstruction<ROL::AbsoluteX>();
			break;
		}
		case ROR::Accumulator::value:
		{
			beginInstruction<ROR::Accumulator>();
			state.a = rotateRight(state.a); // set opcodeResult once
			endInstruction<ROR::Accumulator>();
			break;
		}
		case ROR::ZeroPage::value:
		{
			beginInstruction<ROR::ZeroPage>();
			MemAccess access = memory.fetchZeroPage();
			state.opcodeResult = rotateRight(access.value);
			memory.writeZeroPage(access.address, state.opcodeResult);
			endInstruction<ROR::ZeroPage>();
			break;
		}
		case ROR::ZeroPageX::value:
		{
			beginInstruction<ROR::ZeroPageX>();
			MemAccess access = memory.fetchZeroPageX();
			state.opcodeResult = rotateRight(access.value);
			memory.write(access.address, state.opcodeResult);
			endInstruction<ROR::ZeroPageX>();
			break;
		}
		case ROR::Absolute::value:
		{
			beginInstruction<ROR::Absolute>();
			MemAccess access = memory.fetchAbsolute();
			state.opcodeResult = rotateRight(access.value);
			memory.writeAbsolute(access.address, state.opcodeResult);
			endInstruction<ROR::Absolute>();
			break;
		}
		case ROR::AbsoluteX::value:
		{
			beginInstruction<ROR::AbsoluteX>();
			MemAccess access = memory.fetchAbsoluteX();
			state.opcodeResult = rotateRight(access.value);
			memory.write(access.address, state.opcodeResult);
			endInstruction<ROR::AbsoluteX>();
			break;
		}
		case PHP::value:
		{
			beginInstruction<PHP>();
			stackPush(state.p);
			endInstruction<PHP>();
			break;
		}
		case PHA::value:
		{
			beginInstruction<PHA>();
			stackPush(state.a);
			endInstruction<PHA>();
			break;
		}
		case PLP::value:
		{
			beginInstruction<PLP>();
			state.setP(stackPop());
			endInstruction<PLP>();
			break;
		}
		case BPL::value:
		{
			beginInstruction<BPL>();
			branchIf(Status::NegativeResult, false);
			endInstruction<BPL>();
			break;
		}
		case CLC::value:
		{
			beginInstruction<CLC>();
			updateStatus(Status::Carry, false);
			endInstruction<CLC>();
			break;
		}
		case JSR::value:
		{
			beginInstruction<JSR>();
			MemAddress jumpAddr = memory.addressAbsolute();
			MemAddress returnAddress = state.pc - 1;
			stackPushAddress(returnAddress);
			state.pc = jumpAddr;
			endInstruction<JSR>();
			break;
		}
		case BIT::ZeroPage::value:
		{
			beginInstruction<BIT::ZeroPage>();
			bit(memory.fetchZeroPage().value);
			endInstruction<BIT::ZeroPage>();
			break;
		}
		case BIT::Absolute::value:
		{
			beginInstruction<BIT::Absolute>();
			bit(memory.fetchAbsolute().value);
			endInstruction<BIT::Absolute>();
			break;
		}
		case AND::Immediate::value:
		{
			beginInstruction<AND::Immediate>();
			state.setA(state.a & memory.fetchImmediate());
			endInstruction<AND::Immediate>();
			break;
		}
		case AND::ZeroPage::value:
		{
			beginInstruction<AND::ZeroPage>();
			state.setA(state.a & memory.fetchZeroPage().value);
			endInstruction<AND::ZeroPage>();
			break;
		}
		case AND::ZeroPageX::value:
		{
			beginInstruction<AND::ZeroPageX>();
			state.setA(state.a & memory.fetchZeroPageX().value);
			endInstruction<AND::ZeroPageX>();
			break;
		}
		case AND::Absolute::value:
		{
			beginInstruction<AND::Absolute>();
			state.setA(state.a & memory.fetchAbsolute().value);
			endInstruction<AND::Absolute>();
			break;
		}
		case AND::AbsoluteX::value:
		{
			beginInstruction<AND::AbsoluteX>();
			state.setA(state.a & memory.fetchAbsoluteX().value);
			endInstruction<AND::AbsoluteX>();
			break;
		}
		case AND::AbsoluteY::value:
		{
			beginInstruction<AND::AbsoluteY>();
			state.setA(state.a & memory.fetchAbsoluteY());
			endInstruction<AND::AbsoluteY>();
			break;
		}
		case AND::IndexedIndirect::value:
		{
			beginInstruction<AND::IndexedIndirect>();
			state.setA(state.a & memory.fetchIndexedIndirect());
			endInstruction<AND::IndexedIndirect>();
			break;
		}
		case AND::IndirectIndexed::value:
		{
			beginInstruction<AND::IndirectIndexed>();
			state.setA(state.a & memory.fetchIndirectIndexed());
			endInstruction<AND::IndirectIndexed>();
			break;
		}
		case SEC::value:
		{
			beginInstruction<SEC>();
			updateStatus(Status::Carry, true);
			endInstruction<SEC>();
			break;
		}
		case JMP::Absolute::value:
		{
			beginInstruction<JMP::Absolute>();
			state.pc = memory.addressAbsolute();
			endInstruction<JMP::Absolute>();
			break;
		}
		case JMP::Indirect::value:
		{
			// 6502 has a bug where there is no page cross on this instruction
			beginInstruction<JMP::Indirect>();
			MemAddress ial = memory.fetchNextMemAddress();
			MemAddress iah = ial;
			iah.addLow(1);
			MemAddress indirect(memory.read(ial), memory.read(iah));
			state.pc = indirect;
			endInstruction<JMP::Indirect>();
			break;
		}
		case BVC::value:
		{
			beginInstruction<BVC>();
			branchIf(Status::Overflow, false);
			endInstruction<BVC>();
			break;
		}
		case RTS::value:
		{
			beginInstruction<RTS>();
			state.pc = stackPopAddress() + 1;
			endInstruction<RTS>();
			break;
		}
		case RTI::value:
		{
			beginInstruction<RTI>();
			state.setP(stackPop());
			state.pc = stackPopAddress();
			endInstruction<RTI>();
			break;
		}
		case PLA::value:
		{
			beginInstruction<PLA>();
			state.setA(stackPop());
			endInstruction<PLA>();
			break;
		}
		case BVS::value:
		{
			beginInstruction<BVS>();
			branchIf(Status::Overflow, true);
			endInstruction<BVS>();
			break;
		}
		case SEI::value:
		{
			beginInstruction<SEI>();
			updateStatus(Status::InterruptDisable, true);
			endInstruction<SEI>();
			break;
		}
		case STY::ZeroPage::value:
		{
			beginInstruction<STY::ZeroPage>();
			memory.writeZeroPage(memory.fetchByte(), state.y);
			endInstruction<STY::ZeroPage>();
			break;
		}
		case STY::ZeroPageX::value:
		{
			beginInstruction<STY::ZeroPageX>();
			memory.writeZeroPageX(state.y);
			endInstruction<STY::ZeroPageX>();
			break;
		}
		case STY::Absolute::value:
		{
			beginInstruction<STY::Absolute>();
			memory.writeAbsolute(state.y);
			endInstruction<STY::Absolute>();
			break;
		}
		case STA::ZeroPage::value:
		{
			beginInstruction<STA::ZeroPage>();
			memory.writeZeroPage(memory.fetchByte(), state.a);
			endInstruction<STA::ZeroPage>();
			break;
		}
		case STA::ZeroPageX::value:
		{
			beginInstruction<STA::ZeroPageX>();
			memory.writeZeroPageX(state.a);
			endInstruction<STA::ZeroPageX>();
			break;
		}
		case STA::Absolute::value:
		{
			beginInstruction<STA::Absolute>();
			memory.writeAbsolute(state.a);
			endInstruction<STA::Absolute>();
			break;
		}
		case STA::AbsoluteX::value:
		{
			beginInstruction<STA::AbsoluteX>();
			memory.writeAbsoluteX(state.a);
			endInstruction<STA::AbsoluteX, false>();
			break;
		}
		case STA::AbsoluteY::value:
		{
			beginInstruction<STA::AbsoluteY>();
			memory.writeAbsoluteY(state.a);
			endInstruction<STA::AbsoluteY, false>();
			break;
		}
		case STA::IndexedIndirect::value:
		{
			beginInstruction<STA::IndexedIndirect>();
			memory.writeIndexedIndirect(state.a);
			endInstruction<STA::IndexedIndirect>();
			break;
		}
		case STA::IndirectIndexed::value:
		{
			beginInstruction<STA::IndirectIndexed>();
			memory.writeIndirectIndexed(state.a);
			endInstruction<STA::IndirectIndexed>();
			break;
		}
		case STX::ZeroPage::value:
		{
			beginInstruction<STX::ZeroPage>();
			memory.writeZeroPage(memory.fetchByte(), state.x);
			endInstruction<STX::ZeroPage>();
			break;
		}
		case STX::ZeroPageY::value:
		{
			beginInstruction<STX::ZeroPageY>();
			memory.writeZeroPageY(state.x);
			endInstruction<STX::ZeroPageY>();
			break;
		}
		case STX::Absolute::value:
		{
			beginInstruction<STX::Absolute>();
			memory.writeAbsolute(state.x);
			endInstruction<STX::Absolute>();
			break;
		}
		case BCC::value:
		{
			beginInstruction<BCC>();
			branchIf(Status::Carry, false);
			endInstruction<BCC>();
			break;
		}
		case TXS::value:
		{
			beginInstruction<TXS>();
			state.setSP(state.x);
			endInstruction<TXS>();
			break;
		}
		case LDX::Immediate::value:
		{
			beginInstruction<LDX::Immediate>();
			state.setX(memory.fetchImmediate());
			endInstruction<LDX::Immediate>();
			break;
		}
		case LDX::ZeroPage::value:
		{
			beginInstruction<LDX::ZeroPage>();
			state.setX(memory.fetchZeroPage().value);
			endInstruction<LDX::ZeroPage>();
			break;
		}
		case LDX::ZeroPageY::value:
		{
			beginInstruction<LDX::ZeroPageY>();
			state.setX(memory.fetchZeroPageY().value);
			endInstruction<LDX::ZeroPageY>();
			break;
		}
		case LDX::Absolute::value:
		{
			beginInstruction<LDX::Absolute>();
			state.setX(memory.fetchAbsolute().value);
			endInstruction<LDX::Absolute>();
			break;
		}
		case LDX::AbsoluteY::value:
		{
			beginInstruction<LDX::AbsoluteY>();
			state.setX(memory.fetchAbsoluteY());
			endInstruction<LDX::AbsoluteY>();
			break;
		}
		case LDY::Immediate::value:
		{
			beginInstruction<LDY::Immediate>();
			state.setY(memory.fetchImmediate());
			endInstruction<LDY::Immediate>();
			break;
		}
		case LDY::ZeroPage::value:
		{
			beginInstruction<LDY::ZeroPage>();
			state.setY(memory.fetchZeroPage().value);
			endInstruction<LDY::ZeroPage>();
			break;
		}
		case LDY::ZeroPageX::value:
		{
			beginInstruction<LDY::ZeroPageX>();
			state.setY(memory.fetchZeroPageX().value);
			endInstruction<LDY::ZeroPageX>();
			break;
		}
		case LDY::Absolute::value:
		{
			beginInstruction<LDY::Absolute>();
			state.setY(memory.fetchAbsolute().value);
			endInstruction<LDY::Absolute>();
			break;
		}
		case LDY::AbsoluteX::value:
		{
			beginInstruction<LDY::AbsoluteX>();
			state.setY(memory.fetchAbsoluteX().value);
			endInstruction<LDY::AbsoluteX>();
			break;
		}
		case LDA::Immediate::value:
		{
			beginInstruction<LDA::Immediate>();
			state.setA(memory.fetchImmediate());
			endInstruction<LDA::Immediate>();
			break;
		}
		case LDA::ZeroPage::value:
		{
			beginInstruction<LDA::ZeroPage>();
			state.setA(memory.fetchZeroPage().value);
			endInstruction<LDA::ZeroPage>();
			break;
		}
		case LDA::ZeroPageX::value:
		{
			beginInstruction<LDA::ZeroPageX>();
			state.setA(memory.fetchZeroPageX().value);
			endInstruction<LDA::ZeroPageX>();
			break;
		}
		case LDA::Absolute::value:
		{
			beginInstruction<LDA::Absolute>();
			state.setA(memory.fetchAbsolute().value);
			endInstruction<LDA::Absolute>();
			break;
		}
		case LDA::AbsoluteX::value:
		{
			beginInstruction<LDA::AbsoluteX>();
			state.setA(memory.fetchAbsoluteX().value);
			endInstruction<LDA::AbsoluteX>();
			break;
		}
		case LDA::AbsoluteY::value:
		{
			beginInstruction<LDA::AbsoluteY>();
			state.setA(memory.fetchAbsoluteY());
			endInstruction<LDA::AbsoluteY>();
			break;
		}
		case LDA::IndexedIndirect::value:
		{
			beginInstruction<LDA::IndexedIndirect>();
			state.setA(memory.fetchIndexedIndirect());
			endInstruction<LDA::IndexedIndirect>();
			break;
		}
		case LDA::IndirectIndexed::value:
		{
			beginInstruction<LDA::IndirectIndexed>();
			state.setA(memory.fetchIndirectIndexed());
			endInstruction<LDA::IndirectIndexed>();
			break;
		}
		case BCS::value:
		{
			beginInstruction<BCS>();
			branchIf(Status::Carry, true);
			endInstruction<BCS>();
			break;
		}
		case CMP::Immediate::value:
		{
			using OP = CMP::Immediate;
			beginInstruction<OP>();
			const byte data = memory.fetchImmediate();
			compare(state.a, data);
			endInstruction<OP>(state.a, data);
			break;
		}
		case CMP::ZeroPage::value:
		{
			using OP = CMP::ZeroPage;
			beginInstruction<OP>();
			const byte data = memory.fetchZeroPage().value;
			compare(state.a, data);
			endInstruction<OP>(state.a, data);
			break;
		}
		case CMP::ZeroPageX::value:
		{
			using OP = CMP::ZeroPageX;
			beginInstruction<OP>();
			const byte data = memory.fetchZeroPageX().value;
			compare(state.a, data);
			endInstruction<OP>(state.a, data);
			break;
		}
		case CMP::Absolute::value:
		{
			using OP = CMP::Absolute;
			beginInstruction<OP>();
			const byte data = memory.fetchAbsolute().value;
			compare(state.a, data);
			endInstruction<OP>(state.a, data);
			break;
		}
		case CMP::AbsoluteX::value:
		{
			using OP = CMP::AbsoluteX;
			beginInstruction<OP>();
			const byte data = memory.fetchAbsoluteX().value;
			compare(state.a, data);
			endInstruction<OP>(state.a, data);
			break;
		}
		case CMP::AbsoluteY::value:
		{
			using OP = CMP::AbsoluteY;
			beginInstruction<OP>();
			const byte data = memory.fetchAbsoluteY();
			compare(state.a, data);
			endInstruction<OP>(state.a, data);
			break;
		}
		case CMP::IndexedIndirect::value:
		{
			using OP = CMP::IndexedIndirect;
			beginInstruction<OP>();
			const byte data = memory. fetchIndexedIndirect();
			compare(state.a, data);
			endInstruction<OP>(state.a, data);
			break;
		}
		case CMP::IndirectIndexed::value:
		{
			using OP = CMP::IndirectIndexed;
			beginInstruction<OP>();
			const byte data = memory.fetchIndirectIndexed();
			compare(state.a, data);
			endInstruction<OP>(state.a, data);
			break;
		}
		case CPX::Immediate::value:
		{
			using OP = CPX::Immediate;
			beginInstruction<OP>();
			const byte data = memory.fetchImmediate();
			compare(state.x, data);
			endInstruction<OP>(state.x, data);
			break;
		}
		case CPX::ZeroPage::value:
		{
			using OP = CPX::ZeroPage;
			beginInstruction<OP>();
			const byte data = memory.fetchZeroPage().value;
			compare(state.x, data);
			endInstruction<OP>(state.x, data);
			break;
		}
		case CPX::Absolute::value:
		{
			using OP = CPX::Absolute;
			beginInstruction<OP>();
			byte data = memory.fetchAbsolute().value;
			compare(state.x, data);
			endInstruction<OP>(state.x, data);
			break;
		}
		case CPY::Immediate::value:
		{
			using OP = CPX::Immediate;
			beginInstruction<OP>();
			byte data = memory.fetchImmediate();
			compare(state.y, data);
			endInstruction<OP>(state.y, data);
			break;
		}
		case CPY::ZeroPage::value:
		{
			using OP = CPY::ZeroPage;
			beginInstruction<OP>();
			byte data = memory.fetchZeroPage().value;
			compare(state.y, data);
			endInstruction<OP>(state.y, data);
			break;
		}
		case CPY::Absolute::value:
		{
			using OP = CPY::Absolute;
			beginInstruction<OP>();
			byte data = memory.fetchAbsolute().value;
			compare(state.y, data);
			endInstruction<OP>(state.y, data);
			break;
		}
		case BMI::value:
		{
			beginInstruction<BMI>();
			branchIf(Status::NegativeResult, true);
			endInstruction<BMI>();
			break;
		}
		case BNE::value:
		{
			beginInstruction<BNE>();
			branchIf(Status::ZeroResult, false);
			endInstruction<BNE>();
			break;
		}
		case CLD::value:
		{
			beginInstruction<CLD>();
			updateStatus(Status::DecimalMode, false);
			endInstruction<CLD>();
			break;
		}
		case CLV::value:
		{
			beginInstruction<CLV>();
			updateStatus(Status::Overflow, false);
			endInstruction<CLV>();
			break;
		}
		case BEQ::value:
		{
			beginInstruction<BEQ>();
			branchIf(Status::ZeroResult, true);
			endInstruction<BEQ>();
			break;
		}
		case SED::value:
		{
			beginInstruction<SED>();
			updateStatus(Status::DecimalMode, true);
			endInstruction<SED>();
			break;
		}
		case NOP::Implicit::Official::value:
		{
			beginInstruction<NOP::Implicit::Official>();
			endInstruction<NOP::Implicit::Official>();
			break;
		}
		case NOP::ZeroPage::_1::value:
		{
			noOperation<NOP::ZeroPage::_1>([this]() { memory.fetchZeroPage(); });
			break;
		}
		case NOP::ZeroPage::_2::value:
		{
			noOperation<NOP::ZeroPage::_2>([this]() { memory.fetchZeroPage(); });
			break;
		}
		case NOP::ZeroPage::_3::value:
		{
			noOperation<NOP::ZeroPage::_3>([this]() { memory.fetchZeroPage(); });
			break;
		}
		case NOP::Absolute::_1::value:
		{
			noOperation<NOP::Absolute::_1>([this]() { memory.fetchAbsolute(); });
			break;
		}
		case NOP::ZeroPageX::_1::value:
		{
			noOperation<NOP::ZeroPageX::_1>([this]() { memory.fetchZeroPageX(); });
			break;
		}
		case NOP::ZeroPageX::_2::value:
		{
			noOperation<NOP::ZeroPageX::_2>([this]() { memory.fetchZeroPageX(); });
			break;
		}
		case NOP::ZeroPageX::_3::value:
		{
			noOperation<NOP::ZeroPageX::_3>([this]() { memory.fetchZeroPageX(); });
			break;
		}
		case NOP::ZeroPageX::_4::value:
		{
			noOperation<NOP::ZeroPageX::_4>([this]() { memory.fetchZeroPageX(); });
			break;
		}
		case NOP::ZeroPageX::_5::value:
		{
			noOperation<NOP::ZeroPageX::_5>([this]() { memory.fetchZeroPageX(); });
			break;
		}
		case NOP::ZeroPageX::_6::value:
		{
			noOperation<NOP::ZeroPageX::_6>([this]() { memory.fetchZeroPageX(); });
			break;
		}
		case NOP::Implicit::_1::value:
		{
			noOperation<NOP::Implicit::_1>();
			break;
		}
		case NOP::Implicit::_2::value:
		{
			noOperation<NOP::Implicit::_2>();
			break;
		}
		case NOP::Implicit::_3::value:
		{
			noOperation<NOP::Implicit::_3>();
			break;
		}
		case NOP::Implicit::_4::value:
		{
			noOperation<NOP::Implicit::_4>();
			break;
		}
		case NOP::Implicit::_5::value:
		{
			noOperation<NOP::Implicit::_5>();
			break;
		}
		case NOP::Implicit::_6::value:
		{
			noOperation<NOP::Implicit::_6>();
			break;
		}
		case NOP::Immediate::value:
		{
			noOperation<NOP::Immediate>([this]() { memory.fetchImmediate(); });
			break;
		}
		case NOP::AbsoluteX::_1::value:
		{
			noOperation<NOP::AbsoluteX::_1>([this]() { memory.fetchAbsoluteX(); });
			break;
		}
		case NOP::AbsoluteX::_2::value:
		{
			noOperation<NOP::AbsoluteX::_2>([this]() { memory.fetchAbsoluteX(); });
			break;
		}
		case NOP::AbsoluteX::_3::value:
		{
			noOperation<NOP::AbsoluteX::_3>([this]() { memory.fetchAbsoluteX(); });
			break;
		}
		case NOP::AbsoluteX::_4::value:
		{
			noOperation<NOP::AbsoluteX::_4>([this]() { memory.fetchAbsoluteX(); });
			break;
		}
		case NOP::AbsoluteX::_5::value:
		{
			noOperation<NOP::AbsoluteX::_5>([this]() { memory.fetchAbsoluteX(); });
			break;
		}
		case NOP::AbsoluteX::_6::value:
		{
			noOperation<NOP::AbsoluteX::_6>([this]() { memory.fetchAbsoluteX(); });
			break;
		}
		default:
		{
			std::cout << std::endl << std::hex << "Unsupported opcode \""
					  << std::setw(2) << static_cast<int>(opcode)
					  << "\", stopping." << std::endl;
			exit(1);
			break;
		}
	}
	std::cout << std::endl;
}

template<typename Memory, typename Mapping, bool DecimalMode>
void Core<Memory, Mapping, DecimalMode>::interruptReset()
{
	state.opcodeResult = 0;
	state.sp = -3; // cycle 0: sp = 0, then gets decremented 3 times, look more into this
	state.p = 0x24; // TODO: Properly configure the status flags
	state.pc = memory.readMemAddress(0xfffc);
	state.pc = 0xc000; // TODO: This is only for nestest.nes "auto mode"

	state.totalCycles = 7;
}

template class mos6502::Core<Mem6502<NESMemory>, NESMemory, false>;

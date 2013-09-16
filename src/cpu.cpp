#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

using namespace lib6502;

// =====================================================================================================================
Cpu::State::State()
    : m_A(0),
      m_X(0),
      m_Y(0),
      m_status(0),
      m_SP(0),
      m_PC(0),
      m_inInterrupt(false),
      m_ticks(0)
{
}

#include "statemacros.h"

// =====================================================================================================================
Cpu::Cpu(Memory& memory)
    : m_memory(memory),
      m_instrTracer(NULL),
      m_remainingTicks(0)
{
    buildAddressingModeTable();

    reset();
}

// =====================================================================================================================
bool Cpu::isInInterrupt() const
{
    return m_inInterrupt;
}

// =====================================================================================================================
Cpu::State& Cpu::getState()
{
    return m_state;
}

// =====================================================================================================================
void Cpu::setInstructionTracer(InstructionTracer* t)
{
    m_instrTracer = t;
}

// =====================================================================================================================
void Cpu::tick()
{
    unsigned ticks;

    // increment internal tick counter
    ++m_state.m_ticks;

    if (m_remainingTicks > 0)
    {
	--m_remainingTicks;
	return;
    }

    // save the state of the CPU for tracing
    m_instrState = m_state;

    // read the next opcode
    uint8_t opCode = read8();

    switch (opCode)
    {
#define INSTR(c, f) \
	case c : ticks = f(opCode); break;
	INSTR(0x00, brk);
	INSTR(0x01, oraAddr);
	INSTR(0x04, dop);
	INSTR(0x05, oraAddr);
	INSTR(0x06, aslAddr);
	INSTR(0x08, php);
	INSTR(0x09, oraImm);
	INSTR(0x0a, aslAcc);
	INSTR(0x0d, oraAddr);
	INSTR(0x0e, aslAddr);
	INSTR(0x10, bpl);
	INSTR(0x11, oraAddr);
	INSTR(0x14, dop);
	INSTR(0x15, oraAddr);
	INSTR(0x16, aslAddr);
	INSTR(0x18, clc);
	INSTR(0x19, oraAddr);
	INSTR(0x1a, nop);
	INSTR(0x1d, oraAddr);
	INSTR(0x1e, aslAddr);
	INSTR(0x20, jsrAbs);
	INSTR(0x21, andAddr);
	INSTR(0x24, bit);
	INSTR(0x25, andAddr);
	INSTR(0x26, rolAddr);
	INSTR(0x28, plp);
	INSTR(0x29, andImm);
	INSTR(0x2a, rolAcc);
	INSTR(0x2c, bit);
	INSTR(0x2d, andAddr);
	INSTR(0x2e, rolAddr);
	INSTR(0x30, bmi);
	INSTR(0x31, andAddr);
	INSTR(0x34, dop);
	INSTR(0x35, andAddr);
	INSTR(0x36, rolAddr);
	INSTR(0x38, sec);
	INSTR(0x39, andAddr);
	INSTR(0x3a, nop);
	INSTR(0x3d, andAddr);
	INSTR(0x3e, rolAddr);
	INSTR(0x40, rti);
	INSTR(0x41, eorAddr);
	INSTR(0x44, dop);
	INSTR(0x45, eorAddr);
	INSTR(0x46, lsrAddr);
	INSTR(0x48, pha);
	INSTR(0x49, eorImm);
	INSTR(0x4a, lsrAcc);
	INSTR(0x4c, jmpAbs);
	INSTR(0x4d, eorAddr);
	INSTR(0x4e, lsrAddr);
	INSTR(0x50, bvc);
	INSTR(0x51, eorAddr);
	INSTR(0x54, dop);
	INSTR(0x55, eorAddr);
	INSTR(0x56, lsrAddr);
	INSTR(0x58, cli);
	INSTR(0x59, eorAddr);
	INSTR(0x5a, nop);
	INSTR(0x5d, eorAddr);
	INSTR(0x5e, lsrAddr);
	INSTR(0x60, rts);
	INSTR(0x61, adcAddr);
	INSTR(0x64, dop);
	INSTR(0x65, adcAddr);
	INSTR(0x66, rorAddr);
	INSTR(0x68, pla);
	INSTR(0x69, adcImm);
	INSTR(0x6a, rorAcc);
	INSTR(0x6c, jmpInd);
	INSTR(0x6d, adcAddr);
	INSTR(0x6e, rorAddr);
	INSTR(0x70, bvs);
	INSTR(0x71, adcAddr);
	INSTR(0x74, dop);
	INSTR(0x75, adcAddr);
	INSTR(0x76, rorAddr);
	INSTR(0x78, sei);
	INSTR(0x79, adcAddr);
	INSTR(0x7a, nop);
	INSTR(0x7d, adcAddr);
	INSTR(0x7e, rorAddr);
	INSTR(0x80, dop);
	INSTR(0x81, sta);
	INSTR(0x82, dop);
	INSTR(0x84, sty);
	INSTR(0x85, sta);
	INSTR(0x86, stx);
	INSTR(0x88, dey);
	INSTR(0x89, dop);
	INSTR(0x8a, txa);
	INSTR(0x8c, sty);
	INSTR(0x8d, sta);
	INSTR(0x8e, stx);
	INSTR(0x90, bcc);
	INSTR(0x91, sta);
	INSTR(0x94, sty);
	INSTR(0x95, sta);
	INSTR(0x96, stx);
	INSTR(0x98, tya);
	INSTR(0x99, sta);
	INSTR(0x9a, txs);
	INSTR(0x9d, sta);
	INSTR(0xa0, ldyImm);
	INSTR(0xa1, ldaAddr);
	INSTR(0xa2, ldxImm);
	INSTR(0xa4, ldyAddr);
	INSTR(0xa5, ldaAddr);
	INSTR(0xa6, ldxAddr);
	INSTR(0xa8, tay);
	INSTR(0xa9, ldaImm);
	INSTR(0xaa, tax);
	INSTR(0xac, ldyAddr);
	INSTR(0xad, ldaAddr);
	INSTR(0xae, ldxAddr);
	INSTR(0xb0, bcs);
	INSTR(0xb1, ldaAddr);
	INSTR(0xb4, ldyAddr);
	INSTR(0xb5, ldaAddr);
	INSTR(0xb6, ldxAddr);
	INSTR(0xb8, clv);
	INSTR(0xb9, ldaAddr);
	INSTR(0xba, tsx);
	INSTR(0xbc, ldyAddr);
	INSTR(0xbd, ldaAddr);
	INSTR(0xbe, ldxAddr);
	INSTR(0xc0, cpyImm);
	INSTR(0xc1, cmpAddr);
	INSTR(0xc2, dop);
	INSTR(0xc4, cpyAddr);
	INSTR(0xc5, cmpAddr);
	INSTR(0xc6, dec);
	INSTR(0xc8, iny);
	INSTR(0xc9, cmpImm);
	INSTR(0xca, dex);
	INSTR(0xcc, cpyAddr);
	INSTR(0xcd, cmpAddr);
	INSTR(0xce, dec);
	INSTR(0xd0, bne);
	INSTR(0xd1, cmpAddr);
	INSTR(0xd4, dop);
	INSTR(0xd5, cmpAddr);
	INSTR(0xd6, dec);
	INSTR(0xd8, cld);
	INSTR(0xd9, cmpAddr);
	INSTR(0xda, nop);
	INSTR(0xdd, cmpAddr);
	INSTR(0xde, dec);
	INSTR(0xe0, cpxImm);
	INSTR(0xe1, sbcAddr);
	INSTR(0xe2, dop);
	INSTR(0xe4, cpxAddr);
	INSTR(0xe5, sbcAddr);
	INSTR(0xe6, inc);
	INSTR(0xe8, inx);
	INSTR(0xe9, sbcImm);
	INSTR(0xea, nop);
	INSTR(0xeb, sbcImm); // same as $e9
	INSTR(0xec, cpxAddr);
	INSTR(0xed, sbcAddr);
	INSTR(0xee, inc);
	INSTR(0xf0, beq);
	INSTR(0xf1, sbcAddr);
	INSTR(0xf4, dop);
	INSTR(0xf5, sbcAddr);
	INSTR(0xf6, inc);
	INSTR(0xf8, sed);
	INSTR(0xf9, sbcAddr);
	INSTR(0xfa, nop);
	INSTR(0xfd, sbcAddr);
	INSTR(0xfe, inc);
#undef INSTR
	default :
	    throw CpuException(MakeString(true) << "invalid opcode: " << std::setw(2) << std::setfill('0') << (unsigned)opCode);
    }

    if (ticks == 0)
	throw CpuException(MakeString(true) << "instruction with 0 ticks? (opcode=" << (unsigned)opCode << ")");

    m_remainingTicks = ticks - 1;
}

// =====================================================================================================================
void Cpu::reset()
{
    m_PC = (m_memory.read(0xfffd) << 8) | m_memory.read(0xfffc);
    m_SP = 0xfd;
    m_status = Always1 | IntDisable;
    m_inInterrupt = false;

    // reset our internal tick counter
    m_state.m_ticks = 0;
}

// =====================================================================================================================
void Cpu::nmi()
{
    performInterrupt(0xfffa);
}

// =====================================================================================================================
void Cpu::buildAddressingModeTable()
{
    m_addrModeTable[ZeroPage] = std::bind(&Cpu::addrZero, this, std::placeholders::_1);
    m_addrModeTable[ZeroPageX] = std::bind(&Cpu::addrZeroX, this, std::placeholders::_1);
    m_addrModeTable[ZeroPageY] = std::bind(&Cpu::addrZeroY, this, std::placeholders::_1);
    m_addrModeTable[Absolute] = std::bind(&Cpu::addrAbsolute, this, std::placeholders::_1);
    m_addrModeTable[AbsoluteX] = std::bind(&Cpu::addrAbsoluteX, this, std::placeholders::_1);
    m_addrModeTable[AbsoluteY] = std::bind(&Cpu::addrAbsoluteY, this, std::placeholders::_1);
    m_addrModeTable[IndirectX] = std::bind(&Cpu::addrIndirectX, this, std::placeholders::_1);
    m_addrModeTable[IndirectY] = std::bind(&Cpu::addrIndirectY, this, std::placeholders::_1);
}

#ifdef HAVE_INSTRUCTION_TRACE
// =====================================================================================================================
void Cpu::traceInstruction(const std::string& instr, const std::string& param)
{
    if (m_instrTracer)
    {
	std::string trace = instr;

	if (!param.empty())
	{
	    trace += " ";
	    trace += param;
	}

	m_instrTracer->trace(m_instrState, trace);
    }
}
#endif

// =====================================================================================================================
uint8_t Cpu::read8()
{
    return m_memory.read(m_PC++);
}

// =====================================================================================================================
uint16_t Cpu::read16()
{
    return read8() | (read8() << 8);
}

// =====================================================================================================================
void Cpu::push8(uint8_t data)
{
    m_memory.write(0x100 /* stack prefix? */ + m_SP, data);
    --m_SP;
}

// =====================================================================================================================
uint8_t Cpu::pop8()
{
    ++m_SP;
    return m_memory.read(0x100 /* stack prefix */ + m_SP);
}

// =====================================================================================================================
void Cpu::performInterrupt(uint16_t vectorBase)
{
    m_inInterrupt = true;

    // save the current PC
    push8(m_PC >> 8);
    push8(m_PC & 0xff);
    // save the status
    push8(m_status);

    // disable interrupts
    m_status |= IntDisable;

    // read the NMI vector
    m_PC = (m_memory.read(vectorBase + 1) << 8) | m_memory.read(vectorBase);
}

// =====================================================================================================================
Cpu::AddressingMode Cpu::getAddressingMode(uint8_t opCode)
{
    unsigned bbb = (opCode >> 2) & 0x7;
    unsigned cc = opCode & 0x3;

    switch (cc)
    {
	case 0 :
	    switch (bbb)
	    {
		// 0 is immediate
		case 1 : return ZeroPage;
		case 3 : return Absolute;
		case 5 : return ZeroPageX;
		case 7 : return AbsoluteX;
		default : throw CpuException(MakeString() << "invalid BBB value with CC=00: " << bbb);
	    }
	    break;

	case 1 :
	    switch (bbb)
	    {
		case 0 : return IndirectX;
		case 1 : return ZeroPage;
		// 2 is immediate
		case 3 : return Absolute;
		case 4 : return IndirectY;
		case 5 : return ZeroPageX;
		case 6 : return AbsoluteY;
		case 7 : return AbsoluteX;
		default : throw CpuException(MakeString() << "invalid BBB value with CC=01: " << bbb);
	    }
	    break;

	case 2 :
	    switch (bbb)
	    {
		// 0 is immediate
		case 1 : return ZeroPage;
		// 2 is accumulator
		case 3 : return Absolute;
		case 5 : return ZeroPageX;
		case 7 : return AbsoluteX;
		default : throw CpuException(MakeString() << "invalid BBB value with CC=02: " << bbb);
	    }
	    break;

	default :
	    throw CpuException(MakeString() << "invalid CC value: " << cc);
    }
}

// =====================================================================================================================
uint16_t Cpu::addrZero(std::string& trace)
{
    uint8_t addr = read8();
#ifdef HAVE_INSTRUCTION_TRACE
    trace = MakeString(true) << "$" << std::setw(2) << std::setfill('0') << (int)addr;
#endif
    return addr;
}

// =====================================================================================================================
uint16_t Cpu::addrZeroX(std::string& trace)
{
    uint8_t addr = read8();
#ifdef HAVE_INSTRUCTION_TRACE
    trace = MakeString(true) << "$" << std::setw(2) << std::setfill('0') << (int)addr << ",X";
#endif
    return (addr + m_X) & 0xff;
}

// =====================================================================================================================
uint16_t Cpu::addrZeroY(std::string& trace)
{
    uint8_t addr = read8();
#ifdef HAVE_INSTRUCTION_TRACE
    trace = MakeString(true) << "$" << std::setw(2) << std::setfill('0') << (int)addr << ",Y";
#endif
    return (addr + m_Y) & 0xff;
}

// =====================================================================================================================
uint16_t Cpu::addrAbsolute(std::string& trace)
{
    uint16_t addr = read16();
#ifdef HAVE_INSTRUCTION_TRACE
    trace = MakeString(true) << "$" << std::setw(4) << std::setfill('0') << addr;
#endif
    return addr;
}

// =====================================================================================================================
uint16_t Cpu::addrAbsoluteX(std::string& trace)
{
    uint16_t addr = read16();
#ifdef HAVE_INSTRUCTION_TRACE
    trace = MakeString(true) << "$" << std::setw(4) << std::setfill('0') << addr << ",X";
#endif
    return addr + m_X;
}

// =====================================================================================================================
uint16_t Cpu::addrAbsoluteY(std::string& trace)
{
    uint16_t addr = read16();
#ifdef HAVE_INSTRUCTION_TRACE
    trace = MakeString(true) << "$" << std::setw(4) << std::setfill('0') << addr << ",Y";
#endif
    return addr + m_Y;
}

// =====================================================================================================================
uint16_t Cpu::addrIndirectX(std::string& trace)
{
    uint8_t off = read8();

#ifdef HAVE_INSTRUCTION_TRACE
    trace = MakeString(true) << "($" << std::setw(2) << std::setfill('0') << (int)off << ",X)";
#endif

    uint16_t addr;
    off += m_X;
    addr = m_memory.read(off);
    ++off;
    addr |= (m_memory.read(off) << 8);

    return addr;
}

// =====================================================================================================================
uint16_t Cpu::addrIndirectY(std::string& trace)
{
    uint8_t off = read8();

#ifdef HAVE_INSTRUCTION_TRACE
    trace = MakeString(true) << "$(" << std::setw(2) << std::setfill('0') << (int)off << "),Y";
#endif

    uint16_t addr;
    addr = m_memory.read(off);
    ++off;
    addr |= (m_memory.read(off) << 8);

    return addr + m_Y;
}

// =====================================================================================================================
void Cpu::setOrClearStatus(uint8_t flagMask, bool condition)
{
    if (condition)
	m_status |= flagMask;
    else
	m_status &= ~flagMask;
}

// =====================================================================================================================
void Cpu::updateZero(uint8_t value)
{
    setOrClearStatus(Zero, value == 0);
}

// =====================================================================================================================
void Cpu::updateSign(uint8_t value)
{
    setOrClearStatus(Sign, (value & 0x80) != 0);
}

// =====================================================================================================================
unsigned Cpu::jmpAbs(uint8_t opCode)
{
    std::string addrTrace;
    m_PC = addrAbsolute(addrTrace);
#ifdef HAVE_INSTRUCTION_TRACE
    traceInstruction("JMP", addrTrace);
#endif
    return 3;
}

// =====================================================================================================================
unsigned Cpu::jmpInd(uint8_t opCode)
{
    uint16_t addr = read16();

#ifdef HAVE_INSTRUCTION_TRACE
    traceInstruction("JMP", MakeString(true) << "($" << std::setw(4) << std::setfill('0') << addr << ")");
#endif

    // implement the buggy way how indirect jump works in the 6502, lol :-]
    uint16_t addr2 = addr + 1;
    if ((addr & 0xff) == 0xff)
	addr2 -= 0x100;

    m_PC = m_memory.read(addr) | (m_memory.read(addr2) << 8);

    return 5;
}

// =====================================================================================================================
unsigned Cpu::jsrAbs(uint8_t opCode)
{
    std::string addrTrace;
    uint16_t addr = addrAbsolute(addrTrace);

#ifdef HAVE_INSTRUCTION_TRACE
    traceInstruction("JSR", addrTrace);
#endif

    // push the old PC-1 to the stack
    uint16_t pc = m_PC - 1;
    // push the high part of the PC first
    push8(pc >> 8);
    // .. and push the low after that
    push8(pc & 0xff);

    m_PC = addr;

    return 6;
}

// =====================================================================================================================
unsigned Cpu::rts(uint8_t opCode)
{
#ifdef HAVE_INSTRUCTION_TRACE
    traceInstruction("RTS");
#endif

    uint16_t pc = pop8() | (pop8() << 8);
    m_PC = pc + 1;

    return 6;
}

// =====================================================================================================================
unsigned Cpu::rti(uint8_t opCode)
{
#ifdef HAVE_INSTRUCTION_TRACE
    traceInstruction("RTI");
#endif

    // do some sanity checking
    //if (!m_inInterrupt)
    //	throw CpuException("executed RTI without being in an interrupt");

    // restore status
    m_status = pop8() | Always1;
    // restore PC
    m_PC = pop8() | (pop8() << 8);

    // enable interrupts
    //m_status &= ~IntDisable;

    // we are not handling an interrupt anymore
    m_inInterrupt = false;

    return 6;
}

// =====================================================================================================================
unsigned Cpu::nop(uint8_t opCode)
{
#ifdef HAVE_INSTRUCTION_TRACE
    traceInstruction("NOP");
#endif
    return 2;
}

// =====================================================================================================================
unsigned Cpu::dop(uint8_t opCode)
{
#ifdef HAVE_INSTRUCTION_TRACE
    traceInstruction("DOP");
#endif
    read8();
    // TODO: calculate this properly!
    return 2;
}

// =====================================================================================================================
unsigned Cpu::brk(uint8_t opCode)
{
#ifdef HAVE_INSTRUCTION_TRACE
    traceInstruction("BRK");
#endif

    m_status |= Break;
    ++m_PC;
    performInterrupt(0xfffe);

    return 7;
}

// =====================================================================================================================
unsigned Cpu::txa(uint8_t opCode)
{
#ifdef HAVE_INSTRUCTION_TRACE
    traceInstruction("TXA");
#endif
    m_A = m_X;
    updateZero(m_A);
    updateSign(m_A);
    return 2;
}

// =====================================================================================================================
unsigned Cpu::tax(uint8_t opCode)
{
#ifdef HAVE_INSTRUCTION_TRACE
    traceInstruction("TAX");
#endif
    m_X = m_A;
    updateZero(m_X);
    updateSign(m_X);
    return 2;
}

// =====================================================================================================================
unsigned Cpu::tya(uint8_t opCode)
{
#ifdef HAVE_INSTRUCTION_TRACE
    traceInstruction("TYA");
#endif
    m_A = m_Y;
    updateZero(m_A);
    updateSign(m_A);
    return 2;
}

// =====================================================================================================================
unsigned Cpu::tay(uint8_t opCode)
{
#ifdef HAVE_INSTRUCTION_TRACE
    traceInstruction("TAY");
#endif
    m_Y = m_A;
    updateZero(m_Y);
    updateSign(m_Y);
    return 2;
}

// =====================================================================================================================
unsigned Cpu::inx(uint8_t opCode)
{
#ifdef HAVE_INSTRUCTION_TRACE
    traceInstruction("INX");
#endif
    ++m_X;
    updateZero(m_X);
    updateSign(m_X);
    return 2;
}

// =====================================================================================================================
unsigned Cpu::dex(uint8_t opCode)
{
#ifdef HAVE_INSTRUCTION_TRACE
    traceInstruction("DEX");
#endif
    --m_X;
    updateZero(m_X);
    updateSign(m_X);
    return 2;
}

// =====================================================================================================================
unsigned Cpu::iny(uint8_t opCode)
{
#ifdef HAVE_INSTRUCTION_TRACE
    traceInstruction("INY");
#endif
    ++m_Y;
    updateZero(m_Y);
    updateSign(m_Y);
    return 2;
}

// =====================================================================================================================
unsigned Cpu::dey(uint8_t opCode)
{
#ifdef HAVE_INSTRUCTION_TRACE
    traceInstruction("DEY");
#endif
    --m_Y;
    updateZero(m_Y);
    updateSign(m_Y);
    return 2;
}

// =====================================================================================================================
unsigned Cpu::bit(uint8_t opCode)
{
    std::string addrTrace;
    uint16_t addr = m_addrModeTable[getAddressingMode(opCode)](addrTrace);

#ifdef HAVE_INSTRUCTION_TRACE
    traceInstruction("BIT", addrTrace);
#endif

    uint8_t data = m_memory.read(addr);
    setOrClearStatus(Sign, ((data >> 7) & 1) == 1);
    setOrClearStatus(Overflow, ((data >> 6) & 1) == 1);
    updateZero(m_A & data);

    // TODO
    return 3;
}

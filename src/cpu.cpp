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
      m_inInterrupt(false)
{
}

#include "statemacros.h"

// =====================================================================================================================
Cpu::Cpu(Memory& memory)
    : m_memory(memory),
      m_instrTracer(NULL)
{
    // build the instruction handler table
    buildInstructionTable();

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
    // save the state of the CPU for tracing
    m_instrState = m_state;

    uint8_t opCode = read8();
    auto instr = m_instrTable[opCode];

    if (!instr)
	throw CpuException(MakeString() << "invalid opcode: " << std::hex << std::setw(2) << std::setfill('0') << (int)opCode);

    // perform the instruction
    instr();
}

// =====================================================================================================================
void Cpu::reset()
{
    m_PC = (m_memory.read(0xfffd) << 8) | m_memory.read(0xfffc);
    m_SP = 0xfd;
    m_status = Always1 | IntDisable;
    m_inInterrupt = false;
}

// =====================================================================================================================
void Cpu::nmi()
{
    performInterrupt(0xfffa);
}

// =====================================================================================================================
void Cpu::dumpState(std::ostream& s)
{
    s << "PC=" << std::hex << std::setw(4) << std::setfill('0') << m_PC << std::endl;
    s << "SP=" << std::hex << std::setw(2) << std::setfill('0') << (int)m_SP << std::endl;
}

// =====================================================================================================================
void Cpu::buildInstructionTable()
{
#define INSTR(code, func) \
    m_instrTable[code] = std::bind(&Cpu::func, this)

    INSTR(0x00, brk);
    INSTR(0x01, oraIndX);
    INSTR(0x04, dop);
    INSTR(0x05, oraZero);
    INSTR(0x06, aslZero);
    INSTR(0x08, php);
    INSTR(0x09, oraImm);
    INSTR(0x0a, aslAcc);
    INSTR(0x0d, oraAbs);
    INSTR(0x0e, aslAbs);
    INSTR(0x10, bpl);
    INSTR(0x11, oraIndY);
    INSTR(0x14, dop);
    INSTR(0x15, oraZeroX);
    INSTR(0x16, aslZeroX);
    INSTR(0x18, clc);
    INSTR(0x19, oraAbsY);
    INSTR(0x1a, nop);
    INSTR(0x1d, oraAbsX);
    INSTR(0x1e, aslAbsX);
    INSTR(0x20, jsrAbs);
    INSTR(0x21, andIndX);
    INSTR(0x24, bitZero);
    INSTR(0x25, andZero);
    INSTR(0x26, rolZero);
    INSTR(0x28, plp);
    INSTR(0x29, andImm);
    INSTR(0x2a, rolAcc);
    INSTR(0x2c, bitAbs);
    INSTR(0x2d, andAbs);
    INSTR(0x2e, rolAbs);
    INSTR(0x30, bmi);
    INSTR(0x31, andIndY);
    INSTR(0x34, dop);
    INSTR(0x35, andZeroX);
    INSTR(0x36, rolZeroX);
    INSTR(0x38, sec);
    INSTR(0x39, andAbsY);
    INSTR(0x3a, nop);
    INSTR(0x3d, andAbsX);
    INSTR(0x3e, rolAbsX);
    INSTR(0x40, rti);
    INSTR(0x41, eorIndX);
    INSTR(0x44, dop);
    INSTR(0x45, eorZero);
    INSTR(0x46, lsrZero);
    INSTR(0x48, pha);
    INSTR(0x49, eorImm);
    INSTR(0x4a, lsrAcc);
    INSTR(0x4c, jmpAbs);
    INSTR(0x4d, eorAbs);
    INSTR(0x4e, lsrAbs);
    INSTR(0x50, bvc);
    INSTR(0x51, eorIndY);
    INSTR(0x54, dop);
    INSTR(0x55, eorZeroX);
    INSTR(0x56, lsrZeroX);
    INSTR(0x58, cli);
    INSTR(0x59, eorAbsY);
    INSTR(0x5a, nop);
    INSTR(0x5d, eorAbsX);
    INSTR(0x5e, lsrAbsX);
    INSTR(0x60, rts);
    INSTR(0x61, adcIndX);
    INSTR(0x64, dop);
    INSTR(0x65, adcZero);
    INSTR(0x66, rorZero);
    INSTR(0x68, pla);
    INSTR(0x69, adcImm);
    INSTR(0x6a, rorAcc);
    INSTR(0x6c, jmpInd);
    INSTR(0x6d, adcAbs);
    INSTR(0x6e, rorAbs);
    INSTR(0x70, bvs);
    INSTR(0x71, adcIndY);
    INSTR(0x74, dop);
    INSTR(0x75, adcZeroX);
    INSTR(0x76, rorZeroX);
    INSTR(0x78, sei);
    INSTR(0x79, adcAbsY);
    INSTR(0x7a, nop);
    INSTR(0x7d, adcAbsX);
    INSTR(0x7e, rorAbsX);
    INSTR(0x80, dop);
    INSTR(0x81, staIndX);
    INSTR(0x82, dop);
    INSTR(0x84, styZero);
    INSTR(0x85, staZero);
    INSTR(0x86, stxZero);
    INSTR(0x88, dey);
    INSTR(0x89, dop);
    INSTR(0x8a, txa);
    INSTR(0x8c, styAbs);
    INSTR(0x8d, staAbs);
    INSTR(0x8e, stxAbs);
    INSTR(0x90, bcc);
    INSTR(0x91, staIndY);
    INSTR(0x94, styZeroX);
    INSTR(0x95, staZeroX);
    INSTR(0x96, stxZeroY);
    INSTR(0x98, tya);
    INSTR(0x99, staAbsY);
    INSTR(0x9a, txs);
    INSTR(0x9d, staAbsX);
    INSTR(0xa0, ldyImm);
    INSTR(0xa1, ldaIndX);
    INSTR(0xa2, ldxImm);
    INSTR(0xa4, ldyZero);
    INSTR(0xa5, ldaZero);
    INSTR(0xa6, ldxZero);
    INSTR(0xa8, tay);
    INSTR(0xa9, ldaImm);
    INSTR(0xaa, tax);
    INSTR(0xac, ldyAbs);
    INSTR(0xad, ldaAbs);
    INSTR(0xae, ldxAbs);
    INSTR(0xb0, bcs);
    INSTR(0xb1, ldaIndY);
    INSTR(0xb4, ldyZeroX);
    INSTR(0xb5, ldaZeroX);
    INSTR(0xb6, ldxZeroY);
    INSTR(0xb8, clv);
    INSTR(0xb9, ldaAbsY);
    INSTR(0xba, tsx);
    INSTR(0xbc, ldyAbsX);
    INSTR(0xbd, ldaAbsX);
    INSTR(0xbe, ldxAbsY);
    INSTR(0xc0, cpyImm);
    INSTR(0xc1, cmpIndX);
    INSTR(0xc2, dop);
    INSTR(0xc4, cpyZero);
    INSTR(0xc5, cmpZero);
    INSTR(0xc6, decZero);
    INSTR(0xc8, iny);
    INSTR(0xc9, cmpImm);
    INSTR(0xca, dex);
    INSTR(0xcc, cpyAbs);
    INSTR(0xcd, cmpAbs);
    INSTR(0xce, decAbs);
    INSTR(0xd0, bne);
    INSTR(0xd1, cmpIndY);
    INSTR(0xd4, dop);
    INSTR(0xd5, cmpZeroX);
    INSTR(0xd6, decZeroX);
    INSTR(0xd8, cld);
    INSTR(0xd9, cmpAbsY);
    INSTR(0xda, nop);
    INSTR(0xdd, cmpAbsX);
    INSTR(0xde, decAbsX);
    INSTR(0xe0, cpxImm);
    INSTR(0xe1, sbcIndX);
    INSTR(0xe2, dop);
    INSTR(0xe4, cpxZero);
    INSTR(0xe5, sbcZero);
    INSTR(0xe6, incZero);
    INSTR(0xe8, inx);
    INSTR(0xe9, sbcImm);
    INSTR(0xea, nop);
    INSTR(0xeb, sbcImm); // same as $e9
    INSTR(0xec, cpxAbs);
    INSTR(0xed, sbcAbs);
    INSTR(0xee, incAbs);
    INSTR(0xf0, beq);
    INSTR(0xf1, sbcIndY);
    INSTR(0xf4, dop);
    INSTR(0xf5, sbcZeroX);
    INSTR(0xf6, incZeroX);
    INSTR(0xf8, sed);
    INSTR(0xf9, sbcAbsY);
    INSTR(0xfa, nop);
    INSTR(0xfd, sbcAbsX);
    INSTR(0xfe, incAbsX);

#undef INSTR
}

// =====================================================================================================================
void Cpu::traceInstruction(const std::string& s)
{
    if (m_instrTracer)
	m_instrTracer->trace(m_instrState, s);
}

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
uint8_t Cpu::addrZero()
{
    return read8();
}

// =====================================================================================================================
uint8_t Cpu::addrZeroX(uint8_t& offset)
{
    offset = read8();
    return offset + m_X;
}
// =====================================================================================================================
uint8_t Cpu::addrZeroY(uint8_t& offset)
{
    offset = read8();
    return offset + m_Y;
}

// =====================================================================================================================
uint16_t Cpu::addrAbsolute()
{
    return read16();
}

// =====================================================================================================================
uint16_t Cpu::addrAbsoluteX(uint16_t& abs)
{
    abs = read16();
    return abs + m_X;
}

// =====================================================================================================================
uint16_t Cpu::addrAbsoluteY(uint16_t& abs)
{
    abs = read16();
    return abs + m_Y;
}

// =====================================================================================================================
uint16_t Cpu::addrIndirectX(uint8_t& offset)
{
    uint8_t off = read8();
    off += m_X;
    offset = off;

    uint16_t addr;
    addr = m_memory.read(off);
    ++off;
    addr |= (m_memory.read(off) << 8);

    return addr;
}

// =====================================================================================================================
uint16_t Cpu::addrIndirectY(uint8_t& offset)
{
    uint8_t off = read8();
    offset = off;

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
void Cpu::jmpAbs()
{
    m_PC = addrAbsolute();
    traceInstruction(MakeString() << "JMP $" << std::hex << std::setw(4) << std::setfill('0') << m_PC);
}

// =====================================================================================================================
void Cpu::jmpInd()
{
    uint16_t addr = read16();
    traceInstruction(MakeString() << "JMP ($" << std::hex << std::setw(4) << std::setfill('0') << addr << ")");

    // implement the buggy way how indirect jump works in the 6502, lol :-]
    uint16_t addr2 = addr + 1;
    if ((addr & 0xff) == 0xff)
	addr2 -= 0x100;

    m_PC = m_memory.read(addr) | (m_memory.read(addr2) << 8);
}

// =====================================================================================================================
void Cpu::jsrAbs()
{
    uint16_t addr = addrAbsolute();
    traceInstruction(MakeString(true) << "JSR $" << std::setw(4) << std::setfill('0') << addr);

    // push the old PC-1 to the stack
    uint16_t pc = m_PC - 1;
    // push the high part of the PC first
    push8(pc >> 8);
    // .. and push the low after that
    push8(pc & 0xff);

    m_PC = addr;
}

// =====================================================================================================================
void Cpu::rts()
{
    traceInstruction("RTS");

    uint16_t pc = pop8() | (pop8() << 8);
    m_PC = pc + 1;
}

// =====================================================================================================================
void Cpu::rti()
{
    traceInstruction("RTI");

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
}

// =====================================================================================================================
void Cpu::nop()
{
    traceInstruction("NOP");
}

// =====================================================================================================================
void Cpu::dop()
{
    traceInstruction("DOP");
    read8();
}

// =====================================================================================================================
void Cpu::brk()
{
    traceInstruction("BRK");

    m_status |= Break;
    ++m_PC;
    performInterrupt(0xfffe);
}

// =====================================================================================================================
void Cpu::txa()
{
    traceInstruction("TXA");
    m_A = m_X;
    updateZero(m_A);
    updateSign(m_A);
}

// =====================================================================================================================
void Cpu::tax()
{
    traceInstruction("TAX");
    m_X = m_A;
    updateZero(m_X);
    updateSign(m_X);
}

// =====================================================================================================================
void Cpu::tya()
{
    traceInstruction("TYA");
    m_A = m_Y;
    updateZero(m_A);
    updateSign(m_A);
}

// =====================================================================================================================
void Cpu::tay()
{
    traceInstruction("TAY");
    m_Y = m_A;
    updateZero(m_Y);
    updateSign(m_Y);
}

// =====================================================================================================================
void Cpu::inx()
{
    traceInstruction("INX");
    ++m_X;
    updateZero(m_X);
    updateSign(m_X);
}

// =====================================================================================================================
void Cpu::dex()
{
    traceInstruction("DEX");
    --m_X;
    updateZero(m_X);
    updateSign(m_X);
}

// =====================================================================================================================
void Cpu::iny()
{
    traceInstruction("INY");
    ++m_Y;
    updateZero(m_Y);
    updateSign(m_Y);
}

// =====================================================================================================================
void Cpu::dey()
{
    traceInstruction("DEY");
    --m_Y;
    updateZero(m_Y);
    updateSign(m_Y);
}

// =====================================================================================================================
void Cpu::bitZero()
{
    uint16_t addr = addrZero();
    traceInstruction(MakeString() << "BIT $" << std::hex << std::setw(2) << std::setfill('0') << addr);

    uint8_t data = m_memory.read(addr);
    setOrClearStatus(Sign, ((data >> 7) & 1) == 1);
    setOrClearStatus(Overflow, ((data >> 6) & 1) == 1);
    updateZero(m_A & data);
}

// =====================================================================================================================
void Cpu::bitAbs()
{
    uint16_t addr = addrAbsolute();
    traceInstruction(MakeString() << "BIT $" << std::hex << std::setw(4) << std::setfill('0') << addr);

    uint8_t data = m_memory.read(addr);
    setOrClearStatus(Sign, ((data >> 7) & 1) == 1);
    setOrClearStatus(Overflow, ((data >> 6) & 1) == 1);
    updateZero(m_A & data);
}

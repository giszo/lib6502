#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#define m_A m_state.m_A
#define m_X m_state.m_X
#define m_Y m_state.m_Y
#define m_status m_state.m_status
#define m_SP m_state.m_SP
#define m_PC m_state.m_PC
#define m_inInterrupt m_state.m_inInterrupt

using namespace lib6502;

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
    m_SP = 0xff;
    m_status = 0x00;
    m_inInterrupt = false;
}

// =====================================================================================================================
void Cpu::nmi()
{
    m_inInterrupt = true;

    // disable interrupts
    m_status |= INT_DISABLE;
    // save the current PC
    push8(m_PC >> 8);
    push8(m_PC & 0xff);
    // save the status
    push8(m_status);

    // read the NMI vector
    m_PC = (m_memory.read(0xfffb) << 8) | m_memory.read(0xfffa);
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

    INSTR(0x05, oraZero);
    INSTR(0x09, oraImm);
    INSTR(0x0a, aslAcc);
    INSTR(0x10, bpl);
    INSTR(0x18, clc);
    INSTR(0x20, jsrAbs);
    INSTR(0x24, bitZero);
    INSTR(0x26, rolZero);
    INSTR(0x29, andImm);
    INSTR(0x2a, rolAcc);
    INSTR(0x2c, bitAbs);
    INSTR(0x2e, rolAbs);
    INSTR(0x30, bmi);
    INSTR(0x38, sec);
    INSTR(0x3d, andAbsX);
    INSTR(0x40, rti);
    INSTR(0x45, eorZero);
    INSTR(0x46, lsrZero);
    INSTR(0x48, pha);
    INSTR(0x49, eorImm);
    INSTR(0x4a, lsrAcc);
    INSTR(0x4c, jmpAbs);
    INSTR(0x4e, lsrAbs);
    INSTR(0x60, rts);
    INSTR(0x65, adcZero);
    INSTR(0x68, pla);
    INSTR(0x69, adcImm);
    INSTR(0x6a, rorAcc);
    INSTR(0x6c, jmpInd);
    INSTR(0x6d, adcAbs);
    INSTR(0x78, sei);
    INSTR(0x79, adcAbsY);
    INSTR(0x7e, rorAbsX);
    INSTR(0x84, styZero);
    INSTR(0x85, staZero);
    INSTR(0x86, stxZero);
    INSTR(0x88, dey);
    INSTR(0x8a, txa);
    INSTR(0x8c, styAbs);
    INSTR(0x8d, staAbs);
    INSTR(0x8e, stxAbs);
    INSTR(0x90, bcc);
    INSTR(0x91, staIndY);
    INSTR(0x98, tya);
    INSTR(0x99, staAbsY);
    INSTR(0x9a, txs);
    INSTR(0x9d, staAbsX);
    INSTR(0xa0, ldyImm);
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
    INSTR(0xb9, ldaAbsY);
    INSTR(0xbc, ldyAbsX);
    INSTR(0xbd, ldaAbsX);
    INSTR(0xbe, ldxAbsY);
    INSTR(0xc0, cpyImm);
    INSTR(0xc5, cmpZero);
    INSTR(0xc6, decZero);
    INSTR(0xc8, iny);
    INSTR(0xc9, cmpImm);
    INSTR(0xca, dex);
    INSTR(0xcd, cmpAbs);
    INSTR(0xce, decAbs);
    INSTR(0xd0, bne);
    INSTR(0xd8, cld);
    INSTR(0xd9, cmpAbsY);
    INSTR(0xde, decAbsX);
    INSTR(0xe0, cpxImm);
    INSTR(0xe6, incZero);
    INSTR(0xe8, inx);
    INSTR(0xe9, sbcImm);
    INSTR(0xee, incAbs);
    INSTR(0xf0, beq);
    INSTR(0xf9, sbcAbsY);

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
    setOrClearStatus(ZERO, value == 0);
}

// =====================================================================================================================
void Cpu::updateSign(uint8_t value)
{
    setOrClearStatus(SIGN, (value & 0x80) != 0);
}

// =====================================================================================================================
void Cpu::bpl()
{
    traceInstruction("BPL");

    int8_t off = static_cast<int8_t>(read8());
    if ((m_status & SIGN) == 0)
	m_PC += off;
}

// =====================================================================================================================
void Cpu::bcs()
{
    traceInstruction("BCS");

    int8_t off = static_cast<int8_t>(read8());
    if (m_status & CARRY)
	m_PC += off;
}

// =====================================================================================================================
void Cpu::bne()
{
    traceInstruction("BNE");

    int8_t off = static_cast<int8_t>(read8());
    if ((m_status & ZERO) == 0)
	m_PC += off;
}

// =====================================================================================================================
void Cpu::beq()
{
    traceInstruction("BEQ");

    int8_t off = static_cast<int8_t>(read8());
    if (m_status & ZERO)
	m_PC += off;
}

// =====================================================================================================================
void Cpu::bcc()
{
    traceInstruction("BCC");

    int8_t off = static_cast<int8_t>(read8());
    if ((m_status & CARRY) == 0)
	m_PC += off;
}

// =====================================================================================================================
void Cpu::bmi()
{
    traceInstruction("BMI");

    int8_t off = static_cast<int8_t>(read8());
    if (m_status & SIGN)
	m_PC += off;
}

// =====================================================================================================================
void Cpu::jmpAbs()
{
    m_PC = read16();
    traceInstruction(MakeString() << "JMP $" << std::hex << std::setw(4) << std::setfill('0') << m_PC);
}

// =====================================================================================================================
void Cpu::jmpInd()
{
    uint16_t addr = read16();
    traceInstruction(MakeString() << "JMP ($" << std::hex << std::setw(4) << std::setfill('0') << addr << ")");
    m_PC = m_memory.read(addr) | (m_memory.read(addr + 1) << 8);
}

// =====================================================================================================================
void Cpu::jsrAbs()
{
    uint16_t addr = read16();
    traceInstruction(MakeString() << "JSR $" << std::hex << std::setw(4) << std::setfill('0') << addr);

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

    // restore status
    m_status = pop8();
    // restore PC
    m_PC = pop8() | (pop8() << 8);

    // enable interrupts
    m_status &= INT_DISABLE;

    // we are not handling an interrupt anymore
    m_inInterrupt = false;
}

// =====================================================================================================================
void Cpu::sei()
{
    traceInstruction("SEI");
    m_status |= INT_DISABLE;
}

// =====================================================================================================================
void Cpu::cld()
{
    traceInstruction("CLD");
    m_status &= ~DECIMAL;
}

// =====================================================================================================================
void Cpu::sec()
{
    traceInstruction("SEC");
    m_status |= CARRY;
}

// =====================================================================================================================
void Cpu::clc()
{
    traceInstruction("CLC");
    m_status &= ~CARRY;
}

// =====================================================================================================================
void Cpu::txs()
{
    traceInstruction("TXS");
    m_SP = m_X;
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
void Cpu::pha()
{
    traceInstruction("PHA");
    push8(m_A);
}

// =====================================================================================================================
void Cpu::pla()
{
    traceInstruction("PLA");
    m_A = pop8();
    updateZero(m_A);
    updateSign(m_A);
}

// =====================================================================================================================
void Cpu::incZero()
{
    uint16_t addr = read8();
    traceInstruction(MakeString() << "INC $" << std::hex << std::setw(4) << std::setfill('0') << addr);

    uint8_t data = m_memory.read(addr);
    ++data;
    updateZero(data);
    updateSign(data);
    m_memory.write(addr, data);
}

// =====================================================================================================================
void Cpu::incAbs()
{
    uint16_t addr = read16();
    traceInstruction(MakeString() << "INC $" << std::hex << std::setw(4) << std::setfill('0') << addr);

    uint8_t data = m_memory.read(addr);
    ++data;
    updateZero(data);
    updateSign(data);
    m_memory.write(addr, data);
}

// =====================================================================================================================
void Cpu::decZero()
{
    uint16_t addr = read8();
    traceInstruction(MakeString() << "DEC $" << std::hex << std::setw(4) << std::setfill('0') << addr);

    uint8_t data = m_memory.read(addr);
    --data;
    updateZero(data);
    updateSign(data);
    m_memory.write(addr, data);
}

// =====================================================================================================================
void Cpu::decAbs()
{
    uint16_t addr = read16();
    traceInstruction(MakeString() << "DEC $" << std::hex << std::setw(4) << std::setfill('0') << addr);

    uint8_t data = m_memory.read(addr);
    --data;
    updateZero(data);
    updateSign(data);
    m_memory.write(addr, data);
}

// =====================================================================================================================
void Cpu::decAbsX()
{
    uint16_t addr = read16();
    traceInstruction(MakeString() << "DEC $" << std::hex << std::setw(4) << std::setfill('0') << addr << ",X");

    uint8_t data = m_memory.read(addr + m_X);
    --data;
    updateZero(data);
    updateSign(data);
    m_memory.write(addr, data + m_X);
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
void Cpu::ldxImm()
{
    m_X = read8();
    updateZero(m_X);
    updateSign(m_X);
    traceInstruction(MakeString() << "LDX #$" << std::hex << std::setw(2) << std::setfill('0') << (int)m_X);
}

// =====================================================================================================================
void Cpu::ldxZero()
{
    uint16_t abs = read8();
    traceInstruction(MakeString() << "LDX $" << std::hex << std::setw(4) << std::setfill('0') << abs);
    m_X = m_memory.read(abs);
    updateZero(m_X);
    updateSign(m_X);
}

// =====================================================================================================================
void Cpu::ldxAbs()
{
    uint16_t abs = read16();
    traceInstruction(MakeString() << "LDX $" << std::hex << std::setw(4) << std::setfill('0') << abs);
    m_X = m_memory.read(abs);
    updateZero(m_X);
    updateSign(m_X);
}

// =====================================================================================================================
void Cpu::ldxAbsY()
{
    uint16_t abs = read16();
    traceInstruction(MakeString() << "LDX $" << std::hex << std::setw(4) << std::setfill('0') << abs << ",Y");
    m_X = m_memory.read(abs + m_Y);
    updateZero(m_X);
    updateSign(m_X);
}

// =====================================================================================================================
void Cpu::stxZero()
{
    uint16_t addr = read8();
    traceInstruction(MakeString() << "STX $" << std::hex << std::setw(2) << std::setfill('0') << addr);
    m_memory.write(addr, m_X);
}

// =====================================================================================================================
void Cpu::stxAbs()
{
    uint16_t addr = read16();
    traceInstruction(MakeString() << "STX $" << std::hex << std::setw(4) << std::setfill('0') << addr);
    m_memory.write(addr, m_X);
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
void Cpu::ldyImm()
{
    m_Y = read8();
    updateZero(m_Y);
    updateSign(m_Y);
    traceInstruction(MakeString() << "LDY #$" << std::hex << std::setw(2) << std::setfill('0') << (int)m_Y);
}

// =====================================================================================================================
void Cpu::ldyZero()
{
    uint16_t addr = read8();
    traceInstruction(MakeString() << "LDY $" << std::hex << std::setw(4) << std::setfill('0') << addr);
    m_Y = m_memory.read(addr);
    updateZero(m_Y);
    updateSign(m_Y);
}

// =====================================================================================================================
void Cpu::ldyAbs()
{
    uint16_t abs = read16();
    traceInstruction(MakeString() << "LDY $" << std::hex << std::setw(4) << std::setfill('0') << abs);
    m_Y = m_memory.read(abs);
    updateZero(m_Y);
    updateSign(m_Y);
}

// =====================================================================================================================
void Cpu::ldyAbsX()
{
    uint16_t abs = read16();
    traceInstruction(MakeString() << "LDY $" << std::hex << std::setw(4) << std::setfill('0') << abs << ",X");
    m_Y = m_memory.read(abs + m_X);
    updateZero(m_Y);
    updateSign(m_Y);
}

// =====================================================================================================================
void Cpu::styZero()
{
    uint16_t addr = read8();
    traceInstruction(MakeString() << "STY $" << std::hex << std::setw(4) << std::setfill('0') << addr);
    m_memory.write(addr, m_Y);
}

// =====================================================================================================================
void Cpu::styAbs()
{
    uint16_t abs = read16();
    traceInstruction(MakeString() << "STY $" << std::hex << std::setw(4) << std::setfill('0') << abs);
    m_memory.write(abs, m_Y);
}

// =====================================================================================================================
void Cpu::oraImm()
{
    uint8_t imm = read8();
    m_A |= imm;
    updateZero(m_A);
    updateSign(m_A);
    traceInstruction(MakeString() << "ORA #$" << std::hex << std::setw(2) << std::setfill('0') << (int)imm);
}

// =====================================================================================================================
void Cpu::oraZero()
{
    uint16_t addr = read8();
    m_A |= m_memory.read(addr);
    updateZero(m_A);
    updateSign(m_A);
    traceInstruction(MakeString() << "ORA $" << std::hex << std::setw(2) << std::setfill('0') << addr);
}

// =====================================================================================================================
void Cpu::eorImm()
{
    uint8_t imm = read8();
    m_A ^= imm;
    updateZero(m_A);
    updateSign(m_A);
    traceInstruction(MakeString() << "EOR #$" << std::hex << std::setw(2) << std::setfill('0') << imm);
}

// =====================================================================================================================
void Cpu::eorZero()
{
    uint16_t addr = read8();
    m_A ^= m_memory.read(addr);
    updateZero(m_A);
    updateSign(m_A);
    traceInstruction(MakeString() << "EOR $" << std::hex << std::setw(4) << std::setfill('0') << addr);
}

// =====================================================================================================================
void Cpu::andImm()
{
    uint8_t imm = read8();
    m_A &= imm;
    updateZero(m_A);
    updateSign(m_A);
    traceInstruction(MakeString() << "AND #$" << std::hex << std::setw(2) << std::setfill('0') << (int)imm);
}

// =====================================================================================================================
void Cpu::andAbsX()
{
    uint16_t abs = read16();
    traceInstruction(MakeString() << "AND $" << std::hex << std::setw(4) << std::setfill('0') << abs << ",X");
    m_A &= m_memory.read(abs + m_X);
    updateZero(m_A);
    updateSign(m_A);
}

// =====================================================================================================================
void Cpu::ldaImm()
{
    m_A = read8();
    updateZero(m_A);
    updateSign(m_A);
    traceInstruction(MakeString() << "LDA #$" << std::hex << std::setw(2) << std::setfill('0') << (int)m_A);
}

// =====================================================================================================================
void Cpu::ldaZero()
{
    uint16_t abs = read8();
    traceInstruction(MakeString() << "LDA $" << std::hex << std::setw(4) << std::setfill('0') << abs);
    m_A = m_memory.read(abs);
    updateZero(m_A);
    updateSign(m_A);
}

// =====================================================================================================================
void Cpu::ldaAbs()
{
    uint16_t abs = read16();
    traceInstruction(MakeString() << "LDA $" << std::hex << std::setw(4) << std::setfill('0') << abs);
    m_A = m_memory.read(abs);
    updateZero(m_A);
    updateSign(m_A);
}

// =====================================================================================================================
void Cpu::ldaAbsX()
{
    uint16_t abs = read16();
    traceInstruction(MakeString() << "LDA $" << std::hex << std::setw(4) << std::setfill('0') << abs << ",X");
    m_A = m_memory.read(abs + m_X);
    updateZero(m_A);
    updateSign(m_A);
}

// =====================================================================================================================
void Cpu::ldaAbsY()
{
    uint16_t abs = read16();
    traceInstruction(MakeString() << "LDA $" << std::hex << std::setw(4) << std::setfill('0') << abs << ",Y");
    m_A = m_memory.read(abs + m_Y);
    updateZero(m_A);
    updateSign(m_A);
}

// =====================================================================================================================
void Cpu::ldaIndY()
{
    uint8_t off = read8();
    traceInstruction(MakeString() << "LDA ($" << std::hex << std::setw(2) << std::setfill('0') << off << "),Y");

    uint16_t addr = (m_memory.read(off) | (m_memory.read(off + 1) << 8)) + m_Y;
    m_A = m_memory.read(addr);
    updateZero(m_A);
    updateSign(m_A);
}

// =====================================================================================================================
void Cpu::adcImm()
{
    uint8_t imm = read8();
    traceInstruction(MakeString() << "ADC #$" << std::hex << std::setw(2) << std::setfill('0') << (int)imm);
    int result = m_A + imm + (m_status & CARRY ? 1 : 0);
    m_A = result;
    updateZero(m_A);
    updateSign(m_A);
    setOrClearStatus(CARRY, result > 255);
    // TODO: V status update
}

// =====================================================================================================================
void Cpu::adcZero()
{
    uint16_t addr = read8();
    traceInstruction(MakeString() << "ADC $" << std::hex << std::setw(4) << std::setfill('0') << addr);
    int result = m_A + m_memory.read(addr) + (m_status & CARRY ? 1 : 0);
    m_A = result;
    updateZero(m_A);
    updateSign(m_A);
    setOrClearStatus(CARRY, result > 255);
    // TODO: V status update
}

// =====================================================================================================================
void Cpu::adcAbs()
{
    uint16_t addr = read16();
    traceInstruction(MakeString() << "ADC $" << std::hex << std::setw(4) << std::setfill('0') << addr);
    int result = m_A + m_memory.read(addr) + (m_status & CARRY ? 1 : 0);
    m_A = result;
    updateZero(m_A);
    updateSign(m_A);
    setOrClearStatus(CARRY, result > 255);
    // TODO: V status update
}

// =====================================================================================================================
void Cpu::adcAbsY()
{
    uint16_t addr = read16();
    traceInstruction(MakeString() << "ADC $" << std::hex << std::setw(4) << std::setfill('0') << addr << ",Y");
    int result = m_A + m_memory.read(addr + m_Y) + (m_status & CARRY ? 1 : 0);
    m_A = result;
    updateZero(m_A);
    updateSign(m_A);
    setOrClearStatus(CARRY, result > 255);
    // TODO: V status update
}

// =====================================================================================================================
void Cpu::sbcImm()
{
    uint8_t imm = read8();
    traceInstruction(MakeString() << "SBC #$" << std::hex << std::setw(2) << std::setfill('0') << (int)imm);
    m_A = m_A - imm - (1 - (m_status & CARRY ? 1 : 0));
    updateZero(m_A);
    updateSign(m_A);
    setOrClearStatus(CARRY, m_A > 0);
    // TODO: V status update
}

// =====================================================================================================================
void Cpu::sbcAbsY()
{
    uint16_t addr = read16();
    traceInstruction(MakeString() << "SBC $" << std::hex << std::setw(4) << std::setfill('0') << addr << ",Y");
    m_A = m_A - m_memory.read(addr + m_Y) - (1 - (m_status & CARRY ? 1 : 0));
    updateZero(m_A);
    updateSign(m_A);
    setOrClearStatus(CARRY, m_A > 0);
    // TODO: V status update
}

// =====================================================================================================================
void Cpu::staZero()
{
    uint16_t addr = read8();
    traceInstruction(MakeString() << "STA $" << std::hex << std::setw(2) << std::setfill('0') << addr);
    m_memory.write(addr, m_A);
}

// =====================================================================================================================
void Cpu::staAbs()
{
    uint16_t abs = read16();
    traceInstruction(MakeString() << "STA $" << std::hex << std::setw(4) << std::setfill('0') << abs);
    m_memory.write(abs, m_A);
}

// =====================================================================================================================
void Cpu::staAbsX()
{
    uint16_t abs = read16();
    traceInstruction(MakeString() << "STA $" << std::hex << std::setw(4) << std::setfill('0') << abs << ",X");
    m_memory.write(abs + m_X, m_A);
}

// =====================================================================================================================
void Cpu::staAbsY()
{
    uint16_t abs = read16();
    traceInstruction(MakeString() << "STA $" << std::hex << std::setw(4) << std::setfill('0') << abs << ",Y");
    m_memory.write(abs + m_Y, m_A);
}

// =====================================================================================================================
void Cpu::staIndY()
{
    uint8_t off = read8();
    traceInstruction(MakeString() << "STA ($" << std::hex << std::setw(2) << std::setfill('0') << off << "),Y");

    uint16_t addr = (m_memory.read(off) | (m_memory.read(off + 1) << 8)) + m_Y;
    m_memory.write(addr, m_A);
}

// =====================================================================================================================
void Cpu::cpxImm()
{
    uint8_t imm = read8();
    setOrClearStatus(CARRY, m_X >= imm);
    setOrClearStatus(ZERO, m_X == imm);
    setOrClearStatus(SIGN, m_X < imm);
    traceInstruction(MakeString() << "CPX #$" << std::hex << std::setw(2) << std::setfill('0') << (int)imm);
}

// =====================================================================================================================
void Cpu::cpyImm()
{
    uint8_t imm = read8();
    setOrClearStatus(CARRY, m_Y >= imm);
    setOrClearStatus(ZERO, m_Y == imm);
    setOrClearStatus(SIGN, m_Y < imm);
    traceInstruction(MakeString() << "CPY #$" << std::hex << std::setw(2) << std::setfill('0') << (int)imm);
}

// =====================================================================================================================
void Cpu::cmpImm()
{
    uint8_t imm = read8();
    setOrClearStatus(CARRY, m_A >= imm);
    setOrClearStatus(ZERO, m_A == imm);
    setOrClearStatus(SIGN, m_A < imm);
    traceInstruction(MakeString() << "CMP #$" << std::hex << std::setw(2) << std::setfill('0') << (int)imm);
}

// =====================================================================================================================
void Cpu::cmpZero()
{
    uint16_t addr = read8();
    uint8_t imm = m_memory.read(addr);
    setOrClearStatus(CARRY, m_A >= imm);
    setOrClearStatus(ZERO, m_A == imm);
    setOrClearStatus(SIGN, m_A < imm);
    traceInstruction(MakeString() << "CMP $" << std::hex << std::setw(4) << std::setfill('0') << addr);
}

// =====================================================================================================================
void Cpu::cmpAbs()
{
    uint16_t addr = read16();
    uint8_t imm = m_memory.read(addr);
    setOrClearStatus(CARRY, m_A >= imm);
    setOrClearStatus(ZERO, m_A == imm);
    setOrClearStatus(SIGN, m_A < imm);
    traceInstruction(MakeString() << "CMP $" << std::hex << std::setw(4) << std::setfill('0') << addr);
}

// =====================================================================================================================
void Cpu::cmpAbsY()
{
    uint16_t addr = read16();
    uint8_t imm = m_memory.read(addr + m_Y);
    setOrClearStatus(CARRY, m_A >= imm);
    setOrClearStatus(ZERO, m_A == imm);
    setOrClearStatus(SIGN, m_A < imm);
    traceInstruction(MakeString() << "CMP $" << std::hex << std::setw(4) << std::setfill('0') << addr << ",Y");
}

// =====================================================================================================================
void Cpu::bitZero()
{
    uint16_t addr = read8();
    traceInstruction(MakeString() << "BIT $" << std::hex << std::setw(4) << std::setfill('0') << addr);

    uint8_t data = m_memory.read(addr);
    setOrClearStatus(SIGN, ((data >> 7) & 1) == 1);
    setOrClearStatus(OVERFLOW_, ((data >> 6) & 1) == 1);
    updateZero(m_A & data);
}

// =====================================================================================================================
void Cpu::bitAbs()
{
    uint16_t addr = read16();
    traceInstruction(MakeString() << "BIT $" << std::hex << std::setw(4) << std::setfill('0') << addr);

    uint8_t data = m_memory.read(addr);
    setOrClearStatus(SIGN, ((data >> 7) & 1) == 1);
    setOrClearStatus(OVERFLOW_, ((data >> 6) & 1) == 1);
    updateZero(m_A & data);
}

// =====================================================================================================================
void Cpu::aslAcc()
{
    traceInstruction("ASL");
    // bit #0 is shifted into carry
    setOrClearStatus(CARRY, (m_A & 0x80) == 0x80);
    m_A <<= 1;
    updateZero(m_A);
    updateSign(m_A);
}

// =====================================================================================================================
void Cpu::lsrAcc()
{
    traceInstruction("LSR");
    // bit #0 is shifted into carry
    setOrClearStatus(CARRY, (m_A & 0x1) == 1);
    m_A >>= 1;
    updateZero(m_A);
    updateSign(m_A);
}

// =====================================================================================================================
void Cpu::lsrZero()
{
    uint16_t addr = read8();
    traceInstruction(MakeString() << "LSR $" << std::hex << std::setw(4) << std::setfill('0') << addr);
    uint8_t data = m_memory.read(addr);
    // bit #0 is shifted into carry
    setOrClearStatus(CARRY, (data & 0x1) == 1);
    data >>= 1;
    updateZero(data);
    updateSign(data);
    m_memory.write(addr, data);
}

// =====================================================================================================================
void Cpu::lsrAbs()
{
    uint16_t addr = read16();
    traceInstruction(MakeString() << "LSR $" << std::hex << std::setw(4) << std::setfill('0') << addr);
    uint8_t data = m_memory.read(addr);
    // bit #0 is shifted into carry
    setOrClearStatus(CARRY, (data & 0x1) == 1);
    data >>= 1;
    updateZero(data);
    updateSign(data);
    m_memory.write(addr, data);
}

// =====================================================================================================================
void Cpu::rolAcc()
{
    traceInstruction("ROL");
    // bit #0 is shifted into carry
    bool setCarry = m_A & 0x80;
    m_A <<= 1;
    if (m_status & CARRY)
	m_A |= 1;
    setOrClearStatus(CARRY, setCarry);
    updateZero(m_A);
    updateSign(m_A);
}

// =====================================================================================================================
void Cpu::rolZero()
{
    uint16_t addr = read8();
    traceInstruction(MakeString() << "ROL $" << std::hex << std::setw(4) << std::setfill('0') << addr);
    uint8_t data = m_memory.read(addr);
    // bit #0 is shifted into carry
    bool setCarry = data & 0x80;
    data <<= 1;
    if (m_status & CARRY)
	data |= 1;
    setOrClearStatus(CARRY, setCarry);
    updateZero(data);
    updateSign(data);
    m_memory.write(addr, data);
}

// =====================================================================================================================
void Cpu::rolAbs()
{
    uint16_t addr = read16();
    traceInstruction(MakeString() << "ROL $" << std::hex << std::setw(4) << std::setfill('0') << addr);
    uint8_t data = m_memory.read(addr);
    // bit #0 is shifted into carry
    bool setCarry = data & 0x80;
    data <<= 1;
    if (m_status & CARRY)
	data |= 1;
    setOrClearStatus(CARRY, setCarry);
    updateZero(data);
    updateSign(data);
    m_memory.write(addr, data);
}

// =====================================================================================================================
void Cpu::rorAcc()
{
    traceInstruction("ROR");
    bool setCarry = (m_A & 0x1) == 1;
    m_A >>= 1;
    if (m_status & CARRY)
	m_A |= 0x80;
    setOrClearStatus(CARRY, setCarry);
    updateZero(m_A);
    updateSign(m_A);
}

// =====================================================================================================================
void Cpu::rorAbsX()
{
    uint16_t addr = read16();
    traceInstruction(MakeString() << " ROR $" << std::hex << std::setw(4) << std::setfill('0') << addr << ",X");
    uint8_t data = m_memory.read(addr + m_X);
    bool setCarry = (data & 0x1) == 1;
    data >>= 1;
    if (m_status & CARRY)
	data |= 0x80;
    setOrClearStatus(CARRY, setCarry);
    m_memory.write(addr + m_X, data);
    updateZero(data);
    updateSign(data);
}

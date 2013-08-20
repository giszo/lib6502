#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

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
void Cpu::setInstructionTracer(InstructionTracer* t)
{
    m_instrTracer = t;
}

// =====================================================================================================================
void Cpu::tick()
{
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

    INSTR(0x09, oraImm);
    INSTR(0x10, bpl);
    INSTR(0x20, jsrAbs);
    INSTR(0x29, andImm);
    INSTR(0x2c, bitAbs);
    INSTR(0x4c, jmpAbs);
    INSTR(0x60, rts);
    INSTR(0x78, sei);
    INSTR(0x85, staZero);
    INSTR(0x86, stxZero);
    INSTR(0x88, dey);
    INSTR(0x8a, txa);
    INSTR(0x8d, staAbs);
    INSTR(0x91, staIndY);
    INSTR(0x9a, txs);
    INSTR(0x99, staAbsY);
    INSTR(0xa0, ldyImm);
    INSTR(0xa2, ldxImm);
    INSTR(0xa9, ldaImm);
    INSTR(0xad, ldaAbs);
    INSTR(0xb0, bcs);
    INSTR(0xbd, ldaAbsX);
    INSTR(0xc0, cpyImm);
    INSTR(0xc8, iny);
    INSTR(0xc9, cmpImm);
    INSTR(0xca, dex);
    INSTR(0xd0, bne);
    INSTR(0xd8, cld);
    INSTR(0xe0, cpxImm);
    INSTR(0xee, incAbs);

#undef INSTR
}

// =====================================================================================================================
void Cpu::traceInstruction(const std::string& s)
{
    if (m_instrTracer)
	m_instrTracer->trace(s);
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
void Cpu::jmpAbs()
{
    m_PC = read16();
    traceInstruction(MakeString() << "JMP $" << std::hex << std::setw(4) << std::setfill('0') << m_PC);
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
void Cpu::oraImm()
{
    uint8_t imm = read8();
    m_A |= imm;
    updateZero(m_A);
    updateSign(m_A);
    traceInstruction(MakeString() << "ORA #$" << std::hex << std::setw(2) << std::setfill('0') << (int)imm);
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
void Cpu::ldaImm()
{
    m_A = read8();
    updateZero(m_A);
    updateSign(m_A);
    traceInstruction(MakeString() << "LDA #$" << std::hex << std::setw(2) << std::setfill('0') << (int)m_A);
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
void Cpu::stxZero()
{
    uint16_t addr = read8();
    traceInstruction(MakeString() << "STX $" << std::hex << std::setw(2) << std::setfill('0') << addr);
    m_memory.write(addr, m_X);
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
void Cpu::bitAbs()
{
    uint16_t addr = read16();
    traceInstruction(MakeString() << "BIT $" << std::hex << std::setw(4) << std::setfill('0') << addr);

    uint8_t data = m_memory.read(addr);
    setOrClearStatus(SIGN, ((data >> 7) & 1) == 1);
    setOrClearStatus(OVERFLOW, ((data >> 6) & 1) == 1);
    updateZero(m_A & data);
}

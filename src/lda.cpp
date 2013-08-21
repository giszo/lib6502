#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

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
void Cpu::ldaZeroX()
{
    uint16_t abs = read8();
    traceInstruction(MakeString() << "LDA $" << std::hex << std::setw(4) << std::setfill('0') << abs << ",X");
    m_A = m_memory.read(abs + m_X);
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

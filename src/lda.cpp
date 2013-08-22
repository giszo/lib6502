#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

// =====================================================================================================================
void Cpu::ldaImm()
{
    m_A = read8();
    traceInstruction(MakeString(true) << "LDA #$" << std::setw(2) << std::setfill('0') << (int)m_A);

    updateZero(m_A);
    updateSign(m_A);
}

// =====================================================================================================================
void Cpu::ldaZero()
{
    uint16_t address = addrZero();
    traceInstruction(MakeString(true) << "LDA $" << std::setw(2) << std::setfill('0') << address);

    m_A = m_memory.read(address);
    updateZero(m_A);
    updateSign(m_A);
}

// =====================================================================================================================
void Cpu::ldaZeroX()
{
    uint8_t offset;
    uint16_t address = addrZeroX(offset);
    traceInstruction(MakeString(true) << "LDA $" << std::setw(2) << std::setfill('0') << (int)offset << ",X");

    m_A = m_memory.read(address);
    updateZero(m_A);
    updateSign(m_A);
}

// =====================================================================================================================
void Cpu::ldaAbs()
{
    uint16_t address = addrAbsolute();
    traceInstruction(MakeString(true) << "LDA $" << std::setw(4) << std::setfill('0') << address);

    m_A = m_memory.read(address);
    updateZero(m_A);
    updateSign(m_A);
}

// =====================================================================================================================
void Cpu::ldaAbsX()
{
    uint16_t abs;
    uint16_t address = addrAbsoluteX(abs);
    traceInstruction(MakeString(true) << "LDA $" << std::setw(4) << std::setfill('0') << abs << ",X");

    m_A = m_memory.read(address);
    updateZero(m_A);
    updateSign(m_A);
}

// =====================================================================================================================
void Cpu::ldaAbsY()
{
    uint16_t abs;
    uint16_t address = addrAbsoluteY(abs);
    traceInstruction(MakeString(true) << "LDA $" << std::setw(4) << std::setfill('0') << abs << ",Y");

    m_A = m_memory.read(address);
    updateZero(m_A);
    updateSign(m_A);
}

// =====================================================================================================================
void Cpu::ldaIndX()
{
    uint8_t offset;
    uint16_t address = addrIndirectX(offset);
    traceInstruction(MakeString(true) << "LDA ($" << std::setw(2) << std::setfill('0') << (int)offset << ",X)");

    m_A = m_memory.read(address);
    updateZero(m_A);
    updateSign(m_A);
}

// =====================================================================================================================
void Cpu::ldaIndY()
{
    uint8_t offset;
    uint16_t address = addrIndirectY(offset);
    traceInstruction(MakeString(true) << "LDA ($" << std::setw(2) << std::setfill('0') << (int)offset << "),Y");

    m_A = m_memory.read(address);
    updateZero(m_A);
    updateSign(m_A);
}

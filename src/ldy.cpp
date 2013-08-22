#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

// =====================================================================================================================
void Cpu::ldyImm()
{
    m_Y = read8();
    traceInstruction(MakeString(true) << "LDY #$" << std::setw(2) << std::setfill('0') << (int)m_Y);

    updateZero(m_Y);
    updateSign(m_Y);
}

// =====================================================================================================================
void Cpu::ldyZero()
{
    uint16_t address = addrZero();
    traceInstruction(MakeString(true) << "LDY $" << std::setw(2) << std::setfill('0') << address);

    m_Y = m_memory.read(address);
    updateZero(m_Y);
    updateSign(m_Y);
}

// =====================================================================================================================
void Cpu::ldyZeroX()
{
    uint8_t offset;
    uint16_t address = addrZeroX(offset);
    traceInstruction(MakeString(true) << "LDY $" << std::setw(2) << std::setfill('0') << (int)offset << ",X");

    m_Y = m_memory.read(address);
    updateZero(m_Y);
    updateSign(m_Y);
}

// =====================================================================================================================
void Cpu::ldyAbs()
{
    uint16_t address = addrAbsolute();
    traceInstruction(MakeString(true) << "LDY $" << std::setw(4) << std::setfill('0') << address);

    m_Y = m_memory.read(address);
    updateZero(m_Y);
    updateSign(m_Y);
}

// =====================================================================================================================
void Cpu::ldyAbsX()
{
    uint16_t abs;
    uint16_t address = addrAbsoluteX(abs);
    traceInstruction(MakeString(true) << "LDY $" << std::setw(4) << std::setfill('0') << abs << ",X");

    m_Y = m_memory.read(address);
    updateZero(m_Y);
    updateSign(m_Y);
}

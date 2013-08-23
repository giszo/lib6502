#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

// =====================================================================================================================
void Cpu::ldxImm()
{
    m_X = read8();
    traceInstruction(MakeString(true) << "LDX #$" << std::setw(2) << std::setfill('0') << (int)m_X);

    updateZero(m_X);
    updateSign(m_X);
}

// =====================================================================================================================
void Cpu::ldxZero()
{
    uint16_t abs = addrZero();
    traceInstruction(MakeString(true) << "LDX $" << std::setw(4) << std::setfill('0') << abs);

    m_X = m_memory.read(abs);
    updateZero(m_X);
    updateSign(m_X);
}

// =====================================================================================================================
void Cpu::ldxZeroY()
{
    uint8_t offset;
    uint16_t address = addrZeroY(offset);
    traceInstruction(MakeString(true) << "LDX $" << std::setw(4) << std::setfill('0') << (int)offset << ",Y");

    m_X = m_memory.read(address);
    updateZero(m_X);
    updateSign(m_X);
}

// =====================================================================================================================
void Cpu::ldxAbs()
{
    uint16_t abs = addrAbsolute();
    traceInstruction(MakeString(true) << "LDX $" << std::setw(4) << std::setfill('0') << abs);

    m_X = m_memory.read(abs);
    updateZero(m_X);
    updateSign(m_X);
}

// =====================================================================================================================
void Cpu::ldxAbsY()
{
    uint16_t abs;
    uint16_t addr = addrAbsoluteY(abs);
    traceInstruction(MakeString(true) << "LDX $" << std::setw(4) << std::setfill('0') << abs << ",Y");

    m_X = m_memory.read(addr);
    updateZero(m_X);
    updateSign(m_X);
}

#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

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
void Cpu::andZero()
{
    uint16_t abs = read8();
    traceInstruction(MakeString() << "AND $" << std::hex << std::setw(4) << std::setfill('0') << abs);
    m_A &= m_memory.read(abs);
    updateZero(m_A);
    updateSign(m_A);
}

// =====================================================================================================================
void Cpu::andAbs()
{
    uint16_t abs = read16();
    traceInstruction(MakeString() << "AND $" << std::hex << std::setw(4) << std::setfill('0') << abs);
    m_A &= m_memory.read(abs);
    updateZero(m_A);
    updateSign(m_A);
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
void Cpu::andAbsY()
{
    uint16_t abs = read16();
    traceInstruction(MakeString() << "AND $" << std::hex << std::setw(4) << std::setfill('0') << abs << ",Y");
    m_A &= m_memory.read(abs + m_Y);
    updateZero(m_A);
    updateSign(m_A);
}

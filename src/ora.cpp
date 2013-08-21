#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

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
void Cpu::oraZeroX()
{
    uint16_t addr = read8() + m_X;
    m_A |= m_memory.read(addr);
    updateZero(m_A);
    updateSign(m_A);
    traceInstruction(MakeString() << "ORA $" << std::hex << std::setw(2) << std::setfill('0') << addr << ",X");
}

// =====================================================================================================================
void Cpu::oraAbs()
{
    uint16_t addr = read16();
    m_A |= m_memory.read(addr);
    updateZero(m_A);
    updateSign(m_A);
    traceInstruction(MakeString() << "ORA $" << std::hex << std::setw(2) << std::setfill('0') << addr);
}

// =====================================================================================================================
void Cpu::oraAbsX()
{
    uint16_t addr = read16() + m_X;
    m_A |= m_memory.read(addr);
    updateZero(m_A);
    updateSign(m_A);
    traceInstruction(MakeString() << "ORA $" << std::hex << std::setw(2) << std::setfill('0') << addr << ",X");
}

// =====================================================================================================================
void Cpu::oraAbsY()
{
    uint16_t addr = read16() + m_Y;
    m_A |= m_memory.read(addr);
    updateZero(m_A);
    updateSign(m_A);
    traceInstruction(MakeString() << "ORA $" << std::hex << std::setw(2) << std::setfill('0') << addr << ",Y");
}

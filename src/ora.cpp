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
    traceInstruction(MakeString(true) << "ORA #$" << std::setw(2) << std::setfill('0') << (int)imm);
}

// =====================================================================================================================
void Cpu::oraZero()
{
    uint16_t address = addrZero();
    traceInstruction(MakeString(true) << "ORA $" << std::setw(2) << std::setfill('0') << address);

    m_A |= m_memory.read(address);
    updateZero(m_A);
    updateSign(m_A);
}

// =====================================================================================================================
void Cpu::oraZeroX()
{
    uint8_t offset;
    uint16_t address = addrZeroX(offset);
    traceInstruction(MakeString(true) << "ORA $" << std::setw(2) << std::setfill('0') << (int)offset << ",X");

    m_A |= m_memory.read(address);
    updateZero(m_A);
    updateSign(m_A);
}

// =====================================================================================================================
void Cpu::oraAbs()
{
    uint16_t address = addrAbsolute();
    traceInstruction(MakeString(true) << "ORA $" << std::setw(4) << std::setfill('0') << address);

    m_A |= m_memory.read(address);
    updateZero(m_A);
    updateSign(m_A);
}

// =====================================================================================================================
void Cpu::oraAbsX()
{
    uint16_t abs;
    uint16_t address = addrAbsoluteX(abs);
    traceInstruction(MakeString(true) << "ORA $" << std::setw(4) << std::setfill('0') << abs << ",X");

    m_A |= m_memory.read(address);
    updateZero(m_A);
    updateSign(m_A);
}

// =====================================================================================================================
void Cpu::oraAbsY()
{
    uint16_t abs;
    uint16_t address = addrAbsoluteY(abs);
    traceInstruction(MakeString(true) << "ORA $" << std::setw(4) << std::setfill('0') << abs << ",Y");

    m_A |= m_memory.read(address);
    updateZero(m_A);
    updateSign(m_A);
}

// =====================================================================================================================
void Cpu::oraIndX()
{
    uint8_t offset;
    uint16_t address = addrIndirectX(offset);
    traceInstruction(MakeString(true) << "ORA ($" << std::setw(2) << std::setfill('0') << (int)offset << ",X)");

    m_A |= m_memory.read(address);
    updateZero(m_A);
    updateSign(m_A);
}

// =====================================================================================================================
void Cpu::oraIndY()
{
    uint8_t offset;
    uint16_t address = addrIndirectY(offset);
    traceInstruction(MakeString(true) << "ORA $(" << std::setw(2) << std::setfill('0') << (int)offset << "),Y");

    m_A |= m_memory.read(address);
    updateZero(m_A);
    updateSign(m_A);
}

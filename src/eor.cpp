#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

// =====================================================================================================================
void Cpu::eorImm()
{
    uint8_t imm = read8();
    traceInstruction(MakeString(true) << "EOR #$" << std::setw(2) << std::setfill('0') << (int)imm);

    m_A ^= imm;
    updateZero(m_A);
    updateSign(m_A);
}

// =====================================================================================================================
void Cpu::eorZero()
{
    uint16_t address = addrZero();
    traceInstruction(MakeString(true) << "EOR $" << std::setw(2) << std::setfill('0') << address);

    m_A ^= m_memory.read(address);
    updateZero(m_A);
    updateSign(m_A);
}

// =====================================================================================================================
void Cpu::eorZeroX()
{
    uint8_t offset;
    uint16_t address = addrZeroX(offset);
    traceInstruction(MakeString(true) << "EOR $" << std::setw(2) << std::setfill('0') << (int)offset << ",X");

    m_A ^= m_memory.read(address);
    updateZero(m_A);
    updateSign(m_A);
}

// =====================================================================================================================
void Cpu::eorAbs()
{
    uint16_t address = addrAbsolute();
    traceInstruction(MakeString(true) << "EOR $" << std::setw(4) << std::setfill('0') << address);

    m_A ^= m_memory.read(address);
    updateZero(m_A);
    updateSign(m_A);
}

// =====================================================================================================================
void Cpu::eorAbsX()
{
    uint16_t abs;
    uint16_t address = addrAbsoluteX(abs);
    traceInstruction(MakeString(true) << "EOR $" << std::setw(4) << std::setfill('0') << abs << ",X");

    m_A ^= m_memory.read(address);
    updateZero(m_A);
    updateSign(m_A);
}

// =====================================================================================================================
void Cpu::eorAbsY()
{
    uint16_t abs;
    uint16_t address = addrAbsoluteY(abs);
    traceInstruction(MakeString(true) << "EOR $" << std::setw(4) << std::setfill('0') << abs << ",Y");

    m_A ^= m_memory.read(address);
    updateZero(m_A);
    updateSign(m_A);
}

// =====================================================================================================================
void Cpu::eorIndX()
{
    uint8_t offset;
    uint16_t address = addrIndirectX(offset);
    traceInstruction(MakeString(true) << "EOR ($" << std::setw(2) << std::setfill('0') << (int)offset << ",X)");

    m_A ^= m_memory.read(address);
    updateZero(m_A);
    updateSign(m_A);
}

// =====================================================================================================================
void Cpu::eorIndY()
{
    uint8_t offset;
    uint16_t address = addrIndirectY(offset);
    traceInstruction(MakeString(true) << "EOR $(" << std::setw(2) << std::setfill('0') << (int)offset << "),Y");

    m_A ^= m_memory.read(address);
    updateZero(m_A);
    updateSign(m_A);
}

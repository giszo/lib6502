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
    uint16_t address = addrZero();
    traceInstruction(MakeString(true) << "AND $" << std::setw(2) << std::setfill('0') << address);

    m_A &= m_memory.read(address);
    updateZero(m_A);
    updateSign(m_A);
}

// =====================================================================================================================
void Cpu::andZeroX()
{
    uint8_t offset;
    uint16_t address = addrZeroX(offset);
    traceInstruction(MakeString(true) << "AND $" << std::setw(2) << std::setfill('0') << (int)offset << ",X");

    m_A &= m_memory.read(address);
    updateZero(m_A);
    updateSign(m_A);
}

// =====================================================================================================================
void Cpu::andAbs()
{
    uint16_t address = addrAbsolute();
    traceInstruction(MakeString(true) << "AND $" << std::setw(4) << std::setfill('0') << address);

    m_A &= m_memory.read(address);
    updateZero(m_A);
    updateSign(m_A);
}

// =====================================================================================================================
void Cpu::andAbsX()
{
    uint16_t abs;
    uint16_t address = addrAbsoluteX(abs);
    traceInstruction(MakeString(true) << "AND $" << std::setw(4) << std::setfill('0') << abs << ",X");

    m_A &= m_memory.read(address);
    updateZero(m_A);
    updateSign(m_A);
}

// =====================================================================================================================
void Cpu::andAbsY()
{
    uint16_t abs;
    uint16_t address = addrAbsoluteY(abs);
    traceInstruction(MakeString(true) << "AND $" << std::setw(4) << std::setfill('0') << abs << ",Y");

    m_A &= m_memory.read(address);
    updateZero(m_A);
    updateSign(m_A);
}

// =====================================================================================================================
void Cpu::andIndX()
{
    uint8_t offset;
    uint16_t address = addrIndirectX(offset);
    traceInstruction(MakeString(true) << "AND ($" << std::setw(2) << std::setfill('0') << (int)offset << ",X)");

    m_A &= m_memory.read(address);
    updateZero(m_A);
    updateSign(m_A);
}

// =====================================================================================================================
void Cpu::andIndY()
{
    uint8_t offset;
    uint16_t address = addrIndirectY(offset);
    traceInstruction(MakeString(true) << "AND $(" << std::setw(2) << std::setfill('0') << (int)offset << "),Y");

    m_A &= m_memory.read(address);
    updateZero(m_A);
    updateSign(m_A);
}

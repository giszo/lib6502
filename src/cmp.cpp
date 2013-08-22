#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

// =====================================================================================================================
void Cpu::cmpImm()
{
    uint8_t imm = read8();
    traceInstruction(MakeString(true) << "CMP #$" << std::setw(2) << std::setfill('0') << (int)imm);

    setOrClearStatus(Carry, m_A >= imm);
    setOrClearStatus(Zero, m_A == imm);
    setOrClearStatus(Sign, (m_A - imm) & 0x80);
}

// =====================================================================================================================
void Cpu::cmpZero()
{
    uint16_t address = addrZero();
    traceInstruction(MakeString(true) << "CMP $" << std::setw(2) << std::setfill('0') << address);

    uint8_t imm = m_memory.read(address);
    setOrClearStatus(Carry, m_A >= imm);
    setOrClearStatus(Zero, m_A == imm);
    setOrClearStatus(Sign, (m_A - imm) & 0x80);
}

// =====================================================================================================================
void Cpu::cmpZeroX()
{
    uint8_t offset;
    uint16_t address = addrZeroX(offset);
    traceInstruction(MakeString(true) << "CMP $" << std::setw(2) << std::setfill('0') << (int)offset << ",X");

    uint8_t imm = m_memory.read(address);
    setOrClearStatus(Carry, m_A >= imm);
    setOrClearStatus(Zero, m_A == imm);
    setOrClearStatus(Sign, (m_A - imm) & 0x80);
}

// =====================================================================================================================
void Cpu::cmpAbs()
{
    uint16_t address = addrAbsolute();
    traceInstruction(MakeString(true) << "CMP $" << std::setw(4) << std::setfill('0') << address);

    uint8_t imm = m_memory.read(address);
    setOrClearStatus(Carry, m_A >= imm);
    setOrClearStatus(Zero, m_A == imm);
    setOrClearStatus(Sign, (m_A - imm) & 0x80);
}

// =====================================================================================================================
void Cpu::cmpAbsX()
{
    uint16_t abs;
    uint16_t address = addrAbsoluteX(abs);
    traceInstruction(MakeString(true) << "CMP $" << std::setw(4) << std::setfill('0') << abs << ",X");

    uint8_t imm = m_memory.read(address);
    setOrClearStatus(Carry, m_A >= imm);
    setOrClearStatus(Zero, m_A == imm);
    setOrClearStatus(Sign, (m_A - imm) & 0x80);
}

// =====================================================================================================================
void Cpu::cmpAbsY()
{
    uint16_t abs;
    uint16_t address = addrAbsoluteY(abs);
    traceInstruction(MakeString(true) << "CMP $" << std::setw(4) << std::setfill('0') << abs << ",Y");

    uint8_t imm = m_memory.read(address);
    setOrClearStatus(Carry, m_A >= imm);
    setOrClearStatus(Zero, m_A == imm);
    setOrClearStatus(Sign, (m_A - imm) & 0x80);
}

// =====================================================================================================================
void Cpu::cmpIndX()
{
    uint8_t offset;
    uint16_t address = addrIndirectX(offset);
    traceInstruction(MakeString(true) << "CMP ($" << std::setw(2) << std::setfill('0') << (int)offset << ",X)");

    uint8_t imm = m_memory.read(address);
    setOrClearStatus(Carry, m_A >= imm);
    setOrClearStatus(Zero, m_A == imm);
    setOrClearStatus(Sign, (m_A - imm) & 0x80);
}

// =====================================================================================================================
void Cpu::cmpIndY()
{
    uint8_t offset;
    uint16_t address = addrIndirectY(offset);
    traceInstruction(MakeString(true) << "CMP $(" << std::setw(2) << std::setfill('0') << (int)offset << "),Y");

    uint8_t imm = m_memory.read(address);
    setOrClearStatus(Carry, m_A >= imm);
    setOrClearStatus(Zero, m_A == imm);
    setOrClearStatus(Sign, (m_A - imm) & 0x80);
}

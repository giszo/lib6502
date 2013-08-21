#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

// =====================================================================================================================
void Cpu::cmpImm()
{
    uint8_t imm = read8();
    setOrClearStatus(Carry, m_A >= imm);
    setOrClearStatus(Zero, m_A == imm);
    setOrClearStatus(Sign, m_A < imm);
    traceInstruction(MakeString() << "CMP #$" << std::hex << std::setw(2) << std::setfill('0') << (int)imm);
}

// =====================================================================================================================
void Cpu::cmpZero()
{
    uint16_t addr = read8();
    uint8_t imm = m_memory.read(addr);
    setOrClearStatus(Carry, m_A >= imm);
    setOrClearStatus(Zero, m_A == imm);
    setOrClearStatus(Sign, m_A < imm);
    traceInstruction(MakeString() << "CMP $" << std::hex << std::setw(4) << std::setfill('0') << addr);
}

// =====================================================================================================================
void Cpu::cmpZeroX()
{
    uint16_t addr = read8();
    uint8_t imm = m_memory.read(addr + m_X);
    setOrClearStatus(Carry, m_A >= imm);
    setOrClearStatus(Zero, m_A == imm);
    setOrClearStatus(Sign, m_A < imm);
    traceInstruction(MakeString() << "CMP $" << std::hex << std::setw(4) << std::setfill('0') << addr << ",X");
}

// =====================================================================================================================
void Cpu::cmpAbs()
{
    uint16_t addr = read16();
    uint8_t imm = m_memory.read(addr);
    setOrClearStatus(Carry, m_A >= imm);
    setOrClearStatus(Zero, m_A == imm);
    setOrClearStatus(Sign, m_A < imm);
    traceInstruction(MakeString() << "CMP $" << std::hex << std::setw(4) << std::setfill('0') << addr);
}

// =====================================================================================================================
void Cpu::cmpAbsX()
{
    uint16_t addr = read16();
    uint8_t imm = m_memory.read(addr + m_X);
    setOrClearStatus(Carry, m_A >= imm);
    setOrClearStatus(Zero, m_A == imm);
    setOrClearStatus(Sign, m_A < imm);
    traceInstruction(MakeString() << "CMP $" << std::hex << std::setw(4) << std::setfill('0') << addr << ",X");
}

// =====================================================================================================================
void Cpu::cmpAbsY()
{
    uint16_t addr = read16();
    uint8_t imm = m_memory.read(addr + m_Y);
    setOrClearStatus(Carry, m_A >= imm);
    setOrClearStatus(Zero, m_A == imm);
    setOrClearStatus(Sign, m_A < imm);
    traceInstruction(MakeString() << "CMP $" << std::hex << std::setw(4) << std::setfill('0') << addr << ",Y");
}

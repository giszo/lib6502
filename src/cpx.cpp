#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

// =====================================================================================================================
void Cpu::cpxImm()
{
    uint8_t imm = read8();
    traceInstruction(MakeString(true) << "CPX #$" << std::setw(2) << std::setfill('0') << (int)imm);

    setOrClearStatus(Carry, m_X >= imm);
    setOrClearStatus(Zero, m_X == imm);
    setOrClearStatus(Sign, (m_X - imm) & 0x80);
}

// =====================================================================================================================
void Cpu::cpxZero()
{
    uint16_t address = addrZero();
    traceInstruction(MakeString(true) << "CPX $" << std::setw(2) << std::setfill('0') << address);

    uint8_t imm = m_memory.read(address);
    setOrClearStatus(Carry, m_X >= imm);
    setOrClearStatus(Zero, m_X == imm);
    setOrClearStatus(Sign, (m_X - imm) & 0x80);
}

// =====================================================================================================================
void Cpu::cpxAbs()
{
    uint16_t address = addrAbsolute();
    traceInstruction(MakeString(true) << "CPX $" << std::setw(4) << std::setfill('0') << address);

    uint8_t imm = m_memory.read(address);
    setOrClearStatus(Carry, m_X >= imm);
    setOrClearStatus(Zero, m_X == imm);
    setOrClearStatus(Sign, (m_X - imm) & 0x80);
}

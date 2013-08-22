#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

// =====================================================================================================================
void Cpu::cpyImm()
{
    uint8_t imm = read8();
    traceInstruction(MakeString(true) << "CPY #$" << std::setw(2) << std::setfill('0') << (int)imm);

    setOrClearStatus(Carry, m_Y >= imm);
    setOrClearStatus(Zero, m_Y == imm);
    setOrClearStatus(Sign, (m_Y - imm) & 0x80);

}

// =====================================================================================================================
void Cpu::cpyZero()
{
    uint16_t address = addrZero();
    traceInstruction(MakeString(true) << "CPY $" << std::setw(2) << std::setfill('0') << address);

    uint8_t imm = m_memory.read(address);
    setOrClearStatus(Carry, m_Y >= imm);
    setOrClearStatus(Zero, m_Y == imm);
    setOrClearStatus(Sign, (m_Y - imm) & 0x80);
}

// =====================================================================================================================
void Cpu::cpyAbs()
{
    uint16_t address = addrAbsolute();
    traceInstruction(MakeString(true) << "CPY $" << std::setw(4) << std::setfill('0') << address);

    uint8_t imm = m_memory.read(address);
    setOrClearStatus(Carry, m_Y >= imm);
    setOrClearStatus(Zero, m_Y == imm);
    setOrClearStatus(Sign, (m_Y - imm) & 0x80);
}

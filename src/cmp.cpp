#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

// =====================================================================================================================
unsigned Cpu::cmpImm(uint8_t opCode)
{
    uint8_t imm = read8();
    traceInstruction("CMP", MakeString(true) << "#$" << std::setw(2) << std::setfill('0') << (int)imm);

    setOrClearStatus(Carry, m_A >= imm);
    setOrClearStatus(Zero, m_A == imm);
    setOrClearStatus(Sign, (m_A - imm) & 0x80);

    return 2;
}

// =====================================================================================================================
unsigned Cpu::cmpAddr(uint8_t opCode)
{
    std::string addrTrace;
    uint16_t address = m_addrModeTable[getAddressingMode(opCode)](addrTrace);

    traceInstruction("CMP", addrTrace);

    uint8_t imm = m_memory.read(address);
    setOrClearStatus(Carry, m_A >= imm);
    setOrClearStatus(Zero, m_A == imm);
    setOrClearStatus(Sign, (m_A - imm) & 0x80);

    // TODO
    return 3;
}

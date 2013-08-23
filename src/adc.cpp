#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

// =====================================================================================================================
unsigned Cpu::adcImm(uint8_t opCode)
{
    uint8_t imm = read8();
    traceInstruction(MakeString(true) << "ADC #$" << std::setw(2) << std::setfill('0') << (int)imm);

    int result = m_A + imm + (m_status & Carry ? 1 : 0);
    uint8_t origA = m_A;
    m_A = result;
    updateZero(m_A);
    updateSign(m_A);
    setOrClearStatus(Carry, result > 255);
    setOrClearStatus(Overflow, (origA ^ m_A) & (imm ^ m_A) & 0x80);

    return 2;
}

// =====================================================================================================================
unsigned Cpu::adcAddr(uint8_t opCode)
{
    std::string addrTrace;
    uint16_t address = m_addrModeTable[getAddressingMode(opCode)](addrTrace);

    traceInstruction("ADC", addrTrace);

    uint8_t imm = m_memory.read(address);
    int result = m_A + imm + (m_status & Carry ? 1 : 0);
    uint8_t origA = m_A;
    m_A = result;
    updateZero(m_A);
    updateSign(m_A);
    setOrClearStatus(Carry, result > 255);
    setOrClearStatus(Overflow, (origA ^ m_A) & (imm ^ m_A) & 0x80);

    // TODO
    return 3;
}

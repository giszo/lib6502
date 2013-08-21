#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

// =====================================================================================================================
void Cpu::aslAcc()
{
    traceInstruction("ASL A");
    // bit #0 is shifted into carry
    setOrClearStatus(Carry, (m_A & 0x80) == 0x80);
    m_A <<= 1;
    updateZero(m_A);
    updateSign(m_A);
}

// =====================================================================================================================
void Cpu::aslAbs()
{
    uint16_t addr = read16();
    traceInstruction(MakeString() << "ASL $" << std::hex << std::setw(4) << std::setfill('0') << addr);
    uint8_t data = m_memory.read(addr);
    // bit #0 is shifted into carry
    setOrClearStatus(Carry, (data & 0x80) == 0x80);
    data <<= 1;
    updateZero(data);
    updateSign(data);
    m_memory.write(addr, data);
}

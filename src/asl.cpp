#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

// =====================================================================================================================
unsigned Cpu::aslAcc(uint8_t opCode)
{
    traceInstruction("ASL", "A");

    // bit #0 is shifted into carry
    setOrClearStatus(Carry, (m_A & 0x80) == 0x80);
    m_A <<= 1;
    updateZero(m_A);
    updateSign(m_A);

    return 2;
}

// =====================================================================================================================
unsigned Cpu::aslAddr(uint8_t opCode)
{
    std::string addrTrace;
    uint16_t address = m_addrModeTable[getAddressingMode(opCode)](addrTrace);

    traceInstruction("ASL", addrTrace);

    uint8_t data = m_memory.read(address);
    // bit #0 is shifted into carry
    setOrClearStatus(Carry, (data & 0x80) == 0x80);
    data <<= 1;
    updateZero(data);
    updateSign(data);
    m_memory.write(address, data);

    // TODO
    return 3;
}

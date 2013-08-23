#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

// =====================================================================================================================
unsigned Cpu::rorAcc(uint8_t opCode)
{
    traceInstruction("ROR", "A");

    bool setCarry = (m_A & 0x1) == 1;
    m_A >>= 1;
    if (m_status & Carry)
	m_A |= 0x80;
    setOrClearStatus(Carry, setCarry);
    updateZero(m_A);
    updateSign(m_A);

    return 2;
}

// =====================================================================================================================
unsigned Cpu::rorAddr(uint8_t opCode)
{
    std::string addrTrace;
    uint16_t address = m_addrModeTable[getAddressingMode(opCode)](addrTrace);

    traceInstruction("ROR", addrTrace);

    uint8_t data = m_memory.read(address);
    bool setCarry = (data & 0x1) == 1;
    data >>= 1;
    if (m_status & Carry)
	data |= 0x80;
    setOrClearStatus(Carry, setCarry);
    updateZero(data);
    updateSign(data);
    m_memory.write(address, data);

    // TODO
    return 5;
}

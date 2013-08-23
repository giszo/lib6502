#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

// =====================================================================================================================
unsigned Cpu::rolAcc(uint8_t opCode)
{
    traceInstruction("ROL", "A");

    // bit #0 is shifted into carry
    bool setCarry = m_A & 0x80;
    m_A <<= 1;
    if (m_status & Carry)
	m_A |= 1;
    setOrClearStatus(Carry, setCarry);
    updateZero(m_A);
    updateSign(m_A);

    return 2;
}

// =====================================================================================================================
unsigned Cpu::rolAddr(uint8_t opCode)
{
    std::string addrTrace;
    uint16_t address = m_addrModeTable[getAddressingMode(opCode)](addrTrace);

    traceInstruction("ROL", addrTrace);

    uint8_t data = m_memory.read(address);
    // bit #0 is shifted into carry
    bool setCarry = data & 0x80;
    data <<= 1;
    if (m_status & Carry)
	data |= 1;
    setOrClearStatus(Carry, setCarry);
    updateZero(data);
    updateSign(data);
    m_memory.write(address, data);

    // TODO
    return 5;
}

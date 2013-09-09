#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

static const unsigned s_rolTicks[Cpu::NumOfAddrModes] = {0, 5, 6, 0, 6, 7, 0, 0, 0};

// =====================================================================================================================
unsigned Cpu::rolAcc(uint8_t opCode)
{
#ifdef HAVE_INSTRUCTION_TRACE
    traceInstruction("ROL", "A");
#endif

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
    auto addrMode = getAddressingMode(opCode);
    std::string addrTrace;
    uint16_t address = m_addrModeTable[addrMode](addrTrace);

#ifdef HAVE_INSTRUCTION_TRACE
    traceInstruction("ROL", addrTrace);
#endif

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

    return s_rolTicks[addrMode];
}

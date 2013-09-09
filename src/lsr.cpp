#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

static const unsigned s_lsrTicks[Cpu::NumOfAddrModes] = {0, 5, 6, 0, 6, 7, 0, 0, 0};

// =====================================================================================================================
unsigned Cpu::lsrAcc(uint8_t opCode)
{
#ifdef HAVE_INSTRUCTION_TRACE
    traceInstruction("LSR", " A");
#endif

    // bit #0 is shifted into carry
    setOrClearStatus(Carry, (m_A & 0x1) == 1);
    m_A >>= 1;
    updateZero(m_A);
    updateSign(m_A);

    return 2;
}

// =====================================================================================================================
unsigned Cpu::lsrAddr(uint8_t opCode)
{
    auto addrMode = getAddressingMode(opCode);
    std::string addrTrace;
    uint16_t address = m_addrModeTable[addrMode](addrTrace);

#ifdef HAVE_INSTRUCTION_TRACE
    traceInstruction("LSR", addrTrace);
#endif

    uint8_t data = m_memory.read(address);
    // bit #0 is shifted into carry
    setOrClearStatus(Carry, (data & 0x1) == 1);
    data >>= 1;
    updateZero(data);
    updateSign(data);
    m_memory.write(address, data);

    return s_lsrTicks[addrMode];
}

#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

static const unsigned s_styTicks[Cpu::NumOfAddrModes] = {0, 3, 4, 0, 4, 0, 0, 0, 0};

// =====================================================================================================================
unsigned Cpu::sty(uint8_t opCode)
{
    auto addrMode = getAddressingMode(opCode);
    std::string addrTrace;
    uint16_t address = m_addrModeTable[addrMode](addrTrace);

#ifdef HAVE_INSTRUCTION_TRACE
    traceInstruction("STY", addrTrace);
#endif

    m_memory.write(address, m_Y);

    return s_styTicks[addrMode];
}

#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

static const unsigned s_staTicks[Cpu::NumOfAddrModes] = {0, 3, 4, 0, 4, 5, 5, 6, 6};

// =====================================================================================================================
unsigned Cpu::sta(uint8_t opCode)
{
    auto addrMode = getAddressingMode(opCode);
    std::string addrTrace;
    uint16_t address = m_addrModeTable[addrMode](addrTrace);

#ifdef HAVE_INSTRUCTION_TRACE
    traceInstruction("STA", addrTrace);
#endif

    m_memory.write(address, m_A);

    return s_staTicks[addrMode];
}

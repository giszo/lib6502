#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

static const unsigned s_incTicks[Cpu::NumOfAddrModes] = {0, 5, 6, 0, 6, 7, 0, 0, 0};

// =====================================================================================================================
unsigned Cpu::inc(uint8_t opCode)
{
    auto addrMode = getAddressingMode(opCode);
    std::string addrTrace;
    uint16_t address = m_addrModeTable[addrMode](addrTrace);

#ifdef HAVE_INSTRUCTION_TRACE
    traceInstruction("INC", addrTrace);
#endif

    uint8_t data = m_memory.read(address);
    ++data;
    updateZero(data);
    updateSign(data);
    m_memory.write(address, data);

    return s_incTicks[addrMode];
}

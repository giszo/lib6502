#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

static const unsigned s_stxTicks[Cpu::NumOfAddrModes] = {0, 3, 0, 4, 4, 0, 0, 0, 0};

// =====================================================================================================================
unsigned Cpu::stx(uint8_t opCode)
{
    std::string addrTrace;

    auto addrMode = getAddressingMode(opCode);
    if (addrMode == ZeroPageX) addrMode = ZeroPageY;
    else if (addrMode == AbsoluteX) addrMode = AbsoluteY;
    uint16_t address = m_addrModeTable[addrMode](addrTrace);

    traceInstruction("STX", addrTrace);

    m_memory.write(address, m_X);

    return s_stxTicks[addrMode];
}

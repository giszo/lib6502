#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

// =====================================================================================================================
unsigned Cpu::inc(uint8_t opCode)
{
    std::string addrTrace;
    uint16_t address = m_addrModeTable[getAddressingMode(opCode)](addrTrace);

    traceInstruction("INC", addrTrace);

    uint8_t data = m_memory.read(address);
    ++data;
    updateZero(data);
    updateSign(data);
    m_memory.write(address, data);

    // TODO
    return 5;
}

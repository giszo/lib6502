#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

static const unsigned s_ldaTicks[Cpu::NumOfAddrModes] = {2, 3, 4, 0, 4, 4, 4, 6, 5};

// =====================================================================================================================
unsigned Cpu::ldaImm(uint8_t opCode)
{
    m_A = read8();

#ifdef HAVE_INSTRUCTION_TRACE
    traceInstruction("LDA", MakeString(true) << "#$" << std::setw(2) << std::setfill('0') << (int)m_A);
#endif

    updateZero(m_A);
    updateSign(m_A);

    return s_ldaTicks[Immediate];
}

// =====================================================================================================================
unsigned Cpu::ldaAddr(uint8_t opCode)
{
    auto addrMode = getAddressingMode(opCode);
    std::string addrTrace;
    uint16_t address = m_addrModeTable[addrMode](addrTrace);

#ifdef HAVE_INSTRUCTION_TRACE
    traceInstruction("LDA", addrTrace);
#endif

    m_A = m_memory.read(address);
    updateZero(m_A);
    updateSign(m_A);

    return s_ldaTicks[addrMode];
}

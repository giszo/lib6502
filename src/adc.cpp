#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

static const unsigned s_adcTicks[Cpu::NumOfAddrModes] = {2, 3, 4, 0, 4, 4, 4, 6, 5};

// =====================================================================================================================
unsigned Cpu::adcImm(uint8_t opCode)
{
    uint8_t imm = read8();

#ifdef HAVE_INSTRUCTION_TRACE
    traceInstruction(MakeString(true) << "ADC #$" << std::setw(2) << std::setfill('0') << (int)imm);
#endif

    int result = m_A + imm + (m_status & Carry ? 1 : 0);
    uint8_t origA = m_A;
    m_A = result;
    updateZero(m_A);
    updateSign(m_A);
    setOrClearStatus(Carry, result > 255);
    setOrClearStatus(Overflow, (origA ^ m_A) & (imm ^ m_A) & 0x80);

    return s_adcTicks[Immediate];
}

// =====================================================================================================================
unsigned Cpu::adcAddr(uint8_t opCode)
{
    auto addrMode = getAddressingMode(opCode);
    std::string addrTrace;
    uint16_t address = m_addrModeTable[addrMode](addrTrace);

#ifdef HAVE_INSTRUCTION_TRACE
    traceInstruction("ADC", addrTrace);
#endif

    uint8_t imm = m_memory.read(address);
    int result = m_A + imm + (m_status & Carry ? 1 : 0);
    uint8_t origA = m_A;
    m_A = result;
    updateZero(m_A);
    updateSign(m_A);
    setOrClearStatus(Carry, result > 255);
    setOrClearStatus(Overflow, (origA ^ m_A) & (imm ^ m_A) & 0x80);

    return s_adcTicks[addrMode];
}

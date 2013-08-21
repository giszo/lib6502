#include <lib6502/cpu.h>
#include <lib6502/makestring.h>

#include <iomanip>

#include "statemacros.h"

using lib6502::Cpu;

// TODO: overflow is not updated!
#define SBC_CORE(s) \
    m_A = m_A - (s) - (1 - (m_status & Carry ? 1 : 0)); \
    updateZero(m_A); \
    updateSign(m_A); \
    setOrClearStatus(Carry, (m_A & 0x80) == 0x80);

// =====================================================================================================================
void Cpu::sbcImm()
{
    uint8_t imm = read8();
    traceInstruction(MakeString() << "SBC #$" << std::hex << std::setw(2) << std::setfill('0') << (int)imm);
    SBC_CORE(imm);
}

// =====================================================================================================================
void Cpu::sbcZeroX()
{
    uint16_t addr = read8();
    traceInstruction(MakeString() << "SBC $" << std::hex << std::setw(4) << std::setfill('0') << addr << ",X");
    SBC_CORE(m_memory.read(addr + m_X));
}

// =====================================================================================================================
void Cpu::sbcAbs()
{
    uint16_t addr = read16();
    traceInstruction(MakeString() << "SBC $" << std::hex << std::setw(4) << std::setfill('0') << addr);
    SBC_CORE(m_memory.read(addr));
}

// =====================================================================================================================
void Cpu::sbcAbsY()
{
    uint16_t addr = read16();
    traceInstruction(MakeString() << "SBC $" << std::hex << std::setw(4) << std::setfill('0') << addr << ",Y");
    SBC_CORE(m_memory.read(addr + m_Y));
}

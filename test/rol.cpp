#include <boost/test/unit_test.hpp>

#include "fixture.h"

using lib6502::Cpu;

// $2a
BOOST_FIXTURE_TEST_CASE(testRolAcc, Fixture)
{
    auto& cpuState = m_cpu->getState();

    // prepare test instruction
    m_ram[0x8000] = 0x2a;
    m_ram[0x8001] = 0x2a;

    // tick
    cpuState.m_A = 0x40;
    PREPARE_FLAGS(cpuState.m_status, Cpu::Carry, Cpu::Sign);
    m_cpu->tick();

    // check the result
    BOOST_CHECK_EQUAL(cpuState.m_A, 0x81);
    CHECK_FLAGS(cpuState.m_status, Cpu::Sign, Cpu::Carry);

    // tick
    cpuState.m_A = 0x80;
    PREPARE_FLAGS(cpuState.m_status, Cpu::Sign, Cpu::Zero | Cpu::Carry);
    m_cpu->tick();

    // check the result
    BOOST_CHECK_EQUAL(cpuState.m_A, 0x00);
    CHECK_FLAGS(cpuState.m_status, Cpu::Zero | Cpu::Carry, Cpu::Sign);
}

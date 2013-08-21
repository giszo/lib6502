#include <boost/test/unit_test.hpp>

#include "fixture.h"

using lib6502::Cpu;

// $09
BOOST_FIXTURE_TEST_CASE(testOraImm, Fixture)
{
    auto& cpuState = m_cpu->getState();

    // prepare test instructions
    m_ram[0x8000] = 0x09;
    m_ram[0x8001] = 0x00;

    m_ram[0x8002] = 0x09;
    m_ram[0x8003] = 0x80;

    // tick
    cpuState.m_A = 0x00;
    PREPARE_FLAGS(cpuState.m_status, Cpu::Sign, Cpu::Zero);
    m_cpu->tick();

    // check the result
    BOOST_REQUIRE_EQUAL(m_memAccessHist.size(), 2);
    BOOST_CHECK(m_memAccessHist[0] == READ_AT(0x8000));
    BOOST_CHECK(m_memAccessHist[1] == READ_AT(0x8001));
    BOOST_CHECK_EQUAL(cpuState.m_A, 0x00 /* 0x00 | 0x00 */);
    CHECK_FLAGS(cpuState.m_status, Cpu::Zero, Cpu::Sign);

    // tick
    cpuState.m_A = 0x08;
    PREPARE_FLAGS(cpuState.m_status, Cpu::Zero, Cpu::Sign);
    m_cpu->tick();

    // check the result
    BOOST_CHECK_EQUAL(cpuState.m_A, 0x88 /* 0x80 | 0x08 */);
    CHECK_FLAGS(cpuState.m_status, Cpu::Sign, Cpu::Zero);
}

// $05
BOOST_FIXTURE_TEST_CASE(testOraZero, Fixture)
{
    auto& cpuState = m_cpu->getState();

    // prepare test instructions
    m_ram[0x8000] = 0x05;
    m_ram[0x8001] = 0x42;
    m_ram[0x0042] = 0x00;

    m_ram[0x8002] = 0x05;
    m_ram[0x8003] = 0x43;
    m_ram[0x0043] = 0x80;

    // tick
    cpuState.m_A = 0x00;
    PREPARE_FLAGS(cpuState.m_status, Cpu::Sign, Cpu::Zero);
    m_cpu->tick();

    // check the result
    BOOST_REQUIRE_EQUAL(m_memAccessHist.size(), 3);
    BOOST_CHECK(m_memAccessHist[0] == READ_AT(0x8000));
    BOOST_CHECK(m_memAccessHist[1] == READ_AT(0x8001));
    BOOST_CHECK(m_memAccessHist[2] == READ_AT(0x0042));
    BOOST_CHECK_EQUAL(cpuState.m_A, 0x00 /* 0x00 | 0x00 */);
    CHECK_FLAGS(cpuState.m_status, Cpu::Zero, Cpu::Sign);

    // tick
    cpuState.m_A = 0x08;
    PREPARE_FLAGS(cpuState.m_status, Cpu::Zero, Cpu::Sign);
    m_cpu->tick();

    // check the result
    BOOST_CHECK_EQUAL(cpuState.m_A, 0x88 /* 0x80 | 0x08 */);
    CHECK_FLAGS(cpuState.m_status, Cpu::Sign, Cpu::Zero);
}

// $15
BOOST_FIXTURE_TEST_CASE(testOraZeroX, Fixture)
{
    auto& cpuState = m_cpu->getState();

    // prepare test instructions
    m_ram[0x8000] = 0x15;
    m_ram[0x8001] = 0x42;
    m_ram[0x0084] = 0x00;

    m_ram[0x8002] = 0x15;
    m_ram[0x8003] = 0x43;
    m_ram[0x0085] = 0x80;

    // tick
    cpuState.m_A = 0x00;
    cpuState.m_X = 0x42;
    PREPARE_FLAGS(cpuState.m_status, Cpu::Sign, Cpu::Zero);
    m_cpu->tick();

    // check the result
    BOOST_REQUIRE_EQUAL(m_memAccessHist.size(), 3);
    BOOST_CHECK(m_memAccessHist[0] == READ_AT(0x8000));
    BOOST_CHECK(m_memAccessHist[1] == READ_AT(0x8001));
    BOOST_CHECK(m_memAccessHist[2] == READ_AT(0x0084));
    BOOST_CHECK_EQUAL(cpuState.m_A, 0x00 /* 0x00 | 0x00 */);
    CHECK_FLAGS(cpuState.m_status, Cpu::Zero, Cpu::Sign);

    // tick
    cpuState.m_A = 0x08;
    cpuState.m_X = 0x42;
    PREPARE_FLAGS(cpuState.m_status, Cpu::Zero, Cpu::Sign);
    m_cpu->tick();

    // check the result
    BOOST_CHECK_EQUAL(cpuState.m_A, 0x88 /* 0x80 | 0x08 */);
    CHECK_FLAGS(cpuState.m_status, Cpu::Sign, Cpu::Zero);
}

// $0d
BOOST_FIXTURE_TEST_CASE(testOraAbs, Fixture)
{
    auto& cpuState = m_cpu->getState();

    // prepare test instructions
    m_ram[0x8000] = 0x0d;
    m_ram[0x8001] = 0x20;
    m_ram[0x8002] = 0x10;
    m_ram[0x1020] = 0x00;

    m_ram[0x8003] = 0x0d;
    m_ram[0x8004] = 0x22;
    m_ram[0x8005] = 0x11;
    m_ram[0x1122] = 0x80;

    // tick
    cpuState.m_A = 0x00;
    PREPARE_FLAGS(cpuState.m_status, Cpu::Sign, Cpu::Zero);
    m_cpu->tick();

    // check the result
    BOOST_REQUIRE_EQUAL(m_memAccessHist.size(), 4);
    BOOST_CHECK(m_memAccessHist[0] == READ_AT(0x8000));
    BOOST_CHECK(m_memAccessHist[1] == READ_AT(0x8001));
    BOOST_CHECK(m_memAccessHist[2] == READ_AT(0x8002));
    BOOST_CHECK(m_memAccessHist[3] == READ_AT(0x1020));
    BOOST_CHECK_EQUAL(cpuState.m_A, 0x00 /* 0x00 | 0x00 */);
    CHECK_FLAGS(cpuState.m_status, Cpu::Zero, Cpu::Sign);

    // tick
    cpuState.m_A = 0x08;
    PREPARE_FLAGS(cpuState.m_status, Cpu::Zero, Cpu::Sign);
    m_cpu->tick();

    // check the result
    BOOST_CHECK_EQUAL(cpuState.m_A, 0x88 /* 0x80 | 0x08 */);
    CHECK_FLAGS(cpuState.m_status, Cpu::Sign, Cpu::Zero);
}

// $1d
BOOST_FIXTURE_TEST_CASE(testOraAbsX, Fixture)
{
    auto& cpuState = m_cpu->getState();

    // prepare test instructions
    m_ram[0x8000] = 0x1d;
    m_ram[0x8001] = 0x20;
    m_ram[0x8002] = 0x10;
    m_ram[0x1062] = 0x00;

    m_ram[0x8003] = 0x1d;
    m_ram[0x8004] = 0x22;
    m_ram[0x8005] = 0x11;
    m_ram[0x1164] = 0x80;

    // tick
    cpuState.m_A = 0x00;
    cpuState.m_X = 0x42;
    PREPARE_FLAGS(cpuState.m_status, Cpu::Sign, Cpu::Zero);
    m_cpu->tick();

    // check the result
    BOOST_REQUIRE_EQUAL(m_memAccessHist.size(), 4);
    BOOST_CHECK(m_memAccessHist[0] == READ_AT(0x8000));
    BOOST_CHECK(m_memAccessHist[1] == READ_AT(0x8001));
    BOOST_CHECK(m_memAccessHist[2] == READ_AT(0x8002));
    BOOST_CHECK(m_memAccessHist[3] == READ_AT(0x1062));
    BOOST_CHECK_EQUAL(cpuState.m_A, 0x00 /* 0x00 | 0x00 */);
    CHECK_FLAGS(cpuState.m_status, Cpu::Zero, Cpu::Sign);

    // tick
    cpuState.m_A = 0x08;
    cpuState.m_X = 0x42;
    PREPARE_FLAGS(cpuState.m_status, Cpu::Zero, Cpu::Sign);
    m_cpu->tick();

    // check the result
    BOOST_CHECK_EQUAL(cpuState.m_A, 0x88 /* 0x80 | 0x08 */);
    CHECK_FLAGS(cpuState.m_status, Cpu::Sign, Cpu::Zero);
}

// $19
BOOST_FIXTURE_TEST_CASE(testOraAbsY, Fixture)
{
    auto& cpuState = m_cpu->getState();

    // prepare test instructions
    m_ram[0x8000] = 0x19;
    m_ram[0x8001] = 0x20;
    m_ram[0x8002] = 0x10;
    m_ram[0x1042] = 0x00;

    m_ram[0x8003] = 0x19;
    m_ram[0x8004] = 0x22;
    m_ram[0x8005] = 0x11;
    m_ram[0x1144] = 0x80;

    // tick
    cpuState.m_A = 0x00;
    cpuState.m_Y = 0x22;
    PREPARE_FLAGS(cpuState.m_status, Cpu::Sign, Cpu::Zero);
    m_cpu->tick();

    // check the result
    BOOST_REQUIRE_EQUAL(m_memAccessHist.size(), 4);
    BOOST_CHECK(m_memAccessHist[0] == READ_AT(0x8000));
    BOOST_CHECK(m_memAccessHist[1] == READ_AT(0x8001));
    BOOST_CHECK(m_memAccessHist[2] == READ_AT(0x8002));
    BOOST_CHECK(m_memAccessHist[3] == READ_AT(0x1042));
    BOOST_CHECK_EQUAL(cpuState.m_A, 0x00 /* 0x00 | 0x00 */);
    CHECK_FLAGS(cpuState.m_status, Cpu::Zero, Cpu::Sign);

    // tick
    cpuState.m_A = 0x08;
    cpuState.m_Y = 0x22;
    PREPARE_FLAGS(cpuState.m_status, Cpu::Zero, Cpu::Sign);
    m_cpu->tick();

    // check the result
    BOOST_CHECK_EQUAL(cpuState.m_A, 0x88 /* 0x80 | 0x08 */);
    CHECK_FLAGS(cpuState.m_status, Cpu::Sign, Cpu::Zero);
}

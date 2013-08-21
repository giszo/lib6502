#ifndef FIXTURE_H_INCLUDED
#define FIXTURE_H_INCLUDED

#include <lib6502/cpu.h>

#include <vector>
#include <memory>

#define READ_AT(addr) Fixture::MemoryAccess(Fixture::Read, addr)

/**
 * Prepares the status register of the CPU.
 *
 * The mask 'set' will be set in the status register and the 'not_set' mask will be cleared.
 */
#define PREPARE_FLAGS(status, set, not_set) \
    status |= (set); \
    status &= ~(not_set);

/**
 * Checks the status register of the CPU.
 *
 * The mask 'set' will be checked whether it is set in the status register and 'not_set' will be checked whether those
 * bits are not set.
 */
#define CHECK_FLAGS(status, set, not_set) \
    BOOST_CHECK_EQUAL(status & (set), set); \
    BOOST_CHECK_EQUAL(status & (not_set), 0);

struct Fixture : public lib6502::Memory
{
    Fixture();
    ~Fixture();

    uint8_t read(uint16_t addr) override;
    void write(uint16_t addr, uint8_t data) override;

    void dumpMemoryAccess();

    std::unique_ptr<lib6502::Cpu> m_cpu;
    uint8_t* m_ram;

    enum AccessType
    {
	Read,
	Write
    };

    struct MemoryAccess
    {
	MemoryAccess(AccessType type, uint16_t address)
	    : m_type(type), m_address(address)
	{}

	bool operator==(const MemoryAccess& a)
	{ return m_type == a.m_type && m_address == a.m_address; }

	AccessType m_type;
	uint16_t m_address;
    };

    std::vector<MemoryAccess> m_memAccessHist;
};

#endif

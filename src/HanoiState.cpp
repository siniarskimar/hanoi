#include "HanoiState.hpp"
#include "defer.hpp"
#include <stdexcept>

using DiskRodAllocTraits = std::allocator_traits<HanoiState::DiskRodAllocator>;

HanoiState::HanoiState(size_t rod_count, size_t disk_count)
    : m_move_count(0), m_rod_count(rod_count), m_diskstack_alloc(),
      m_rods(nullptr) {
    m_rods = DiskRodAllocTraits::allocate(m_diskstack_alloc, rod_count);

    size_t rod_idx = 0;
    auto defer = Defer{[&]() {
        while (rod_idx > 0) {
            DiskRodAllocTraits::destroy(m_diskstack_alloc, &m_rods[rod_idx]);
            rod_idx--;
        }
        DiskRodAllocTraits::deallocate(m_diskstack_alloc, m_rods, m_rod_count);
    }};

    while (rod_idx < rod_count) {
        DiskRodAllocTraits::construct(m_diskstack_alloc, &m_rods[rod_idx],
                                      disk_count);
        rod_idx++;
    }

    defer.cancel();
}

HanoiState::~HanoiState() {
    for (size_t rod_idx = 0; rod_idx < m_rod_count; rod_idx++) {
        DiskRodAllocTraits::destroy(m_diskstack_alloc, &m_rods[m_rod_count]);
    }
    DiskRodAllocTraits::deallocate(m_diskstack_alloc, m_rods, m_rod_count);
}

bool HanoiState::isValidDiskMove(size_t source_rod, size_t dest_rod) {
    if (source_rod >= m_rod_count) {
        throw std::out_of_range("source_rod argument out of range");
    }
    if (dest_rod >= m_rod_count) {
        throw std::out_of_range("dest_rod out of range");
    }

    return !m_rods[source_rod].empty() &&
           m_rods[dest_rod].top() > m_rods[source_rod].top();
}

bool HanoiState::moveDisk(size_t source_rod, size_t dest_rod) {
    if (!isValidDiskMove(source_rod, dest_rod)) {
        return false;
    }

    m_rods[dest_rod].push(m_rods[source_rod].pop());
    return true;
}

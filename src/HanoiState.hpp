#pragma once
#include <cstddef>

#include "DiskRod.hpp"
#include "span.hpp"

class HanoiState {
  public:
    using DiskRodAllocator = std::allocator<DiskRod>;

    HanoiState(size_t rod_count, size_t disk_count);
    ~HanoiState();

    bool isGameWon();

    bool isValidDiskMove(size_t source_rod, size_t dest_rod);
    bool moveDisk(size_t source_rod, size_t dest_rod);

    // Getters
    size_t rodCount() noexcept { return m_rod_count; }
    size_t moveCount() noexcept { return m_move_count; }
    stdex::span<const DiskRod> rods() noexcept {
        return stdex::span<const DiskRod>(m_rods, m_rod_count);
    }

  private:
    size_t m_move_count;
    size_t m_rod_count;
    DiskRodAllocator m_diskstack_alloc;
    DiskRod *m_rods;
};

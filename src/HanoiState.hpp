#pragma once
#include <cstddef>
#include <vector>

#include "DiskRod.hpp"
#include "span.hpp"

class HanoiState {
  public:
    HanoiState(size_t rod_count, size_t disk_count);

    bool isGameWon();

    bool isValidDiskMove(size_t source_rod, size_t dest_rod);
    bool moveDisk(size_t source_rod, size_t dest_rod);

    // Getters
    size_t rodCount() noexcept { return m_rod_count; }
    size_t moveCount() noexcept { return m_move_count; }
    stdex::span<const DiskRod> rods() noexcept {
        return stdex::span<const DiskRod>(m_rods.data(), m_rod_count);
    }

  private:
    size_t m_move_count;
    size_t m_rod_count;
    std::vector<DiskRod> m_rods;
};

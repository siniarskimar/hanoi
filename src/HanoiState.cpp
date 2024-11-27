#include "HanoiState.hpp"
#include <stdexcept>


HanoiState::HanoiState(size_t rod_count, size_t disk_count)
    : m_move_count(0), m_rod_count(rod_count), m_rods() {

    for (size_t rod_idx = 0; rod_idx < rod_count; rod_idx++) {
        m_rods.emplace_back(disk_count);
    }

    for (size_t disk_idx = 0; disk_idx < disk_count; disk_idx++) {
        m_rods[0].push(disk_count - disk_idx);
    }
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

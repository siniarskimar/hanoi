#include "DiskRod.hpp"
#include <stdexcept>

DiskRod::DiskRod(size_t height)
    : m_height(height), m_top(0), m_disks(new size_t[height]()) {}

size_t DiskRod::top() noexcept { return m_disks[m_top - 1]; }
size_t DiskRod::pop() {
    if (m_top == 0) {
        throw std::length_error("cannot pop from empty DiskRod");
    }
    m_top--;
    auto disk = m_disks[m_top];
    m_disks[m_top] = 0;
    return disk;
}

void DiskRod::push(size_t disk) {
    if (m_top == m_height) {
        throw std::length_error("cannot push onto full DiskRod");
    }
    m_disks[m_top] = disk;
    m_top++;
}

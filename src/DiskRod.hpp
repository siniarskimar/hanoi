#pragma once
#include <cstddef>
#include <memory>

#include "span.hpp"

class DiskRod {
  public:
    DiskRod(size_t height);

    size_t top() noexcept;
    size_t pop();
    void push(size_t disk);

    bool empty() const noexcept { return m_top == 0; }
    size_t height() const noexcept { return m_height; };

    stdex::span<const size_t> disks() const noexcept {
        return stdex::span<const size_t>(m_disks.get(), m_top);
    }

  private:
    size_t m_height;
    size_t m_top;
    std::unique_ptr<size_t[]> m_disks;
};

#pragma once

#include <functional>

struct Defer {
  std::function<void(void)> func;

  ~Defer() {
    if (this->func) {
      this->func();
    }
  }

  void cancel() noexcept { this->func = {}; }

private:
  // Disallow **ALL** copying
  Defer(const Defer &) = delete;
  Defer &operator=(const Defer &) = delete;
};

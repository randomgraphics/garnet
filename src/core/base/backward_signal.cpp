#include "pch.h"

// Inline backward-cpp signal handling (same as backward.cpp in the submodule).
// Ensures crash handler runs on SIGSEGV/SIGABRT etc. and dumps full stack trace.
#include <backward.hpp>

namespace backward {

backward::SignalHandling sh;

} // namespace backward

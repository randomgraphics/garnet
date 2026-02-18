// Intentionally crashes to verify backward-cpp signal handler dumps stack trace
// to stderr. Linked with GNcore so backward_signal.cpp (SignalHandling) is active.
#include "garnet/GNbase.h"

static void level3() {
    abort(); // triggers SIGABRT; backward prints stack then process exits
}

static void level2() { level3(); }
static void level1() { level2(); }

int main() {
    // Force GNcore (and backward SignalHandling) to load before we abort
    (void) GN::backtrace(0, false);
    level1();
    return 0;
}

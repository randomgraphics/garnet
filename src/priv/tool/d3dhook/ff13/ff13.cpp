#include "pch.h"
#include <conio.h>

using namespace GN;

static GN::Logger * sLogger = GN::getLogger("GN::ff13");

int main( int argc, const char * argv[] )
{
    GN_UNUSED_PARAM(argc);
    GN_UNUSED_PARAM(argv);

    HMODULE dll = LoadLibraryA("GNff13.dll");

    if (0 == dll)
    {
        GN_ERROR(sLogger)("Failed to load GNff13.dll: %s", getWin32LastErrorInfo());
        return -1;
    }

    HOOKPROC proc = (HOOKPROC)GetProcAddress(dll, "HookProc");
    if (0 == proc)
    {
        GN_ERROR(sLogger)("Failed to get address of HookProc: %s", getWin32LastErrorInfo());
        return -1;
    }

    HHOOK hook = SetWindowsHookEx(WH_CBT, proc, dll, 0);
    if (0 == hook)
    {
        GN_ERROR(sLogger)("Failed to get address of HookProc: %s", getWin32LastErrorInfo());
        return -1;
    }

    // now wait for the signal to quit.
    _cputs("Type 'Q' when ready to quit...");
    int ch;
    do
    {
        ch = toupper(_getch());
    } while( ch != 'Q');

    // done
    UnhookWindowsHookEx(hook);
    return 0;
}

// Do _NOT_ compile this file directly. Instead, include this file as part of
// API specific cpp file, to avoid compile confliction

// *****************************************************************************
// Class Factory
// *****************************************************************************

HookedClassFactory HookedClassFactory::s_instance;

// *****************************************************************************
// Call Trace
// *****************************************************************************

namespace calltrace
{
    __declspec(thread) int g_level = 0;

    bool g_printCall = false;

    int enter(const wchar_t * text)
    {
        if (g_printCall)
        {
            wchar_t ident[256] = {};
            for(int i = 0; i < g_level && i < _countof(ident); ++i)
            {
                ident[i] = L' ';
            }

            wchar_t buf[256] = {};
            swprintf_s(buf, L"{%d}", GetCurrentThreadId());
            wcscat_s(buf, ident);
            wcscat_s(buf, text);
            wcscat_s(buf, L"\n");

            if (IsDebuggerPresent())
            {
                OutputDebugStringW(buf);
            }
        }
        return ++g_level;
    }

    int enter(const char * text)
    {
        if (g_printCall)
        {
            wchar_t textw[256];
            swprintf_s(textw, L"%S", text);
            return enter(textw);
        }
        else
        {
            return enter(L"");
        }
    }

    void leave()
    {
        --g_level;
    }
}


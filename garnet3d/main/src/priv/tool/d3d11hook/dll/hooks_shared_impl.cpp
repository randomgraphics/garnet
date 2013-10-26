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
    class CallTrace
    {
        int _level;

    public:

        CallTrace() : _level(0)
        {
        }

        ~CallTrace()
        {
        }

        int enter(const wchar_t * text)
        {
            wchar_t buf[256];
            int i;
            for(i = 0; i < _level && i < _countof(buf); ++i)
            {
                buf[i] = L' ';
            }
            swprintf_s(&buf[i], (_countof(buf) - i), L"%s\n", text);
            if (IsDebuggerPresent())
            {
                OutputDebugStringW(buf);
            }
            return ++_level;
        }

        int enter(const char * text)
        {
            char buf[256];
            int i;
            for(i = 0; i < _level && i < _countof(buf); ++i)
            {
                buf[i] = ' ';
            }
            sprintf_s(&buf[i], (_countof(buf) - i), "%s\n", text);
            if (IsDebuggerPresent())
            {
                OutputDebugStringA(buf);
            }
            return ++_level;
        }

        void leave()
        {
            --_level;
        }
    };

    CallTrace g_callTrace;

    int enter(const wchar_t * text)
    {
        return g_callTrace.enter( text );
    }

    int enter(const char * text)
    {
        return g_callTrace.enter( text );
    }

    void leave()
    {
        g_callTrace.leave();
    }
}

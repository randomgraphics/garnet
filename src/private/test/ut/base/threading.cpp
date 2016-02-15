#include "../testCommon.h"

class ThreadingTest : public CxxTest::TestSuite
{
public:

    void testThreadInvalidID()
    {
        using namespace GN;

        Thread::Identifier candidates[] = { 0, 0xabcd };

        for( uint32 i = 0; i < GN_ARRAY_COUNT(candidates); ++i )
        {
            Thread::Identifier id = candidates[i];

            TS_ASSERT_EQUALS( WaitResult::FAILED, Thread::sJoin( id ) );
        }
    }

    static void dummpyThreadProc( void * )
    {
        using namespace GN;
        Thread::Identifier t = Thread::sGetCurrentThread();
        TS_ASSERT( Thread::sIsCurrentThread(t) );
        // Join to current thread should always return TIMEDOUT.
        TS_ASSERT_EQUALS( WaitResult::TIMEDOUT, Thread::sJoin( t ) );
    }

    void testCreateThread()
    {
        using namespace GN;

        Thread::Identifier t = Thread::sCreate( dummpyThreadProc, (void*)1 );

        Thread::sSleepCurrentThread( ONE_SECOND_IN_NS / 2 );

        // Thread 't' should have terminated and self-destructed.
        TS_ASSERT_EQUALS( WaitResult::KILLED, Thread::sJoin( t ) );
    }

    void testThreadCurrentThread()
    {
        using namespace GN;

        Thread::Identifier t = Thread::sGetCurrentThread();

        TS_ASSERT( Thread::sIsCurrentThread(t) );

        // Join to current thread should always return TIMEDOUT.
        TS_ASSERT_EQUALS( WaitResult::TIMEDOUT, Thread::sJoin( t ) );
    }
};

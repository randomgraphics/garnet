#include "../testCommon.h"

class AssertTest : public CxxTest::TestSuite {
public:
    void testSuccess() { GN_ASSERT(1); }

    void * context;

    static void OnAssertFail(void * userContext,
                             const char *, // msg,
                             const char *, // file,
                             int,          // line,
                             bool *)       // ignoreTheFailureForEver )
    {
        ((AssertTest *) userContext)->context = userContext;
    }

    void TestFailure() {
        GN::RuntimeAssertBehavior    oldb;
        GN::AssertFailuerUserRoutine oldr;
        void *                       oldc;

        oldb = GN::setRuntimeAssertBehavior(GN::RAB_CALL_USER_ROUTINE);
        GN::setAssertFailerUserRoutine(OnAssertFail, this, &oldr, &oldc);

        context = NULL;

        GN_ASSERT(0);

#if GN_BUILD_DEBUG_ENABLED
        // In debug build, assert failrure shoudl trigger assert failure routine.
        TS_ASSERT_EQUALS(context, this);
#else
        // In profile and release build, assert macro does nothing
        TS_ASSERT_EQUALS(context, (void *) NULL);
#endif

        GN::setRuntimeAssertBehavior(oldb);
        GN::setAssertFailerUserRoutine(oldr, oldc);
    }
};

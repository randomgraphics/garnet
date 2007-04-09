#include "pch.h"
#include "garnet/base/thread.h"

#if GN_MSWIN

static GN::Logger * sLogger = GN::getLogger("GN.base.Semaphore");

using namespace GN;

// *****************************************************************************
// local class and functions
// *****************************************************************************

extern UInt32 sec2usec( float time );

///
/// semaphore implementation on MS windows.
///
class SemaphoreMsw : public Semaphore, public StdClass
{
    GN_DECLARE_STDCLASS( SemaphoreMsw, StdClass );

    // ********************************
    // ctor/dtor
    // ********************************

    //@{
public:
    SemaphoreMsw()          { clear(); }
    virtual ~SemaphoreMsw() { quit(); }
    //@}

    // ********************************
    // from StdClass
    // ********************************

    //@{
public:
    bool init( size_t maxcount, size_t initialcount, const char * name )
    {
        GN_GUARD;

        // standard init procedure
        GN_STDCLASS_INIT( SemaphoreMsw, () );

        name = strEmpty(name) ? "Unnamed garnet semaphore" : name;

        mHandle = CreateSemaphoreA( 0, (LONG)initialcount, (LONG)maxcount, name );

        GN_MSW_CHECK_RV( mHandle, failure() );

        // success
        return success();

        GN_UNGUARD;
    }

    void quit()
    {
        GN_GUARD;

        if( mHandle ) CloseHandle( mHandle );

        // standard quit procedure
        GN_STDCLASS_QUIT();

        GN_UNGUARD;
    }

private:
    void clear()
    {
        mHandle = 0;
    }
    //@}

    // ********************************
    // public functions
    // ********************************
public:

    virtual bool wait( float seconds )
    {
        GN_ASSERT( mHandle );
        DWORD result = WaitForSingleObject( mHandle, sec2usec(seconds) );
        return WAIT_OBJECT_0 == result;
    }

    virtual void wake()
    {
        GN_ASSERT( mHandle );
        ReleaseSemaphore( mHandle, 1, 0 );
    }

    // ********************************
    // private variables
    // ********************************
private:

    HANDLE mHandle;

    // ********************************
    // private functions
    // ********************************
private:
};

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::Semaphore *
GN::createSemaphore( size_t maxcount, size_t initialcount, const char * name )
{
    GN_GUARD;

    AutoObjPtr<SemaphoreMsw> s( new SemaphoreMsw );

    if( !s->init( maxcount, initialcount, name ) ) return 0;

    return s.detach();

    GN_UNGUARD;
}

#endif

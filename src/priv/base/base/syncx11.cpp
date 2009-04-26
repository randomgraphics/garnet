#include "pch.h"

float const GN::INFINITE_TIME = 1e38f;

#if GN_POSIX

static GN::Logger * sLogger = GN::getLogger("GN.base.Sync");

using namespace GN;

// *****************************************************************************
// mutex class
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::Mutex::Mutex()
{
    GN_UNIMPL_WARNING();
}

//
//
// -----------------------------------------------------------------------------
GN::Mutex::~Mutex()
{
    GN_UNIMPL_WARNING();
}

//
//
// -----------------------------------------------------------------------------
bool GN::Mutex::trylock()
{
    GN_UNIMPL_WARNING();
	return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::Mutex::lock()
{
    GN_UNIMPL_WARNING();
}

//
//
// -----------------------------------------------------------------------------
void GN::Mutex::unlock()
{
    GN_UNIMPL_WARNING();
}

// *****************************************************************************
// syncevent class
// *****************************************************************************

///
/// sync event on MS Windows.
///
class SyncEventX11 : public SyncEvent, public StdClass
{
    GN_DECLARE_STDCLASS( SyncEventX11, StdClass );

    // ********************************
    // ctor/dtor
    // ********************************

    //@{
public:
    SyncEventX11()          { clear(); }
    virtual ~SyncEventX11() { quit(); }
    //@}

    // ********************************
    // from StdClass
    // ********************************

    //@{
public:
    bool init( bool initialSignaled, bool autoreset, const char * name )
    {
        GN_GUARD;

        // standard init procedure
        GN_STDCLASS_INIT( SyncEventX11, () );

        GN_UNIMPL_WARNING();

        // success
        return success();

        GN_UNGUARD;
    }
    void quit()
    {
        GN_GUARD;

        GN_UNIMPL_WARNING();

        // standard quit procedure
        GN_STDCLASS_QUIT();

        GN_UNGUARD;
    }
private:
    void clear() {}
    //@}

    // ********************************
    // from SyncEvent
    // ********************************
public:

    virtual void signal()
    {
        GN_UNIMPL_WARNING();
    }

    virtual void unsignal()
    {
        GN_UNIMPL_WARNING();
    }

    virtual bool wait( float seconds )
    {
        GN_UNIMPL_WARNING();
        return true;
    }

    // ********************************
    // private variables
    // ********************************
private:

    // ********************************
    // private functions
    // ********************************
private:
};

// *****************************************************************************
// semaphore class
// *****************************************************************************

///
/// semaphore on MS Windows.
///
class SemaphoreX11 : public Semaphore, public StdClass
{
    GN_DECLARE_STDCLASS( SemaphoreX11, StdClass );

    // ********************************
    // ctor/dtor
    // ********************************

    //@{
public:
    SemaphoreX11()          { clear(); }
    virtual ~SemaphoreX11() { quit(); }
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
        GN_STDCLASS_INIT( SemaphoreX11, () );

        GN_UNIMPL_WARNING();

        // success
        return success();

        GN_UNGUARD;
    }
    void quit()
    {
        GN_GUARD;

        GN_UNIMPL_WARNING();

        // standard quit procedure
        GN_STDCLASS_QUIT();

        GN_UNGUARD;
    }
private:
    void clear() {}
    //@}

    // ********************************
    // from Semaphore
    // ********************************
public:

    virtual bool wait( float seconds )
    {
        GN_UNIMPL_WARNING();
        return true;
    }

    virtual void wake( size_t count )
    {
        GN_UNIMPL_WARNING();
    }

    // ********************************
    // private variables
    // ********************************
private:

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
GN::SyncEvent * GN::createSyncEvent(
    bool initialSignaled,
    bool autoreset,
    const char * name )
{
    GN_GUARD;

    AutoObjPtr<SyncEventX11> s( new SyncEventX11 );

    if( !s->init( initialSignaled, autoreset, name ) ) return 0;

    return s.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::Semaphore * GN::createSemaphore(
    size_t maxcount,
    size_t initialcount,
    const char * name )
{
    GN_GUARD;

    AutoObjPtr<SemaphoreX11> s( new SemaphoreX11 );

    if( !s->init( maxcount, initialcount, name ) ) return 0;

    return s.detach();

    GN_UNGUARD;
}

#endif

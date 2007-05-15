#ifndef __GN_TEST_ENGINE_TESTCASE_H__
#define __GN_TEST_ENGINE_TESTCASE_H__
// *****************************************************************************
//! \file    engine/testCase.h
//! \brief   common test case interface
//! \author  chen@@CHENLI-HOMEPC (2007.5.7)
// *****************************************************************************

#include "garnet/GNengine.h"

using namespace GN;
using namespace GN::input;
using namespace GN::gfx;
using namespace GN::engine;

///
/// coming soon ...
///
class TestCase
{
    EntityManager & mEntityManager;
    RenderEngine & mRenderEngine;

public:

    ///
    /// ctor
    ///
    TestCase( EntityManager & em, RenderEngine & re ) : mEntityManager(em), mRenderEngine( re ) {}

    ///
    /// get engine reference
    ///
    RenderEngine & renderEngine() const { return mRenderEngine; }

    ///
    /// get entity manager
    ///
    EntityManager & entityManager() const { return mEntityManager; }

    //@{

    virtual bool init() = 0;
    virtual void quit() = 0;
    virtual void draw() = 0;

    //@}
};

///
/// coming soon ...
///
class DummyLoader : public GraphicsResourceLoader
{
public:
    virtual bool load( const GraphicsResourceDesc &, void * & outbuf, size_t & outbytes, int )
    {
        outbuf = 0;
        outbytes = 0;
        return true;
    }

    bool decompress( const GraphicsResourceDesc &, void * & outbuf, size_t & outbytes, const void *, size_t, int )
    {
        outbuf = 0;
        outbytes = 0;
        return true;
    }

    virtual bool copy( GraphicsResource &, const void * , size_t, int )
    {
        return true;
    }

    virtual void freebuf( void *, size_t )
    {
    }
};

///
/// coming soon ...
///
class BasicVtxBufLoader : public GraphicsResourceLoader
{
public:

    virtual bool load( const GraphicsResourceDesc &, void * & outbuf, size_t & outbytes, int )
    {
        outbuf = 0;
        outbytes = 0;
        return true;
    }

    virtual bool copy( GraphicsResource & gfxres, const void * inbuf, size_t inbytes, int )
    {
        VtxBuf * vb = gfxres.vtxbuf;
        void * data = vb->lock( 0, 0, LOCK_DISCARD );
        if( 0 == data ) return false;
        memcpy( data, inbuf, inbytes );
        vb->unlock();
        return true;
    }

    virtual void freebuf( void * inbuf, size_t )
    {
        safeDeleteArray( inbuf );
    }
};

///
/// coming soon ...
///
class BasicIdxBufLoader : public GraphicsResourceLoader
{
public:
    virtual bool load( const GraphicsResourceDesc &, void * & outbuf, size_t & outbytes, int )
    {
        outbuf = 0;
        outbytes = 0;
        return true;
    }

    virtual bool copy( GraphicsResource & gfxres, const void * inbuf, size_t inbytes, int )
    {
        IdxBuf * ib = gfxres.idxbuf;
        void * data = ib->lock( 0, 0, LOCK_DISCARD );
        if( 0 == data ) return false;
        memcpy( data, inbuf, inbytes );
        ib->unlock();
        return true;
    }

    virtual void freebuf( void * inbuf, size_t )
    {
        safeDeleteArray( inbuf );
    }
};



// *****************************************************************************
//                           End of testCase.h
// *****************************************************************************
#endif // __GN_TEST_ENGINE_TESTCASE_H__

#include "pch.h"
#include "camera.h"

using namespace GN;
using namespace GN::scene;

static GN::Logger * sLogger = GN::getLogger("GN.scene");

// *****************************************************************************
// Camera::Impl public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::scene::Camera::Impl::setViewMatrix( const Matrix44f & m )
{
    mView = m;
}

//
//
// -----------------------------------------------------------------------------
void GN::scene::Camera::Impl::setProjectionMatrix( const Matrix44f & m )
{
    mProj = m;
}

//
//
// -----------------------------------------------------------------------------
void GN::scene::Camera::Impl::setViewport( const Rect<UInt32> & v )
{
    mViewport = v;
}

// *****************************************************************************
// Camera::Impl public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------

// *****************************************************************************
// Camera public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::scene::Camera::Camera()
{
    mImpl = new Impl( *this );
}

//
//
// -----------------------------------------------------------------------------
GN::scene::Camera::~Camera()
{
    delete mImpl;
}

//
//
// -----------------------------------------------------------------------------
void                 GN::scene::Camera::setViewMatrix( const Matrix44f & m ) { return mImpl->setViewMatrix( m ); }
void                 GN::scene::Camera::setProjectionMatrix( const Matrix44f & m ) { return mImpl->setProjectionMatrix( m ); }
void                 GN::scene::Camera::setViewport( const Rect<UInt32> & v ) { return mImpl->setViewport( v ); }
const Matrix44f    & GN::scene::Camera::getViewMatrix() const { return mImpl->getViewMatrix(); }
const Matrix44f    & GN::scene::Camera::getProjectionMatrix() const { return mImpl->getProjectionMatrix(); }
const Rect<UInt32> & GN::scene::Camera::getViewport() const { return mImpl->getViewport(); }

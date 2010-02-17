#include "pch.h"
#include "camera.h"
#include "visualGraph.h"

using namespace GN;
using namespace GN::util;

static GN::Logger * sLogger = GN::GetLogger("GN.util");

// *****************************************************************************
// Camera::Impl public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::util::Camera::Impl::setViewMatrix( const Matrix44f & m )
{
    mView = m;
}

//
//
// -----------------------------------------------------------------------------
void GN::util::Camera::Impl::setProjectionMatrix( const Matrix44f & m )
{
    mProj = m;
}

//
//
// -----------------------------------------------------------------------------
void GN::util::Camera::Impl::setViewport( const Rect<UInt32> & v )
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
GN::util::Camera::Camera()
{
    mImpl = new Impl( *this );
}

//
//
// -----------------------------------------------------------------------------
GN::util::Camera::~Camera()
{
    delete mImpl;
}

//
//
// -----------------------------------------------------------------------------
void                 GN::util::Camera::setViewMatrix( const Matrix44f & m ) { return mImpl->setViewMatrix( m ); }
const Matrix44f    & GN::util::Camera::getViewMatrix() const { return mImpl->getViewMatrix(); }
void                 GN::util::Camera::setProjectionMatrix( const Matrix44f & m ) { return mImpl->setProjectionMatrix( m ); }
const Matrix44f    & GN::util::Camera::getProjectionMatrix() const { return mImpl->getProjectionMatrix(); }
void                 GN::util::Camera::setViewport( const Rect<UInt32> & v ) { return mImpl->setViewport( v ); }
const Rect<UInt32> & GN::util::Camera::getViewport() const { return mImpl->getViewport(); }

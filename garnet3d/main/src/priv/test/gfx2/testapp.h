#ifndef __GN_TESTGFX2_TESTAPP_H__
#define __GN_TESTGFX2_TESTAPP_H__
// *****************************************************************************
/// \file
/// \brief   mini GFX2 test application.
/// \author  chenli@@FAREAST (2007.6.25)
// *****************************************************************************

#include "garnet/GNgfx2.h"

using namespace GN;
using namespace GN::gfx;

///
/// GFX2 test application
///
class Gfx2TestApp
{
public:

    //@{

    virtual ~Gfx2TestApp() {}
    virtual bool init( GraphicsSystem & gs ) = 0;
    virtual void quit( GraphicsSystem & gs ) = 0;
    virtual void draw( GraphicsSystem & gs ) = 0;

    //@}
};

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_TESTGFX2_TESTAPP_H__

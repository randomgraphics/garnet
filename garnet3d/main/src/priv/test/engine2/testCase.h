#ifndef __GN_TEST_ENGINE2_TESTCASE_H__
#define __GN_TEST_ENGINE2_TESTCASE_H__
// *****************************************************************************
/// \file
/// \brief   common test case interface
/// \author  chen@@CHENLI-HOMEPC (2007.5.7)
// *****************************************************************************

#include "garnet/GNengine2.h"

using namespace GN::engine2;

///
/// coming soon ...
///
class TestCase
{
    RenderEngine  & mRenderEngine;

public:

    ///
    /// ctor
    ///
    TestCase( RenderEngine & re ) : mRenderEngine( re ) {}

    ///
    /// get engine reference
    ///
    RenderEngine & renderEngine() const { return mRenderEngine; }

    //@{

    virtual bool init() = 0;
    virtual void quit() = 0;
    virtual void render() = 0;

    //@}
};

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_TEST_ENGINE2_TESTCASE_H__

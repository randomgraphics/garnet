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

class TestCase
{
    RenderEngine & mEngine;

public:

    ///
    /// ctor
    ///
    TestCase( RenderEngine & engine ) : mEngine( engine ) {}

    ///
    /// get engine reference
    ///
    RenderEngine & engine() const { return mEngine; }

    //@{

    virtual bool init() = 0;
    virtual void quit() = 0;
    virtual void draw() = 0;

    //@}
};

// *****************************************************************************
//                           End of testCase.h
// *****************************************************************************
#endif // __GN_TEST_ENGINE_TESTCASE_H__

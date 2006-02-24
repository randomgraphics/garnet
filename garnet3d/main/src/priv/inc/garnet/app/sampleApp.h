#ifndef __GN_APP_SAMPLEAPP_H__
#define __GN_APP_SAMPLEAPP_H__
// *****************************************************************************
//! \file    app/sampleApp.h
//! \brief   Sample application framework
//! \author  chenlee (2006.1.7)
// *****************************************************************************

#include "garnet/GNgfx.h"
#include "garnet/GNinput.h"

namespace GN { namespace app
{
    //!
    //! Sample application framework
    //!
    class SampleApp : public StdClass, public SlotBase
    {
         GN_DECLARE_STDCLASS( SampleApp, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        SampleApp()          { clear(); }
        virtual ~SampleApp() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( int argc, const char * argv[] );
        void quit();
        bool ok() const { return MyParent::ok(); }
    private:
        void clear() {}
        //@}

        // ********************************
        // sample framework
        // ********************************
    public:

        //@{

        virtual int run();

        virtual bool onAppInit() { return true; }
        virtual void onAppQuit() {}
        virtual bool onRendererCreate() { return true; }
        virtual bool onRendererRestore() { return true; }
        virtual void onRendererDispose() {}
        virtual void onRendererDestroy() {}
        virtual void onUpdate() {}
        virtual void onRender() {}
        virtual void onKeyPress( input::KeyEvent );
        virtual void onCharPress( wchar_t ) {}
        virtual void onAxisMove( input::Axis, int ) {}

        void postExistEvent() { mDone = true; }

        bool switchRenderer();

        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        struct InitParam
        {
            gfx::RendererAPI rapi;
            gfx::RendererOptions ro;
            input::InputApi iapi;
        };

        InitParam mInitParam;

        GN::Clock mClock;

        bool mDone;

        // ********************************
        // private functions
        // ********************************
    private:

        bool checkCmdLine( int argc, const char * argv[] );
        bool initRenderer();
        void quitRenderer();
        bool initInput();
        void quitInput();
    };
}}

// *****************************************************************************
//                           End of sampleApp.h
// *****************************************************************************
#endif // __GN_APP_SAMPLEAPP_H__

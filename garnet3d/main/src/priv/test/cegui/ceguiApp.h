#ifndef __GN_TEST_CEGUIAPP_H__
#define __GN_TEST_CEGUIAPP_H__
// *****************************************************************************
/// \file    cegui/ceguiApp.h
/// \brief   CEGUI test application
/// \author  chenlee (2006.2.25)
// *****************************************************************************

using namespace GN;

///
/// CEGUI test application
///
class GuiTest : public GN::app::SampleApp
{
public:

    /// \name from SampleApp
    //@{

    bool onAppInit()
    {
        return true;
    }

    void onAppQuit()
    {
    }

    bool onRendererCreate()
    {
        using namespace CEGUI;

        class MyResourceProvider : public ResourceProvider
        {
        public:
            MyResourceProvider() {}
            ~MyResourceProvider(void) {}
            void loadRawDataContainer(const String& filename, RawDataContainer& output, const String&)
            {
                if( filename.empty() )
                {
                    throw InvalidRequestException((utf8*)
                        "DefaultResourceProvider::load - Filename supplied for data loading must be valid");
                }

                StrA name;
                name.format( "d:/software/green/cegui/datafiles/%s", filename.c_str() );

                DiskFile fp;
                if( !fp.open( name, "rb" ) )
                {
                    throw InvalidRequestException((utf8*)
                        "DefaultResourceProvider::load - " + filename + " does not exist");
                }

                AutoObjPtr<UInt8> buffer( new UInt8[fp.size()] );
                if( !fp.read( buffer, fp.size(), NULL ) )
                {
                    throw GenericException((utf8*)
                        "DefaultResourceProvider::loadRawDataContainer - Problem reading " + filename);
                }

                output.setData( buffer.detach() );
                output.setSize( fp.size() );
            }
            virtual void unloadRawDataContainer(RawDataContainer& data)
            {
                delete [] data.getDataPtr();
                data.setData( 0 );
            }

        };

        new System( new GarnetRenderer, new MyResourceProvider );
        Imageset* taharezImages = ImagesetManager::getSingleton().createImageset("../datafiles/imagesets/TaharezLook.imageset");
        System::getSingleton().setDefaultMouseCursor(&taharezImages->getImage("MouseArrow"));
        FontManager::getSingleton().createFont("../datafiles/fonts/Commonwealth-10.font");
        SchemeManager::getSingleton().loadScheme("../datafiles/schemes/TaharezLookWidgets.scheme");
        WindowManager& winMgr = WindowManager::getSingleton();
        DefaultWindow* root = (DefaultWindow*)winMgr.createWindow("DefaultWindow", "Root");
        System::getSingleton().setGUISheet(root);
        FrameWindow* wnd = (FrameWindow*)winMgr.createWindow("TaharezLook/FrameWindow", "Demo Window");
        root->addChildWindow(wnd);
        wnd->setPosition(Point(0.25f, 0.25f));
        wnd->setSize(Size(0.5f, 0.5f));
        wnd->setMaximumSize(Size(1.0f, 1.0f));
        wnd->setMinimumSize(Size(0.1f, 0.1f));
        wnd->setText("Hello World!");

        // success
        return true;
    }

    void onRendererDestroy()
    {
        CEGUI::System * sys = CEGUI::System::getSingletonPtr();
        if( sys )
        {
            CEGUI::Renderer * r = sys->getRenderer();
            CEGUI::ResourceProvider * rp = sys->getResourceProvider();
            delete sys;
            delete r;
            delete rp;
        }
    }

    void onKeyPress( GN::input::KeyEvent key )
    {
        SampleApp::onKeyPress( key );
        using namespace GN::input;
        if( key.status.down )
        {
            if( KEY_MOUSEBTN_FIRST <= key.code && key.code <= KEY_MOUSEBTN_LAST )
            {
                CEGUI::System::getSingleton().injectMouseButtonDown(
                    (CEGUI::MouseButton)(CEGUI::LeftButton + key.code - KEY_MOUSEBTN_FIRST) );
            }
        }
        else
        {
            if( KEY_MOUSEBTN_FIRST <= key.code && key.code <= KEY_MOUSEBTN_LAST )
            {
                CEGUI::System::getSingleton().injectMouseButtonUp(
                    (CEGUI::MouseButton)(CEGUI::LeftButton + key.code - KEY_MOUSEBTN_FIRST) );
            }
        }
    }

    void onAxisMove( GN::input::Axis, int )
    {
        int x, y;
        gInput.getMousePosition( x, y );
        CEGUI::System::getSingleton().injectMousePosition( (float)x, (float)y );
    }

    void onUpdate()
    {
    }

    void onRender()
    {
        gRenderer.clearScreen();
        //for( int i = 0; i < 30; ++i )
        CEGUI::System::getSingleton().renderGUI();
    }

    //@}
};

// *****************************************************************************
//                           End of ceguiApp.h
// *****************************************************************************
#endif // __GN_TEST_CEGUIAPP_H__

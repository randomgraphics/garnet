#ifndef __GN_TEST_CEGUIAPP_H__
#define __GN_TEST_CEGUIAPP_H__
// *****************************************************************************
/// \file
/// \brief   CEGUI test application
/// \author  chenlee (2006.2.25)
// *****************************************************************************

using namespace GN;

///
/// CEGUI test application
///
class GuiTest : public GN::util::SampleApp
{
public:

    /// \name from SampleApp
    //@{

    bool onInit()
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
                name.Format( "d:/software/green/cegui/datafiles/%s", filename.c_str() );

                DiskFile fp;
                if( !fp.Open( name, "rb" ) )
                {
                    throw InvalidRequestException((utf8*)
                        "DefaultResourceProvider::load - " + filename + " does not exist");
                }

                AutoObjPtr<UInt8> buffer( new UInt8[fp.Size()] );
                if( !fp.Read( buffer, fp.Size(), NULL ) )
                {
                    throw GenericException((utf8*)
                        "DefaultResourceProvider::loadRawDataContainer - Problem reading " + filename);
                }

                output.setData( buffer.Detach() );
                output.setSize( fp.Size() );
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

    void onQuit()
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
            if( KeyCode::MOUSEBTN_FIRST <= key.code && key.code <= KeyCode::LAST_MOUSE_BUTTON )
            {
                CEGUI::System::getSingleton().injectMouseButtonDown(
                    (CEGUI::MouseButton)(CEGUI::LeftButton + key.code - KeyCode::MOUSEBTN_FIRST) );
            }
        }
        else
        {
            if( KeyCode::MOUSEBTN_FIRST <= key.code && key.code <= KeyCode::LAST_MOUSE_BUTTON )
            {
                CEGUI::System::getSingleton().injectMouseButtonUp(
                    (CEGUI::MouseButton)(CEGUI::LeftButton + key.code - KeyCode::MOUSEBTN_FIRST) );
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
//                                     EOF
// *****************************************************************************
#endif // __GN_TEST_CEGUIAPP_H__

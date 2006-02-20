#include "pch.h"
#include "foxAll.h"

#if GN_XENON
int main() { GN_INFO( "FOX TOOLKIT does not support Xenon platform." ); return 0; }
#else
using namespace FX;

int main(int argc, char * argv[] )
{
    FXApp application("Hello","FoxTest");

    application.init(argc,argv);

    FXMainWindow *main=new FXMainWindow(&application,"Hello",NULL,NULL,DECOR_ALL);

    new FXButton(main,L"ÄãºÃ°¡£¡",NULL,&application,FXApp::ID_QUIT);

    application.create();

    main->show(PLACEMENT_SCREEN);

    return application.run();
}
#endif

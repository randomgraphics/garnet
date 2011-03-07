/////////////////////////////////////////////////////////////////////////
//
// This example illustrates how to display the content of a FBX or a OBJ file
// in a graphical window. This program is based on the OpenGL Utility Toolkit
// (GLUT). Start the program on the command line by providing a FBX or a
// OBJ file name. A menu is provided to select the current camera and the current
// animation stack.
//
// Among other things, the example illustrates how to:
// 1)  import a scene from a .FBX or .OBJ file;
// 2)  convert the nurbs and patch attribute types of a scene into mesh
//     node attributes;
// 3)  get the list of all the cameras in the scene;
// 4)  find the current camera;
// 5)  get the relevant settings of a camera depending on it's projection
//     type and aperture mode;
// 6)  compute the local and global positions of a node;
// 7)  compute the orientation of a camera;
// 8)  compute the orientation of a light;
// 9)  compute the shape deformation of mesh vertices;
// 10) compute the link deformation of mesh vertices.
// 11) display the point cache simulation of a mesh.
// 12) get the list of all pose in the scene;
// 13) show the scene using at a specific pose.
//
/////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "InitScene.h"
#include "SetCamera.h"
#include "DrawScene.h"
#include "Texture.h"
#include "Common.h"

void ExitFunction();
void CreateMenus();
void DrawText(KString& pText);
void CameraSelectionCallback(int pItem);
void AnimStackSelectionCallback(int pItem);
void MenuSelectionCallback(int pItem);
void PoseSelectionCallback(int pItem);
void TimerCallback(int);
void DisplayCallback();
void ReshapeCallback(int pWidth, int pHeight);
void KeyboardCallback(unsigned char pKey, int, int);
void MouseCallback(int button, int state, int x, int y);
void MotionCallback(int x, int y);

KString ExtractDirectory(KString pFilePath);

// Global variables.
KFbxSdkManager* gSdkManager;
KFbxImporter* gImporter;
KFbxScene* gScene;
KFbxAnimStack* gCurrentAnimationStack;
KFbxAnimLayer* gCurrentAnimationLayer; // This is the animation layer that is currently active and where
                                       // the animation curves are extracted from
int gSceneStatus;

template<class KString>
class DeletionPolicy
{
public:
    static inline void DeleteIt(KString** ptr)
    {
        if ( *ptr != NULL )
        {
            delete *ptr;
            *ptr = NULL;
        }
    }
};

KScopedPtr<KString, DeletionPolicy<KString> > gWindowMessage;
KScopedPtr<KString, DeletionPolicy<KString> > gFileName;

KArrayTemplate<KFbxNode*>  gCameraArray;
KArrayTemplate<KString*>   gAnimStackNameArray;
KArrayTemplate<KFbxPose*>  gPoseArray;
KArrayTemplate<VSTexture*> gTextureArray;
KTime gPeriod, gStart, gStop, gCurrentTime;
int gPoseIndex;
int gX, gY;
KFbxVector4 gCamPosition, gCamCenter;
double gRoll;
int gCameraStatus;


// Scene status.
enum { UNLOADED, MUST_BE_LOADED, MUST_BE_REFRESHED, REFRESHED };
// Camera status.
enum { CAMERA_NOTHING, CAMERA_ORBIT, CAMERA_ZOOM, CAMERA_PAN};


// Menu item ids.
#define PRODUCER_PERSPECTIVE_ITEM   100
#define PRODUCER_TOP_ITEM           101
#define PRODUCER_BOTTOM_ITEM        102
#define PRODUCER_FRONT_ITEM         103
#define PRODUCER_BACK_ITEM          104
#define PRODUCER_RIGHT_ITEM         105
#define PRODUCER_LEFT_ITEM          106
#define CAMERA_SWITCHER_ITEM        107
#define PLAY_ANIMATION              200
#define EXIT_ITEM                   300

#define SAMPLE_FILENAME "../media/model/humanoid.fbx"

class MyMemoryAllocator : public KFbxMemoryAllocator
{
public:
    MyMemoryAllocator()
        : KFbxMemoryAllocator(MyMalloc, MyCalloc, MyRealloc, MyFree, MyMsize)
    {
    }

    ~MyMemoryAllocator()
    {
    }

    static void* MyMalloc(size_t pSize)
    {
        char *p = (char*)malloc(pSize+1);
        *p = '#';
        return p+1;
    }

    static void* MyCalloc(size_t pCount,size_t pSize)
    {
        char *p = (char*)calloc(pCount, pSize+1);
        *p = '#';
        return p+1;
    }

    static void* MyRealloc(void* pData, size_t pSize)
    {
        if (pData)
        {
//            K_ASSERT(*((char*)pData-1)=='#');
            if (*((char*)pData-1)=='#')
            {
                char *p = (char*)realloc((char*)pData-1, pSize+1);
                *p = '#';
                return p+1;
            }
            else
            {   // Mismatch
                char *p = (char*)realloc((char*)pData, pSize+1);
                *p = '#';
                return p+1;
            }
        }
        else
        {
            char *p = (char*)realloc(NULL, pSize+1);
            *p = '#';
            return p+1;
        }
    }

    static void MyFree(void* pData)
    {
//        K_ASSERT(*((char*)pData-1)=='#');
        if (*((char*)pData-1)=='#')
        {
            free((char*)pData-1);
        }
        else
        {   // Mismatch
            free(pData);
        }
    }

    static size_t MyMsize(void* pData)
    {
//        K_ASSERT(*((char*)pData-1)=='#');
        if (*((char*)pData-1)=='#')
        {
            return _msize((char*)pData-1);
        }
        else
        {   // Mismatch
            return _msize((char*)pData);
        }
    }
};

int main(int argc, char** argv)
{
    // Set exit function to destroy objects created by the FBX SDK.
    atexit(ExitFunction);

#ifdef _DEBUG
    MyMemoryAllocator lMyMemoryAllocator;
    KFbxSdkManager::SetMemoryAllocator(&lMyMemoryAllocator);
#endif

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(720, 486);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("ViewScene");

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);

    glutDisplayFunc(DisplayCallback);
    glutReshapeFunc(ReshapeCallback);
    glutKeyboardFunc(KeyboardCallback);
    glutMouseFunc(MouseCallback);
    glutMotionFunc(MotionCallback);

    // Initialize the scene status flag.
    gSceneStatus = UNLOADED;

    // Create the FBX SDK manager which is the object allocator for almost
    // all the classes in the SDK and create the scene.
    InitializeSdkObjects(gSdkManager, gScene);

	gWindowMessage.Reset(new KString());
	gFileName.Reset(new KString());

	if(argc <= 1)
    {
        *gFileName = SAMPLE_FILENAME;
    }
    else
    {
        *gFileName = argv[1];
    }

    if (gSdkManager)
    {
        // Create the importer.
		int lFileFormat = -1;
        gImporter = KFbxImporter::Create(gSdkManager,"");
        if (!gSdkManager->GetIOPluginRegistry()->DetectReaderFileFormat(*gFileName, lFileFormat) )
        {
            // Unrecognizable file format. Try to fall back to KFbxImporter::eFBX_BINARY
            lFileFormat = gSdkManager->GetIOPluginRegistry()->FindReaderIDByDescription( "FBX binary (*.fbx)" );;
        }

        // Initialize the importer by providing a filename.
        if(gImporter->Initialize(gFileName->Buffer(), lFileFormat) == true)
        {
            // The file is going to be imported at
            // the end of the first display callback.
            *gWindowMessage = "Importing file ";
            *gWindowMessage += *gFileName;
            *gWindowMessage += "\nPlease wait!";

            // Set scene status flag to ready to load.
            gSceneStatus = MUST_BE_LOADED;
        }
        else
        {
            *gWindowMessage = "Unable to open file ";
            *gWindowMessage += *gFileName;
            *gWindowMessage += "\nError reported: ";
            *gWindowMessage += gImporter->GetLastErrorString();
            *gWindowMessage += "\nEsc to exit";
        }
    }
    else
    {
        *gWindowMessage = "Unable to create the FBX SDK manager";
        *gWindowMessage += "\nEsc to exit";
    }

    // When gPoseIndex is not -1, draw the scene at that pose
    gPoseIndex = -1;

    glutMainLoop();

    return 0;
}


// Function to destroy objects created by the FBX SDK.
void ExitFunction()
{
    // Delete the FBX SDK manager. All the objects that have been allocated
    // using the FBX SDK manager and that haven't been explicitly destroyed
    // are automatically destroyed at the same time.
    if (gSdkManager) gSdkManager->Destroy();
    gSdkManager = NULL;

    // Delete the array used to associate menu ids with animation stack names.
    FbxSdkDeleteAndClear(gAnimStackNameArray);

    // Delete any texture allocated
    FbxSdkDeleteAndClear(gTextureArray);
}


// This function is only called in the first display callback
// to make sure that the application window is opened and a
// status message is displayed before.
void ImportFunction()
{
    // Make sure that the scene is ready to load.
    if (gSceneStatus == MUST_BE_LOADED)
    {
        if(gImporter->Import(gScene) == true)
        {
            // Set the scene status flag to refresh
            // the scene in the first timer callback.
            gSceneStatus = MUST_BE_REFRESHED;

            // Convert Axis System to what is used in this example, if needed
            KFbxAxisSystem SceneAxisSystem = gScene->GetGlobalSettings().GetAxisSystem();
            KFbxAxisSystem OurAxisSystem(KFbxAxisSystem::YAxis, KFbxAxisSystem::ParityOdd, KFbxAxisSystem::RightHanded);
            if( SceneAxisSystem != OurAxisSystem )
            {
                OurAxisSystem.ConvertScene(gScene);
            }

            // Convert Unit System to what is used in this example, if needed
            KFbxSystemUnit SceneSystemUnit = gScene->GetGlobalSettings().GetSystemUnit();
            if( SceneSystemUnit.GetScaleFactor() != 1.0 )
            {
                KFbxSystemUnit OurSystemUnit(1.0);
                OurSystemUnit.ConvertScene(gScene);
            }

            // Nurbs and patch attribute types are not supported yet.
            // Convert them into mesh node attributes to have them drawn.
            ConvertNurbsAndPatch(gSdkManager, gScene);

            // Convert any .PC2 point cache data into the .MC format for
            // vertex cache deformer playback.
            PreparePointCacheData(gScene);

            // Get the list of all the cameras in the scene.
            FillCameraArray(gScene, gCameraArray);

            // Get the list of all the animation stack.
            gScene->FillAnimStackNameArray(gAnimStackNameArray);

            // Get the list of pose in the scene
            FillPoseArray(gScene, gPoseArray);

            // Load the texture data in memory (for supported formats)
            LoadSupportedTextures(gScene, gTextureArray);

            // Create the menus to select the current camera and the current animation stack.
            CreateMenus();

            // Initialize the window message.
            *gWindowMessage = "File ";
            *gWindowMessage += *gFileName;
            *gWindowMessage += "\nClick on the right mouse button to enter menu.";
            *gWindowMessage += "\nEsc to exit.";

            // Initialize the frame period.
            gPeriod.SetTime(0, 0, 0, 1, 0, gScene->GetGlobalSettings().GetTimeMode());

            // Call the timer to display the first frame.
            glutTimerFunc((unsigned int) gPeriod.GetMilliSeconds(), TimerCallback, 0);
        }
        else
        {
            // Import failed, set the scene status flag accordingly.
            gSceneStatus = UNLOADED;

            *gWindowMessage = "Unable to import file ";
            *gWindowMessage += *gFileName;
            *gWindowMessage += "\nError reported: ";
            *gWindowMessage += gImporter->GetLastErrorString();
        }

        // Destroy the importer to release the file.
        gImporter->Destroy();
    }
}

KString ExtractDirectory(KString pFilePath)
{
    KString lFilePath = pFilePath;
    while (lFilePath.FindAndReplace("\\","/")) {}
    int lIndex = lFilePath.ReverseFind('/');
    if (lIndex >= 0)
    {
        lFilePath = lFilePath.Left(lIndex+1);
    }
    return lFilePath;
}

// Create the menus to select the current camera and the current animation stack.
void CreateMenus()
{
    int i;

    // Create the submenu to select the current camera.
    int lCameraMenu = glutCreateMenu(CameraSelectionCallback);

    // Add the producer cameras.
    glutAddMenuEntry(PRODUCER_PERSPECTIVE, PRODUCER_PERSPECTIVE_ITEM);
    glutAddMenuEntry(PRODUCER_TOP, PRODUCER_TOP_ITEM);
    glutAddMenuEntry(PRODUCER_BOTTOM, PRODUCER_BOTTOM_ITEM);
    glutAddMenuEntry(PRODUCER_FRONT, PRODUCER_FRONT_ITEM);
    glutAddMenuEntry(PRODUCER_BACK, PRODUCER_BACK_ITEM);
    glutAddMenuEntry(PRODUCER_RIGHT, PRODUCER_RIGHT_ITEM);
    glutAddMenuEntry(PRODUCER_LEFT, PRODUCER_LEFT_ITEM);

    // Add the camera switcher if there is at least one camera in the scene.
    if (gCameraArray.GetCount() > 0)
    {
        glutAddMenuEntry(CAMERA_SWITCHER, CAMERA_SWITCHER_ITEM);
    }

    // Add the cameras contained in the scene.
    for (i = 0; i < gCameraArray.GetCount(); i++)
    {
        glutAddMenuEntry(gCameraArray[i]->GetName(), i);
    }

    // Create the submenu to select the current animation stack.
    int lAnimStackMenu = glutCreateMenu(AnimStackSelectionCallback);
    int lCurrentAnimStackIndex = 0;

    // Add the animation stack names.
    for (i = 0; i < gAnimStackNameArray.GetCount(); i++)
    {
        glutAddMenuEntry(gAnimStackNameArray[i]->Buffer(), i);

        // Track the current animation stack index.
        if (gAnimStackNameArray[i]->Compare(KFbxGet<KString>(gScene->ActiveAnimStackName)) == 0)
        {
            lCurrentAnimStackIndex = i;
        }
    }

    // Call the animation stack selection callback immediately to
    // initialize the start, stop and current time.
    AnimStackSelectionCallback(lCurrentAnimStackIndex);

    int lBindPoseCount = 0;
    int lRestPoseCount = 0;
    // Create a submenu for bind poses
    int lBindPoseMenu = glutCreateMenu(PoseSelectionCallback);

    // Add the list of bind poses
    for (i = 0; i < gPoseArray.GetCount(); i++)
    {
        if (gPoseArray[i]->IsBindPose())
        {
            glutAddMenuEntry(gPoseArray[i]->GetName(), i);
            lBindPoseCount++;
        }
    }

    // Create a submenu for rest poses
    int lRestPoseMenu = glutCreateMenu(PoseSelectionCallback);

    // Add the list of bind poses
    for (i = 0; i < gPoseArray.GetCount(); i++)
    {
        if (gPoseArray[i]->IsRestPose())
        {
            glutAddMenuEntry(gPoseArray[i]->GetName(), i);
            lRestPoseCount++;
        }
    }

    // Create the submenu to go to a specific pose
    int lPoseMenu = 0;
    if (lBindPoseCount>0 || lRestPoseCount>0)
    {
        lPoseMenu = glutCreateMenu(PoseSelectionCallback);
        if (lBindPoseCount>0)
            glutAddSubMenu("Bind Pose", lBindPoseMenu);
        if (lRestPoseCount>0)
            glutAddSubMenu("Rest Pose", lRestPoseMenu);
    }

    // Build the main menu.
    glutCreateMenu(MenuSelectionCallback);
    glutAddSubMenu("Select Camera", lCameraMenu);
    glutAddSubMenu("Select Animation Stack", lAnimStackMenu);
    if (lBindPoseCount>0 || lRestPoseCount>0)
        glutAddSubMenu("Go to Pose", lPoseMenu);
    glutAddMenuEntry("Play", PLAY_ANIMATION);
    glutAddMenuEntry("Exit", EXIT_ITEM);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}


// Draw text in the upper left area of the window.
void DrawText(KString& pText)
{
    glColor3f(1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    float lX = 5;
    float lY = glutGet(GLUT_WINDOW_HEIGHT) - 20;

    glRasterPos2f(lX, lY);

    int i, lCount =(int) pText.GetLen();

    for (i = 0; i < lCount; i++)
    {
        char lC = pText.Buffer()[i];

        if (lC == '\n')
        {
            lY -= 20;
            glRasterPos2f(lX, lY);
        }
        else
        {
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, lC);
        }
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}


// Select the current camera.
void CameraSelectionCallback(int pItem)
{
    KFbxGlobalSettings& lGlobalCameraSettings = gScene->GetGlobalSettings();

    if (pItem == PRODUCER_PERSPECTIVE_ITEM)
    {
        lGlobalCameraSettings.SetDefaultCamera(PRODUCER_PERSPECTIVE);
    }
    else if (pItem == PRODUCER_TOP_ITEM)
    {
        lGlobalCameraSettings.SetDefaultCamera(PRODUCER_TOP);
    }
    else if (pItem == PRODUCER_BOTTOM_ITEM)
    {
        lGlobalCameraSettings.SetDefaultCamera(PRODUCER_BOTTOM);
    }
    else if (pItem == PRODUCER_FRONT_ITEM)
    {
        lGlobalCameraSettings.SetDefaultCamera(PRODUCER_FRONT);
    }
    else if (pItem == PRODUCER_BACK_ITEM)
    {
        lGlobalCameraSettings.SetDefaultCamera(PRODUCER_BACK);
    }
    else if (pItem == PRODUCER_RIGHT_ITEM)
    {
        lGlobalCameraSettings.SetDefaultCamera(PRODUCER_RIGHT);
    }
    else if (pItem == PRODUCER_LEFT_ITEM)
    {
        lGlobalCameraSettings.SetDefaultCamera(PRODUCER_LEFT);
    }
    else if (pItem == CAMERA_SWITCHER_ITEM)
    {
        lGlobalCameraSettings.SetDefaultCamera(CAMERA_SWITCHER);
    }
    else if (pItem >= 0 && pItem < gCameraArray.GetCount())
    {
        lGlobalCameraSettings.SetDefaultCamera((char *)gCameraArray[pItem]->GetName());
    }

    // Set the scene status flag to refresh
    // the scene in the next timer callback.
    gSceneStatus = MUST_BE_REFRESHED;
}


// Select the current animation stack and set the start, stop and current time.
void AnimStackSelectionCallback( int pItem )
{
	int lNbAnimStacks = gAnimStackNameArray.GetCount();
	if (!lNbAnimStacks || pItem >= lNbAnimStacks)
	{
		return;
	}

	// select the base layer from the animation stack
    gCurrentAnimationStack = gScene->FindMember(FBX_TYPE(KFbxAnimStack), gAnimStackNameArray[pItem]->Buffer());
    if (gCurrentAnimationStack == NULL)
    {
        // this is a problem. The anim stack should be found in the scene!
        return;
    }

    // we assume that the first animation layer connected to the animation stack is the base layer
    // (this is the assumption made in the FBXSDK)
    gCurrentAnimationLayer = gCurrentAnimationStack->GetMember(FBX_TYPE(KFbxAnimLayer), 0);
    gScene->GetEvaluator()->SetContext(gCurrentAnimationStack);

    KFbxTakeInfo* lCurrentTakeInfo = gScene->GetTakeInfo(*(gAnimStackNameArray[pItem]));
    if (lCurrentTakeInfo)
    {
        gStart = lCurrentTakeInfo->mLocalTimeSpan.GetStart();
        gStop = lCurrentTakeInfo->mLocalTimeSpan.GetStop();
    }
    else
    {
        // Take the time line value
        KTimeSpan lTimeLineTimeSpan;
        gScene->GetGlobalSettings().GetTimelineDefaultTimeSpan(lTimeLineTimeSpan);

        gStart = lTimeLineTimeSpan.GetStart();
        gStop  = lTimeLineTimeSpan.GetStop();
    }

    gCurrentTime = gStart;

    // Set the scene status flag to refresh
    // the scene in the next timer callback.
    gSceneStatus = MUST_BE_REFRESHED;
}

void PoseSelectionCallback(int pItem)
{
    gPoseIndex = pItem;
    gSceneStatus = MUST_BE_REFRESHED;
}

// Exit the application from the main menu.
void MenuSelectionCallback(int pItem)
{
    if (pItem == PLAY_ANIMATION)
    {
        gPoseIndex = -1;
    }
    else if (pItem == EXIT_ITEM)
    {
        exit(0);
    }

    gSceneStatus = MUST_BE_REFRESHED;
}


// Trigger the display of the current frame.
void TimerCallback(int)
{
    // Ask to display the current frame only if necessary.
    if (gSceneStatus == MUST_BE_REFRESHED)
    {
        glutPostRedisplay();
    }

    // Loop in the animation stack.
    if (gStop > gStart)
    {
        // Set the scene status flag to refresh
        // the scene in the next timer callback.
        gSceneStatus = MUST_BE_REFRESHED;

        gCurrentTime += gPeriod;

        if (gCurrentTime > gStop)
        {
            gCurrentTime = gStart;
        }
    }
    // Avoid displaying the same frame on
    // and on if the animation stack has no length.
    else
    {
        // Set the scene status flag to avoid refreshing
        // the scene in the next timer callback.
        gSceneStatus = REFRESHED;
    }

    // Call the timer to display the next frame.
    glutTimerFunc((unsigned int) gPeriod.GetMilliSeconds(), TimerCallback, 0);
}


// Refresh the appplication window.
void DisplayCallback()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Test if the scene has been loaded yet.
    if (gSceneStatus != UNLOADED && gSceneStatus != MUST_BE_LOADED)
    {
        glEnable(GL_DEPTH_TEST);

        // Set the view to the current camera settings.
        SetCamera(gScene, gCurrentTime, gCurrentAnimationLayer, gCameraArray);

        if (gPoseIndex == -1)
        {
            // Draw the scene at the current time.
            DrawScene(gScene, gCurrentTime);
        }
        else
        {
            // Draw the scene in the specified pose
            DrawSceneAtPose(gScene, gCurrentTime, gPoseIndex);
        }
    }

    glDisable(GL_DEPTH_TEST);

    // Draw the window message.
    DrawText(*gWindowMessage);

    glFlush();
    glutSwapBuffers();

    // Import the scene if it's ready to load.
    if (gSceneStatus == MUST_BE_LOADED)
    {
        // This function is only called in the first display callback
        // to make sure that the application window is opened and a
        // status message is displayed before.
        ImportFunction();
    }
}


// Resize the application window.
void ReshapeCallback(int pWidth, int pHeight)
{
    glViewport(0, 0, (GLsizei) pWidth, (GLsizei) pHeight);
}


// Exit the application from the keyboard.
void KeyboardCallback(unsigned char pKey, int, int)
{
    // Exit on ESC key.
    if (pKey == 27)
    {
        exit(0);
    }

    // Zoom In on '+' or '=' keypad keys
    if (pKey == 43 || pKey == 61)
    {
        KFbxCamera* lCamera = GetCurrentCamera(gScene);
        if(lCamera)
        {
            //double lOriginalAperture = sqrt(lCamera->GetApertureWidth());
            CameraZoomIn(gScene, 10);
            gSceneStatus = MUST_BE_REFRESHED;
        }
    }

    // Zoom Out on '-' or '_' keypad keys
    if (pKey == 45 || pKey == 95)
    {
        KFbxCamera* lCamera = GetCurrentCamera(gScene);
        if(lCamera)
        {
            //double lOriginalAperture = sqrt(lCamera->GetApertureWidth());
            CameraZoomOut(gScene, 10);
            gSceneStatus = MUST_BE_REFRESHED;
        }
    }
}

void MouseCallback(int button, int state, int x, int y) {
    // Move the camera (orbit, zoom or pan) with the mouse.

    KFbxCamera* lCamera = GetCurrentCamera(gScene);


    if (lCamera)
    {
        gCamPosition = lCamera->Position.Get();
        gCamCenter = lCamera->InterestPosition.Get();
        gRoll = lCamera->Roll.Get();
    }
    gX = x;
    gY = y;

    switch (button)
    {
    case GLUT_LEFT_BUTTON:
        // ORBIT (or PAN)
        switch (state) {
    case GLUT_DOWN:
        if (gCameraStatus == CAMERA_ZOOM) {
            gCameraStatus = CAMERA_PAN;
        } else {
            gCameraStatus = CAMERA_ORBIT;
        }
        break;
    default:
        if (gCameraStatus == CAMERA_PAN) {
            gCameraStatus = CAMERA_ZOOM;
        } else {
            gCameraStatus = CAMERA_NOTHING;
        }
        break;
        }
        break;
    case GLUT_MIDDLE_BUTTON:
        // ZOOM (or PAN)
        switch (state) {
    case GLUT_DOWN:
        if (gCameraStatus == CAMERA_ORBIT) {
            gCameraStatus = CAMERA_PAN;
        } else {
            gCameraStatus = CAMERA_ZOOM;
        }
        break;
    default:
        if (gCameraStatus == CAMERA_PAN) {
            gCameraStatus = CAMERA_ORBIT;
        } else {
            gCameraStatus = CAMERA_NOTHING;
        }
        break;
        }
        break;
    }

}


void MotionCallback(int x, int y) {

    int motion;

    switch (gCameraStatus) {
    case CAMERA_ORBIT:
        CameraOrbit(gScene, gCamPosition, gRoll, x-gX, gY-y);
        gSceneStatus = MUST_BE_REFRESHED;
        break;
    case CAMERA_ZOOM:
        motion = gY-y;
        if (motion > 0) {
            CameraZoomIn(gScene, motion);
        } else {
            CameraZoomOut(gScene, -motion);
        }
        gY = y;
        gSceneStatus = MUST_BE_REFRESHED;
        break;
    case CAMERA_PAN:
        CameraPan(gScene, gCamPosition, gCamCenter, gRoll, x-gX, gY-y);
        gSceneStatus = MUST_BE_REFRESHED;
        break;
    }

}


#ifdef WIN32

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine,
                     int nCmdShow)
{
    return main(__argc,__argv);
}

#endif





﻿#include "pch.h"

using namespace GN;
using namespace GN::gfx;

#if GN_MSVC
    #pragma warning(disable : 4309)
    #pragma warning(disable : 4018)
    #pragma warning(disable : 4100)
    #pragma warning(disable : 4267)
    #pragma warning(disable : 4505)
    #include <windows.h>
#endif

#include <stdio.h>
#include <GL/glut.h>

#define MAX_NO_TEXTURES 1

#define CUBE_TEXTURE 0

GLuint texture_id[MAX_NO_TEXTURES];

struct xCharTexture {
    GLuint  m_texID;
    wchar_t m_chaID;
    int     m_Width;
    int     m_Height;

    int m_adv_x;
    int m_adv_y;
    int m_delta_x;
    int m_delta_y;

public:
    xCharTexture() {
        m_texID  = 0;
        m_chaID  = 0;
        m_Width  = 0;
        m_Height = 0;
    }
} g_TexID[65536];

class xFreeTypeLib {
    AutoRef<FontFace> mFace;

public:
    xFreeTypeLib() {}

    void load(const char * font_file1, const char * font_file2, int _w, int _h) {
        //加载一个字体,取默认的Face,一般为Regualer
        FontFaceCreationDesc defaultFont;
        defaultFont.fontname = font_file1;
        defaultFont.width    = (uint16_t) _w;
        defaultFont.height   = (uint16_t) _h;
        defaultFont.quality  = FontFaceDesc::ANTIALIASED;

        MixedFontCreationDesc asciiFont;
        asciiFont.font.fontname = font_file2;
        asciiFont.font.width    = (uint16_t) _w;
        asciiFont.font.height   = (uint16_t) _h;
        asciiFont.font.quality  = FontFaceDesc::ANTIALIASED;
        asciiFont.firstChar     = 0;
        asciiFont.numChars      = 127;

        // mFace.attach( createFontFace( defaultFont ) );
        mFace.attach(createMixedFontFace(defaultFont, &asciiFont, 1));
        if (!mFace) exit(-1);
    }

    GLuint loadChar(wchar_t ch) {
        if (g_TexID[ch].m_texID) return g_TexID[ch].m_texID;

        xCharTexture & charTex = g_TexID[ch];

        //取道位图数据
        FontImage fbm;
        if (!mFace->loadFontImage(fbm, ch)) exit(-1);

        // imdebug( "rgba b=8 w=%d h=%d %p", bitmap.width, bitmap.rows, bitmap.buffer );

        //把位图数据拷贝自己定义的数据区里.这样旧可以画到需要的东西上面了。
        int width  = fbm.width;
        int height = fbm.height;

        charTex.m_Width   = width;
        charTex.m_Height  = height;
        charTex.m_adv_x   = fbm.horiAdvance;
        charTex.m_adv_y   = fbm.vertAdvance;
        charTex.m_delta_x = fbm.horiBearingX;
        charTex.m_delta_y = fbm.horiBearingY;
        glGenTextures(1, &charTex.m_texID);
        glBindTexture(GL_TEXTURE_2D, charTex.m_texID);
        char * pBuf = new char[width * height * 4];
        if (fbm.format == FontImage::RGBA) {
            for (int j = 0; j < height; j++) {
                for (int i = 0; i < width; i++) {
                    pBuf[(4 * i + (height - j - 1) * width * 4)]     = fbm.buffer[(i + j * width) * 4 + 0];
                    pBuf[(4 * i + (height - j - 1) * width * 4) + 1] = fbm.buffer[(i + j * width) * 4 + 1];
                    pBuf[(4 * i + (height - j - 1) * width * 4) + 2] = fbm.buffer[(i + j * width) * 4 + 2];
                    pBuf[(4 * i + (height - j - 1) * width * 4) + 3] = fbm.buffer[(i + j * width) * 4 + 3];
                }
            }
        } else {
            for (int j = 0; j < height; j++) {
                for (int i = 0; i < width; i++) {
                    pBuf[(4 * i + (height - j - 1) * width * 4)]     = 0xff;
                    pBuf[(4 * i + (height - j - 1) * width * 4) + 1] = 0xff;
                    pBuf[(4 * i + (height - j - 1) * width * 4) + 2] = 0xff;
                    pBuf[(4 * i + (height - j - 1) * width * 4) + 3] = fbm.buffer[i + j * width];
                }
            }
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pBuf);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        delete[] pBuf;
        return charTex.m_chaID;
    }

} g_FreeTypeLib;

float ratio;

xCharTexture * getTextChar(wchar_t ch) {
    g_FreeTypeLib.loadChar(ch);
    return &g_TexID[ch];
}

GN::StrW        g_text(L"文件格式：\n"
                       L"若不明确就标为未知\n"
                       L"表演者：	若不明确就标为未知\n"
                       L"专辑：		若不明确就标为未知\n"
                       L"持续时间：01:01:00超过1小时；\n"
                       L"09:09不足小时；00:09不足1分钟\n"
                       L"   glBindTexture(GL_TEXTURE_2D,pCharTex->m_texID);\n"
                       L"   glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );\n"
                       L"   glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );\n"
                       L"   glEnable(GL_BLEND);\n"
                       L"   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);");
const wchar_t * g_UnicodeString = g_text.data();

void drawText(const wchar_t * _strText, int x, int y, int maxW, int h) {
    int sx   = x;
    int sy   = y;
    int maxH = h;
    for (int i = 0; i < (int) wcslen(_strText); i++) {

        if (_strText[i] == '\n') {
            sx = x;
            sy += maxH + 12;
        } else if (_strText[i] == '\t') {
            xCharTexture * space = getTextChar(' ');
            sx += space->m_adv_x * 4;
        } else {
            xCharTexture * pCharTex = getTextChar(_strText[i]);
            glBindTexture(GL_TEXTURE_2D, pCharTex->m_texID);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            // glDisable(GL_TEXTURE_2D);
            int w = pCharTex->m_Width;
            int h = pCharTex->m_Height;

            int ch_x = sx + pCharTex->m_delta_x;
            int ch_y = sy + pCharTex->m_delta_y;

            if (maxH < h) maxH = h;
            glBegin(GL_QUADS);
            {
                glTexCoord2f(0.0f, 1.0f);
                glVertex3f(ch_x, ch_y, 1.0f);
                glTexCoord2f(1.0f, 1.0f);
                glVertex3f(ch_x + w, ch_y, 1.0f);
                glTexCoord2f(1.0f, 0.0f);
                glVertex3f(ch_x + w, ch_y + h, 1.0f);
                glTexCoord2f(0.0f, 0.0f);
                glVertex3f(ch_x, ch_y + h, 1.0f);
            }
            glEnd();
            sx += pCharTex->m_adv_x;
        }

        if (sx > x + maxW) {
            sx = x;
            sy += maxH + 12;
        }
    }
}

static const char * font_face   = "font::/simsun.ttc";
static const char * ascii_font  = "font::/lsans.ttf";
static int          font_width  = 16;
static int          font_height = 16;
void                init(void) {
                   glShadeModel(GL_SMOOTH);              // Enable Smooth Shading
                   glClearColor(0.0f, 0.0f, 0.0f, 0.5f); // Black Background
                   glEnable(GL_COLOR_MATERIAL);
                   glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

                   printf("load font %s %dx%d\n", font_face, font_width, font_height);
                   g_FreeTypeLib.load(font_face, ascii_font, font_width, font_height);

                   glDisable(GL_CULL_FACE);

                   // glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void reshape(int w, int h) {
    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if (h == 0) h = 1;

    ratio = 1.0f * w / h;
    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);
    glOrtho(0, w, h, 0, -100, 200);
    // Set the clipping volume
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 30, 0, 0, 10, 0.0f, 1.0f, 0.0f);
}

void display(void) {
    glClearColor(0.0f, 0.0f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glLoadIdentity ( );
    // glPushMatrix();
    // glTranslatef ( 0.0, 0.0, -5.0 );
    glEnable(GL_TEXTURE_2D);
    drawText(g_UnicodeString, 50, 50, 900, 25);
    // glPopMatrix();
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) // Create Keyboard Function
{
    switch (key) {
    case 27:     // When Escape Is Pressed...
        exit(0); // Exit The Program
        break;   // Ready For Next Case
    default:     // Now Wrap It Up
        break;
    }
}

void arrow_keys(int a_keys, int x, int y) // Create Special Function (required for arrow keys)
{
    switch (a_keys) {
    case GLUT_KEY_UP:     // When Up Arrow Is Pressed...
        glutFullScreen(); // Go Into Full Screen Mode
        break;
    case GLUT_KEY_DOWN:              // When Down Arrow Is Pressed...
        glutReshapeWindow(500, 500); // Go Into A 500 By 500 Window
        break;
    default:
        break;
    }
}

int main(int argc, char ** argv) // Create Main Function For Bringing It All Together
{
    enableCRTMemoryCheck();
    if (argc > 4) {
        font_face   = argv[1];
        ascii_font  = argv[2];
        font_width  = GN::str::toInetger(argv[3], font_width);
        font_height = GN::str::toInetger(argv[4], font_height);
    }

    glutInit(&argc, argv);                                     // Erm Just Write It =)
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA); // Display Mode
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(640, 480);                        // If glutFullScreen wasn't called this is the window size
    glutCreateWindow("NeHe Lesson 6- Ported by Rustad"); // Window Title (argv[0] for current directory as title)
    init();
    // glutFullScreen      ( );          // Put Into Full Screen
    glutDisplayFunc(display); // Matching Earlier Functions To Their Counterparts
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(arrow_keys);
    glutIdleFunc(display);
    glutMainLoop(); // Initialize The Main Loop
    return 1;
}

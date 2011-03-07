/////////////////////////////////////////////////////////////////////////
//
// This file contains the functions to encapsulate the GLUT and OpenGL
// functions calls needed to set the camera and draw the various elements
// of a scene.
//
/////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "GlFunctions.h"
#include "Texture.h"

// On Irix GL_BGR_EXT does not exit it has to be GL_BGR
#ifndef GL_BGR_EXT
#define GL_BGR_EXT GL_BGR
#endif

extern KArrayTemplate<VSTexture*> gTextureArray;

void GlGetWindowSize(int& pWidth, int& pHeight)
{
    pWidth = glutGet(GLUT_WINDOW_WIDTH);
    pHeight = glutGet(GLUT_WINDOW_HEIGHT);
}


void GlSetCameraPerspective(double pFieldOfViewY,
                            double pAspect,
                            double pNearPlane,
                            double pFarPlane,
                            KFbxVector4& pEye,
                            KFbxVector4& pCenter,
                            KFbxVector4& pUp)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(pFieldOfViewY,
        pAspect,
        pNearPlane,
        pFarPlane);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(pEye[0], pEye[1], pEye[2],
        pCenter[0], pCenter[1], pCenter[2],
        pUp[0], pUp[1], pUp[2]);
}


void GlSetCameraOrthogonal(double pLeftPlane,
                           double pRightPlane,
                           double pBottomPlane,
                           double pTopPlane,
                           double pNearPlane,
                           double pFarPlane,
                           KFbxVector4& pEye,
                           KFbxVector4& pCenter,
                           KFbxVector4& pUp)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(pLeftPlane,
        pRightPlane,
        pBottomPlane,
        pTopPlane,
        pNearPlane,
        pFarPlane);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(pEye[0], pEye[1], pEye[2],
        pCenter[0], pCenter[1], pCenter[2],
        pUp[0], pUp[1], pUp[2]);
}


void GlDrawMarker(KFbxXMatrix& pGlobalPosition)
{
    glColor3f(0.0, 1.0, 1.0);
    glLineWidth(1.0);

    glPushMatrix();
    glMultMatrixd((double*) pGlobalPosition);

    glutWireCube(2.0);

    glPopMatrix();
}


void GlDrawLimbNode(KFbxXMatrix& pGlobalBasePosition, KFbxXMatrix& pGlobalEndPosition)
{
    glColor3f(1.0, 0.0, 0.0);
    glLineWidth(2.0);

    glBegin(GL_LINES);

    glVertex3dv((GLdouble *)pGlobalBasePosition.GetT());
    glVertex3dv((GLdouble *)pGlobalEndPosition.GetT());

    glEnd();
}


void GlDrawMesh(KFbxXMatrix& pGlobalPosition, KFbxMesh* pMesh, KFbxVector4* pVertexArray, int pDrawMode)
{
    int                            lDrawMode    = (pDrawMode == DRAW_MODE_TEXTURED && pMesh->GetTextureUVCount() == 0 && pMesh->GetLayer(0)) ? DRAW_MODE_WIREFRAME : pDrawMode;

    KFbxLayerElementArrayTemplate<KFbxVector2>* lUVArray = NULL;
    pMesh->GetTextureUV(&lUVArray, KFbxLayerElement::eDIFFUSE_TEXTURES);

    KFbxLayerElement::EMappingMode lMappingMode = KFbxLayerElement::eNONE;
    VSTexture*                     lTexture     = NULL;

    if(pMesh->GetLayer(0) && pMesh->GetLayer(0)->GetUVs())
        lMappingMode = pMesh->GetLayer(0)->GetUVs()->GetMappingMode();


    // Find the texture data
    if (lDrawMode == DRAW_MODE_TEXTURED)
    {
        KFbxTexture* lCurrentTexture           = NULL;
        KFbxSurfaceMaterial* lSurfaceMaterial= KFbxCast <KFbxSurfaceMaterial>(pMesh->GetNode()->GetSrcObject(KFbxSurfaceMaterial::ClassId, 0));

        if(lSurfaceMaterial)
        {
            KFbxProperty lProperty;
            lProperty = lSurfaceMaterial->FindProperty(KFbxSurfaceMaterial::sDiffuse);
            if(lProperty.IsValid())
            {
                lCurrentTexture = KFbxCast <KFbxTexture>(lProperty.GetSrcObject(KFbxTexture::ClassId, 0));

                int i, lCount = gTextureArray.GetCount();
                for (i=0; i<lCount; i++)
                {
                    if (gTextureArray[i]->mRefTexture == lCurrentTexture)
                    {
                        lTexture = gTextureArray[i];
                        break;
                    }
                }
            }
        }
    }

    lDrawMode = (lDrawMode == DRAW_MODE_TEXTURED && lTexture) ? lDrawMode : DRAW_MODE_WIREFRAME;

    int lGLPrimitive = lDrawMode == DRAW_MODE_WIREFRAME ? GL_LINE_LOOP : GL_POLYGON;

    glColor3f(0.5, 0.5, 0.5);
    glLineWidth(1.0);

    glPushMatrix();
    glMultMatrixd((double*) pGlobalPosition);

    int lPolygonIndex;
    int lPolygonCount = pMesh->GetPolygonCount();

    if (lDrawMode == DRAW_MODE_TEXTURED)
    {
        glEnable(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP );
        glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP );
        glTexEnvi( GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, GL_REPLACE);

        glTexImage2D(GL_TEXTURE_2D, 0,  3, lTexture->mW, lTexture->mH, 0,  GL_BGR_EXT,  GL_UNSIGNED_BYTE,  lTexture->mImageData);
    }

    for (lPolygonIndex = 0; lPolygonIndex < lPolygonCount; lPolygonIndex++)
    {
        int lVerticeIndex;
        int lVerticeCount = pMesh->GetPolygonSize(lPolygonIndex);

        glBegin(lGLPrimitive);

        for (lVerticeIndex = 0; lVerticeIndex < lVerticeCount; lVerticeIndex++)
        {
            if (lDrawMode == DRAW_MODE_TEXTURED)
            {
                int lCurrentUVIndex;

                if (lMappingMode == KFbxLayerElement::eBY_POLYGON_VERTEX)
                {
                    lCurrentUVIndex = pMesh->GetTextureUVIndex(lPolygonIndex, lVerticeIndex);
                }
                else // KFbxLayerElement::eBY_CONTROL_POINT
                {
                    lCurrentUVIndex = pMesh->GetPolygonVertex(lPolygonIndex, lVerticeIndex);
                }
                if(lUVArray)
                    glTexCoord2dv(lUVArray->GetAt(lCurrentUVIndex).mData);
            }

            glVertex3dv((GLdouble *)pVertexArray[pMesh->GetPolygonVertex(lPolygonIndex, lVerticeIndex)]);
        }

        glEnd();
    }

    if (lDrawMode == DRAW_MODE_TEXTURED)
    {
        glDisable(GL_TEXTURE_2D);
    }

    glPopMatrix();
}


void GlDrawCamera(KFbxXMatrix& pGlobalPosition, double pRoll)
{
    glColor3d(1.0, 1.0, 1.0);
    glLineWidth(1.0);

    glPushMatrix();
    glMultMatrixd((double*) pGlobalPosition);
    glRotated(pRoll, 1.0, 0.0, 0.0);

    int i;
    float lCamera[10][2] = {{ 0, 5.5 }, { -3, 4.5 },
    { -3, 7.5 }, { -6, 10.5 }, { -23, 10.5 },
    { -23, -4.5 }, { -20, -7.5 }, { -3, -7.5 },
    { -3, -4.5 }, { 0, -5.5 }   };

    glBegin( GL_LINE_LOOP );
    {
        for (i = 0; i < 10; i++)
        {
            glVertex3f(lCamera[i][0], lCamera[i][1], 4.5);
        }
    }
    glEnd();

    glBegin( GL_LINE_LOOP );
    {
        for (i = 0; i < 10; i++)
        {
            glVertex3f(lCamera[i][0], lCamera[i][1], -4.5);
        }
    }
    glEnd();

    for (i = 0; i < 10; i++)
    {
        glBegin( GL_LINES );
        {
            glVertex3f(lCamera[i][0], lCamera[i][1], -4.5);
            glVertex3f(lCamera[i][0], lCamera[i][1], 4.5);
        }
        glEnd();
    }

    glPopMatrix();
}


void GlDrawLight(KFbxXMatrix& pGlobalPosition,
                 KFbxLight* pLight,
                 KFbxColor& pColor,
                 double pConeAngle)
{
    glColor3d(pColor.mRed, pColor.mGreen, pColor.mBlue);
    glLineWidth(1.0);

    glPushMatrix();
    glMultMatrixd((double*) pGlobalPosition);

    if (pLight->LightType.Get() == KFbxLight::eSPOT)
    {
        double lRadians = 3.1416 * pConeAngle / 180.0;
        double lHeight = 15.0;
        double lBase = lHeight * tan(lRadians / 2);
        glutWireCone(lBase, lHeight, 18, 1);
    }
    else
    {
        glutWireSphere(10.0, 10, 10);
    }

    glPopMatrix();
}


void GlDrawCrossHair(KFbxXMatrix& pGlobalPosition)
{
    glColor3f(1.0, 1.0, 1.0);
    glLineWidth(1.0);

    glPushMatrix();
    glMultMatrixd((double*) pGlobalPosition);

    double lCrossHair[6][3] = { { -3, 0, 0 }, { 3, 0, 0 },
    { 0, -3, 0 }, { 0, 3, 0 },
    { 0, 0, -3 }, { 0, 0, 3 } };

    glBegin(GL_LINES);

    glVertex3dv(lCrossHair[0]);
    glVertex3dv(lCrossHair[1]);

    glEnd();

    glBegin(GL_LINES);

    glVertex3dv(lCrossHair[2]);
    glVertex3dv(lCrossHair[3]);

    glEnd();

    glBegin(GL_LINES);

    glVertex3dv(lCrossHair[4]);
    glVertex3dv(lCrossHair[5]);

    glEnd();

    glPopMatrix();
}

void GlDrawGrid(KFbxXMatrix& pGlobalPosition)
{
    glPushMatrix();
    glMultMatrixd((double*) pGlobalPosition);

    // Draw a grid 500*500
    glColor3f(0.3, 0.3, 0.3);
    glLineWidth(1.0);
    const int hw = 500;
    const int step = 20;
    const int bigstep = 100;
    int       i;

    // Draw Grid
    for (i = -hw; i <= hw; i+=step) {

        if (i % bigstep == 0) {
            glLineWidth(2.0);
        } else {
            glLineWidth(1.0);
        }
        glBegin(GL_LINES);
        glVertex3f(i,0,-hw);
        glVertex3f(i,0,hw);
        glEnd();
        glBegin(GL_LINES);
        glVertex3f(-hw,0,i);
        glVertex3f(hw,0,i);
        glEnd();

    }

    // Write some grid info
    const int zoffset = -2;
    const int xoffset = 1;
    for (i = -hw; i <= hw; i+=bigstep) {

        KString scoord;
        int is, lCount;

        // Don't display origin
        //if (i == 0) continue;
        if (i == 0) {
            scoord = "0";
            lCount = (int)scoord.GetLen();
            glPushMatrix();
            glTranslatef(i+xoffset,0,zoffset);
            glRotatef(-90,1,0,0);
            glScalef(.03,.03,.03);
            for (is = 0; is < lCount; is++) {
                char lC = scoord.Buffer()[is];
                glutStrokeCharacter(GLUT_STROKE_ROMAN, lC);
            }
            glPopMatrix();

            continue;
        }

        // X coordinates
        scoord = "X: ";
        scoord += i;
        lCount = (int)scoord.GetLen();

        glPushMatrix();
        glTranslatef(i+xoffset,0,zoffset);
        glRotatef(-90,1,0,0);
        glScalef(.03,.03,.03);
        for (is = 0; is < lCount; is++) {
            char lC = scoord.Buffer()[is];
            glutStrokeCharacter(GLUT_STROKE_ROMAN, lC);
        }
        glPopMatrix();

        // Z coordinates
        scoord = "Z: ";
        scoord += i;
        lCount = (int)scoord.GetLen();

        glPushMatrix();
        glTranslatef(xoffset,0,i+zoffset);
        glRotatef(-90,1,0,0);
        glScalef(.03,.03,.03);
        for (is = 0; is < lCount; is++) {
            char lC = scoord.Buffer()[is];
            glutStrokeCharacter(GLUT_STROKE_ROMAN, lC);
        }
        glPopMatrix();

    }

    glPopMatrix();
}



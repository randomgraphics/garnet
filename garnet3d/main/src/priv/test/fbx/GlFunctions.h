#ifndef _GL_FUNCTIONS_H
#define _GL_FUNCTIONS_H

void GlGetWindowSize(int& pWidth, int& pHeight);

void GlSetCameraPerspective(double pFieldOfViewY,
							double pAspect,
							double pNearPlane,
							double pFarPlane,
							KFbxVector4& pEye,
							KFbxVector4& pCenter,
							KFbxVector4& pUp);
void GlSetCameraOrthogonal(double pLeftPlane,
						   double pRightPlane,
						   double pBottomPlane,
						   double pTopPlane,
						   double pNearPlane,
						   double pFarPlane,
						   KFbxVector4& pEye,
						   KFbxVector4& pCenter,
                           KFbxVector4& pUp);

void GlDrawMarker(KFbxXMatrix& pGlobalPosition);
void GlDrawLimbNode(KFbxXMatrix& pGlobalBasePosition,
					KFbxXMatrix& pGlobalEndPosition);
void GlDrawMesh(KFbxXMatrix &pGlobalPosition,
				KFbxMesh* pMesh,
				KFbxVector4* pVertexArray,
				int pDrawMode);
void GlDrawCamera(KFbxXMatrix& pGlobalPosition,
				  double pRoll);
void GlDrawLight(KFbxXMatrix& pGlobalPosition,
				 KFbxLight* pLight,
				 KFbxColor& pColor,
				 double pConeAngle);
void GlDrawCrossHair(KFbxXMatrix& pGlobalPosition);
void GlDrawGrid(KFbxXMatrix& pGlobalPosition);

#define DRAW_MODE_WIREFRAME 0
#define DRAW_MODE_LIGHTED   1
#define DRAW_MODE_TEXTURED  2

#endif // #ifndef _GL_FUNCTIONS_H



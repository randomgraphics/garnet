#ifndef _SET_CAMERA_H
#define _SET_CAMERA_H

void SetCamera(KFbxScene* pScene,
               KTime& pTime,
               KFbxAnimLayer* pAnimLayer,
               KArrayTemplate<KFbxNode*>& pCameraArray);

KFbxCamera* GetCurrentCamera(KFbxScene* pScene);

void CameraZoomIn(KFbxScene* pScene, int dY);

void CameraZoomOut(KFbxScene* pScene, int dY);

void CameraOrbit(KFbxScene* pScene, KFbxVector4 lOrigCamPos, double OrigRoll, int dX, int dY);

void CameraPan(KFbxScene* pScene, KFbxVector4 lOrigCamPos, KFbxVector4 lOrigCamCenter,
			   double OrigRoll, int dX, int dY);

#endif // #ifndef _SET_CAMERA_H







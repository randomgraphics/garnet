/////////////////////////////////////////////////////////////////////////
//
// This file contains functions to:
// 1) find the current camera;
// 2) get the relevant settings of a camera depending on it's projection
//    type and aperture mode;
// 3) compute the orientation of a camera.
//
/////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "GetPosition.h"
#include "GlFunctions.h"
#include "SetCamera.h"

extern KFbxAnimStack* gCurrentAnimationStack;
extern KFbxAnimLayer* gCurrentAnimationLayer;

KFbxCamera* GetCurrentCamera(KFbxScene* pScene,
                             KTime& pTime,
                             KArrayTemplate<KFbxNode*>& pCameraArray);
void GetCameraAnimatedParameters(KFbxNode* pNode,
                                 KTime& pTime);
bool IsProducerCamera(KFbxScene*  pScene, KFbxCamera* pCamera);

static double gsOrthoCameraScale = 178.0;

// Set the view to the current camera settings.
void SetCamera(KFbxScene* pScene,
               KTime& pTime,
               KFbxAnimLayer* pAnimLayer,
               KArrayTemplate<KFbxNode*>& pCameraArray)
{
    // Find the current camera at the given time.
    KFbxCamera* lCamera = GetCurrentCamera(pScene, pTime, pCameraArray);
    KFbxNode*   lCameraNode = lCamera ? lCamera->GetNode() : NULL;

    // Compute the camera position and direction.
    KFbxVector4 lEye(0,0,1);
    KFbxVector4 lCenter(0,0,0);
    KFbxVector4 lUp(0,1,0);
    KFbxVector4 lForward, lRight;

    if (lCamera)
    {
        lEye = lCamera->Position.Get();
        lUp = lCamera->UpVector.Get();
    }

    if (lCameraNode && lCameraNode->GetTarget())
    {
        lCenter = GetGlobalPosition(lCameraNode->GetTarget(), pTime).GetT();
    }
    else
    {
        if (!lCameraNode || IsProducerCamera(pScene, lCamera))
        {
            if (lCamera)
                lCenter = lCamera->InterestPosition.Get();
        }
        else
        {
            // Get the direction
            KFbxXMatrix lGlobalRotation;
            KFbxVector4 lRotationVector(GetGlobalPosition(lCameraNode, pTime).GetR());
            lGlobalRotation.SetR(lRotationVector);

            // Get the length
            KFbxVector4 lInterestPosition(lCamera->InterestPosition.Get());
            KFbxVector4 lCameraGlobalPosition(GetGlobalPosition(lCameraNode, pTime).GetT());
            double      lLength = (KFbxVector4(lInterestPosition - lCameraGlobalPosition).Length());

            // Set the center.
            // A camera with rotation = {0,0,0} points to the X direction. So create a
            // vector in the X direction, rotate that vector by the global rotation amount
            // and then position the center by scaling and translating the resulting vector
            lRotationVector = KFbxVector4(1.0,0,0);
            lCenter = lGlobalRotation.MultT(lRotationVector);
            lCenter *= lLength;
            lCenter += lEye;

            // Update the default up vector with the camera rotation.
            lRotationVector = KFbxVector4(0,1.0,0);
            lUp = lGlobalRotation.MultT(lRotationVector);
        }
    }

    // Align the up vector.
    lForward = lCenter - lEye;
    lForward.Normalize();
    lRight = lForward.CrossProduct(lUp);
    lRight.Normalize();
    lUp = lRight.CrossProduct(lForward);
    lUp.Normalize();

    // Rotate the up vector with the roll value.
    double lRadians = 0;

    if (lCamera)
        lRadians = 3.1416 * lCamera->Roll.Get() / 180.0;
    lUp *= cos(lRadians);
    lRight *= sin(lRadians);
    lUp = lUp + lRight;

    double lNearPlane = 0.01;
    if (lCamera)
        lNearPlane = lCamera->GetNearPlane();
    double lFarPlane = 1000.0;
    if (lCamera)
        lFarPlane = lCamera->GetFarPlane();

    // Get the relevant camera settings for a perspective view.
    if (lCamera && lCamera->ProjectionType.Get() == KFbxCamera::ePERSPECTIVE)
    {
        double lFieldOfViewY=0.0;
        double lAspect = lCamera->GetApertureWidth() * lCamera->GetSqueezeRatio() / lCamera->GetApertureHeight();

        if (lCamera->GetApertureMode() == KFbxCamera::eHORIZONTAL
            || lCamera->GetApertureMode() == KFbxCamera::eVERTICAL) {
                lFieldOfViewY = lCamera->FieldOfView.Get();
                if (lCamera->GetApertureMode() == KFbxCamera::eHORIZONTAL)
                    lFieldOfViewY /= lAspect;
        } else if (lCamera->GetApertureMode() == KFbxCamera::eFOCAL_LENGTH) {
            lFieldOfViewY = lCamera->ComputeFieldOfView(lCamera->FocalLength.Get());
            lFieldOfViewY /= lAspect;
        } else if (lCamera->GetApertureMode() == KFbxCamera::eHORIZONTAL_AND_VERTICAL) {
            lFieldOfViewY = lCamera->FieldOfViewY.Get();
        }

        GlSetCameraPerspective(lFieldOfViewY,
            lAspect,
            lNearPlane,
            lFarPlane,
            lEye,
            lCenter,
            lUp);
    }
    // Get the relevant camera settings for an orthogonal view.
    else
    {
        double lPixelRatio = 1.0;
        if (lCamera)
            lPixelRatio = lCamera->GetPixelRatio();

        int lWidth, lHeight;
        double lLeftPlane, lRightPlane, lBottomPlane, lTopPlane;

        GlGetWindowSize(lWidth, lHeight);

        if(lWidth < lHeight)
        {
            lLeftPlane   = -gsOrthoCameraScale * lPixelRatio;
            lRightPlane  =  gsOrthoCameraScale * lPixelRatio;
            lBottomPlane = -gsOrthoCameraScale * lHeight / lWidth;
            lTopPlane    =  gsOrthoCameraScale * lHeight / lWidth;
        }
        else
        {
            lWidth *= (int) lPixelRatio;
            lLeftPlane   = -gsOrthoCameraScale * lWidth / lHeight;
            lRightPlane  =  gsOrthoCameraScale * lWidth / lHeight;
            lBottomPlane = -gsOrthoCameraScale;
            lTopPlane    =  gsOrthoCameraScale;
        }

        GlSetCameraOrthogonal(lLeftPlane,
            lRightPlane,
            lBottomPlane,
            lTopPlane,
            lNearPlane,
            lFarPlane,
            lEye,
            lCenter,
            lUp);
    }
}


// Find the current camera at the given time.
KFbxCamera* GetCurrentCamera(KFbxScene* pScene,
                             KTime& pTime,
                             KArrayTemplate<KFbxNode*>& pCameraArray)
{
    KFbxGlobalSettings& lGlobalSettings = pScene->GetGlobalSettings();
    KFbxGlobalCameraSettings& lGlobalCameraSettings = pScene->GlobalCameraSettings();
    KString lCurrentCameraName = lGlobalSettings.GetDefaultCamera();

    // check if we need to create the Producer cameras!
    if (lGlobalCameraSettings.GetCameraProducerPerspective() == NULL &&
        lGlobalCameraSettings.GetCameraProducerBottom() == NULL &&
        lGlobalCameraSettings.GetCameraProducerTop() == NULL &&
        lGlobalCameraSettings.GetCameraProducerFront() == NULL &&
        lGlobalCameraSettings.GetCameraProducerBack() == NULL &&
        lGlobalCameraSettings.GetCameraProducerRight() == NULL &&
        lGlobalCameraSettings.GetCameraProducerLeft() == NULL)
    {
        lGlobalCameraSettings.CreateProducerCameras();
    }

    if (lCurrentCameraName.Compare(PRODUCER_PERSPECTIVE) == 0)
    {
        return lGlobalCameraSettings.GetCameraProducerPerspective();
    }
    else if (lCurrentCameraName.Compare(PRODUCER_BOTTOM) == 0)
    {
        return lGlobalCameraSettings.GetCameraProducerBottom();
    }
    else if (lCurrentCameraName.Compare(PRODUCER_TOP) == 0)
    {
        return lGlobalCameraSettings.GetCameraProducerTop();
    }
    else if (lCurrentCameraName.Compare(PRODUCER_FRONT) == 0)
    {
        return lGlobalCameraSettings.GetCameraProducerFront();
    }
    else if (lCurrentCameraName.Compare(PRODUCER_BACK) == 0)
    {
        return lGlobalCameraSettings.GetCameraProducerBack();
    }
    else if (lCurrentCameraName.Compare(PRODUCER_RIGHT) == 0)
    {
        return lGlobalCameraSettings.GetCameraProducerRight();
    }
    else if (lCurrentCameraName.Compare(PRODUCER_LEFT) == 0)
    {
        return lGlobalCameraSettings.GetCameraProducerLeft();
    }
    else if (lCurrentCameraName.Compare(CAMERA_SWITCHER) == 0)
    {
        KFbxCameraSwitcher* lCameraSwitcher = pScene->GlobalCameraSettings().GetCameraSwitcher();
		KFbxAnimCurve* lCurve = NULL;
		if (lCameraSwitcher)
		{
			lCurve = lCameraSwitcher->CameraIndex.GetCurve<KFbxAnimCurve>(gCurrentAnimationLayer);
			KFCurve* lFCurve = (lCurve)?lCurve->GetKFCurve():NULL;

			int lCameraIndex = 0;
			if (lFCurve)
				// Get the index of the camera in the camera array.
				lCameraIndex = (int) lFCurve->Evaluate(pTime) - 1;

			if (lCameraIndex >= 0 && lCameraIndex < pCameraArray.GetCount())
			{
				KFbxNode* lNode = pCameraArray[lCameraIndex];

				// Get the animated parameters of the camera.
				GetCameraAnimatedParameters(lNode, pTime);

				return (KFbxCamera*) lNode->GetNodeAttribute();
			}
		}
    }
    else
    {
        int i;
        KFbxNode* lNode = NULL;

        // Find the camera in the camera array.
        for (i = 0; i < pCameraArray.GetCount(); i++)
        {
            if (lCurrentCameraName.Compare(pCameraArray[i]->GetName()) == 0)
            {
                lNode = pCameraArray[i];
                break;
            }
        }

        if (lNode)
        {
            // Get the animated parameters of the camera.
            GetCameraAnimatedParameters(lNode, pTime);

            return (KFbxCamera*) lNode->GetNodeAttribute();
        }
    }

    return lGlobalCameraSettings.GetCameraProducerPerspective();
}

// Get the animated parameters of a camera contained in the scene
// and store them in the associated member variables contained in
// the camera.
void GetCameraAnimatedParameters(KFbxNode* pNode, KTime& pTime)
{
    KFbxCamera* lCamera = (KFbxCamera*) pNode->GetNodeAttribute();
    lCamera->Position.Set(GetGlobalPosition(pNode, pTime).GetT());

    KFbxAnimCurve* fc = lCamera->Roll.GetCurve<KFbxAnimCurve>(gCurrentAnimationLayer);
    if (fc)
        lCamera->Roll.Set(fc->Evaluate(pTime));

    if (lCamera->GetApertureMode() == KFbxCamera::eHORIZONTAL ||
        lCamera->GetApertureMode() == KFbxCamera::eVERTICAL)
    {
        fc = lCamera->FieldOfView.GetCurve<KFbxAnimCurve>(gCurrentAnimationLayer);
        if (fc)
            lCamera->FieldOfView.Set(fc->Evaluate(pTime));
    }
    else if (lCamera->GetApertureMode() == KFbxCamera::eHORIZONTAL_AND_VERTICAL)
    {
        // In this aperture mode, the squeeze ratio is not relevant
        // because the fields of view in X and Y are independent.
        // It's value is set to retrieve the proper aspect in function SetCamera().
        //
        // if:
        //
        // aspect = field of view X / field of view Y = aperture width * squeeze ratio / aperture height
        //
        // then:
        //
        // squeeze ratio = (field of view X * aperture height) / (field of view Y * aperture width)
        //
        double lFieldOfViewX = lCamera->FieldOfViewX.Get();
        double lFieldOfViewY = lCamera->FieldOfViewY.Get();
        fc = lCamera->FieldOfViewX.GetCurve<KFbxAnimCurve>(gCurrentAnimationLayer);
        if (fc)
            lFieldOfViewX = fc->Evaluate(pTime);

        fc = lCamera->FieldOfViewY.GetCurve<KFbxAnimCurve>(gCurrentAnimationLayer);
        if (fc)
            lFieldOfViewY = fc->Evaluate(pTime);

        double lSqueezeRatio = (lFieldOfViewX * lCamera->GetApertureHeight()) / (lFieldOfViewY * lCamera->GetApertureWidth());

        lCamera->FieldOfViewX.Set(lFieldOfViewX);
        lCamera->FieldOfViewY.Set(lFieldOfViewY);
        lCamera->SetSqueezeRatio(lSqueezeRatio);
    }
    else if (lCamera->GetApertureMode() == KFbxCamera::eFOCAL_LENGTH)
    {
        double lFocalLength = lCamera->FocalLength.Get();
        fc = lCamera->FocalLength.GetCurve<KFbxAnimCurve>(gCurrentAnimationLayer);
        if (fc && fc ->Evaluate(pTime))
            lCamera->FocalLength.Set(lFocalLength);
    }
}

bool IsProducerCamera(KFbxScene*  pScene, KFbxCamera* pCamera)
{
    KFbxGlobalCameraSettings& lGlobalCameraSettings = pScene->GlobalCameraSettings();
    if (pCamera == lGlobalCameraSettings.GetCameraProducerPerspective())
        return true;
    if (pCamera == lGlobalCameraSettings.GetCameraProducerTop())
        return true;
    if (pCamera == lGlobalCameraSettings.GetCameraProducerBottom())
        return true;
    if (pCamera == lGlobalCameraSettings.GetCameraProducerFront())
        return true;
    if (pCamera == lGlobalCameraSettings.GetCameraProducerBack())
        return true;
    if (pCamera == lGlobalCameraSettings.GetCameraProducerRight())
        return true;
    if (pCamera == lGlobalCameraSettings.GetCameraProducerLeft())
        return true;

    return false;
}

KFbxCamera* GetCurrentCamera(KFbxScene*  pScene)
{
    KFbxCamera* lRet = NULL;
    KString     lCurrentCameraName;

    KFbxGlobalCameraSettings& lGlobalCameraSettings = pScene->GlobalCameraSettings();

    lCurrentCameraName = lGlobalCameraSettings.GetDefaultCamera();

    if (lCurrentCameraName == PRODUCER_PERSPECTIVE)
    {
        lRet = lGlobalCameraSettings.GetCameraProducerPerspective();
    }
    else if (lCurrentCameraName == PRODUCER_TOP)
    {
        lRet = lGlobalCameraSettings.GetCameraProducerTop();
    }
    else if (lCurrentCameraName == PRODUCER_BOTTOM)
    {
        lRet = lGlobalCameraSettings.GetCameraProducerBottom();
    }
    else if (lCurrentCameraName == PRODUCER_FRONT)
    {
        lRet = lGlobalCameraSettings.GetCameraProducerFront();
    }
    else if (lCurrentCameraName == PRODUCER_BACK)
    {
        lRet = lGlobalCameraSettings.GetCameraProducerBack();
    }
    else if (lCurrentCameraName == PRODUCER_RIGHT)
    {
        lRet = lGlobalCameraSettings.GetCameraProducerRight();
    }
    else if (lCurrentCameraName == PRODUCER_LEFT)
    {
        lRet = lGlobalCameraSettings.GetCameraProducerLeft();
    }
    return lRet;
}


void CameraZoomIn(KFbxScene* pScene, int dY)
{
    KFbxCamera* lCamera = GetCurrentCamera(pScene);

    if (lCamera)
    {
        if (lCamera->ProjectionType.Get() == KFbxCamera::ePERSPECTIVE)
        {
            double lOriginalAperture = sqrt(lCamera->GetApertureWidth());
            double lApertureTransform = lOriginalAperture-dY/400.0;

            if (lApertureTransform <0.25)
            {
                lApertureTransform =0.25;
            }

            if (lApertureTransform*lApertureTransform >179)
            {
                lApertureTransform = sqrt(179.0);
            }

            double lApertureH = lApertureTransform/lOriginalAperture*sqrt(lCamera->GetApertureHeight());

            // update focal length or field of view.
            double lApertureWidthOrig = lCamera->GetApertureWidth();
            double lApertureHeightOrig = lCamera->GetApertureHeight();

            if (lCamera->GetApertureMode() == KFbxCamera::eFOCAL_LENGTH) {
                // update focal length according to hypothetic new apertures.
                double lFLOrig  = lCamera->FocalLength.Get();
                double lFOVOrig = lCamera->ComputeFieldOfView(lFLOrig); // recompute to be sure.
                // transform aperture width and height temporarly
                lCamera->SetApertureWidth(lApertureTransform * lApertureTransform);
                lCamera->SetApertureHeight(lApertureH * lApertureH);
                double lFLNew  = lCamera->ComputeFocalLength(lFOVOrig);
                double lFOVNew = lCamera->ComputeFieldOfView(lFLNew);
                lCamera->FocalLength.Set(lFLNew);
                lCamera->FieldOfView.Set(lFOVNew);
            } else if (lCamera->GetApertureMode() == KFbxCamera::eVERTICAL
                || lCamera->GetApertureMode() == KFbxCamera::eHORIZONTAL) {
                    double lFOVOrig = lCamera->FieldOfView.Get();
                    double lFLOrig = lCamera->ComputeFocalLength(lFOVOrig); // recompute to be sure.
                    // transform aperture width and height temporarly
                    lCamera->SetApertureWidth(lApertureTransform * lApertureTransform);
                    lCamera->SetApertureHeight(lApertureH * lApertureH);
                    double lFOVNew = lCamera->ComputeFieldOfView(lFLOrig);
                    double lFLNew = lCamera->ComputeFocalLength(lFOVNew);
                    lCamera->FieldOfView.Set(lFOVNew);
                    lCamera->FocalLength.Set(lFLNew);
            } else if (lCamera->GetApertureMode() == KFbxCamera::eHORIZONTAL_AND_VERTICAL) {
                double lFOVOrig = lCamera->FieldOfViewY.Get();
                double lFLOrig = lCamera->ComputeFocalLength(lFOVOrig); // recompute to be sure.
                // transform aperture width and height temporarly
                lCamera->SetApertureWidth(lApertureTransform * lApertureTransform);
                lCamera->SetApertureHeight(lApertureH * lApertureH);
                double lFOVNew = lCamera->ComputeFieldOfView(lFLOrig);
                double lFLNew = lCamera->ComputeFocalLength(lFOVNew);
                lCamera->FieldOfViewY.Set(lFOVNew);
                lCamera->FocalLength.Set(lFLNew);
            }
            // reset aperture width and height
            lCamera->SetApertureWidth(lApertureWidthOrig);
            lCamera->SetApertureHeight(lApertureHeightOrig);
        }
        else
        {
            gsOrthoCameraScale *= 0.8;
        }
    }
}

void CameraZoomOut(KFbxScene* pScene, int dY)
{
    KFbxCamera* lCamera = GetCurrentCamera(pScene);

    if (lCamera)
    {
        if (lCamera->ProjectionType.Get() == KFbxCamera::ePERSPECTIVE)
        {
            double lOriginalAperture = sqrt(lCamera->GetApertureWidth());
            double lApertureTransform = lOriginalAperture+dY/400.0;

            if (lApertureTransform <0.25)
            {
                lApertureTransform =0.25;
            }

            if (lApertureTransform*lApertureTransform >179)
            {
                lApertureTransform = sqrt(179.0);
            }

            double lApertureH = lApertureTransform/lOriginalAperture*sqrt(lCamera->GetApertureHeight());

            // update focal length or field of view.
            double lApertureWidthOrig = lCamera->GetApertureWidth();
            double lApertureHeightOrig = lCamera->GetApertureHeight();

            if (lCamera->GetApertureMode() == KFbxCamera::eFOCAL_LENGTH) {
                // update focal length according to hypothetic new apertures.
                double lFLOrig  = lCamera->FocalLength.Get();
                double lFOVOrig = lCamera->ComputeFieldOfView(lFLOrig); // recompute to be sure.
                // transform aperture width and height temporarly
                lCamera->SetApertureWidth(lApertureTransform * lApertureTransform);
                lCamera->SetApertureHeight(lApertureH * lApertureH);
                double lFLNew  = lCamera->ComputeFocalLength(lFOVOrig);
                double lFOVNew = lCamera->ComputeFieldOfView(lFLNew);
                lCamera->FocalLength.Set(lFLNew);
                lCamera->FieldOfView.Set(lFOVNew);
            } else if (lCamera->GetApertureMode() == KFbxCamera::eVERTICAL
                || lCamera->GetApertureMode() == KFbxCamera::eHORIZONTAL) {
                    double lFOVOrig = lCamera->FieldOfView.Get();
                    double lFLOrig = lCamera->ComputeFocalLength(lFOVOrig); // recompute to be sure.
                    // transform aperture width and height temporarly
                    lCamera->SetApertureWidth(lApertureTransform * lApertureTransform);
                    lCamera->SetApertureHeight(lApertureH * lApertureH);
                    double lFOVNew = lCamera->ComputeFieldOfView(lFLOrig);
                    double lFLNew = lCamera->ComputeFocalLength(lFOVNew);
                    lCamera->FieldOfView.Set(lFOVNew);
                    lCamera->FocalLength.Set(lFLNew);
            } else if (lCamera->GetApertureMode() == KFbxCamera::eHORIZONTAL_AND_VERTICAL) {
                double lFOVOrig = lCamera->FieldOfViewY.Get();
                double lFLOrig = lCamera->ComputeFocalLength(lFOVOrig); // recompute to be sure.
                // transform aperture width and height temporarly
                lCamera->SetApertureWidth(lApertureTransform * lApertureTransform);
                lCamera->SetApertureHeight(lApertureH * lApertureH);
                double lFOVNew = lCamera->ComputeFieldOfView(lFLOrig);
                double lFLNew = lCamera->ComputeFocalLength(lFOVNew);
                lCamera->FieldOfViewY.Set(lFOVNew);
                lCamera->FocalLength.Set(lFLNew);
            }
            // reset aperture width and height
            lCamera->SetApertureWidth(lApertureWidthOrig);
            lCamera->SetApertureHeight(lApertureHeightOrig);
        }
        else
        {
            gsOrthoCameraScale *= 1.25;
        }
    }
}

void CameraOrbit(KFbxScene* pScene, KFbxVector4 lOrigCamPos, double OrigRoll, int dX, int dY)
{
    // Orbit the camera horizontally dX degrees, vertically dY degrees.
    KFbxCamera* lCamera = GetCurrentCamera(pScene);
    if (!lCamera) return;
    KFbxGlobalCameraSettings& lGlobalCameraSettings = pScene->GlobalCameraSettings();
    if (lCamera != lGlobalCameraSettings.GetCameraProducerPerspective()) return;
    if (lCamera->LockMode.Get()) return;
    if (dX == 0 && dY == 0) return;

    KFbxVector4 lRotationVector, lNewPosition, lCurPosition;
    KFbxXMatrix lRotation;
    KFbxVector4 lCenter = lCamera->InterestPosition.Get();

    // current position
    KFbxVector4 lPosition = lCamera->Position.Get();
    lCurPosition = lPosition-lCenter;

    // translate
    lNewPosition = lOrigCamPos-lCenter;

    int rotX;
    if (lNewPosition[2] == 0) {
        rotX = 90;
    } else {
        rotX = (int) (180.0* atan((double)lNewPosition[0]/(double)lNewPosition[2]) / 3.14159);
    }
    bool bRoll = (((int)OrigRoll % 360) != 0);
    if (   (lNewPosition[2] < 0 && !bRoll)
        || (lNewPosition[2] > 0 && bRoll) ) {
            dY = -dY;
    }
    if (bRoll) dX = -dX;

    // Center on the X axis (push)
    lRotationVector[1] = -rotX;
    lRotation.SetR(lRotationVector);
    lNewPosition = lRotation.MultT(lNewPosition);
    // Rotation for the vertical movement: around the X axis
    lRotationVector[1] = 0;
    lRotationVector[0] = dY;
    lRotation.SetR(lRotationVector);
    lNewPosition = lRotation.MultT(lNewPosition);
    // Back from the X axis (pop)
    lRotationVector[0] = 0;
    lRotationVector[1] = rotX;
    lRotation.SetR(lRotationVector);
    lNewPosition = lRotation.MultT(lNewPosition);
    // Rotation for the horizontal movement
    lRotationVector[1] = -dX;
    lRotation.SetR(lRotationVector);
    lNewPosition = lRotation.MultT(lNewPosition);

    // Detect camera flip
    if (   lNewPosition[0]*lCurPosition[0] < 0
        && lNewPosition[2]*lCurPosition[2] < 0) {
            // flip -> roll 180.
            double lRoll = lCamera->Roll.Get();
            lRoll = 180.0-lRoll;
            lCamera->Roll.Set(lRoll);
    }

    // Back from center
    lNewPosition = lNewPosition + lCenter;

    lCamera->Position.Set(lNewPosition);

}

void CameraPan(KFbxScene* pScene, KFbxVector4 lOrigCamPos, KFbxVector4 lOrigCamCenter, double OrigRoll, int dX, int dY) {
    // Pan the camera horizontally dX degrees, vertically dY degrees.
    KFbxCamera* lCamera = GetCurrentCamera(pScene);
    if (!lCamera) return;
    if (!IsProducerCamera(pScene, lCamera)) return;
    if (lCamera->LockMode.Get()) return;
    if (dX == 0 && dY == 0) return;

    KFbxGlobalCameraSettings& lGlobalCameraSettings = pScene->GlobalCameraSettings();

    KFbxVector4 lRotationXV, lRotationYV, lTranslationV;
    KFbxXMatrix lRotationX, lRotationY, lRotationXInverse, lRotationYInverse, lTranslation;
    KFbxVector4 lNewPosition = lOrigCamPos;
    KFbxVector4 lNewCenter = lOrigCamCenter;

    // Translate the camera in dX and dY according to its point of view.
    if (lCamera == lGlobalCameraSettings.GetCameraProducerTop()) {
        lTranslationV[0] = -dX;
        lTranslationV[1] = 0;
        lTranslationV[2] = dY;
    } else if (lCamera == lGlobalCameraSettings.GetCameraProducerBottom()) {
        lTranslationV[0] = dX;
        lTranslationV[1] = 0;
        lTranslationV[2] = dY;
    } else if (lCamera == lGlobalCameraSettings.GetCameraProducerFront()) {
        lTranslationV[0] = -dX;
        lTranslationV[1] = -dY;
        lTranslationV[2] = 0;
    } else if (lCamera == lGlobalCameraSettings.GetCameraProducerBack()) {
        lTranslationV[0] = dX;
        lTranslationV[1] = -dY;
        lTranslationV[2] = 0;
    } else if (lCamera == lGlobalCameraSettings.GetCameraProducerRight()) {
        lTranslationV[0] = 0;
        lTranslationV[1] = -dY;
        lTranslationV[2] = dX;
    } else if (lCamera == lGlobalCameraSettings.GetCameraProducerLeft()) {
        lTranslationV[0] = 0;
        lTranslationV[1] = -dY;
        lTranslationV[2] = -dX;
    } else {
        // Perspective view. More computation.

        // Adjust displacement when there is roll
        bool bRoll = (((int)OrigRoll % 360) != 0);
        if (bRoll) {
            dX = -dX;
            dY = -dY;
        }

        // Compute angles aY and aZ of the camera with Y and Z axis.
        double aZ, aY;
        /// Vector of distance between camera and center (lookat)
        KFbxVector4 lDist = lNewPosition - lNewCenter;
        // Euclidian distance between camera and lookat
        double dist = (double)(lDist[0]*lDist[0]+lDist[1]*lDist[1]+lDist[2]*lDist[2]);
        // aZ
        if (lDist[2] == 0) {
            aZ = 90.0;
        } else {
            aZ = (180.0* atan((double)lDist[0]/(double)lDist[2]) / 3.14159);
        }
        if (lNewPosition[2] < lNewCenter[2]) aZ += 180;
        // aY
        if (dist > 0.001) {
            aY = (180.0 * asin(sqrt((double)(lDist[1]*lDist[1])/ dist)) / 3.14159);
        } else {
            aY = 0;
        }
        if (lNewPosition[1] < lNewCenter[1]) aY = -aY;


        // Basis translation
        lTranslationV[0] = -dX;
        lTranslationV[1] = -dY;
        lTranslationV[2] = 0;

        // Rotation around Y axis
        lRotationYV[0] = 0;
        lRotationYV[1] = -aZ;
        lRotationYV[2] = 0;
        lRotationY.SetR(lRotationYV);
        // Rotation around X axis
        lRotationXV[0] = aY;
        lRotationXV[1] = 0;
        lRotationXV[2] = 0;
        lRotationX.SetR(lRotationXV);

        // Modify translation according to aY and aZ.
        lTranslation.SetT(lTranslationV);
        lRotationYInverse = lRotationY.Inverse();
        lRotationXInverse = lRotationX.Inverse();
        lTranslation = lRotationYInverse * lRotationXInverse * lTranslation * lRotationY * lRotationX;
        lTranslationV = lTranslation.GetT();
    }

    // Translate camera and center according to pan.
    lNewPosition += lTranslationV;
    lNewCenter   += lTranslationV;

    lCamera->Position.Set(lNewPosition);
    lCamera->InterestPosition.Set(lNewCenter);
}


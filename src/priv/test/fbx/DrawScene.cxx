/////////////////////////////////////////////////////////////////////////
//
// This file contains the functions to draw the content of a FBX scene
// at a given time. The node tree is browsed to draw the nodes with the
// following node attribute types: marker, skeleton, mesh, camera, light
// and null. The nurbs and patch node attribute types are not supported
// though the nurbs and patch node attributes should have been converted
// into mesh node attributes upon import.
//
// This file contains functions to show how to:
// 1) compute the orientation of a camera;
// 2) compute the orientation of a light;
// 3) compute the shape deformation of mesh vertices;
// 4) compute the link deformation of mesh vertices;
// 5) playback vertex cached deformations.
//
/////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "GetPosition.h"
#include "GlFunctions.h"
#include "DrawScene.h"
#include "Texture.h"

void DrawNodeRecursive(KFbxNode* pNode,
                       KTime& pTime,
                       KFbxXMatrix& pParentGlobalPosition);
void DrawNodeRecursiveAtPose(KFbxNode* pNode,
                             KTime& pTime,
                             KFbxXMatrix& pParentGlobalPosition,
                             KFbxPose* pPose);
void DrawNode(KFbxNode* pNode,
              KTime& lTime,
              KFbxXMatrix& pParentGlobalPosition,
              KFbxXMatrix& pGlobalPosition,
              KFbxPose* pPose = NULL);
void DrawMarker(KFbxXMatrix& pGlobalPosition);
void DrawSkeleton(KFbxNode* pNode,
                  KFbxXMatrix& pParentGlobalPosition,
                  KFbxXMatrix& pGlobalPosition);
void DrawMesh(KFbxNode* pNode,
              KTime& pTime,
              KFbxXMatrix& pGlobalPosition,
              KFbxPose* pPose);
void ComputeShapeDeformation(KFbxNode* pNode,
                             KFbxMesh* pMesh,
                             KTime& pTime,
                             KFbxVector4* pVertexArray);
void ComputeClusterDeformation(KFbxXMatrix& pGlobalPosition,
                               KFbxMesh* pMesh,
                               KTime& pTime,
                               KFbxVector4* pVertexArray,
                               KFbxPose* pPose);
void ReadVertexCacheData(KFbxMesh* pMesh,
                         KTime& pTime,
                         KFbxVector4* pVertexArray);
void DrawCamera(KFbxNode* pNode,
                KTime& pTime,
                KFbxXMatrix& pGlobalPosition);
void DrawLight(KFbxNode* pNode,
               KTime& pTime,
               KFbxXMatrix& pGlobalPosition);
void DrawNull(KFbxXMatrix& pGlobalPosition);
void MatrixScale(KFbxXMatrix& pMatrix, double pValue);
void MatrixAddToDiagonal(KFbxXMatrix& pMatrix, double pValue);
void MatrixAdd(KFbxXMatrix& pDstMatrix, KFbxXMatrix& pSrcMatrix);
void DrawGrid(KFbxXMatrix& pGlobalPosition);

extern KFbxAnimLayer* gCurrentAnimationLayer;

// Draw the scene at a given time for the current animation stack.
void DrawScene(KFbxScene* pScene,
               KTime& pTime)
{
    KFbxXMatrix lDummyGlobalPosition;

    int i, lCount = pScene->GetRootNode()->GetChildCount();

    for (i = 0; i < lCount; i++)
    {
        DrawNodeRecursive(pScene->GetRootNode()->GetChild(i), pTime, lDummyGlobalPosition);
    }

    DrawGrid(lDummyGlobalPosition);
}

// Draw the scene at a given pose. The elements not part of the pose
// will be drawn at the given time for the current animation stack.
void DrawSceneAtPose(KFbxScene* pScene, KTime& pTime, int pPoseIndex)
{
    KFbxXMatrix lDummyGlobalPosition;
    KFbxPose*   lPose = pScene->GetPose(pPoseIndex);

    int i, lCount = pScene->GetRootNode()->GetChildCount();

    for (i = 0; i < lCount; i++)
    {
        DrawNodeRecursiveAtPose(pScene->GetRootNode()->GetChild(i), pTime, lDummyGlobalPosition, lPose);
    }
}

// Draw recursively each node of the scene. To avoid recomputing
// uselessly the global positions, the global position of each
// node is passed to it's children while browsing the node tree.
void DrawNodeRecursive(KFbxNode* pNode,
                       KTime& pTime,
                       KFbxXMatrix& pParentGlobalPosition)
{
    // Compute the node's global position.
    KFbxXMatrix lGlobalPosition = GetGlobalPosition(pNode, pTime, &pParentGlobalPosition);

    // Geometry offset.
    // it is not inherited by the children.
    KFbxXMatrix lGeometryOffset = GetGeometry(pNode);
    KFbxXMatrix lGlobalOffPosition = lGlobalPosition * lGeometryOffset;

    DrawNode(pNode, pTime, pParentGlobalPosition, lGlobalOffPosition);

    int i, lCount = pNode->GetChildCount();

    for (i = 0; i < lCount; i++)
    {
        DrawNodeRecursive(pNode->GetChild(i), pTime, lGlobalPosition);
    }
}

// Draw recursively each node of the scene. To avoid recomputing
// uselessly the global positions, the global position of each
// node is passed to it's children while browsing the node tree.
// If the node is part of the given pose for the current scene,
// it will be drawn at the position specified in the pose, Otherwise
// it will be drawn at the given time.
void DrawNodeRecursiveAtPose(KFbxNode* pNode,
                             KTime& pTime,
                             KFbxXMatrix& pParentGlobalPosition,
                             KFbxPose* pPose)
{
    KFbxXMatrix lGlobalPosition;

    lGlobalPosition = GetGlobalPosition(pNode, pTime, pPose, &pParentGlobalPosition);

    // Geometry offset.
    // it is not inherited by the children.
    KFbxXMatrix lGeometryOffset = GetGeometry(pNode);
    KFbxXMatrix lGlobalOffPosition = lGlobalPosition * lGeometryOffset;

    DrawNode(pNode, pTime, pParentGlobalPosition, lGlobalOffPosition, pPose);

    int i, lCount = pNode->GetChildCount();

    for (i = 0; i < lCount; i++)
    {
        DrawNodeRecursiveAtPose(pNode->GetChild(i), pTime, lGlobalPosition, pPose);
    }
}

// Draw the node following the content of it's node attribute.
void DrawNode(KFbxNode* pNode,
              KTime& pTime,
              KFbxXMatrix& pParentGlobalPosition,
              KFbxXMatrix& pGlobalPosition,
              KFbxPose* pPose)
{
    KFbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();

    if (lNodeAttribute)
    {

        if (lNodeAttribute->GetAttributeType() == KFbxNodeAttribute::eMARKER)
        {
            DrawMarker(pGlobalPosition);
        }
        else if (lNodeAttribute->GetAttributeType() == KFbxNodeAttribute::eSKELETON)
        {
            DrawSkeleton(pNode, pParentGlobalPosition, pGlobalPosition);
        }
        else if (lNodeAttribute->GetAttributeType() == KFbxNodeAttribute::eMESH)
        {
            DrawMesh(pNode, pTime, pGlobalPosition, pPose);
        }
        else if (lNodeAttribute->GetAttributeType() == KFbxNodeAttribute::eNURB)
        {
            // Not supported yet.
            // Should have been converted into a mesh in function ConvertNurbsAndPatch().
        }
        else if (lNodeAttribute->GetAttributeType() == KFbxNodeAttribute::ePATCH)
        {
            // Not supported yet.
            // Should have been converted into a mesh in function ConvertNurbsAndPatch().
        }
        else if (lNodeAttribute->GetAttributeType() == KFbxNodeAttribute::eCAMERA)
        {
            DrawCamera(pNode, pTime, pGlobalPosition);
        }
        else if (lNodeAttribute->GetAttributeType() == KFbxNodeAttribute::eLIGHT)
        {
            DrawLight(pNode, pTime, pGlobalPosition);
        }
        else if (lNodeAttribute->GetAttributeType() == KFbxNodeAttribute::eNULL)
        {
            DrawNull(pGlobalPosition);
        }

    }
    else
    {
        DrawNull(pGlobalPosition);
    }
}


// Draw a small box where the node is located.
void DrawMarker(KFbxXMatrix& pGlobalPosition)
{
    GlDrawMarker(pGlobalPosition);
}


// Draw a limb between the node and its parent.
void DrawSkeleton(KFbxNode* pNode, KFbxXMatrix& pParentGlobalPosition, KFbxXMatrix& pGlobalPosition)
{
    KFbxSkeleton* lSkeleton = (KFbxSkeleton*) pNode->GetNodeAttribute();

    // Only draw the skeleton if it's a limb node and if
    // the parent also has an attribute of type skeleton.
    if (lSkeleton->GetSkeletonType() == KFbxSkeleton::eLIMB_NODE &&
        pNode->GetParent() &&
        pNode->GetParent()->GetNodeAttribute() &&
        pNode->GetParent()->GetNodeAttribute()->GetAttributeType() == KFbxNodeAttribute::eSKELETON)
    {
        GlDrawLimbNode(pParentGlobalPosition, pGlobalPosition);
    }
}


// Draw the vertices of a mesh.
void DrawMesh(KFbxNode* pNode, KTime& pTime, KFbxXMatrix& pGlobalPosition, KFbxPose* pPose)
{
    KFbxMesh* lMesh = (KFbxMesh*) pNode->GetNodeAttribute();
    int lClusterCount = 0;
    int lSkinCount= 0;
    int lVertexCount = lMesh->GetControlPointsCount();

    // No vertex to draw.
    if (lVertexCount == 0)
    {
        return;
    }

    // Create a copy of the vertex array to receive vertex deformations.
    KFbxVector4* lVertexArray = new KFbxVector4[lVertexCount];
    memcpy(lVertexArray, lMesh->GetControlPoints(), lVertexCount * sizeof(KFbxVector4));

    // Active vertex cache deformer will overwrite any other deformer
    if (lMesh->GetDeformerCount(KFbxDeformer::eVERTEX_CACHE) &&
        (static_cast<KFbxVertexCacheDeformer*>(lMesh->GetDeformer(0, KFbxDeformer::eVERTEX_CACHE)))->IsActive())
    {
        ReadVertexCacheData(lMesh, pTime, lVertexArray);
    }
    else
    {
        if (lMesh->GetShapeCount())
        {
            // Deform the vertex array with the shapes.
            ComputeShapeDeformation(pNode, lMesh, pTime, lVertexArray);
        }

        //we need to get the number of clusters
        lSkinCount = lMesh->GetDeformerCount(KFbxDeformer::eSKIN);
        for( int i=0; i< lSkinCount; i++)
            lClusterCount += ((KFbxSkin *)(lMesh->GetDeformer(i, KFbxDeformer::eSKIN)))->GetClusterCount();
        if (lClusterCount)
        {
            // Deform the vertex array with the links.
            ComputeClusterDeformation(pGlobalPosition, lMesh, pTime, lVertexArray, pPose);
        }
    }

    GlDrawMesh(pGlobalPosition,
        lMesh,
        lVertexArray,
        DRAW_MODE_TEXTURED);

    delete [] lVertexArray;
}


// Deform the vertex array with the shapes contained in the mesh.
void ComputeShapeDeformation(KFbxNode* pNode,
                             KFbxMesh* pMesh,
                             KTime& pTime,
                             KFbxVector4* pVertexArray)
{
    int i, j;
    int lShapeCount = pMesh->GetShapeCount();
    int lVertexCount = pMesh->GetControlPointsCount();

    KFbxVector4* lSrcVertexArray = pVertexArray;
    KFbxVector4* lDstVertexArray = new KFbxVector4[lVertexCount];
    memcpy(lDstVertexArray, pVertexArray, lVertexCount * sizeof(KFbxVector4));

    for (i = 0; i < lShapeCount; i++)
    {
        KFbxShape* lShape = pMesh->GetShape(i);

        // Get the percentage of influence of the shape.
        KFbxAnimCurve* lFCurve = pMesh->GetShapeChannel(i, gCurrentAnimationLayer);
        if (!lFCurve) continue;
        double lWeight = lFCurve->Evaluate(pTime) / 100.0;

        for (j = 0; j < lVertexCount; j++)
        {
            // Add the influence of the shape vertex to the mesh vertex.
            KFbxVector4 lInfluence = (lShape->GetControlPoints()[j] - lSrcVertexArray[j]) * lWeight;
            lDstVertexArray[j] += lInfluence;
        }
    }

    memcpy(pVertexArray, lDstVertexArray, lVertexCount * sizeof(KFbxVector4));

    delete [] lDstVertexArray;
}


// Deform the vertex array with the links contained in the mesh.
void ComputeClusterDeformation(KFbxXMatrix& pGlobalPosition,
                               KFbxMesh* pMesh,
                               KTime& pTime,
                               KFbxVector4* pVertexArray,
                               KFbxPose* pPose)
{
    // All the links must have the same link mode.
    KFbxCluster::ELinkMode lClusterMode = ((KFbxSkin*)pMesh->GetDeformer(0, KFbxDeformer::eSKIN))->GetCluster(0)->GetLinkMode();

    int i, j;
    int lClusterCount=0;

    int lVertexCount = pMesh->GetControlPointsCount();
    int lSkinCount = pMesh->GetDeformerCount(KFbxDeformer::eSKIN);

    KFbxXMatrix* lClusterDeformation = new KFbxXMatrix[lVertexCount];
    memset(lClusterDeformation, 0, lVertexCount * sizeof(KFbxXMatrix));
    double* lClusterWeight = new double[lVertexCount];
    memset(lClusterWeight, 0, lVertexCount * sizeof(double));

    if (lClusterMode == KFbxCluster::eADDITIVE)
    {
        for (i = 0; i < lVertexCount; i++)
        {
            lClusterDeformation[i].SetIdentity();
        }
    }

    for ( i=0; i<lSkinCount; ++i)
    {
        lClusterCount =( (KFbxSkin *)pMesh->GetDeformer(i, KFbxDeformer::eSKIN))->GetClusterCount();
        for (j=0; j<lClusterCount; ++j)
        {
            KFbxCluster* lCluster =((KFbxSkin *) pMesh->GetDeformer(i, KFbxDeformer::eSKIN))->GetCluster(j);
            if (!lCluster->GetLink())
                continue;
            KFbxXMatrix lReferenceGlobalInitPosition;
            KFbxXMatrix lReferenceGlobalCurrentPosition;
            KFbxXMatrix lClusterGlobalInitPosition;
            KFbxXMatrix lClusterGlobalCurrentPosition;
            KFbxXMatrix lReferenceGeometry;
            KFbxXMatrix lClusterGeometry;

            KFbxXMatrix lClusterRelativeInitPosition;
            KFbxXMatrix lClusterRelativeCurrentPositionInverse;
            KFbxXMatrix lVertexTransformMatrix;

            if (lClusterMode == KFbxLink::eADDITIVE && lCluster->GetAssociateModel())
            {
                lCluster->GetTransformAssociateModelMatrix(lReferenceGlobalInitPosition);
                lReferenceGlobalCurrentPosition = GetGlobalPosition(lCluster->GetAssociateModel(), pTime, pPose);
                // Geometric transform of the model
                lReferenceGeometry = GetGeometry(lCluster->GetAssociateModel());
                lReferenceGlobalCurrentPosition *= lReferenceGeometry;
            }
            else
            {
                lCluster->GetTransformMatrix(lReferenceGlobalInitPosition);
                lReferenceGlobalCurrentPosition = pGlobalPosition;
                // Multiply lReferenceGlobalInitPosition by Geometric Transformation
                lReferenceGeometry = GetGeometry(pMesh->GetNode());
                lReferenceGlobalInitPosition *= lReferenceGeometry;
            }
            // Get the link initial global position and the link current global position.
            lCluster->GetTransformLinkMatrix(lClusterGlobalInitPosition);
            lClusterGlobalCurrentPosition = GetGlobalPosition(lCluster->GetLink(), pTime, pPose);

            // Compute the initial position of the link relative to the reference.
            lClusterRelativeInitPosition = lClusterGlobalInitPosition.Inverse() * lReferenceGlobalInitPosition;

            // Compute the current position of the link relative to the reference.
            lClusterRelativeCurrentPositionInverse = lReferenceGlobalCurrentPosition.Inverse() * lClusterGlobalCurrentPosition;

            // Compute the shift of the link relative to the reference.
            lVertexTransformMatrix = lClusterRelativeCurrentPositionInverse * lClusterRelativeInitPosition;

            int k;
            int lVertexIndexCount = lCluster->GetControlPointIndicesCount();

            for (k = 0; k < lVertexIndexCount; ++k)
            {
                int lIndex = lCluster->GetControlPointIndices()[k];

                // Sometimes, the mesh can have less points than at the time of the skinning
                // because a smooth operator was active when skinning but has been deactivated during export.
                if (lIndex >= lVertexCount)
                    continue;

                double lWeight = lCluster->GetControlPointWeights()[k];

                if (lWeight == 0.0)
                {
                    continue;
                }

                // Compute the influence of the link on the vertex.
                KFbxXMatrix lInfluence = lVertexTransformMatrix;
                MatrixScale(lInfluence, lWeight);

                if (lClusterMode == KFbxCluster::eADDITIVE)
                {
                    // Multiply with to the product of the deformations on the vertex.
                    MatrixAddToDiagonal(lInfluence, 1.0 - lWeight);
                    lClusterDeformation[lIndex] = lInfluence * lClusterDeformation[lIndex];

                    // Set the link to 1.0 just to know this vertex is influenced by a link.
                    lClusterWeight[lIndex] = 1.0;
                }
                else // lLinkMode == KFbxLink::eNORMALIZE || lLinkMode == KFbxLink::eTOTAL1
                {
                    // Add to the sum of the deformations on the vertex.
                    MatrixAdd(lClusterDeformation[lIndex], lInfluence);

                    // Add to the sum of weights to either normalize or complete the vertex.
                    lClusterWeight[lIndex] += lWeight;
                }

            }
        }
    }

    for (i = 0; i < lVertexCount; i++)
    {
        KFbxVector4 lSrcVertex = pVertexArray[i];
        KFbxVector4& lDstVertex = pVertexArray[i];
        double lWeight = lClusterWeight[i];

        // Deform the vertex if there was at least a link with an influence on the vertex,
        if (lWeight != 0.0)
        {
            lDstVertex = lClusterDeformation[i].MultT(lSrcVertex);

            if (lClusterMode == KFbxCluster::eNORMALIZE)
            {
                // In the normalized link mode, a vertex is always totally influenced by the links.
                lDstVertex /= lWeight;
            }
            else if (lClusterMode == KFbxCluster::eTOTAL1)
            {
                // In the total 1 link mode, a vertex can be partially influenced by the links.
                lSrcVertex *= (1.0 - lWeight);
                lDstVertex += lSrcVertex;
            }
        }
    }

    delete [] lClusterDeformation;
    delete [] lClusterWeight;
}

void ReadVertexCacheData(KFbxMesh* pMesh,
                         KTime& pTime,
                         KFbxVector4* pVertexArray)
{
    KFbxVertexCacheDeformer* lDeformer     = static_cast<KFbxVertexCacheDeformer*>(pMesh->GetDeformer(0, KFbxDeformer::eVERTEX_CACHE));
    KFbxCache*               lCache        = lDeformer->GetCache();
    int                      lChannelIndex = -1;
    unsigned int             lVertexCount  = (unsigned int)pMesh->GetControlPointsCount();
    bool                     lReadSucceed  = false;
    double*                  lReadBuf      = new double[3*lVertexCount];

    if (lCache->GetCacheFileFormat() == KFbxCache::eMC)
    {
        if ((lChannelIndex = lCache->GetChannelIndex(lDeformer->GetCacheChannel())) > -1)
        {
            lReadSucceed = lCache->Read(lChannelIndex, pTime, lReadBuf, lVertexCount);
        }
    }
    else // ePC2
    {
        lReadSucceed = lCache->Read((unsigned int)pTime.GetFrame(true), lReadBuf, lVertexCount);
    }

    if (lReadSucceed)
    {
        unsigned int lReadBufIndex = 0;

        while (lReadBufIndex < 3*lVertexCount)
        {
            // In statements like "pVertexArray[lReadBufIndex/3].SetAt(2, lReadBuf[lReadBufIndex++])",
            // on Mac platform, "lReadBufIndex++" is evaluated before "lReadBufIndex/3".
            // So separate them.
            pVertexArray[lReadBufIndex/3].SetAt(0, lReadBuf[lReadBufIndex]); lReadBufIndex++;
            pVertexArray[lReadBufIndex/3].SetAt(1, lReadBuf[lReadBufIndex]); lReadBufIndex++;
            pVertexArray[lReadBufIndex/3].SetAt(2, lReadBuf[lReadBufIndex]); lReadBufIndex++;
        }
    }

    delete [] lReadBuf;
}


// Draw an oriented camera box where the node is located.
void DrawCamera(KFbxNode* pNode,
                KTime& pTime,
                KFbxXMatrix& pGlobalPosition)
{
    KFbxXMatrix lCameraGlobalPosition;
    KFbxVector4 lCameraPosition, lCameraDefaultDirection, lCameraInterestPosition;

    lCameraPosition = pGlobalPosition.GetT();

    // By default, FBX cameras point towards the X positive axis.
    KFbxVector4 lXPositiveAxis(1.0, 0.0, 0.0);
    lCameraDefaultDirection = lCameraPosition + lXPositiveAxis;

    lCameraGlobalPosition = pGlobalPosition;

    // If the camera is linked to an interest, get the interest position.
    if (pNode->GetTarget())
    {
        lCameraInterestPosition = GetGlobalPosition(pNode->GetTarget(), pTime).GetT();

        // Compute the required rotation to make the camera point to it's interest.
        KFbxVector4 lCameraDirection;
        KFbxVector4::AxisAlignmentInEulerAngle(lCameraPosition,
            lCameraDefaultDirection,
            lCameraInterestPosition,
            lCameraDirection);

        // Must override the camera rotation
        // to make it point to it's interest.
        lCameraGlobalPosition.SetR(lCameraDirection);
    }

    // Get the camera roll.
    KFbxCamera* cam = pNode->GetCamera();
    double lRoll = 0;

    if (cam)
    {
        lRoll = cam->Roll.Get();
        KFbxAnimCurve* fc = cam->Roll.GetCurve<KFbxAnimCurve>(gCurrentAnimationLayer);
        if (fc) fc->Evaluate(pTime);
    }
    GlDrawCamera(lCameraGlobalPosition, lRoll);
}


// Draw a colored sphere or cone where the node is located.
void DrawLight(KFbxNode* pNode,
               KTime& pTime,
               KFbxXMatrix& pGlobalPosition)
{
    KFbxLight* lLight = (KFbxLight*) pNode->GetNodeAttribute();
    KFbxXMatrix lLightRotation, lLightGlobalPosition;
    KFbxColor lColor(1,1,1);
    double lConeAngle = 0.0;

    // Must rotate the light's global position because
    // FBX lights point towards the Y negative axis.
    KFbxVector4 lYNegativeAxis(-90.0, 0.0, 0.0);
    lLightRotation.SetR(lYNegativeAxis);
    lLightGlobalPosition = pGlobalPosition * lLightRotation;

    // Get the light color.
    KFbxAnimCurve* fc;

    fc = lLight->Color.GetCurve<KFbxAnimCurve>(gCurrentAnimationLayer, KFCURVENODE_COLOR_RED);
    if (fc) lColor.mRed = fc->Evaluate(pTime);
    fc = lLight->Color.GetCurve<KFbxAnimCurve>(gCurrentAnimationLayer, KFCURVENODE_COLOR_GREEN);
    if (fc) lColor.mGreen = fc->Evaluate(pTime);
    fc = lLight->Color.GetCurve<KFbxAnimCurve>(gCurrentAnimationLayer, KFCURVENODE_COLOR_BLUE);
    if (fc) lColor.mBlue = fc->Evaluate(pTime);

    // The cone angle is only relevant if the light is a spot.
    if (lLight->LightType.Get() == KFbxLight::eSPOT)
    {
        fc = lLight->ConeAngle.GetCurve<KFbxAnimCurve>(gCurrentAnimationLayer);
        if (fc) lConeAngle = fc->Evaluate(pTime);
    }

    GlDrawLight(lLightGlobalPosition,
        lLight,
        lColor,
        lConeAngle);
}


// Draw a cross hair where the node is located.
void DrawNull(KFbxXMatrix& pGlobalPosition)
{
    GlDrawCrossHair(pGlobalPosition);
}


// Scale all the elements of a matrix.
void MatrixScale(KFbxXMatrix& pMatrix, double pValue)
{
    int i,j;

    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            pMatrix[i][j] *= pValue;
        }
    }
}


// Add a value to all the elements in the diagonal of the matrix.
void MatrixAddToDiagonal(KFbxXMatrix& pMatrix, double pValue)
{
    pMatrix[0][0] += pValue;
    pMatrix[1][1] += pValue;
    pMatrix[2][2] += pValue;
    pMatrix[3][3] += pValue;
}


// Sum two matrices element by element.
void MatrixAdd(KFbxXMatrix& pDstMatrix, KFbxXMatrix& pSrcMatrix)
{
    int i,j;

    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            pDstMatrix[i][j] += pSrcMatrix[i][j];
        }
    }
}

void DrawGrid(KFbxXMatrix& pGlobalPosition)
{
    GlDrawGrid(pGlobalPosition);
}


/////////////////////////////////////////////////////////////////////////
//
// This file contains the functions to get the local and the global
// position of a node for a given time in the current animation stack.
//
/////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "GetPosition.h"


// Get the global position.
// Do not take in account the geometric transform.
KFbxXMatrix GetGlobalPosition(KFbxNode* pNode, KTime& pTime, KFbxXMatrix* pParentGlobalPosition)
{
    // Ideally this function would use parent global position and local position to
    // compute the global position.
    // Unfortunately the equation
    //    lGlobalPosition = pParentGlobalPosition * lLocalPosition
    // does not hold when inheritance type is other than "Parent" (RSrs). To compute
    // the parent rotation and scaling is tricky in the RrSs and Rrs cases.
    // This is why GetNodeGlobalTransform() is used: it always computes the right
    // global position.

	return pNode->GetScene()->GetEvaluator()->GetNodeGlobalTransform(pNode, pTime);
}


// Get the global position of the node for the current pose.
// If the specified node is not part of the pose, get its
// global position at the current time.
KFbxXMatrix GetGlobalPosition(KFbxNode* pNode, KTime& pTime, KFbxPose* pPose, KFbxXMatrix* pParentGlobalPosition)
{
    KFbxXMatrix lGlobalPosition;
    bool        lPositionFound = false;

    if (pPose)
    {
        int lNodeIndex = pPose->Find(pNode);

        if (lNodeIndex > -1)
        {
            // The bind pose is always a global matrix.
            // If we have a rest pose, we need to check if it is
            // stored in global or local space.
            if (pPose->IsBindPose() || !pPose->IsLocalMatrix(lNodeIndex))
            {
                lGlobalPosition = GetPoseMatrix(pPose, lNodeIndex);
            }
            else
            {
                // We have a local matrix, we need to convert it to
                // a global space matrix.
                KFbxXMatrix lParentGlobalPosition;

                if (pParentGlobalPosition)
                {
                    lParentGlobalPosition = *pParentGlobalPosition;
                }
                else
                {
                    if (pNode->GetParent())
                    {
                        lParentGlobalPosition = GetGlobalPosition(pNode->GetParent(), pTime, pPose);
                    }
                }

                KFbxXMatrix lLocalPosition = GetPoseMatrix(pPose, lNodeIndex);
                lGlobalPosition = lParentGlobalPosition * lLocalPosition;
            }

            lPositionFound = true;
        }
    }

    if (!lPositionFound)
    {
        // There is no pose entry for that node, get the current global position instead
        lGlobalPosition = GetGlobalPosition(pNode, pTime, pParentGlobalPosition);
    }

    return lGlobalPosition;
}

// Get the matrix of the given pose
KFbxXMatrix GetPoseMatrix(KFbxPose* pPose, int pNodeIndex)
{
    KFbxXMatrix lPoseMatrix;
    KFbxMatrix lMatrix = pPose->GetMatrix(pNodeIndex);

    memcpy((double*)lPoseMatrix, (double*)lMatrix, sizeof(lMatrix.mData));

    return lPoseMatrix;
}

// Get the geometry deformation local to a node. It is never inherited by the
// children.
KFbxXMatrix GetGeometry(KFbxNode* pNode) {
    KFbxVector4 lT, lR, lS;
    KFbxXMatrix lGeometry;

    lT = pNode->GetGeometricTranslation(KFbxNode::eSOURCE_SET);
    lR = pNode->GetGeometricRotation(KFbxNode::eSOURCE_SET);
    lS = pNode->GetGeometricScaling(KFbxNode::eSOURCE_SET);

    lGeometry.SetT(lT);
    lGeometry.SetR(lR);
    lGeometry.SetS(lS);

    return lGeometry;
}


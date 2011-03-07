#ifndef _GET_POSITION_H
#define _GET_POSITION_H

KFbxXMatrix GetGlobalPosition(KFbxNode* pNode,
							  KTime& pTime,
							  KFbxXMatrix* pParentGlobalPosition = NULL);
KFbxXMatrix GetGlobalPosition(KFbxNode* pNode,
							  KTime& pTime,
							  KFbxPose* pPose,
							  KFbxXMatrix* pParentGlobalPosition = NULL);
KFbxXMatrix GetPoseMatrix(KFbxPose* pPose,
                          int pNodeIndex);
KFbxXMatrix GetGeometry(KFbxNode* pNode);


#endif // #ifndef _GET_POSITION_H




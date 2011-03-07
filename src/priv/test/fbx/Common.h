#ifndef _COMMON_H
#define _COMMON_H

#include <fbxsdk.h>
#include <fbxfilesdk/kfbxio/kfbximporter.h>
#include <fbxfilesdk/kfbxplugins/kfbxsdkmanager.h>
#include <fbxfilesdk/kfbxplugins/kfbxscene.h>

#include <stdio.h>

#include <fbxfilesdk/fbxfilesdk_nsuse.h>

void InitializeSdkObjects(KFbxSdkManager*& pSdkManager, KFbxScene*& pScene);
void DestroySdkObjects(KFbxSdkManager* pSdkManager);
void CreateAndFillIOSettings(KFbxSdkManager* pSdkManager);

bool SaveScene(KFbxSdkManager* pSdkManager, KFbxDocument* pScene, const char* pFilename, int pFileFormat=-1, bool pEmbedMedia=false);
bool LoadScene(KFbxSdkManager* pSdkManager, KFbxDocument* pScene, const char* pFilename);

#endif // #ifndef _COMMON_H



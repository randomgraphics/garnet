/////////////////////////////////////////////////////////////////////////
//
// This file contains the useful functions to:
// 1) convert the nurbs and patch attribute types of a scene into mesh
//    node attributes;
// 2) get the list of all the cameras in the scene.
//
/////////////////////////////////////////////////////////////////////////

#include "pch.h"

#include "InitScene.h"
#include "Texture.h"

#include "targa.h"

void ConvertNurbsAndPatchRecursive(KFbxSdkManager* pSdkManager,
                                   KFbxNode* pNode);
void FillCameraArrayRecursive(KFbxNode* pNode,
                              KArrayTemplate<KFbxNode*>& pCameraArray);
void LoadSupportedTexturesRecursive(KFbxNode* pNode, KArrayTemplate<VSTexture*>& pTextureArray);
void LoadTexture(KFbxTexture* pTexture, KArrayTemplate<VSTexture*>& pTextureArray);


void ConvertNurbsAndPatch(KFbxSdkManager* pSdkManager, KFbxScene* pScene)
{
    ConvertNurbsAndPatchRecursive(pSdkManager, pScene->GetRootNode());
}


void ConvertNurbsAndPatchRecursive(KFbxSdkManager* pSdkManager, KFbxNode* pNode)
{
    KFbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();

    if (lNodeAttribute)
    {
        if (lNodeAttribute->GetAttributeType() == KFbxNodeAttribute::eNURB ||
            lNodeAttribute->GetAttributeType() == KFbxNodeAttribute::ePATCH)
        {
            KFbxGeometryConverter lConverter(pSdkManager);
            lConverter.TriangulateInPlace(pNode);
        }
    }

    int i, lCount = pNode->GetChildCount();

    for (i = 0; i < lCount; i++)
    {
        ConvertNurbsAndPatchRecursive(pSdkManager, pNode->GetChild(i));
    }
}


void FillCameraArray(KFbxScene* pScene, KArrayTemplate<KFbxNode*>& pCameraArray)
{
    pCameraArray.Clear();

    FillCameraArrayRecursive(pScene->GetRootNode(), pCameraArray);
}


void FillCameraArrayRecursive(KFbxNode* pNode, KArrayTemplate<KFbxNode*>& pCameraArray)
{
    int i, lCount;

    if (pNode)
    {
        if (pNode->GetNodeAttribute())
        {
            if (pNode->GetNodeAttribute()->GetAttributeType() == KFbxNodeAttribute::eCAMERA)
            {
                pCameraArray.Add(pNode);
            }
        }

        lCount = pNode->GetChildCount();

        for (i = 0; i < lCount; i++)
        {
            FillCameraArrayRecursive(pNode->GetChild(i), pCameraArray);
        }
    }
}


void FillPoseArray(KFbxScene* pScene, KArrayTemplate<KFbxPose*>& pPoseArray)
{
    int      i, lPoseCount;

    for (i=0, lPoseCount = pScene->GetPoseCount(); i < lPoseCount; i++)
    {
        pPoseArray.Add(pScene->GetPose(i));
    }
}

void LoadSupportedTextures(KFbxScene* pScene, KArrayTemplate<VSTexture*>& pTextureArray)
{
    pTextureArray.Clear();

    LoadSupportedTexturesRecursive(pScene->GetRootNode(), pTextureArray);
}

void LoadSupportedTexturesRecursive(KFbxNode* pNode, KArrayTemplate<VSTexture*>& pTextureArray)
{
    if (pNode)
    {
        int i, lCount;
        KFbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();

        if (lNodeAttribute)
        {
            KFbxLayerContainer* lLayerContainer = NULL;

            switch (lNodeAttribute->GetAttributeType())
            {
            case KFbxNodeAttribute::eNURB:
                lLayerContainer = pNode->GetNurb();
                break;

            case KFbxNodeAttribute::ePATCH:
                lLayerContainer = pNode->GetPatch();
                break;

            case KFbxNodeAttribute::eMESH:
                lLayerContainer = pNode->GetMesh();
                break;

            default:
                // unhandled type
                break;
            }

            if (lLayerContainer){
                int lMaterialIndex;
                int lTextureIndex;
                KFbxProperty lProperty;
                int lNbTex;
                KFbxTexture* lTexture = NULL;
                KFbxSurfaceMaterial *lMaterial = NULL;
                int lNbMat = pNode->GetSrcObjectCount(KFbxSurfaceMaterial::ClassId);
                for (lMaterialIndex = 0; lMaterialIndex < lNbMat; lMaterialIndex++){
                    lMaterial = KFbxCast <KFbxSurfaceMaterial>(pNode->GetSrcObject(KFbxSurfaceMaterial::ClassId, lMaterialIndex));
                    if(lMaterial){
                        lProperty = lMaterial->FindProperty(KFbxSurfaceMaterial::sDiffuse);
                        if(lProperty.IsValid()){
                            lNbTex = lProperty.GetSrcObjectCount(KFbxTexture::ClassId);
                            for (lTextureIndex = 0; lTextureIndex < lNbTex; lTextureIndex++){
                                lTexture = KFbxCast <KFbxTexture> (lProperty.GetSrcObject(KFbxTexture::ClassId, lTextureIndex));
                                if(lTexture)
                                    LoadTexture(lTexture, pTextureArray);
                            }
                        }
                    }
                }
            }

        }

        lCount = pNode->GetChildCount();

        for (i = 0; i < lCount; i++)
        {
            LoadSupportedTexturesRecursive(pNode->GetChild(i), pTextureArray);
        }
    }
}

void LoadTexture(KFbxTexture* pTexture, KArrayTemplate<VSTexture*>& pTextureArray)
{
    // First find if the texture is already loaded
    int i, lCount = pTextureArray.GetCount();

    for (i = 0; i < lCount; i++)
    {
        if (pTextureArray[i]->mRefTexture == pTexture) return;
    }

    // Right now, only Targa textures are loaded by this sample
    KString lFileName = pTexture->GetFileName();

    if (lFileName.Right(3).Upper() == "TGA")
    {
        tga_image lTGAImage;

        if (tga_read(&lTGAImage, lFileName.Buffer()) == TGA_NOERR)
        {
            // Make sure the image is left to right
            if (tga_is_right_to_left(&lTGAImage)) tga_flip_horiz(&lTGAImage);

            // Make sure the image is bottom to top
            if (tga_is_top_to_bottom(&lTGAImage)) tga_flip_vert(&lTGAImage);

            // Make the image BGR 24
            tga_convert_depth(&lTGAImage, 24);

            VSTexture* lTexture = new VSTexture;

            lTexture->mW = lTGAImage.width;
            lTexture->mH = lTGAImage.height;
            lTexture->mRefTexture = pTexture;
            lTexture->mImageData  = new unsigned char[lTGAImage.width*lTGAImage.height*lTGAImage.pixel_depth/8];
            memcpy(lTexture->mImageData, lTGAImage.image_data, lTGAImage.width*lTGAImage.height*lTGAImage.pixel_depth/8);

            tga_free_buffers(&lTGAImage);

            pTextureArray.Add(lTexture);
        }
    }
}

void PreparePointCacheData(KFbxScene* pScene)
{
    // This function show how to cycle thru scene elements in a linear way.
    int lIndex, lNodeCount = KFbxGetSrcCount<KFbxNode>(pScene);

    for (lIndex=0; lIndex<lNodeCount; lIndex++)
    {
        KFbxNode* lNode = KFbxGetSrc<KFbxNode>(pScene, lIndex);

        if (lNode->GetGeometry())
        {
            int i, lVertexCacheDeformerCount = lNode->GetGeometry()->GetDeformerCount(KFbxDeformer::eVERTEX_CACHE);

            // There should be a maximum of 1 Vertex Cache Deformer for the moment
            lVertexCacheDeformerCount = lVertexCacheDeformerCount > 0 ? 1 : 0;

            for (i=0; i<lVertexCacheDeformerCount; ++i )
            {
                // Get the Point Cache object
                KFbxVertexCacheDeformer* lDeformer = static_cast<KFbxVertexCacheDeformer*>(lNode->GetGeometry()->GetDeformer(i, KFbxDeformer::eVERTEX_CACHE));
                if( !lDeformer ) continue;
                KFbxCache* lCache = lDeformer->GetCache();
                if( !lCache ) continue;

                // Process the point cache data only if the constraint is active
                if (lDeformer->IsActive())
                {
                    if (lCache->GetCacheFileFormat() == KFbxCache::ePC2)
                    {
                        // This code show how to convert from PC2 to MC point cache format
                        // turn it on if you need it.
#if 0
                        if (!lCache->ConvertFromPC2ToMC(KFbxCache::eMC_ONE_FILE,
                            KTime::GetFrameRate(pScene->GetGlobalTimeSettings().GetTimeMode())))
                        {
                            // Conversion failed, retrieve the error here
                            KString lTheErrorIs = lCache->GetError().GetLastErrorString();
                        }
#endif
                    }
                    else if (lCache->GetCacheFileFormat() == KFbxCache::eMC)
                    {
                        // This code show how to convert from MC to PC2 point cache format
                        // turn it on if you need it.
                        //#if 0
                        if (!lCache->ConvertFromMCToPC2(KTime::GetFrameRate(pScene->GetGlobalSettings().GetTimeMode()), 0))
                        {
                            // Conversion failed, retrieve the error here
                            KString lTheErrorIs = lCache->GetError().GetLastErrorString();
                        }
                        //#endif
                    }


                    // Now open the cache file to read from it
                    if (!lCache->OpenFileForRead())
                    {
                        // Cannot open file
                        KString lTheErrorIs = lCache->GetError().GetLastErrorString();

                        // Set the deformer inactive so we don't play it back
                        lDeformer->SetActive(false);
                    }
                }
            }
        }
    }
}

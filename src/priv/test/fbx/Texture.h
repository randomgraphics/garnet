#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <fbxfilesdk/kfbxplugins/kfbxtexture.h>

class VSTexture
{
	public:
		inline VSTexture()
		{
			mW = mH     = 0;
			mImageData  = NULL;
			mRefTexture = NULL;
		}

		~VSTexture()
		{
			delete mImageData;
		}

		unsigned int   mW;
		unsigned int   mH;
		unsigned char* mImageData;
		KFbxTexture*   mRefTexture;
};


#endif // #ifndef _TEXTURE_H






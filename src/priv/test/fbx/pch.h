#ifndef __GN_PCH_H__
#define __GN_PCH_H__
// *****************************************************************************
// \file    pch.h
// \brief   PCH header
// \author  chenlee (2005.10.1)
// *****************************************************************************

#define _CRT_SECURE_NO_WARNINGS

#include <fbxsdk.h>
#include <fbxfilesdk/kfbxio/kfbxiosettings.h>
#include <fbxfilesdk/kfbxio/kfbximporter.h>
#include <fbxfilesdk/kfbxplugins/kfbxsdkmanager.h>
#include <fbxfilesdk/kfbxplugins/kfbxscene.h>
#include <fbxfilesdk/kfbxplugins/kfbxtexture.h>
#include <fbxfilesdk/fbxfilesdk_nsuse.h>

#include <math.h>

#ifdef WIN32
#include <windows.h>
#endif

#define GLUT_DISABLE_ATEXIT_HACK
#if defined(__MACH__)
#include <GLUT/glut.h>
#else
#include "GL/glut.h"
#endif

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_PCH_H__

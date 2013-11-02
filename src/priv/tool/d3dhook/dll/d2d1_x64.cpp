#include <windows.h>
#include <stdio.h>
HINSTANCE mHinst = 0, mHinstDLL = 0;
extern "C" UINT_PTR mProcs[11] = {0};

LPCSTR mImportNames[] = {"D2D1ConvertColorSpace", "D2D1CreateDevice", "D2D1CreateDeviceContext", "D2D1CreateFactory", "D2D1InvertMatrix", "D2D1IsMatrixInvertible", "D2D1MakeRotateMatrix", "D2D1MakeSkewMatrix", "D2D1SinCos", "D2D1Tan", "D2D1Vec3Length"};
BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved ) {
	mHinst = hinstDLL;
	if ( fdwReason == DLL_PROCESS_ATTACH ) {
		mHinstDLL = LoadLibrary( "ori_.\d2d1.dll" );
		if ( !mHinstDLL )
			return ( FALSE );
		for ( int i = 0; i < 11; i++ )
			mProcs[ i ] = (UINT_PTR)GetProcAddress( mHinstDLL, mImportNames[ i ] );
	} else if ( fdwReason == DLL_PROCESS_DETACH ) {
		FreeLibrary( mHinstDLL );
	}
	return ( TRUE );
}

extern "C" void D2D1ConvertColorSpace_wrapper();
extern "C" void D2D1CreateDevice_wrapper();
extern "C" void D2D1CreateDeviceContext_wrapper();
extern "C" void D2D1CreateFactory_wrapper();
extern "C" void D2D1InvertMatrix_wrapper();
extern "C" void D2D1IsMatrixInvertible_wrapper();
extern "C" void D2D1MakeRotateMatrix_wrapper();
extern "C" void D2D1MakeSkewMatrix_wrapper();
extern "C" void D2D1SinCos_wrapper();
extern "C" void D2D1Tan_wrapper();
extern "C" void D2D1Vec3Length_wrapper();

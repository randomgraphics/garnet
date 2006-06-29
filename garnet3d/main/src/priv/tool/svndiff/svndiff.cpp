#if ( defined(_WIN32) || defined(WIN32) ) && !defined(_XBOX_VER)
#include <windows.h>
#include <stdio.h>
#include <process.h>

int main( int argc, const char * argv[] )
{
    // looking for bc2.exe
    HKEY key;
    char bc2[MAX_PATH];
    DWORD bufSize = MAX_PATH;
    if( ERROR_SUCCESS != RegOpenKeyExA(
        HKEY_CURRENT_USER,
        "Software\\Scooter Software\\Beyond Compare",
        0,
        KEY_QUERY_VALUE,
        &key ) )
    {
        fprintf( stderr, "Fail to open registry key: HKEY_CURRENT_USER\\Software\\Scooter Software\\Beyond Compare" );
        return -1;
    }

    if( ERROR_SUCCESS != RegQueryValueExA(
        key,
        "ExePath",
        0,
        0,
        (LPBYTE)bc2,
        &bufSize ) )
    {
        fprintf( stderr, "Fail to get registry value: HKEY_CURRENT_USER\\Software\\Scooter Software\\Beyond Compare\\ExePath" );
        RegCloseKey( key );
        return -1;
    }
    RegCloseKey( key );
    printf( "Run %s ...\n", bc2 );

    if( argc < 3 )
    {
        fprintf( stderr, "No enough command line parameters." );
        return -1;
    }

    const char * left = argv[argc-2];
    const char * right = argv[argc-1];
    printf( "%s VS. %s\n", left, right );
    _spawnl( _P_WAIT, bc2, bc2, left, right, NULL );

    // success
    return 0;
}
#else
#include <stdio.h>
int main() { printf( "no implementation on platform other then MS Windows." ); return -1; }
#endif

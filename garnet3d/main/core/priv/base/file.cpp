#include "pch.h"
#include "nativeFileDevice.h"
#include <pcrecpp.h>

// ****************************************************************************
//                   implementation of StdFile
// ****************************************************************************

//
size_t GN::StdFile::read( void * buffer, size_t size )
{
    GN_GUARD;
    size_t r = ::fread( buffer, 1, size, mFile );
    if ( (size_t)-1 == r ) GN_ERROR( "fread() failed!" );
    return r;
    GN_UNGUARD;
}
//
size_t GN::StdFile::write( const void * buffer, size_t size )
{
    GN_GUARD;
    size_t r = ::fwrite( buffer, 1, size, mFile );
    if ( (size_t)-1 == r ) GN_ERROR( "fwrite() failed!" );
    return r;
    GN_UNGUARD;
}

// ****************************************************************************
//                   implementation of AnsiFile
// ****************************************************************************

//
//
// ----------------------------------------------------------------------------
bool GN::AnsiFile::open(  const StrA & filename, int32_t openmode )
{
    GN_GUARD;

    // NOTE : these tables must always be syncronized with
    //        defintion of fopen_t ( see types/file.h )
    static const char * rw_table[] = {
        "",
        "r",
        "w",
        "r+",
    };
    static const char * fmt_table[] = {
        "",
        "b",
        "t",
    };

    // build open mode string
    StrA mode;
    mode  = rw_table[openmode&0xF];
    mode += fmt_table[openmode>>4];

    // do open
    return open( filename, mode );

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
bool GN::AnsiFile::open( const StrA & filename, const StrA & mode )
{
    GN_GUARD;

    // close previous file
    close();

    // check parameter(s)
    if( filename.empty() )
    {
        GN_ERROR( "empty filename!" );
        close(); return false;
    }

    // 打开文件
    FILE * fp = ::fopen( filename.cstr(), mode.cstr() );
    if( 0 == fp )
    {
        GN_ERROR( "fail to open file '%s' with mode '%s'!",
            filename.cstr(), mode.cstr() );
        close(); return false;
    }

    // 得到文件大小
    ::fseek( fp, 0, SEEK_END );
    mSize = ::ftell( fp );
    ::fseek( fp, 0, SEEK_SET );

    // success
    setName( filename );
    mFile = fp;
    return true;

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
void GN::AnsiFile::close() throw()
{
    GN_GUARD_ALWAYS;

    // close file
    if( mFile ) ::fclose( mFile );

    // clear data members
    mSize = 0;
    mFile = 0;
    setName( "" );

    GN_UNGUARD_ALWAYS_NO_THROW;
}

//
//
// ----------------------------------------------------------------------------
size_t GN::AnsiFile::read( void * buffer, size_t size )
{
    GN_GUARD;

    // check parameter(s)
    if( 0 == buffer && 0 != size )
    {
        GN_ERROR( "invalid parameter(s)!" );
        return static_cast<size_t>(-1);
    }

    // read file
    return ::fread( buffer, 1, size, mFile );

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
size_t GN::AnsiFile::write( const void * buffer, size_t size )
{
    GN_GUARD;

    // write to native file
    GN_ASSERT( mFile );
    return ::fwrite( buffer, 1, size, mFile );

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
bool GN::AnsiFile::eof() const
{
    GN_GUARD;

    if( 0 == mFile )
    {
        GN_WARN( "file not open!" );
        return true;
    }

    return 0 != feof( mFile );

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
bool GN::AnsiFile::seek( int offset, FileSeekMode origin )
{
    GN_GUARD;

    // NOTE : this table must be always synchronized with definition of
    //        fseek_t ( see types/file.h )
    static int seek_table[] =
    {
        SEEK_CUR,
        SEEK_END,
        SEEK_SET,
    };

    // check parameter
    if( origin >= NUM_FSEEKS )
    {
        GN_ERROR( "invalid seek origin!" );
        return false;
    }

    return 0 == ::fseek( mFile, offset, seek_table[origin] );

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
size_t GN::AnsiFile::tell() const
{
    GN_GUARD;

    size_t r = ::ftell( mFile );
    if( size_t(-1) == r ) GN_ERROR( "ftell() failed!" );
    return r;

    GN_UNGUARD;
}

// ****************************************************************************
//                   implementation of FileSys
// ****************************************************************************

//
//! convert '\\' to '/', then remove pending '/'
// ----------------------------------------------------------------------------
static inline GN::StrA sNormalizePath( const GN::StrA & path )
{
    GN::StrA ret;

    // convert '\\' to '/'
    ret.setCaps( path.size() );
    for( size_t i = 0; i < path.size(); ++i )
    {
        const char & ch = path[i];

        if( '\\' == ch )
        {
            ret.append( '/' );
        }
        else
        {
            ret.append( ch );
        }
    }

    // Detect pending '/'
    size_t n = ret.size();
    while( n > 0 && '/' == ret[n-1] )
    {
        --n;
    }

    // success
    return GN::StrA( ret.cstr(), n );
}

//!
//! Split fullpath to device name and path name
// ----------------------------------------------------------------------------
static inline bool
sParsePath( GN::StrA & device, GN::StrA & path, const GN::StrA & fullpath )
{
    GN_GUARD;

    // normalize the fullpath
    GN::StrA normalized = sNormalizePath(fullpath);

    // compile a regex parser
    static const pcrecpp::RE sParser( "^(([^:/?#]+)::)?([^?#]*)" );
    //                                  12             3

    std::string s2, s3;
    if( !sParser.FullMatch( normalized.cstr(), pcrecpp::no_arg, &s2, &s3 ) )
    {
        GN_ERROR( "'%s' is not a valid URL!", fullpath.cstr() );
        return false;
    }
    device = s2.c_str();
    path   = s3.c_str();

    // default device is "native"
    if( device.empty() )
    {
        device = "native";
    }

    // convert empty path to "."
    if( path.empty() )
    {
        path = ".";
    }

    // success
    return true;

    GN_UNGUARD;
}

// ****************************************************************************
//                        init / quit
// ****************************************************************************

//
//
// ----------------------------------------------------------------------------
bool GN::FileSys::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::FileSys, () );

    // create & register native file device
    AutoRef<FileDevice> nd( new detail::NativeFileDevice );
    if( !registerDevice( "native", nd.get(), false ) )
    {
        GN_ERROR( "fail to register native file device!" );
        quit(); return selfOK();
    }

    // success
    return selfOK();

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
void GN::FileSys::quit()
{
    GN_GUARD;

    // unregister all devices
    mDevices.clear();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// ****************************************************************************
//                        interface functions
// ****************************************************************************

//
//
// ----------------------------------------------------------------------------
bool GN::FileSys::registerDevice( const StrA & deviceName,
                                  const FileDevice * device,
                                  bool  override )
{
    GN_GUARD;

    // check parameters
    if( deviceName.empty() )
    {
        GN_ERROR( "Empty device name!" );
        return false;
    }
    if( NULL == device )
    {
        GN_ERROR( "Null device pointer!" );
        return false;
    }

    // find device
    DeviceMap::iterator i = mDevices.find( deviceName );
    if( mDevices.end() != i && !override )
    {
        GN_ERROR( "device %s already exist!", deviceName.cstr() );
        return false;
    }

    // insert to device map
    mDevices[deviceName].reset( device );

    // success
    return true;

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
void GN::FileSys::unregDevice( const StrA & deviceName )
{
    GN_GUARD;

    if( deviceName.empty() ) return;
    mDevices.erase( deviceName );

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
GN::AutoRef<GN::File>
GN::FileSys::openFile( const StrA & iPath, int mode ) const
{
    GN_GUARD;

    StrA device, path;
    if( !sParsePath( device, path, iPath ) )
    {
        return AutoRef<File>();
    }

    if( "native" == device )
    {
        DeviceMap::const_iterator di = mDevices.find( "native" );
        GN_ASSERT( mDevices.end() != di && !di->second.empty() );
        return di->second->openFile( path, mode );
    }
    else
    {
        GN_ERROR( "unknown device '%s'", device.cstr() );
        return AutoRef<File>();
    }

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
bool GN::FileSys::isExist( const StrA & iPath ) const
{
    GN_GUARD;

    StrA device, path;
    if( !sParsePath( device, path, iPath ) ) return false;

    if( "native" == device )
    {
        DeviceMap::const_iterator di = mDevices.find( "native" );
        GN_ASSERT( mDevices.end() != di && !di->second.empty() );
        return di->second->isExist( path );
    }
    else
    {
        GN_ERROR( "unknown device '%s'", device.cstr() );
        return false;
    }

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
bool GN::FileSys::isDir( const StrA & iPath ) const
{
    GN_GUARD;

    StrA device, path;
    if( ! sParsePath( device, path, iPath ) ) return false;

    if( "native" == device )
    {
        DeviceMap::const_iterator di = mDevices.find( "native" );
        GN_ASSERT( mDevices.end() != di && !di->second.empty() );
        return di->second->isDir( path );
    }
    else
    {
        GN_ERROR( "unknown device '%s'", device.cstr() );
        return false;
    }

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
void
GN::FileSys::findFiles( std::vector<StrA> & result,
                        const StrA & dirName,
                        const StrA & pattern,
                        bool         recursive,
                        bool         useRegex ) const
{
    GN_GUARD;

    StrA device, path;
    if( !sParsePath( device, path, dirName ) ) return;

    if( "native" == device )
    {
        DeviceMap::const_iterator di = mDevices.find( "native" );
        GN_ASSERT( mDevices.end() != di && !di->second.empty() );
        di->second->findFiles( result, path, pattern, recursive, useRegex );
    }
    else
    {
        GN_ERROR( "unknown device '%s'", device.cstr() );
    }

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
GN::StrA GN::FileSys::normalizePath( const StrA & path ) const
{
    return sNormalizePath( path );
}

//
//
// ----------------------------------------------------------------------------
GN::StrA GN::FileSys::rel2abs( const StrA & relPath, const StrA & base ) const
{
    GN_GUARD;

    // just need to resolve relPath
    if( base.empty() )
    {
        StrA d1, p1;
        if( !sParsePath(d1,p1,relPath) ) return "";

        DeviceMap::const_iterator di = mDevices.find( d1 );
        if( mDevices.end() == di )
        {
            GN_ERROR( "unknown device '%s'", d1.cstr() );
            return "";
        }

        GN_ASSERT( !di->second.empty() );

        return d1 + "::" + di->second->rel2abs( p1 );
    }

    // analyze relPath and base
    StrA d1, d2, p1, p2;
    if( !sParsePath(d1,p1,relPath) || !sParsePath(d2,p2,base) )
    {
        return "";
    }

    // relPath and relPath must have same device name
    if( d1 != d2 )
    {
        GN_ERROR( "relPath and base must be on same device!" );
        return "";
    }

    // find the device
    DeviceMap::const_iterator di = mDevices.find( d1 );
    if( mDevices.end() == di )
    {
        GN_ERROR( "unknown device '%s'", relPath.cstr() );
        return "";
    }
    GN_ASSERT( !di->second.empty() );

    // resolve base path
    StrA abspath = di->second->rel2abs( p1, p2 );
    return abspath.empty() ? "" : d1 + "::" + abspath;

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
GN::StrA GN::FileSys::getParent( const StrA & iPath ) const
{
    GN_GUARD;

    StrA device, path;
    if( ! sParsePath( device, path, iPath ) ) return false;

    size_t i = path.size();
    while( i > 0 && '/' != path[i-1] )
    {
        --i;
    }

    // success
    return device + "::" + StrA(path.cstr(),i);

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
GN::StrA GN::FileSys::getDevice( const StrA & iPath ) const
{
    GN_GUARD;

    StrA device, path;
    if( !sParsePath( device, path, iPath ) ) return "";

    // success
    return device;

    GN_UNGUARD;
}

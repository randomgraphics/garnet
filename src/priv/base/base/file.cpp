#include "pch.h"

using namespace GN;

static Logger * sLogger = GetLogger("GN.base.File");

static FILE * sOpenFile( const char * filename, const char * mode )
{
    FILE * fp;

#if GN_MSVC8
    if( 0 != fopen_s( &fp, filename, mode ) ) fp = 0;
#else
    fp = fopen( filename, mode );
#endif

    if( 0 == fp )
    {
        GN_ERROR(sLogger)(
            "fopen() fail to open file '%s' with mode '%s' : %s.",
            filename,
            mode,
            GN::Errno2Str( errno ) );
    }

    return fp;
}

// *****************************************************************************
//                   implementation of StdFile
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::StdFile::read( void * buffer, size_t size, size_t * readen )
{
    GN_GUARD;

    // check parameter(s)
    if( 0 == buffer && 0 != size )
    {
        GN_ERROR(sLogger)( "invalid parameter(s)!" );
        return false;
    }

    if( 0 == mFile )
    {
        GN_ERROR(sLogger)( "NULL file pointer!" );
        return false;
    }

    if( eof() )
    {
        GN_VERBOSE(sLogger)( "Already reach the end of the file." );
        if( readen ) *readen = 0;
        return true;
    }

    size_t r = ::fread( buffer, 1, size, mFile );

    if( (size_t)-1 == r )
    {
        GN_ERROR(sLogger)( "%s : fread() failed!", name() );
        return false;
    }

    // success
    if( readen ) *readen = r;
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::StdFile::write( const void * buffer, size_t size, size_t * written )
{
    GN_GUARD;

    // check parameter(s)
    if( 0 == buffer && 0 != size )
    {
        GN_ERROR(sLogger)( "invalid parameter(s)!" );
        return false;
    }

    if( 0 == mFile )
    {
        GN_ERROR(sLogger)( "NULL file pointer!" );
        return false;
    }

    size_t r = ::fwrite( buffer, 1, size, mFile );
    if ( (size_t)-1 == r )
    {
        GN_ERROR(sLogger)( "%s: fwrite() failed!", name() );
        return false;
    }

    // success
    if( written ) *written = r;
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::StdFile::eof() const
{
    GN_GUARD;

    if( 0 == mFile )
    {
        GN_ERROR(sLogger)( "NULL file pointer!" );
        return true;
    }

    return 0 != feof( mFile );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::StdFile::seek( size_t offset, FileSeek origin )
{
    GN_GUARD;

    // NOTE : this table must be always synchronized with definition of
    //        FileSeek
    static int seek_table[] =
    {
        SEEK_CUR,
        SEEK_END,
        SEEK_SET,
    };

    // check parameter
    if( origin >= FileSeek::NUM_MODES )
    {
        GN_ERROR(sLogger)( "%s: invalid seek origin!", name() );
        return false;
    }

    if( 0 == mFile )
    {
        GN_ERROR(sLogger)( "NULL file pointer!" );
        return false;
    }

    if( 0 != ::fseek( mFile, (int)offset, seek_table[origin] ) )
    {
        GN_ERROR(sLogger)( "%s : fseek() failed!", name() );
        return false;
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
size_t GN::StdFile::tell() const
{
    GN_GUARD;

    if( 0 == mFile )
    {
        GN_ERROR(sLogger)( "NULL file pointer!" );
        return (size_t)-1;
    }

    size_t r = ::ftell( mFile );

    if( (size_t)-1 == r )
    {
        GN_ERROR(sLogger)( "%s : ftell() failed!", name() );
    }

    return r;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
size_t GN::StdFile::size() const
{
    GN_GUARD;

    if( 0 == mFile )
    {
        GN_ERROR(sLogger)( "NULL file pointer!" );
        return 0;
    }

    // store current file position
    long oldPos = ::ftell( mFile );
    if( -1 == oldPos )
    {
        GN_ERROR(sLogger)( "%s : fail to get current file position!", name() );
        return 0;
    }

    // seek to the end of the file
    if( 0 == ::fseek( mFile, SEEK_END, 0 ) )
    {
        GN_ERROR(sLogger)( "%s : fail to seek to the end of file!", name() );
        return 0;
    }

    // get new position
    size_t newPos = ::ftell( mFile );

    // restore file position
    if( 0 == ::fseek( mFile, SEEK_SET, oldPos ) )
    {
        GN_ERROR(sLogger)( "%s : fail to restore file position!", name() );
        return 0;
    }

    // success
    return newPos;

    GN_UNGUARD;
}

// *****************************************************************************
//                   implementation of DiskFile
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::DiskFile::open( const StrA & filename, const StrA & mode )
{
    GN_GUARD_ALWAYS;

    // close previous file
    close();

    // check parameter(s)
    if( filename.Empty() )
    {
        GN_ERROR(sLogger)( "empty filename!" );
        close(); return false;
    }

    // 打开文件
    FILE * fp = sOpenFile( filename.GetRawPtr(), mode.GetRawPtr() );
    if( 0 == fp )
    {
        close(); return false;
    }

    // 得到文件大小
    ::fseek( fp, 0, SEEK_END );
    mSize = ::ftell( fp );
    ::fseek( fp, 0, SEEK_SET );

    // success
    setFile( fp );
    setName( filename );
    return true;

    GN_UNGUARD_ALWAYS_DO( return false; );
}

//
//
// -----------------------------------------------------------------------------
void GN::DiskFile::close() throw()
{
    GN_GUARD_ALWAYS;

    // close file
    if( getFILE() ) ::fclose( getFILE() );

    // clear data members
    mSize = 0;
    setFile( 0 );
    setName( "" );

    GN_UNGUARD_ALWAYS_NO_THROW;
}

// *****************************************************************************
//                   implementation of TempFile
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::TempFile::open( const StrA & prefix, const StrA & mode, Behavior beh )
{
    GN_GUARD_ALWAYS;

    mBehavior = beh;

#if GN_MSWIN

    // generate temporary file name
    AutoMallocPtr<const char> filename( _tempnam( NULL, "GN_" + prefix ) );

    // open the file
    FILE * fp = sOpenFile( filename.GetRawPtr(), mode );
    if( 0 == fp )
    {
        close();
        return false;
    }

    // success
    setFile( fp );
    setName( filename.GetRawPtr() );
    return true;

#else

    StrA fileNameTempl = "/tmp/GN_" + prefix + "XXXXXX";
    mFileDesc = mkstemp( fileNameTempl );
    if( -1 == mFileDesc )
    {
        GN_ERROR(sLogger)( "Fail to generate temporary file name." );
        return false;
    }

    // unlink the temporary file name. So the file will be deleted automatically after beging closed.
    if( AUTO_DELETE == beh )
    {
        unlink( fileNameTempl );
    }

    // open file
    FILE * fp = fdopen( mFileDesc, mode );
    if( 0 == fp )
    {
        GN_ERROR(sLogger)( "fail to open file '%s' with mode '%s' : %s",
            fileNameTempl.GetRawPtr(), mode.GetRawPtr(), Errno2Str( errno ) );
        close();
        return false;
    }

    // success
    setFile( fp );
    setName( fileNameTempl );
    return true;

#endif

    GN_UNGUARD_ALWAYS_DO( return false; );
}

//
//
// -----------------------------------------------------------------------------
void GN::TempFile::close()
{
    GN_GUARD_ALWAYS;

    // close file pointer
    if( getFILE() ) ::fclose( getFILE() );
    setFile( 0 );
    setName( "" );

#if GN_POSIX
    // close file descriptor
    if( -1 != mFileDesc )
    {
        ::close( mFileDesc );

        mFileDesc = -1;
    }
#endif

    GN_UNGUARD_ALWAYS;
}

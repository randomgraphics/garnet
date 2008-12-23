#include "pch.h"

static FILE * sOpenFile( const char * filename, const char * mode )
{
#if GN_MSVC8
    FILE * fp;
    if( 0 != fopen_s( &fp, filename, mode ) ) return 0;
    return fp;
#else
    return fopen( filename, mode );
#endif
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
        GN_ERROR(myLogger())( "invalid parameter(s)!" );
        return false;
    }

    if( 0 == mFile )
    {
        GN_ERROR(myLogger())( "NULL file pointer!" );
        return false;
    }

    if( eof() )
    {
        GN_VERBOSE(myLogger())( "Already reach the end of the file." );
        if( readen ) *readen = 0;
        return true;
    }

    size_t r = ::fread( buffer, 1, size, mFile );

    if( (size_t)-1 == r )
    {
        GN_ERROR(myLogger())( "%s : fread() failed!", name().cptr() );
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
        GN_ERROR(myLogger())( "invalid parameter(s)!" );
        return false;
    }

    if( 0 == mFile )
    {
        GN_ERROR(myLogger())( "NULL file pointer!" );
        return false;
    }

    size_t r = ::fwrite( buffer, 1, size, mFile );
    if ( (size_t)-1 == r )
    {
        GN_ERROR(myLogger())( "%s: fwrite() failed!", name().cptr() );
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
        GN_ERROR(myLogger())( "NULL file pointer!" );
        return true;
    }

    return 0 != feof( mFile );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::StdFile::seek( int offset, FileSeekMode origin )
{
    GN_GUARD;

    // NOTE : this table must be always synchronized with definition of
    //        FileSeekMode
    static int seek_table[] =
    {
        SEEK_CUR,
        SEEK_END,
        SEEK_SET,
    };

    // check parameter
    if( origin >= NUM_FILE_SEEK_MODES )
    {
        GN_ERROR(myLogger())( "%s: invalid seek origin!", name().cptr() );
        return false;
    }

    if( 0 == mFile )
    {
        GN_ERROR(myLogger())( "NULL file pointer!" );
        return false;
    }

    if( 0 != ::fseek( mFile, offset, seek_table[origin] ) )
    {
        GN_ERROR(myLogger())( "%s : fseek() failed!", name().cptr() );
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
        GN_ERROR(myLogger())( "NULL file pointer!" );
        return (size_t)-1;
    }

    size_t r = ::ftell( mFile );

    if( (size_t)-1 == r )
    {
        GN_ERROR(myLogger())( "%s : ftell() failed!", name().cptr() );
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
        GN_ERROR(myLogger())( "NULL file pointer!" );
        return 0;
    }

    // store current file position
    long oldPos = ::ftell( mFile );
    if( -1 == oldPos )
    {
        GN_ERROR(myLogger())( "%s : fail to get current file position!", name().cptr() );
        return 0;
    }

    // seek to the end of the file
    if( 0 == ::fseek( mFile, SEEK_END, 0 ) )
    {
        GN_ERROR(myLogger())( "%s : fail to seek to the end of file!", name().cptr() );
        return 0;
    }

    // get new position
    size_t newPos = ::ftell( mFile );

    // restore file position
    if( 0 == ::fseek( mFile, SEEK_SET, oldPos ) )
    {
        GN_ERROR(myLogger())( "%s : fail to restore file position!", name().cptr() );
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
bool GN::DiskFile::open(  const StrA & filename, SInt32 openmode )
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
// -----------------------------------------------------------------------------
bool GN::DiskFile::open( const StrA & filename, const StrA & mode )
{
    GN_GUARD;

    // close previous file
    close();

    // check parameter(s)
    if( filename.empty() )
    {
        GN_ERROR(myLogger())( "empty filename!" );
        close(); return false;
    }

    // 打开文件
    FILE * fp = sOpenFile( filename.cptr(), mode.cptr() );
    if( 0 == fp )
    {
        GN_ERROR(myLogger())( "fail to open file '%s' with mode '%s'!",
            filename.cptr(), mode.cptr() );
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

    GN_UNGUARD;
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

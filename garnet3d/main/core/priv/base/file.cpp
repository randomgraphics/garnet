#include "pch.h"

// *****************************************************************************
//                   implementation of StdFile
// *****************************************************************************

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

// *****************************************************************************
//                   implementation of AnsiFile
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
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
// -----------------------------------------------------------------------------
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
// -----------------------------------------------------------------------------
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
// -----------------------------------------------------------------------------
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
// -----------------------------------------------------------------------------
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
// -----------------------------------------------------------------------------
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
// -----------------------------------------------------------------------------
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
// -----------------------------------------------------------------------------
size_t GN::AnsiFile::tell() const
{
    GN_GUARD;

    size_t r = ::ftell( mFile );
    if( size_t(-1) == r ) GN_ERROR( "ftell() failed!" );
    return r;

    GN_UNGUARD;
}

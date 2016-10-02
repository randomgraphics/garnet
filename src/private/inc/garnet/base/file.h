#ifndef __GN_BASE_FILE_H__
#define __GN_BASE_FILE_H__
// *****************************************************************************
/// \file
/// \brief   general file stream class
/// \author  chenlee (2005.4.20)
// *****************************************************************************

#include <stdio.h>

namespace GN
{
    ///
    /// 文件定位模式
    ///
    struct FileSeek
    {
        enum ENUM
        {
            CUR,       ///< same as standard SEEK_CUR
            END,       ///< same as standard SEEK_END
            SET,       ///< same as standard SEEK_SET
            NUM_MODES, ///< number of avaliable seeking modes
        };

        GN_DEFINE_ENUM_CLASS_HELPERS( FileSeek, ENUM );
    };

    ///
    /// File operation caps
    ///
    union FileOperationCaps
    {
        unsigned char u8; ///< File operation caps as unsigned char
        signed char   i8; ///< File operation caps as char
        struct
        {
            bool read      : 1; ///< support reading
            bool write     : 1; ///< support writing
            bool eof       : 1; ///< support EOF quering
            bool seek      : 1; ///< support position seeking
            bool tell      : 1; ///< support position querying
            bool size      : 1; ///< support size querying
            bool map       : 1; ///< support memory-mapping
            bool _reserved : 1; ///< reserved.
        };
    };

    ///
    /// basic file interface used throughout of the garnet system
    ///
    /// 用户实现该文件类时，不一定要实现下面的所有操作。
    ///
    /// TODO: replace size_t with uint64 or sint64, to support large file on x86 system
    ///
    struct File : public NoCopy
    {
        ///
        /// Get file operation caps
        ///
        const FileOperationCaps & caps() const { return mCaps; }

        ///
        /// 读取size个字节到buffer中
        ///
        virtual bool read( void * /*buffer*/, size_t /*size*/, size_t * /*readen*/ ) = 0;

        ///
        /// 向文件中写入size个字节
        ///
        /// \return   -1 means failed
        ///
        virtual bool write( const void * /*buffer*/, size_t /*size*/, size_t * /*written*/ ) = 0;

        ///
        /// 是否已经到文件结尾. Return true, if something goes wrong.
        ///
        virtual bool eof() const = 0;

        ///
        /// write string to file
        ///
        inline bool print( const StrA & s ) { return write( s.rawptr(), s.size(), 0 ); }

        ///
        /// write formatted string to file
        ///
        inline bool printf( const char * fmt, ... )
        {
            StrA s;
            va_list arglist;
            va_start( arglist, fmt );
            s.formatv( fmt, arglist );
            va_end( arglist );
            return print( s );
        }

        ///
        /// 设定文件读写游标的位置
        ///
        /// \return   return false if error
        ///
        virtual bool seek( size_t /*offset*/, FileSeek /*origin*/ ) = 0;

        ///
        /// 返回当前文件读写游标的位置. Return -1 if something goes wrong.
        ///
        virtual size_t tell() const = 0;

        ///
        /// 返回文件的总长度. Return 0 if something goes wrong.
        ///
        virtual size_t size() const = 0;

        ///
        /// get memory mapping of the file content. Return NULL if failed.
        ///
        virtual void * map( size_t offset, size_t length, bool readonly ) = 0;

        ///
        /// unmap file content
        ///
        virtual void unmap() = 0;

        ///
        /// return file name string
        ///
        const StrA & name() const { return mName; }

        ///
        /// dtor
        ///
        virtual ~File() {}

    protected :

        ///
        /// protected ctor
        ///
        File() { mCaps.u8 = 0; }

        ///
        /// Set file name
        ///
        void setName( const StrA & name ) { mName = name; }

        ///
        /// Set operation caps
        ///
        void setCaps( const FileOperationCaps & caps ) { mCaps = caps; }

        ///
        /// Set operation caps
        ///
        void setCaps( int caps ) { mCaps.i8 = (signed char)caps; }

        ///
        /// File logger
        ///
        static inline Logger * myLogger() { static Logger * logger = GetLogger("GN.base.File"); return logger; }

    private:

        StrA              mName;
        FileOperationCaps mCaps;
    };

    ///
    /// stream operator
    ///
    inline File & operator<<( File & fp, int i )
    {
        char buf[256];
        str::formatTo( buf, 256, "%d", i );
        fp.write( buf, str::length(buf), 0 );
        return fp;
    }

    ///
    /// stream operator
    ///
    inline File & operator<<( File & fp, size_t s )
    {
        char buf[256];
        str::formatTo( buf, 256, "%Iu", s );
        fp.write( buf, str::length(buf), 0 );
        return fp;
    }

    ///
    /// stream operator
    ///
    inline File & operator<<( File & fp, const char * s )
    {
        if( 0 == s ) return fp;
        fp.write( s, str::length(s), 0 );
        return fp;
    }

    ///
    /// stream operator
    ///
    inline File & operator<<( File & fp, const StrA & s )
    {
        if( s.empty() ) return fp;
        fp.write( s.rawptr(), s.size(), 0 );
        return fp;
    }

    ///
    /// 用File包装的standard file stream
    ///
    class GN_API StdFile : public File
    {
        FILE * mFile;

    protected:

        ///
        /// Change the internal file pointer
        ///
        void setFile( FILE * fp )
        {
            mFile = fp;
            if( stdin == fp ) setName( "stdin" );
            else if( stdout == fp ) setName( "stdout" );
            else if( stderr == fp ) setName( "stderr" );
            else setName( str::format( "#%p", fp ) );
        }

    public :

        ///
        /// constructor
        ///
        StdFile( FILE * fp )
        {
            setCaps( 0x3F ); // support all operations, except mapping
            setFile(fp);
        }

        ///
        /// get internal file pointer
        ///
        FILE * getFILE() const { return mFile; }

        ///
        /// Convert to ANSI FILE *
        ///
        operator FILE* () const { return mFile; }

        // from File
    public:
        bool read( void *, size_t, size_t* );
        bool write( const void * buffer, size_t size, size_t* );
        bool eof() const;
        bool seek( size_t, FileSeek );
        size_t tell() const;
        size_t size() const;
        void * map( size_t, size_t, bool ) { GN_ERROR(myLogger())( "StdFile: does not support memory mapping operation!" ); return 0; }
        void unmap() { GN_ERROR(myLogger())( "StdFile: does not support memory mapping operation!" ); }
    };

    ///
    /// disk file class
    ///
    class GN_API DiskFile : public StdFile
    {
        size_t mSize;
    public:

        DiskFile() : StdFile(0), mSize(0) {}
        ~DiskFile() { close(); }

        ///
        /// open a file
        ///
        /// \param fname File name
        /// \param mode  ANSI compatible open mode, such as "r", "w+".
        ///
        bool open( const StrA & fname, const StrA & mode );

        ///
        /// close the file
        ///
        void close() throw();

        ///
        /// Convert to ANSI FILE *
        ///
        operator FILE* () const { return getFILE(); }

        // from File
    public:
        size_t size() const { return mSize; }
    };

    ///
    /// temporary file
    ///
    class GN_API TempFile : public StdFile
    {
    public:

        ///
        /// Temporary file behavior flags
        ///
        enum Behavior
        {
            AUTO_DELETE,    ///< The temporary file will be deleted automatically after file is closed.
            MANUAL_DELETE,  ///< The temporary file will remain on the disc.
        };

        /// default constructor
        TempFile() : StdFile(0), mFileDesc(-1) {};

        ///
        /// open a temporary file with user specified prefix
        ///
        bool open( const StrA & prefix, const StrA & mode, Behavior behavior );

        ///
        /// close the temporary file
        ///
        void close();

        ///
        /// Convert to ANSI FILE *
        ///
        operator FILE*() const { return getFILE(); }

    private:
        Behavior mBehavior;
        int      mFileDesc; ///< file descriptor (used on linux system only)
    };

    ///
    /// file class that wraps a fixed-sized memory buffer
    ///
    template< typename T >
    class MemFile : public File
    {
        uint8 * mStart;
        uint8 * mPtr;
        size_t  mSize;

    public:

        /// \name ctor/dtor
        //@{
        MemFile( T * buf = 0, size_t size = 0, const StrA & name = "" )
            : mStart((uint8*)buf)
            , mPtr((uint8*)buf)
            , mSize(size)
        {
            setCaps( 0xFF ); // support all operations
            setName(name);
        }
        ~MemFile() {}
        //@}

        ///
        /// reset memory buf
        ///
        void reset( T * buf = 0, size_t size = 0, const StrA & name = "" );

        /// \name from File
        //@{
        bool read( void *, size_t, size_t* );
        bool write( const void * buffer, size_t size, size_t* );
        bool eof() const { return (mStart+mSize) == mPtr; }
        bool seek( size_t offset, FileSeek origin );
        size_t tell() const { return mPtr - mStart; }
        size_t size() const { return mSize; }
        void * map( size_t offset, size_t length, bool )
        {
            if( offset >= mSize || (offset + length) > mSize )
            {
                GN_ERROR(myLogger())( "invalid mapping range!" );
                return 0;
            }
            return mStart + offset;
        }
        void unmap() {}
        //@}
    };

    ///
    /// File class the wraps a vector class
    ///
    class VectorFile : public File
    {
        DynaArray<uint8> mBuffer;
        size_t mCursor;

    public:

        ///
        /// ctor
        ///
        VectorFile() : mCursor(0)
        {
            setCaps( 0xFF ); // support all operations
        }

        ///
        /// dtor
        ///
        ~VectorFile() {}

        /// \name from File
        //@{
        bool read( void *, size_t, size_t* );
        bool write( const void * buffer, size_t size, size_t* );
        bool eof() const { return mBuffer.size() == mCursor; }
        bool seek( size_t offset, FileSeek origin );
        size_t tell() const { return mCursor; }
        size_t size() const { return mBuffer.size(); }
        void * map( size_t offset, size_t length, bool )
        {
            if( offset >= mBuffer.size() || (offset + length) > mBuffer.size() )
            {
                GN_ERROR(myLogger())( "invalid mapping range!" );
                return 0;
            }
            return &mBuffer[offset];
        }
        void unmap() {}
        //@}
    };
}

#include "file.inl"

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_FILE_H__

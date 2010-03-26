#ifndef __GN_BASE_FILE_H__
#define __GN_BASE_FILE_H__
// *****************************************************************************
/// \file
/// \brief   general file stream class
/// \author  chenlee (2005.4.20)
// *****************************************************************************

#include <map>

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
    /// TODO: replace size_t with UInt64 or SInt64, to support large file on x86 system
    ///
    struct File : public NoCopy
    {
        ///
        /// Get file operation caps
        ///
        const FileOperationCaps & GetCaps() const { return mCaps; }

        ///
        /// 读取size个字节到buffer中
        ///
        virtual bool Read( void * /*buffer*/, size_t /*size*/, size_t * /*readen*/ ) = 0;

        ///
        /// 向文件中写入size个字节
        ///
        /// \return   -1 means failed
        ///
        virtual bool Write( const void * /*buffer*/, size_t /*size*/, size_t * /*written*/ ) = 0;

        ///
        /// 是否已经到文件结尾. Return true, if something goes wrong.
        ///
        virtual bool Eof() const = 0;

        ///
        /// Write string to file
        ///
        inline bool Print( const StrA & s ) { return Write( s.ToRawPtr(), s.Size(), 0 ); }

        ///
        /// Write formatted string to file
        ///
        inline bool Printf( const char * fmt, ... )
        {
            StrA s;
            va_list arglist;
            va_start( arglist, fmt );
            s.FormatV( fmt, arglist );
            va_end( arglist );
            return Print( s );
        }

        ///
        /// 设定文件读写游标的位置
        ///
        /// \return   return false if error
        ///
        virtual bool Seek( size_t /*offset*/, FileSeek /*origin*/ ) = 0;

        ///
        /// 返回当前文件读写游标的位置. Return -1 if something goes wrong.
        ///
        virtual size_t Tell() const = 0;

        ///
        /// 返回文件的总长度. Return 0 if something goes wrong.
        ///
        virtual size_t Size() const = 0;

        ///
        /// get memory mapping of the file content. Return NULL if failed.
        ///
        virtual void * Map( size_t offset, size_t length, bool readonly ) = 0;

        ///
        /// Unmap file content
        ///
        virtual void Unmap() = 0;

        ///
        /// return file Name string
        ///
        const char * Name() const { return mName; }

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
        /// Set file Name
        ///
        void SetName( const StrA & name ) { mName = name; }

        ///
        /// Set operation caps
        ///
        void SetCaps( const FileOperationCaps & caps ) { mCaps = caps; }

        ///
        /// Set operation caps
        ///
        void SetCaps( int caps ) { mCaps.i8 = (signed char)caps; }

        ///
        /// File logger
        ///
        static inline Logger * MyLogger() { static Logger * logger = GetLogger("GN.base.File"); return logger; }

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
        StringPrintf( buf, 256, "%d", i );
        fp.Write( buf, StringLength(buf), 0 );
        return fp;
    }

    ///
    /// stream operator
    ///
    inline File & operator<<( File & fp, size_t s )
    {
        char buf[256];
        StringPrintf( buf, 256, "%Iu", s );
        fp.Write( buf, StringLength(buf), 0 );
        return fp;
    }

    ///
    /// stream operator
    ///
    inline File & operator<<( File & fp, const char * s )
    {
        if( 0 == s ) return fp;
        fp.Write( s, StringLength(s), 0 );
        return fp;
    }

    ///
    /// stream operator
    ///
    inline File & operator<<( File & fp, const StrA & s )
    {
        if( s.Empty() ) return fp;
        fp.Write( s.ToRawPtr(), s.Size(), 0 );
        return fp;
    }

    ///
    /// 用File包装的standard file stream
    ///
    class StdFile : public File
    {
        FILE * mFile;

    protected:

        ///
        /// Change the internal file pointer
        ///
        void setFile( FILE * fp )
        {
            mFile = fp;
            if( stdin == fp ) SetName( "stdin" );
            else if( stdout == fp ) SetName( "stdout" );
            else if( stderr == fp ) SetName( "stderr" );
            else SetName( StringFormat( "#%p", fp ) );
        }

    public :

        ///
        /// constructor
        ///
        StdFile( FILE * fp )
        {
            SetCaps( 0x3F ); // support all operations, except mapping
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
        bool Read( void *, size_t, size_t* );
        bool Write( const void * buffer, size_t size, size_t* );
        bool Eof() const;
        bool Seek( size_t, FileSeek );
        size_t Tell() const;
        size_t Size() const;
        void * Map( size_t, size_t, bool ) { GN_ERROR(MyLogger())( "StdFile: does not support memory mapping operation!" ); return 0; }
        void Unmap() { GN_ERROR(MyLogger())( "StdFile: does not support memory mapping operation!" ); }
    };

    ///
    /// disk file class
    ///
    class DiskFile : public StdFile
    {
        size_t mSize;
    public:

        DiskFile() : StdFile(0), mSize(0) {}
        ~DiskFile() { Close(); }

        ///
        /// Open a file
        ///
        /// \param fname File Name
        /// \param mode  ANSI compatible Open mode, such as "r", "w+".
        ///
        bool Open( const StrA & fname, const StrA & mode );

        ///
        /// Close the file
        ///
        void Close() throw();

        ///
        /// Convert to ANSI FILE *
        ///
        operator FILE* () const { return getFILE(); }

        // from File
    public:
        size_t Size() const { return mSize; }
    };

    ///
    /// temporary file
    ///
    class TempFile : public StdFile
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
        /// Open a temporary file with user specified prefix
        ///
        bool Open( const StrA & prefix, const StrA & mode, Behavior behavior );

        ///
        /// Close the temporary file
        ///
        void Close();

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
        UInt8 * mStart;
        UInt8 * mPtr;
        size_t  mSize;

    public:

        /// \Name ctor/dtor
        //@{
        MemFile( T * buf = 0, size_t size = 0, const StrA & name = "" )
            : mStart((UInt8*)buf)
            , mPtr((UInt8*)buf)
            , mSize(size)
        {
            SetCaps( 0xFF ); // support all operations
            SetName(name);
        }
        ~MemFile() {}
        //@}

        ///
        /// reset memory buf
        ///
        void Reset( T * buf = 0, size_t size = 0, const StrA & name = "" );

        /// \Name from File
        //@{
        bool Read( void *, size_t, size_t* );
        bool Write( const void * buffer, size_t size, size_t* );
        bool Eof() const { return (mStart+mSize) == mPtr; }
        bool Seek( size_t offset, FileSeek origin );
        size_t Tell() const { return mPtr - mStart; }
        size_t Size() const { return mSize; }
        void * Map( size_t offset, size_t length, bool )
        {
            if( offset >= mSize || (offset + length) > mSize )
            {
                GN_ERROR(MyLogger())( "invalid mapping range!" );
                return 0;
            }
            return mStart + offset;
        }
        void Unmap() {}
        //@}
    };

    ///
    /// File class the wraps a vector class
    ///
    class VectorFile : public File
    {
        DynaArray<UInt8> mBuffer;
        size_t mCursor;

    public:

        ///
        /// ctor
        ///
        VectorFile() : mCursor(0)
        {
            SetCaps( 0xFF ); // support all operations
        }

        ///
        /// dtor
        ///
        ~VectorFile() {}

        /// \Name from File
        //@{
        bool Read( void *, size_t, size_t* );
        bool Write( const void * buffer, size_t size, size_t* );
        bool Eof() const { return mBuffer.Size() == mCursor; }
        bool Seek( size_t offset, FileSeek origin );
        size_t Tell() const { return mCursor; }
        size_t Size() const { return mBuffer.Size(); }
        void * Map( size_t offset, size_t length, bool )
        {
            if( offset >= mBuffer.Size() || (offset + length) > mBuffer.Size() )
            {
                GN_ERROR(MyLogger())( "invalid mapping range!" );
                return 0;
            }
            return &mBuffer[offset];
        }
        void Unmap() {}
        //@}
    };
}

#include "file.inl"

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_FILE_H__

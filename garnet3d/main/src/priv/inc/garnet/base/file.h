#ifndef __GN_BASE_FILE_H__
#define __GN_BASE_FILE_H__
// *****************************************************************************
/// \file    file.h
/// \brief   general file stream class
/// \author  chenlee (2005.4.20)
// *****************************************************************************

#include <vector>
#include <map>

namespace GN
{
    ///
    /// 文件打开模式
    ///
    /// 有效的模式必须是一个读写标志加上一个格式标志
    ///
    enum FileOpenMode
    {
        FOPEN_NONE  = 0x00, ///< indicate a invalid value

        // 读写标志
        FOPEN_READ  = 0x01, ///< 只读模式（文件必须存在），same as "r"
        FOPEN_WRITE = 0x02, ///< 只写模式，same as "w"
        FOPEN_RW    = 0x03, ///< 读写模式（文件必须存在），same as "r+"

        // 格式标志
        FOPEN_BIN   = 0x10, ///< 二进制模式
        FOPEN_TXT   = 0x20, ///< 文本模式
    };

    ///
    /// 文件定位模式
    ///
    enum FileSeekMode
    {
        FSEEK_CUR,     ///< same as standard SEEK_CUR
        FSEEK_END,     ///< same as standard SEEK_END
        FSEEK_SET,     ///< same as standard SEEK_SET
        NUM_FSEEKS,    ///< number of avaliable seeking modes
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
    struct File : public NoCopy
    {
        ///
        /// Get file operation caps
        ///
        const FileOperationCaps & getCaps() const { return mCaps; }
        
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
        /// 设定文件读写游标的位置
        ///
        /// \return   return false if error
        ///
        virtual bool seek( int /*offset*/, FileSeekMode /*origin*/ ) = 0;

        ///
        /// 返回当前文件读写游标的位置. Return 0 if something goes wrong.
        ///
        /// \return
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

    protected :

        // ctor / dtor
        File()          { mCaps.u8 = 0; }
        virtual ~File() {}

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
        static Logger * sLogger;

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
        strPrintf( buf, 256, "%d", i );
        fp.write( buf, strLen(buf), 0 );
        return fp;
    }

    ///
    /// stream operator
    ///
    inline File & operator<<( File & fp, size_t s )
    {
        char buf[256];
        strPrintf( buf, 256, "%Iu", s );
        fp.write( buf, strLen(buf), 0 );
        return fp;
    }

    ///
    /// stream operator
    ///
    inline File & operator<<( File & fp, const char * s )
    {
        if( 0 == s ) return fp;
        fp.write( s, strLen(s), 0 );
        return fp;
    }

    ///
    /// stream operator
    ///
    inline File & operator<<( File & fp, const StrA & s )
    {
        if( s.empty() ) return fp;
        fp.write( s.cptr(), s.size(), 0 );
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
            if( stdin == fp ) setName( "stdin" );
            else if( stdout == fp ) setName( "stdout" );
            else if( stderr == fp ) setName( "stderr" );
            else setName( strFormat( "#%p", fp ) );
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
        bool seek( int, FileSeekMode );
        size_t tell() const;
        size_t size() const;
        void * map( size_t, size_t, bool ) { GN_ERROR(sLogger)( "StdFile: does not support memory mapping operation!" ); return 0; }
        void unmap() { GN_ERROR(sLogger)( "StdFile: does not support memory mapping operation!" ); }
    };

    ///
    /// disk file class
    ///
    class DiskFile : public StdFile
    {
        size_t mSize;
    public:

        DiskFile() : StdFile(0), mSize(0) {}
        ~DiskFile() { close(); }

        ///
        /// open a file
        ///
        /// \param fname File name
        /// \param flags File open flags
        ///
        bool open( const StrA & fname, SInt32 flags );

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
    /// file class that wraps a fixed-sized memory buffer
    ///
    template< typename T >
    class MemFile : public File
    {
        UInt8 * mStart;
        UInt8 * mPtr;
        size_t    mSize;

    public:

        /// \name ctor/dtor
        //@{
        MemFile( T * buf = 0, size_t size = 0, const StrA & name = "" )
            : mStart((UInt8*)buf)
            , mPtr((UInt8*)buf)
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
        bool seek( int offset, FileSeekMode origin );
        size_t tell() const { return mPtr - mStart; }
        size_t size() const { return mSize; }
        void * map( size_t offset, size_t length, bool )
        {
            if( offset >= mSize || (offset + length) > mSize )
            {
                GN_ERROR(sLogger)( "invalid mapping range!" );
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
        std::vector<UInt8> mBuffer;
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
        bool seek( int offset, FileSeekMode origin );
        size_t tell() const { return mCursor; }
        size_t size() const { return mBuffer.size(); }
        void * map( size_t offset, size_t length, bool )
        {
            if( offset >= mBuffer.size() || (offset + length) > mBuffer.size() )
            {
                GN_ERROR(sLogger)( "invalid mapping range!" );
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
//                           End of file.h
// *****************************************************************************
#endif // __GN_BASE_FILE_H__

#ifndef __GN_BASE_FILE_H__
#define __GN_BASE_FILE_H__
// *****************************************************************************
//! \file    file.h
//! \brief   general file stream class
//! \author  chenlee (2005.4.20)
// *****************************************************************************

#include <vector>
#include <map>

namespace GN
{
    //!
    //! 文件打开模式
    //!
    //! 有效的模式必须是一个读写标志加上一个格式标志
    //!
    enum FileOpenMode
    {
        FOPEN_NONE  = 0x00, //!< indicate a invalid value

        // 读写标志
        FOPEN_READ  = 0x01, //!< 只读模式（文件必须存在），same as "r"
        FOPEN_WRITE = 0x02, //!< 只写模式，same as "w"
        FOPEN_RW    = 0x03, //!< 读写模式（文件必须存在），same as "r+"

        // 格式标志
        FOPEN_BIN   = 0x10, //!< 二进制模式
        FOPEN_TXT   = 0x20, //!< 文本模式
    };

    //!
    //! 文件定位模式
    //!
    enum FileSeekMode
    {
        FSEEK_CUR,     //!< same as standard SEEK_CUR
        FSEEK_END,     //!< same as standard SEEK_END
        FSEEK_SET,     //!< same as standard SEEK_SET
        NUM_FSEEKS,    //!< number of avaliable seeking modes
    };

    //!
    //! File operation caps
    //!
    union FileOperationCaps
    {
        unsigned char u8; //!< File operation caps as unsigned char
        signed char   i8; //!< File operation caps as char
        struct
        {
            bool read      : 1; //!< support reading
            bool write     : 1; //!< support writing
            bool eof       : 1; //!< support EOF quering
            bool seek      : 1; //!< support position seeking
            bool tell      : 1; //!< support position querying
            bool size      : 1; //!< support size querying
            bool map       : 1; //!< support memory-mapping
            bool _reserved : 1; //!< reserved.
        };
    };

    //!
    //! basic file interface used throughout of the garnet system
    //!
    //! 用户实现该文件类时，不一定要实现下面的所有操作。
    struct File : public NoCopy
    {
        //!
        //! Get file operation caps
        //!
        const FileOperationCaps & getCaps() const { return mCaps; }
        
        //!
        //! 读取size个字节到buffer中
        //!
        virtual bool read( void * /*buffer*/, size_t /*size*/, size_t * /*readen*/ ) = 0;

        //!
        //! 向文件中写入size个字节
        //!
        //! \return   -1 means failed
        //!
        virtual bool write( const void * /*buffer*/, size_t /*size*/, size_t * /*written*/ ) = 0;

        //!
        //! 是否已经到文件结尾. Return true, if something goes wrong.
        //!
        virtual bool eof() const = 0;

        //!
        //! 设定文件读写游标的位置
        //!
        //! \return   return false if error
        //!
        virtual bool seek( int /*offset*/, FileSeekMode /*origin*/ ) = 0;

        //!
        //! 返回当前文件读写游标的位置. Return 0 if something goes wrong.
        //!
        //! \return
        virtual size_t tell() const = 0;

        //!
        //! 返回文件的总长度. Return 0 if something goes wrong.
        //!
        virtual size_t size() const = 0;

        //!
        //! get memory mapping of the file content. Return NULL if failed.
        //!
        virtual void * map( size_t offset, size_t length, bool readonly ) = 0;

        //!
        //! unmap file content
        //!
        virtual void unmap() = 0;

        //!
        //! return file name string
        //!
        const StrA & name() const { return mName; }

    protected :

        // ctor / dtor
        File()          { mCaps.u8 = 0; }
        virtual ~File() {}

        //!
        //! Set file name
        //!
        void setName( const StrA & name ) { mName = name; }

        //!
        //! Set operation caps
        //!
        void setCaps( const FileOperationCaps & caps ) { mCaps = caps; }

        //!
        //! Set operation caps
        //!
        void setCaps( int caps ) { mCaps.i8 = (signed char)caps; }

        //!
        //! File logger
        //!
        static Logger * sLogger;

    private:

        StrA              mName;
        FileOperationCaps mCaps;
    };

    //!
    //! stream operator
    //!
    inline File & operator<<( File & fp, int i )
    {
        char buf[256];
        strPrintf( buf, 256, "%d", i );
        fp.write( buf, strLen(buf), 0 );
        return fp;
    }

    //!
    //! stream operator
    //!
    inline File & operator<<( File & fp, size_t s )
    {
        char buf[256];
        strPrintf( buf, 256, "%Iu", s );
        fp.write( buf, strLen(buf), 0 );
        return fp;
    }

    //!
    //! stream operator
    //!
    inline File & operator<<( File & fp, const char * s )
    {
        if( 0 == s ) return fp;
        fp.write( s, strLen(s), 0 );
        return fp;
    }

    //!
    //! stream operator
    //!
    inline File & operator<<( File & fp, const StrA & s )
    {
        if( s.empty() ) return fp;
        fp.write( s.cptr(), s.size(), 0 );
        return fp;
    }

    //!
    //! 用File包装的standard file stream
    //!
    class StdFile : public File
    {
        FILE * mFile;

    protected:

        //!
        //! Change the internal file pointer
        //!
        void setFile( FILE * fp )
        {
            mFile = fp;
            if( stdin == fp ) setName( "stdin" );
            else if( stdout == fp ) setName( "stdout" );
            else if( stderr == fp ) setName( "stderr" );
            else setName( strFormat( "#%p", fp ) );
        }

    public :

        //!
        //! constructor
        //!
        StdFile( FILE * fp )
        {
            setCaps( 0x3F ); // support all operations, except mapping
            setFile(fp);
        }

        //!
        //! get internal file pointer
        //!
        FILE * getFILE() const { return mFile; }

        //!
        //! Convert to ANSI FILE *
        //!
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

    //!
    //! disk file class
    //!
    class DiskFile : public StdFile
    {
        size_t mSize;
    public:

        DiskFile() : StdFile(0), mSize(0) {}
        ~DiskFile() { close(); }

        //!
        //! open a file
        //!
        //! \param fname File name
        //! \param flags File open flags
        //!
        bool open( const StrA & fname, SInt32 flags );

        //!
        //! open a file
        //!
        //! \param fname File name
        //! \param mode  ANSI compatible open mode, such as "r", "w+".
        //!
        bool open( const StrA & fname, const StrA & mode );

        //!
        //! close the file
        //!
        void close() throw();

        //!
        //! Convert to ANSI FILE *
        //!
        operator FILE* () const { return getFILE(); }

        // from File
    public:
        size_t size() const { return mSize; }
    };

    //!
    //! file class that wraps a fixed-sized memory buffer
    //!
    template< typename T >
    class MemFile : public File
    {
        UInt8 * mStart;
        UInt8 * mPtr;
        size_t    mSize;

    public:

        //! \name ctor/dtor
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

        //!
        //! reset memory buf
        //!
        void reset( T * buf = 0, size_t size = 0, const StrA & name = "" );

        //! \name from File
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

    //!
    //! File class the wraps a vector class
    //!
    class VectorFile : public File
    {
        std::vector<UInt8> mBuffer;
        size_t mCursor;

    public:

        //!
        //! ctor
        //!
        VectorFile() : mCursor(0)
        {
            setCaps( 0xFF ); // support all operations
        }

        //!
        //! dtor
        //!
        ~VectorFile() {}

        //! \name from File
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

    //!
    //! namespace for path functions
    //!
    //! \Note some special path prefixes are supported:
    //!     - "app:"     : refer to application's directory
    //!     - "pwd:"     : refer to current working directory
    //!     - "startup:" : refer to application startup directory
    //!
    namespace path
    {
        //!
        //! 路径是否存在
        //!
        bool exist( const StrA & );

        //!
        //! if the path points to a directoy?
        //!
        bool isDir( const StrA & );

        //!
        //! if the path points to a file
        //!
        inline bool isFile( const StrA & path ) { return exist(path) && !isDir(path); }

        //!
        //! Conver path to platform native format. This function will do:
        //!   - Normalize path separators
        //!     - Convert all path separators to platform specific format.
        //!     - Remove redundant path separators, such as "c:\\path\" will be
        //!       convert to "c:\path".
        //!   - Resolve special path prefixes. Currently, 3 prefixes are supported:
        //!     - "app:"      : executable directory
        //!     - "startup:"  : startup directory
        //!     - "pwd:"      : current working directory
        //!   - Resolve embbed environment variable, like this:
        //!         "${windir}/system32" -> "c:\\windows\\system32"
        //!     - Note this feature is unimplemented yet.
        //!
        void toNative( StrA & result, const StrA & path );

        //!
        //! Conver path to platform native format.
        //!
        inline StrA toNative( const StrA & path )
        {
            StrA ret;
            toNative( ret, path );
            return ret;
        }

        //!
        //! Return true for path like: "/....", "aaa:..."
        //!
        bool isAbsPath( const StrA & path );

        //!
        //! Return true for relative (root-less) path
        //!
        inline bool isRelPath( const StrA & path ) { return !isAbsPath(path); }

        //!
        //! Get the parent path (directory) of the path.
        //! Samples:
        //!     - "a/b" -> "a"
        //!     - "a/b/" -> "a"
        //!     - "/a" -> "/"
        //!     - "/a/" -> "/"
        //!     - "a" -> ""
        //!     - "a/" -> ""
        //!     - "/" -> "/"
        //!     - "" -> ""
        //!     - "c:a/b" -> "c:/a"
        //!     - "c:a" -> "c:/"
        //!     - "c:" -> "c:/"
        //!
        void getParent( StrA &, const StrA & );

        //!
        //! Get the parent path (directory) of the path
        //!
        inline StrA getParent( const StrA & path ) { StrA ret; getParent(ret,path); return ret; }

        //!
        //! Get extension name of the path, with prefixing dot.
        //! Samples:
        //!     - "a.b.c" -> ".c"
        //!     - "a.c" -> ".c"
        //!     - ".c" -> ".c"
        //!     - "a." -> "."
        //!     - "a" -> ""
        //!
        void getExt( StrA &, const StrA & );

        //!
        //! Get extension name of the path.
        //!
        inline StrA getExt( const StrA & path ) { StrA ret; getExt(ret,path); return ret; }

        //!
        //! get basename of a path
        //!
        void baseName( StrA & result, const StrA & path );

        //!
        //! get basename of a path
        //!
        inline StrA baseName( const StrA & path ) { StrA result; baseName( result, path ); return result; }

        //!
        //! get dirname of a path ( alias of getParent() )
        //!
        inline void dirName( StrA & result, const StrA & path ) { getParent( result, path ); }

        //!
        //! get dirname of a path ( alias of getParent() )
        //!
        inline StrA dirName( const StrA & path ) { return getParent( path ); }

        //!
        //! Join path
        //!
        void joinTo(
            StrA & result,
            const StrA & path1 = StrA::EMPTYSTR,
            const StrA & path2 = StrA::EMPTYSTR,
            const StrA & path3 = StrA::EMPTYSTR,
            const StrA & path4 = StrA::EMPTYSTR,
            const StrA & path5 = StrA::EMPTYSTR );

        //!
        //! Join path
        //!
        inline StrA join(
            const StrA & path1 = StrA::EMPTYSTR,
            const StrA & path2 = StrA::EMPTYSTR,
            const StrA & path3 = StrA::EMPTYSTR,
            const StrA & path4 = StrA::EMPTYSTR,
            const StrA & path5 = StrA::EMPTYSTR )
        {
            StrA ret;
            joinTo( ret, path1, path2, path3, path4, path5 );
            return ret;
        }

        //!
        //! Resolve relative path to absolute path.
        //!
        //! This function will fail, if:
        //! - input path does not exist;
        //! - Read or search permission was denied for input path;
        //! - A loop exists in symbolic links encountered during resolve;
        //!
        //! \note This function will also convert input path to native format.
        //!
        bool resolve( StrA & result, const StrA & path );

        //!
        //! Resolve relative path to absolute path.
        //!
        //! \return Empty string, if failed.
        //!
        inline StrA resolve( const StrA & path )
        {
            StrA ret;
            if( resolve( ret, path ) ) return ret;
            return StrA::EMPTYSTR;
        }

        //!
        //! Convert a path to relative path from and base dir.
        //!
        void getRelative( StrA & result, const StrA & path, const StrA & base );

        //!
        //! Convert a path to relative path from and base dir.
        //!
        inline StrA getRelative( const StrA & path, const StrA & base )
        {
            StrA ret;
            getRelative( ret, path, base );
            return ret;
        }

        //!
        //! 查找指定文件
        //!
        //! \param result     返回搜索结果
        //! \param dirName    在什么目录下
        //! \param pattern    搜索什么文件
        //! \param recursive  是否递归搜索子目录
        //! \param useRegex   是否使用正则匹配
        //! \return           返回参数 result
        //!
        std::vector<StrA> &
        glob(
            std::vector<StrA> & result,
            const StrA & dirName,
            const StrA & pattern,
            bool         recursive,
            bool         useRegex );
    }
}

#include "file.inl"

// *****************************************************************************
//                           End of file.h
// *****************************************************************************
#endif // __GN_BASE_FILE_H__


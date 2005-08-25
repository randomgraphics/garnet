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
    //! basic file interface used throughout of the garnet system
    //!
    //! 用户实现该文件类时，不一定要实现下面的所有操作。
    struct File : public RefCounter
    {
        //!
        //! 读取size个字节到buffer中，返回实际读取的字节数,
        //!
        //! \return   -1 means failed.
        //!
        virtual size_t read( void * /*buffer*/, size_t /*size*/ ) { return size_t(-1); }

        //!
        //! 向文件中写入size个字节，返回实际写入到字节数
        //!
        //! \return   -1 means failed
        //!
        virtual size_t write( const void * /*buffer*/, size_t /*size*/ ) { return size_t(-1); }

        //!
        //! 是否已经到文件结尾
        //!
        virtual bool   eof() const { return false; }

        //!
        //! 设定文件读写游标的位置
        //!
        //! \return   return false if error
        //!
        virtual bool   seek( int /*offset*/, FileSeekMode /*origin*/ ) { return false; }

        //!
        //! 返回当前文件读写游标的位置
        //!
        //! \return   On error, return -1L
        virtual size_t tell() const { return size_t(-1); }

        //!
        //! 返回文件的总长度. If error, return -1L.
        //!
        virtual size_t size() const { return size_t(-1); }

        //!
        //! return file name string
        //!
        const StrA & name() const { return mName; }

    protected :

        // ctor / dtor
        File()          {}
        virtual ~File() {}

        //!
        //! Set file name
        //!
        void setName( const StrA & name ) { mName = name; }

    private:

        StrA mName;
    };

    //!
    //! 用File包装的standard file stream
    //!
    class StdFile : public File
    {
        FILE * mFile;
    public :

        //!
        //! constructor
        //!
        StdFile( FILE * );

        // from File
    public:
        size_t read( void *, size_t );
        size_t write( const void * buffer, size_t size );
    };

    //!
    //! file class using ANSI file functions
    //!
    class AnsiFile : public File
    {
        FILE * mFile;
        size_t mSize;
    public:

        AnsiFile() : mFile(0), mSize(0) {}
        ~AnsiFile() { close(); }

        //!
        //! open a file
        //!
        //! \param fname File name
        //! \param flags File open flags
        //!
        bool open( const StrA & fname, int32_t flags );

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
        operator FILE* () const { GN_ASSERT(mFile); return mFile; }

        // from File
    public:
        size_t read( void * buffer, size_t size );
        size_t write( const void * buffer, size_t size );
        bool   eof() const;
        bool   seek( int offset, FileSeekMode );
        size_t tell() const;
        size_t size() const { return mSize; }
    };

    //!
    //! file class that wraps a memory buffer
    //!
    template< typename T >
    class MemFile : public File
    {
        uint8_t * mStart;
        uint8_t * mPtr;
        size_t    mSize;

    public:

        //! \name ctor/dtor
        //@{
        MemFile( T * buf = 0, size_t size = 0, const StrA & name = "" )
            : mStart((uint8_t*)buf)
            , mPtr((uint8_t*)buf)
            , mSize(size)
        { setName(name); }
        ~MemFile() {}
        //@}

        //!
        //! reset memory buf
        //!
        void reset( T * buf = 0, size_t size = 0, const StrA & name = "" );

        //! \name from File
        //@{
        size_t read( void * buf, size_t size );
        size_t write( const void * buf, size_t size );
        bool   eof() const { return (mStart+mSize) == mPtr; }
        bool   seek( int offset, FileSeekMode origin );
        size_t tell() const { return mPtr - mStart; }
        size_t size() const { return mSize; }
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
        //! 文件是否存在
        //!
        bool exist( const StrA & );

        //!
        //! if the path points to a directoy?
        //!
        bool isDir( const StrA & );

        //!
        //! Conver path to platform native format. This function will do:
        //!   - Normalize path separators to platform specific format.
        //!   - Remove redundant path separators, such as "c:\\path\" will be
        //!     convert to "c:\path".
        //!   - Resolve special path prefixes
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
        //! Get the parent path (directory) of the path
        //!
        void getParent( StrA &, const StrA & );

        //!
        //! Get the parent path (directory) of the path
        //!
        inline StrA getParent( const StrA & path ) { StrA ret; getParent(ret,path); return ret; }

        //!
        //! Join path
        //!
        void join(
            StrA & result,
            const StrA & path1 = StrA::EMPTYSTR,
            const StrA & path2 = StrA::EMPTYSTR,
            const StrA & path3 = StrA::EMPTYSTR,
            const StrA & path4 = StrA::EMPTYSTR );

        //!
        //! Join path
        //!
        inline StrA join(
            const StrA & path1 = StrA::EMPTYSTR,
            const StrA & path2 = StrA::EMPTYSTR,
            const StrA & path3 = StrA::EMPTYSTR,
            const StrA & path4 = StrA::EMPTYSTR )
        {
            StrA ret;
            join( ret, path1, path2, path3, path4 );
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


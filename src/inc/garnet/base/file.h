#ifndef __GN_BASE_FILE_H__
#define __GN_BASE_FILE_H__
// *****************************************************************************
/// \file
/// \brief   general file stream class
/// \author  chenlee (2005.4.20)
// *****************************************************************************

#include <stdio.h>
#include <fstream>
#include <sstream>
#include <memory>

namespace GN {

///
/// File operation caps
///
union FileOperationCaps {
    unsigned char u8; ///< File operation caps as unsigned char
    signed char   i8; ///< File operation caps as char
    struct {
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
struct GN_API File : public NoCopy {
    /// construct from input stream
    File(std::istream & i, const StrA & name) {
        setStream(&i, nullptr);
        setName(name);
    }
 
    /// construct from output stream
    File(std::ostream & o, const StrA & name) {
        setStream(nullptr, &o);
        setName(name);
    }

    /// @brief Construct from input and output stream
    File(std::istream & i, std::ostream & o, const StrA & name) {
        setStream(&i, &o);
        setName(name);
    }

    /// construct from iostream
    File(std::iostream & s, const StrA & name) {
        setStream(&s, &s);
        setName(name);
    }

    /// dtor
    virtual ~File() {}

    /// return file name string
    const StrA & name() const { return mName; }

    // /// Get file operation caps
    // const FileOperationCaps & caps() const { return mCaps; }

    /// @brief Check if the file is readable.
    bool readable() const { return !!mInput; }

    /// @brief Check if the file is writable.
    bool writeable() const { return !!mOutput; }

    /// @brief Acquire the input stream object for reading. Assuming the file is readable.
    std::istream & input() {
        GN_ASSERT(readable());
        return *mInput;
    }

    /// @brief Acquire the output stream object for writing. Assuming the file is writable.
    std::ostream & output() {
        GN_ASSERT(writeable());
        return *mOutput;
    }

    /// 读取up to size个字节到buffer中.
    /// \return Number of bytes readen. 0 means failed.
    size_t read(void * /*buffer*/, size_t /*size*/);

    /// 向文件中写入up to size个字节
    /// \return Number of bytes written. 0 means failed.
    size_t write(const void * /*buffer*/, size_t /*size*/);

    /// 是否已经到文件结尾. Return true, if failed or not support reading.
    bool eof() const;

    /// 返回文件的总长度. Return 0 if failed or not support reading.
    size_t size() const;

    /// write string to file
    bool print(const StrA & s) { return write(s.rawptr(), s.size()) == s.size(); }

    /// write formatted string to file
    inline bool printf(const char * fmt, ...) {
        StrA    s;
        va_list arglist;
        va_start(arglist, fmt);
        s.formatv(fmt, arglist);
        va_end(arglist);
        return print(s);
    }

protected:
    /// protected default ctor
    File() = default;

    /// Set file name
    void setName(const StrA & name) { mName = name; }

    void setStream(std::istream * i, std::ostream * o) {
        mInput  = i;
        mOutput = o;
    }

private:
    StrA           mName;
    std::istream * mInput  = nullptr;
    std::ostream * mOutput = nullptr;
};

/// stream output operator
template<typename T>
inline File & operator<<(File & fp, const T & t) {
    GN_ASSERT(fp.writeable());
    fp.output() << t;
    return fp;
}

///
/// disk file class
///
class GN_API DiskFile : public File {
public:
    DiskFile() {}
    ~DiskFile() { close(); }

    /// open a file
    bool open(const StrA & filename, std::ios_base::openmode mode);

    /// close the file
    void close() throw();

private:
    std::fstream mFile;
};

/// A temporary file that will be automatically deleted after closed.
class GN_API TempFile : public File {
public:
    /// default constructor
    TempFile() { open(); }

    /// open a temporary file with user specified prefix
    bool open();

    /// close the temporary file
    void close() throw();

private:
    std::unique_ptr<std::iostream>  mStream;
    std::unique_ptr<std::streambuf> mBuf;
    FILE *                          mFile {};
};

///
/// file class that wraps a fixed-sized memory buffer
///
class GN_API MemFile : public File {
public:
    MemFile(void * buf = 0, size_t size = 0, const StrA & name = "");

private:
    std::unique_ptr<std::streambuf> mBuf;
    std::iostream                   mStream;
};

} // namespace GN

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_FILE_H__

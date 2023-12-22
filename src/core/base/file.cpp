#include "pch.h"

using namespace GN;

static Logger * sLogger = getLogger("GN.base.File");

// *****************************************************************************
//                   implementation of File
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN_API size_t GN::File::read(void * buffer, size_t size) {
    // check parameter(s)
    if (0 == buffer && 0 != size) {
        GN_ERROR(sLogger)("Failed to read file (%s): invalid parameter(s)!", name().rawptr());
        return 0;
    }

    if (!readable()) {
        GN_ERROR(sLogger)("Failed to read file (%s): the file is not readable!", name().rawptr());
        return 0;
    }

    auto & is = input();
    if (!is) {
        GN_ERROR(sLogger)("Failed to read file (%s): the file is not in good state!", name().rawptr());
        return 0;
    }

    if (eof()) {
        GN_VERBOSE(sLogger)("Already reach the end of the file.");
        return 0;
    }

    is.read((char *) buffer, size);
    return is.gcount();
}

//
//
// -----------------------------------------------------------------------------
GN_API size_t GN::File::write(const void * buffer, size_t size) {
    // check parameter(s)
    if (0 == buffer && 0 != size) {
        GN_ERROR(sLogger)("Failed to write to file (%s): invalid parameter(s)!", name().rawptr());
        return 0;
    }

    if (!writeable()) {
        GN_ERROR(sLogger)("Failed to write to file (%s): the file is not writable!", name().rawptr());
        return 0;
    }

    auto & os = output();
    if (!os) {
        GN_ERROR(sLogger)("Failed to write to file (%s): the file is not in good state!", name().rawptr());
        return 0;
    }

    os.write((const char *) buffer, size);

    return os.good() ? size : 0;
}

//
//
// -----------------------------------------------------------------------------
GN_API bool GN::File::eof() const {
    if (!readable()) return true; // not a readable file, always EOF.
    return const_cast<File *>(this)->input().eof();
}

//
//
// -----------------------------------------------------------------------------
GN_API size_t GN::File::size() const {
    if (!readable()) return 0; // not a readable file.
    auto & is = const_cast<File *>(this)->input();

    if (is.bad()) return 0; // stream is in bad state.

    // store current file position
    auto oldpos = is.tellg();

    // seek to the end of the file
    auto end = is.seekg(0, std::ios_base::end).tellg();

    // restore file position
    is.seekg(oldpos, std::ios_base::beg);

    // success
    return end;
}

// *****************************************************************************
//                   implementation of DiskFile
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN_API bool GN::DiskFile::open(const StrA & filename, const std::ios_base::openmode mode) {
    close();

    // check parameter(s)
    if (filename.empty()) {
        GN_ERROR(sLogger)("empty filename!");
        close();
        return false;
    }

    mFile.open(filename.rawptr(), mode);

    if (!mFile) {
        GN_ERROR(sLogger)("fail to open file '%s' : %s", filename.rawptr(), mode, GN::errno2str(errno));
        close();
        return false;
    }

    // success
    setStream(&mFile, &mFile);
    setName(filename);
    return true;
}

//
//
// -----------------------------------------------------------------------------
GN_API void GN::DiskFile::close() throw() {
    GN_GUARD_ALWAYS;
    setName("");
    setStream(0, 0);
    if (mFile.is_open()) mFile.close();
    GN_UNGUARD_ALWAYS_NO_THROW;
}

// *****************************************************************************
//                   implementation of TempFile
// *****************************************************************************

// A wrapper class to use FILE* with c++ streams.
class stdiobuf : public std::streambuf {
public:
    stdiobuf(FILE * f, int om) { init(f, om); }

    ~stdiobuf() { close(); }

    bool is_open() const { return m_filedes >= 0; }

    void close() {
        // We don't really close the file here. Should leave it to whoever instantiated this class.
        if (is_open()) sync();
        m_filedes = nullptr;
        m_mode    = 0;
    }

    virtual int underflow() override {
        // is read position before end of buffer?
        if (gptr() < egptr()) return *gptr();

        size_t numPutback = 0;
        if (s_pback_size > 0) {
            // process size of putback area
            // -use number of characters read
            // -but at most four
            numPutback = gptr() - eback();
            if (numPutback > 4) numPutback = 4;

            // copy up to four characters previously read into the putback
            // buffer (area of first four characters)
            std::memcpy(buffer + (4 - numPutback), gptr() - numPutback, numPutback);
        }

        // read new characters
        auto num = ::fread(buffer + s_pback_size, 1, s_buf_size - s_pback_size, m_filedes);

        if (num < s_buf_size - s_pback_size) return EOF;

        // reset buffer pointers
        setg(buffer + s_pback_size - numPutback, buffer + s_pback_size, buffer + s_pback_size + num);

        // return next character Hrmph. We have to cast to unsigned char to
        // avoid problems with eof. Problem is, -1 is a valid char value to
        // return. However, without a cast, char(-1) (0xff) gets converted
        // to int(-1), which is 0xffffffff, which is EOF! What we want is
        // int(0x000000ff), which we have to get by int(unsigned char(-1)).
        return static_cast<unsigned char>(*gptr());
    }

    virtual int overflow(int c) override {
        if (!(m_mode & std::ios::out) || !is_open()) return EOF;

        if (c != EOF) {
            // insert the character into the buffer
            *pptr() = (char)c;
            pbump(1);
        }

        if (!flushoutput()) {
            return -1; // ERROR
        }

        return c;
    }

    virtual int sync() override { return !flushoutput() ? -1 : 0; }

private:
    static const int s_buf_size   = 4092;
    static const int s_pback_size = 4;

    int    m_mode    = 0;
    FILE * m_filedes = 0;
    char   buffer[s_buf_size];

private:
    // not copyable
    stdiobuf(const stdiobuf &);
    stdiobuf & operator=(const stdiobuf &);

    void init(FILE * fd, int om) {
        m_filedes = fd;
        if (m_filedes) m_mode = om;
        setg(buffer + s_pback_size, buffer + s_pback_size, buffer + s_pback_size);
        setp(buffer, buffer + s_buf_size - 1);
    }

    bool flushoutput() {
        if (!(m_mode & std::ios::out) || !is_open()) return false;
        size_t num = pptr() - pbase();
        if (::fwrite(pbase(), 1, num, m_filedes) != num) return false;
        pbump(-(int)num);
        return true;
    }
};

//
//
// -----------------------------------------------------------------------------
GN_API bool GN::TempFile::open() {
    close(); // close previous file

    // open a new temporary file
#if _MSC_VER
    auto err = tmpfile_s(&mFile);
    if (err) {
        GN_ERROR(sLogger)("fail to open temporary file : %s", errno2str(err));
        return false;
    }
#else
    mFile = std::tmpfile();
    if (0 == mFile) {
        GN_ERROR(sLogger)("fail to open temporary file : %s", errno2str(errno));
        return false;
    }
#endif

    mBuf = std::make_unique<stdiobuf>(mFile, std::ios::in | std::ios::out | std::ios::binary);
    mStream = std::make_unique<std::iostream>(mBuf.get());
    setName(std::to_string(_fileno(mFile)).c_str());
    setStream(mStream.get(), mStream.get());
    return true;
}

//
//
// -----------------------------------------------------------------------------
GN_API void GN::TempFile::close() throw() {
    GN_GUARD_ALWAYS;
    setName("");
    setStream(0, 0);
    mStream.reset();
    mBuf.reset();
    if (mFile) std::fclose(mFile), mFile = 0;
    GN_UNGUARD_ALWAYS_NO_THROW;
}

class MemFileBuf : public std::streambuf {
public:
    MemFileBuf(void * buf, size_t size) {
        char * p = (char *)buf;
        setg(p, p, p + size);
        setp(p, p + size);
    }
};

//
//
// -----------------------------------------------------------------------------
GN_API GN::MemFile::MemFile(void * buf, size_t size, const StrA & name) : mBuf(std::make_unique<MemFileBuf>(buf, size)), mStream(mBuf.get()) {
    setName(name);
    setStream(&mStream, &mStream);
}

// //
// //
// // -----------------------------------------------------------------------------
// GN_API void * GN::MemFile::map(size_t offset, size_t length, bool) {
//     if (offset >= mSize || (offset + length) > mSize) {
//         GN_ERROR(sLogger)("invalid mapping range!");
//         return 0;
//     }
//     return mStart + offset;
// }

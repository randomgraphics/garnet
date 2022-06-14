#include "pch.h"

using namespace GN;

static Logger * sLogger = getLogger("GN.base.File");

static FILE * sOpenFile(const char * filename, const char * mode) {
    FILE * fp;

#if GN_MSVC8
    if (0 != fopen_s(&fp, filename, mode)) fp = 0;
#else
    fp = fopen(filename, mode);
#endif

    if (0 == fp) { GN_ERROR(sLogger)("fopen() fail to open file '%s' with mode '%s' : %s.", filename, mode, GN::errno2str(errno)); }

    return fp;
}

#if GN_MSWIN
///
/// Automatic C-style array created by malloc. Can NOT be used in STL containers.
///
template<typename T>
class AutoMallocPtr : public detail::BaseAutoPtr<T, AutoMallocPtr<T>> {
    typedef detail::BaseAutoPtr<T, AutoMallocPtr<T>> ParentType;
    #if GN_GCC
    friend class detail::BaseAutoPtr<T, AutoMallocPtr<T>>;
    #else
    friend class ParentType;
    #endif

    static void sDoRelease(T * p) {
        if (p) ::free((void *) p);
    }

public:
    ///
    /// Construct from C-style pointer
    ///
    explicit AutoMallocPtr(T * p = 0) throw(): ParentType(p) {}
};
#endif

// *****************************************************************************
//                   implementation of StdFile
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN_API bool GN::StdFile::read(void * buffer, size_t size, size_t * readen) {
    GN_GUARD;

    // check parameter(s)
    if (0 == buffer && 0 != size) {
        GN_ERROR(sLogger)("invalid parameter(s)!");
        return false;
    }

    if (0 == mFile) {
        GN_ERROR(sLogger)("NULL file pointer!");
        return false;
    }

    if (eof()) {
        GN_VERBOSE(sLogger)("Already reach the end of the file.");
        if (readen) *readen = 0;
        return true;
    }

    size_t r = ::fread(buffer, 1, size, mFile);

    if ((size_t) -1 == r) {
        GN_ERROR(sLogger)("%s : fread() failed!", name().rawptr());
        return false;
    }

    // success
    if (readen) *readen = r;
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN_API bool GN::StdFile::write(const void * buffer, size_t size, size_t * written) {
    GN_GUARD;

    // check parameter(s)
    if (0 == buffer && 0 != size) {
        GN_ERROR(sLogger)("invalid parameter(s)!");
        return false;
    }

    if (0 == mFile) {
        GN_ERROR(sLogger)("NULL file pointer!");
        return false;
    }

    size_t r = ::fwrite(buffer, 1, size, mFile);
    if ((size_t) -1 == r) {
        GN_ERROR(sLogger)("%s: fwrite() failed!", name().rawptr());
        return false;
    }

    // success
    if (written) *written = r;
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN_API bool GN::StdFile::eof() const {
    GN_GUARD;

    if (0 == mFile) {
        GN_ERROR(sLogger)("NULL file pointer!");
        return true;
    }

    return 0 != feof(mFile);

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN_API bool GN::StdFile::seek(size_t offset, FileSeek origin) {
    GN_GUARD;

    // NOTE : this table must be always synchronized with definition of
    //        FileSeek
    static int seek_table[] = {
        SEEK_CUR,
        SEEK_END,
        SEEK_SET,
    };

    // check parameter
    if (origin >= FileSeek::NUM_MODES) {
        GN_ERROR(sLogger)("%s: invalid seek origin!", name().rawptr());
        return false;
    }

    if (0 == mFile) {
        GN_ERROR(sLogger)("NULL file pointer!");
        return false;
    }

    if (0 != ::fseek(mFile, (int) offset, seek_table[(int) origin])) {
        GN_ERROR(sLogger)("%s : fseek() failed!", name().rawptr());
        return false;
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN_API size_t GN::StdFile::tell() const {
    GN_GUARD;

    if (0 == mFile) {
        GN_ERROR(sLogger)("NULL file pointer!");
        return (size_t) -1;
    }

    size_t r = ::ftell(mFile);

    if ((size_t) -1 == r) { GN_ERROR(sLogger)("%s : ftell() failed!", name().rawptr()); }

    return r;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN_API size_t GN::StdFile::size() const {
    GN_GUARD;

    if (0 == mFile) {
        GN_ERROR(sLogger)("NULL file pointer!");
        return 0;
    }

    // store current file position
    long oldPos = ::ftell(mFile);
    if (-1 == oldPos) {
        GN_ERROR(sLogger)("%s : fail to get current file position!", name().rawptr());
        return 0;
    }

    // seek to the end of the file
    if (0 == ::fseek(mFile, SEEK_END, 0)) {
        GN_ERROR(sLogger)("%s : fail to seek to the end of file!", name().rawptr());
        return 0;
    }

    // get new position
    size_t newPos = ::ftell(mFile);

    // restore file position
    if (0 == ::fseek(mFile, SEEK_SET, oldPos)) {
        GN_ERROR(sLogger)("%s : fail to restore file position!", name().rawptr());
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
GN_API bool GN::DiskFile::open(const StrA & filename, const StrA & mode) {
    GN_GUARD_ALWAYS;

    // close previous file
    close();

    // check parameter(s)
    if (filename.empty()) {
        GN_ERROR(sLogger)("empty filename!");
        close();
        return false;
    }

    // ���ļ�
    FILE * fp = sOpenFile(filename.rawptr(), mode.rawptr());
    if (0 == fp) {
        close();
        return false;
    }

    // �õ��ļ���С
    ::fseek(fp, 0, SEEK_END);
    mSize = ::ftell(fp);
    ::fseek(fp, 0, SEEK_SET);

    // success
    setFile(fp);
    setName(filename);
    return true;

    GN_UNGUARD_ALWAYS_DO(return false;);
}

//
//
// -----------------------------------------------------------------------------
GN_API void GN::DiskFile::close() throw() {
    GN_GUARD_ALWAYS;

    // close file
    if (getFILE()) ::fclose(getFILE());

    // clear data members
    mSize = 0;
    setFile(0);
    setName("");

    GN_UNGUARD_ALWAYS_NO_THROW;
}

// *****************************************************************************
//                   implementation of TempFile
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN_API bool GN::TempFile::open(const StrA & prefix, const StrA & mode, Behavior beh) {
    GN_GUARD_ALWAYS;

    mBehavior = beh;

#if GN_MSWIN

    // generate temporary file name
    AutoMallocPtr<const char> filename(_tempnam(NULL, "GN_" + prefix));

    // open the file
    FILE * fp = sOpenFile(filename.rawptr(), mode);
    if (0 == fp) {
        close();
        return false;
    }

    // success
    setFile(fp);
    setName(filename.rawptr());
    return true;

#else

    StrA fileNameTempl = "/tmp/GN_" + prefix + "XXXXXX";
    mFileDesc          = mkstemp(fileNameTempl);
    if (-1 == mFileDesc) {
        GN_ERROR(sLogger)("Fail to generate temporary file name.");
        return false;
    }

    // unlink the temporary file name. So the file will be deleted automatically after beging closed.
    if (AUTO_DELETE == beh) { unlink(fileNameTempl); }

    // open file
    FILE * fp = fdopen(mFileDesc, mode);
    if (0 == fp) {
        GN_ERROR(sLogger)("fail to open file '%s' with mode '%s' : %s", fileNameTempl.rawptr(), mode.rawptr(), errno2str(errno));
        close();
        return false;
    }

    // success
    setFile(fp);
    setName(fileNameTempl);
    return true;

#endif

    GN_UNGUARD_ALWAYS_DO(return false;);
}

//
//
// -----------------------------------------------------------------------------
GN_API void GN::TempFile::close() {
    GN_GUARD_ALWAYS;

    // close file pointer
    if (getFILE()) ::fclose(getFILE());
    setFile(0);
    setName("");

#if GN_POSIX
    // close file descriptor
    if (-1 != mFileDesc) {
        ::close(mFileDesc);

        mFileDesc = -1;
    }
#endif

    GN_UNGUARD_ALWAYS;
}

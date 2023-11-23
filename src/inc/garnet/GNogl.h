#pragma once

#include "garnet/GNbase.h"
#include "garnet/gfx/colorFormat.h"

#define GLEW_STATIC
#include <GL/glew.h>
#if defined(__ANDROID__) || defined(__linux__)
    #include <GL/eglew.h>
#endif

#include <memory>
#include <type_traits>
#include <vector>
#include <unordered_map>
#include <atomic>
#include <variant>
#include <algorithm>
#include <sstream>

namespace GN {
namespace ogl {
using GN::gfx::ColorFormat;

/// The 'optionalFilename' parameter is optional and is only used when printing
/// shader compilation error.
GN_API GLuint loadShaderFromString(const char * source, size_t length, GLenum shaderType, const char * optionalFilename = nullptr);

/// the program name parameter is optional and is only used to print link error.
GN_API GLuint linkProgram(const GLuint * shaders, size_t count, const char * optionalProgramName = nullptr);

/// link program from a list of shader stored in stl vector
inline GLuint linkProgram(const std::vector<GLuint> & shaders, const char * optionalProgramName = nullptr) {
    return linkProgram(shaders.data(), shaders.size(), optionalProgramName);
}

// a utility function to upload uniform values
template<typename T>
void updateUniformValue(GLint location, const T & value) {
    if (location < 0) return;

    if constexpr (std::is_same<int, T>()) {
        glUniform1i(location, (int) value);
    } else if constexpr (std::is_same<unsigned int, T>()) {
        glUniform1ui(location, (unsigned int) value);
    } else if constexpr (std::is_same<float, T>()) {
        glUniform1f(location, value);
    } else if constexpr (std::is_same<Vector2f, T>()) {
        glUniform2fv(location, 1, (const float *) &value);
    } else if constexpr (std::is_same<Vector3f, T>()) {
        glUniform3fv(location, 1, (const float *) &value);
    } else if constexpr (std::is_same<Vector4f, T>()) {
        glUniform4fv(location, 1, (const float *) &value);
    } else if constexpr (std::is_same<Matrix33f, T>()) {
        glUniformMatrix3fv(location, 1, false, (const float *) &value);
    } else if constexpr (std::is_same<Matrix44f, T>()) {
        glUniformMatrix4fv(location, 1, false, (const float *) &value);
    } else if constexpr (std::is_same<std::vector<float>, T>()) {
        auto count = static_cast<GLsizei>(value.size());
        glUniform1fv(location, count, value.data());
    } else {
        struct DependentFalse : public std::false_type {};
        static_assert(DependentFalse::value, "unsupported uniform type");
    }
}

inline void clearScreen(const Vector4f & color = {0.f, 0.f, 0.f, 1.f}, float depth = 1.0f, uint32_t stencil = 0,
                        GLbitfield flags = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT) {
    if (flags | GL_COLOR_BUFFER_BIT) glClearColor(color.x, color.y, color.z, color.w);
    if (flags | GL_DEPTH_BUFFER_BIT) glClearDepthf(depth);
    if (flags | GL_STENCIL_BUFFER_BIT) glClearStencil(stencil);
    glClear(flags);
}

// -----------------------------------------------------------------------------
//
inline GLint getInt(GLenum name) {
    GLint value;
    glGetIntegerv(name, &value);
    return value;
}

inline GLint getInt(GLenum name, GLint i) {
    GLint value;
    glGetIntegeri_v(name, i, &value);
    return value;
}

inline void bindTexture(GLenum target, uint32_t stage, GLuint texture) {
    glActiveTexture(GL_TEXTURE0 + stage);
    glBindTexture(target, texture);
}

// -----------------------------------------------------------------------------
//
template<GLenum TARGET>
struct QueryObject {
    enum Status_ { // can use "Status" since Xlib.h has it defined to int.
        EMPTY,   // the query object is not created yet.
        IDLE,    // the query object is idle and ready to use.
        RUNNING, // in between begin() and end()
        PENDING, // query is issued. but result is yet to returned.
    };

    GLuint  qo     = 0;
    Status_ status = EMPTY;

    QueryObject() = default;
    ~QueryObject() { cleanup(); }

    GN_NO_COPY(QueryObject);

    // can move
    QueryObject(QueryObject && that) {
        qo          = that.qo;
        status      = that.status;
        that.qo     = 0;
        that.status = EMPTY;
    }
    QueryObject & operator=(QueryObject && that) {
        if (this != &that) {
            qo          = that.qo;
            status      = that.status;
            that.qo     = 0;
            that.status = EMPTY;
        }
    }

    bool empty() const { return EMPTY == status; }
    bool idle() const { return IDLE == status; }
    bool running() const { return RUNNING == status; }
    bool pending() const { return PENDING == status; }

    void cleanup() {
#ifdef __ANDROID__
        if (qo) glDeleteQueriesEXT(1, &qo), qo = 0;
#else
        if (qo) glDeleteQueries(1, &qo), qo = 0;
#endif
        status = IDLE;
    }

    void allocate() {
        cleanup();
#ifdef __ANDROID__
        glGenQueriesEXT(1, &qo);
#else
        glGenQueries(1, &qo);
#endif
        status = IDLE;
    }

    void begin() {
        if (IDLE == status) {
#ifdef __ANDROID__
            glBeginQueryEXT(TARGET, qo);
#else
            glBeginQuery(TARGET, qo);
#endif
            status = RUNNING;
        }
    }

    void end() {
        if (RUNNING == status) {
#ifdef __ANDROID__
            glEndQueryEXT(TARGET);
#else
            glEndQuery(TARGET);
#endif
            status = PENDING;
        }
    }

    void mark() {
        if (IDLE == status) {
#ifdef __ANDROID__
            glQueryCounterEXT(qo, TARGET);
#else
            glQueryCounter(qo, TARGET);
#endif
            status = PENDING;
        }
    }

    bool getResult(uint64_t & result) {
        if (PENDING != status) return false;
        GLint available;
#ifdef __ANDROID__
        glGetQueryObjectivEXT(qo, GL_QUERY_RESULT_AVAILABLE, &available);
#else
        glGetQueryObjectiv(qo, GL_QUERY_RESULT_AVAILABLE, &available);
#endif
        if (!available) return false;

#ifdef __ANDROID__
        glGetQueryObjectui64vEXT(qo, GL_QUERY_RESULT, &result);
#else
        glGetQueryObjectui64v(qo, GL_QUERY_RESULT, &result);
#endif
        status = IDLE;
        return true;
    }

    // returns 0, if the query is still pending.
    template<uint64_t DEFAULT_VALUE = 0>
    uint64_t getResult() const {
        uint64_t ret = DEFAULT_VALUE;
        return getResult(ret) ? ret : DEFAULT_VALUE;
    }
};

// -----------------------------------------------------------------------------
// Helper class to manage GL buffer object.
template<GLenum TARGET, size_t MIN_GPU_BUFFER_LENGH = 0>
struct BufferObject {
    GLuint bo            = 0;
    size_t length        = 0; // buffer length in bytes.
    GLenum mapped_target = 0;

    GN_NO_COPY(BufferObject);
    GN_NO_MOVE(BufferObject);

    BufferObject() {}

    ~BufferObject() { cleanup(); }

    static GLenum GetTarget() { return TARGET; }

    template<typename T, GLenum T2 = TARGET>
    void allocate(size_t count, const T * ptr, GLenum usage = GL_STATIC_DRAW) {
        cleanup();
        glGenBuffers(1, &bo);
        // Note: ARM Mali GPU doesn't work well with zero sized buffers. So
        // we create buffer that is large enough to hold at least one element.
        length = std::max(count, MIN_GPU_BUFFER_LENGH) * sizeof(T);
        glBindBuffer(T2, bo);
        glBufferData(T2, length, ptr, usage);
        glBindBuffer(T2, 0); // unbind
    }

    void cleanup() {
        if (bo) glDeleteBuffers(1, &bo), bo = 0;
        length = 0;
    }

    bool empty() const { return 0 == bo; }

    template<typename T, GLenum T2 = TARGET>
    void update(const T * ptr, size_t offset = 0, size_t count = 1) {
        glBindBuffer(T2, bo);
        glBufferSubData(T2, offset * sizeof(T), count * sizeof(T), ptr);
    }

    template<GLenum T2 = TARGET>
    void bind() const {
        glBindBuffer(T2, bo);
    }

    template<GLenum T2 = TARGET>
    static void unbind() {
        glBindBuffer(T2, 0);
    }

    template<GLenum T2 = TARGET>
    void bindBase(GLuint base) const {
        glBindBufferBase(T2, base, bo);
    }

    template<typename T, GLenum T2 = TARGET>
    void getData(T * ptr, size_t offset, size_t count) {
        glBindBuffer(T2, bo);
        void * mapped = nullptr;
        mapped        = glMapBufferRange(T2, offset * sizeof(T), count * sizeof(T), GL_MAP_READ_BIT);
        if (mapped) {
            memcpy(ptr, mapped, count * sizeof(T));
            glUnmapBuffer(T2);
        }
    }

    template<GLenum T2 = TARGET>
    void * map(size_t offset, size_t count) {
        bind();
        void * ptr = nullptr;
        ptr        = glMapBufferRange(T2, offset, count, GL_MAP_READ_BIT);
        assert(ptr);
        mapped_target = TARGET;
        return ptr;
    }

    template<GLenum T2 = TARGET>
    void * map() {
        return map<T2>(0, length);
    }

    void unmap() {
        if (mapped_target) {
            bind();
            glUnmapBuffer(mapped_target);
            mapped_target = 0;
        }
    }

    operator GLuint() const { return bo; }
};

// -----------------------------------------------------------------------------
//
template<typename T, GLenum TARGET, size_t MIN_GPU_BUFFER_LENGTH = 0>
struct TypedBufferObject {
    std::vector<T>                              c; // CPU data
    BufferObject<TARGET, MIN_GPU_BUFFER_LENGTH> g; // GPU data

    void allocateGpuBuffer() { g.allocate(c.size(), c.data()); }

    void syncGpuBuffer() { g.update(c.data(), 0, c.size()); }

    // Synchornosly copy buffer content from GPU to CPU.
    // Note that this call is EXTREMELY expensive, since it stalls both CPU and GPU.
    void syncToCpu() {
        glFinish();
        g.getData(c.data(), 0, c.size());
    }

    void cleanup() {
        c.clear();
        g.cleanup();
    }
};

// -----------------------------------------------------------------------------
//
template<typename T, GLenum TARGET1, GLenum TARGET2, size_t MIN_GPU_BUFFER_LENGTH = 0>
struct TypedBufferObject2 {
    std::vector<T>                               c;  // CPU data
    BufferObject<TARGET1, MIN_GPU_BUFFER_LENGTH> g1; // GPU data
    BufferObject<TARGET2, MIN_GPU_BUFFER_LENGTH> g2; // GPU data

    void allocateGpuBuffer() {
        g1.allocate(c.size(), c.data());
        g2.allocate(c.size(), c.data());
    }

    void syncGpuBuffer() {
        g1.update(c.data(), 0, c.size());
        g2.update(c.data(), 0, c.size());
    }

    void cleanup() {
        c.clear();
        g1.cleanup();
        g2.cleanup();
    }

    template<GLenum TT>
    void bind() const {
        if constexpr (TT == TARGET1) {
            g1.bind();
        } else {
            static_assert(TT == TARGET2);
            g2.bind();
        }
    }

    template<GLenum TT>
    void bindBase(GLuint base) const {
        if constexpr (TT == TARGET1) {
            g1.bindBase(base);
        } else {
            static_assert(TT == TARGET2);
            g2.bindBase(base);
        }
    }
};

// -----------------------------------------------------------------------------
//
class VertexArrayObject {
    GLuint _va = 0;

public:
    ~VertexArrayObject() { cleanup(); }

    void allocate() {
        cleanup();
        glGenVertexArrays(1, &_va);
    }

    void cleanup() {
        if (_va) glDeleteVertexArrays(1, &_va), _va = 0;
    }

    void bind() const { glBindVertexArray(_va); }

    void unbind() const { glBindVertexArray(0); }

    operator GLuint() const { return _va; }
};

// -----------------------------------------------------------------------------
//
struct AutoShader {
    GLuint shader;

    AutoShader(GLuint s = 0): shader(s) {}
    ~AutoShader() { cleanup(); }

    void cleanup() {
        if (shader) glDeleteShader(shader), shader = 0;
    }

    GN_NO_COPY(AutoShader);

    // can move
    AutoShader(AutoShader && rhs): shader(rhs.shader) { rhs.shader = 0; }
    AutoShader & operator=(AutoShader && rhs) {
        if (this != &rhs) {
            cleanup();
            shader     = rhs.shader;
            rhs.shader = 0;
        }
        return *this;
    }

    operator GLuint() const { return shader; }
};

class SamplerObject {
    GLuint _id = 0;

public:
    SamplerObject() {}
    ~SamplerObject() { cleanup(); }

    GN_NO_COPY(SamplerObject);

    // can move
    SamplerObject(SamplerObject && that) {
        _id      = that._id;
        that._id = 0;
    }
    SamplerObject & operator=(SamplerObject && that) {
        if (this != &that) {
            cleanup();
            _id      = that._id;
            that._id = 0;
        }
        return *this;
    }

    operator GLuint() const { return _id; }

    void allocate() {
        cleanup();
        glGenSamplers(1, &_id);
    }
    void cleanup() {
        if (_id) glDeleteSamplers(1, &_id), _id = 0;
    }
    void bind(size_t unit) const {
        GN_ASSERT(glIsSampler(_id));
        glBindSampler((GLuint) unit, _id);
    }
};

class TextureObject {
public:
    // no copy
    TextureObject(const TextureObject &) = delete;
    TextureObject & operator=(const TextureObject &) = delete;

    // can move
    TextureObject(TextureObject && rhs) noexcept: _desc(rhs._desc) { rhs._desc.id = 0; }
    TextureObject & operator=(TextureObject && rhs) noexcept {
        if (this != &rhs) {
            cleanup();
            _desc        = rhs._desc;
            rhs._desc.id = 0;
        }
        return *this;
    }

    // default constructor
    TextureObject() { cleanup(); }

    ~TextureObject() { cleanup(); }

    struct TextureDesc {
        GLuint      id = 0; // all other fields are undefined, if id is 0.
        GLenum      target;
        ColorFormat format;
        uint32_t    width;
        uint32_t    height;
        uint32_t    depth; // this is number of layers for 2D array texture and is always 6 for cube texture.
        uint32_t    mips;
    };

    const TextureDesc & getDesc() const { return _desc; }

    GLenum target() const { return _desc.target; }
    GLenum id() const { return _desc.id; }

    bool empty() const { return 0 == _desc.id; }

    bool is2D() const { return GL_TEXTURE_2D == _desc.target; }

    bool isArray() const { return GL_TEXTURE_2D_ARRAY == _desc.target; }

    void attach(GLenum target, GLuint id);

    void attach(const TextureObject & that) { attach(that._desc.target, that._desc.id); }

    void allocate2D(ColorFormat f, size_t w, size_t h, size_t m = 1);

    void allocate2DArray(ColorFormat f, size_t w, size_t h, size_t l, size_t m = 1);

    void allocateCube(ColorFormat f, size_t w, size_t m = 1);

    void setPixels(size_t level, size_t x, size_t y, size_t w, size_t h,
                   size_t       rowPitchInBytes, // set to 0, if pixels are tightly packed.
                   const void * pixels) const;

    // Set to rowPitchInBytes 0, if pixels are tightly packed.
    void setPixels(size_t layer, size_t level, size_t x, size_t y, size_t w, size_t h, size_t rowPitchInBytes, const void * pixels) const;

    void cleanup() {
        if (_owned && _desc.id) { glDeleteTextures(1, &_desc.id); }
        _desc.id     = 0;
        _desc.target = GL_NONE;
        _desc.format = ColorFormat::UNKNOWN;
        _desc.width  = 0;
        _desc.height = 0;
        _desc.depth  = 0;
        _desc.mips   = 0;
    }

    void bind(size_t stage) const {
        glActiveTexture(GL_TEXTURE0 + (int) stage);
        glBindTexture(_desc.target, _desc.id);
    }

    void unbind() const { glBindTexture(_desc.target, 0); }

    operator GLuint() const { return _desc.id; }

private:
    TextureDesc _desc;
    bool        _owned = false;
    void        applyDefaultParameters();
};

// -----------------------------------------------------------------------------
// Helper class to manage frame buffer object.
class SimpleFBO {
    struct MipLevel {
        uint32_t width = 0, height = 0;
        uint32_t fbo = 0;
    };

    struct RenderTarget {
        ColorFormat format  = ColorFormat::UNKNOWN;
        GLuint      texture = 0;
    };

    GLenum                _colorTextureTarget = GL_TEXTURE_2D;
    std::vector<MipLevel> _mips;
    RenderTarget          _colors[8] = {};
    GLuint                _depth     = 0;
    const GLsizei         COLOR_BUFFER_COUNT;
    const bool            HAS_DEPTH;

public:
    SimpleFBO(GLsizei colorBufferCount = 1, bool depth = true): COLOR_BUFFER_COUNT(colorBufferCount), HAS_DEPTH(depth) {
        GN_ASSERT(std::size_t(colorBufferCount) < std::size(_colors));
    }

    ~SimpleFBO() { cleanup(); }

    void cleanup();

    void allocate(uint32_t w, uint32_t h, uint32_t levels, const ColorFormat * cf);

    void allocate(uint32_t w, uint32_t h, uint32_t levels, ColorFormat cf) {
        GN_ASSERT(1 == COLOR_BUFFER_COUNT);
        allocate(w, h, levels, &cf);
    }

    void allocate(uint32_t w, uint32_t h, uint32_t levels, ColorFormat cf1, ColorFormat cf2) {
        GN_ASSERT(2 == COLOR_BUFFER_COUNT);
        ColorFormat formats[] = {cf1, cf2};
        allocate(w, h, levels, formats);
    }

    uint32_t getLevels() const { return (uint32_t) _mips.size(); }

    uint32_t getWidth(uint32_t level) const { return _mips[level].width; }

    uint32_t getHeight(uint32_t level) const { return _mips[level].height; }

    uint32_t getFBO(size_t level) const { return _mips[level].fbo; }

    void setColorTextureFilter(uint32_t rt, GLenum minFilter, GLenum maxFilter) {
        glBindTexture(_colorTextureTarget, _colors[rt].texture);
        glTexParameteri(_colorTextureTarget, GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameteri(_colorTextureTarget, GL_TEXTURE_MAG_FILTER, maxFilter);
    }

    void bind(size_t level = 0) const {
        for (GLsizei i = 0; i < COLOR_BUFFER_COUNT + 1; ++i) {
            glActiveTexture(GLenum(GL_TEXTURE0 + i));
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        const auto & m = _mips[level];
        glBindFramebuffer(GL_FRAMEBUFFER, m.fbo);
        glViewport(0, 0, (GLsizei) m.width, (GLsizei) m.height);
    }

    void bindColorAsTexture(uint32_t rt, uint32_t stage) const {
        GN_ASSERT(rt < (uint32_t) COLOR_BUFFER_COUNT);
        glActiveTexture(GLenum(GL_TEXTURE0 + stage));
        glBindTexture(_colorTextureTarget, _colors[rt].texture);
    }

    void bindDepthAsTexture(uint32_t stage) const {
        glActiveTexture(GLenum(GL_TEXTURE0 + stage));
        glBindTexture(GL_TEXTURE_2D, _depth);
    }

    GLenum      getColorTarget() const { return _colorTextureTarget; }
    GLuint      getColorTexture(size_t rt) const { return _colors[rt].texture; }
    ColorFormat getColorFormat(size_t rt) const { return _colors[rt].format; }

    void saveColorToFile(uint32_t rt, const std::string & filepath) const;
    void saveDepthToFile(const std::string & filepath) const;
};

// -----------------------------------------------------------------------------
// Helper class to manage cube map frame buffer object
class CubeFBO {
    struct MipLevel {
        uint32_t width;
        uint32_t fbo[6];
    };

    std::vector<MipLevel> _mips;
    GLuint                _color = 0, _depth = 0;

public:
    ~CubeFBO() { cleanup(); }

    void cleanup() {
        if (_color) glDeleteTextures(1, &_color), _color = 0;
        if (_depth) glDeleteTextures(1, &_depth), _depth = 0;
        for (size_t i = 0; i < _mips.size(); ++i) {
            for (int f = 0; f < 6; ++f) {
                if (_mips[i].fbo[f]) glDeleteFramebuffers(1, &_mips[i].fbo[f]), _mips[i].fbo[f] = 0;
            }
        }
        _mips.clear();
    }

    void allocate(uint32_t w, uint32_t levels, ColorFormat cf);

    uint32_t getLevels() const { return (uint32_t) _mips.size(); }

    uint32_t getWidth(size_t level) const { return _mips[level].width; }

    GLuint getColorTexture() const { return _color; }

    GLuint getDepthTexture() const { return _depth; }

    void bind(uint32_t face, uint32_t level = 0) const {
        const auto & m = _mips[level];
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, m.fbo[face]);
        glViewport(0, 0, (GLsizei) m.width, (GLsizei) m.width);
    }

    void bindColorAsTexture(int slot = -1) const {
        GLenum stage = (slot >= 0) ? slot : 0;
        glActiveTexture(GLenum(GL_TEXTURE0 + stage));
        glBindTexture(GL_TEXTURE_CUBE_MAP, _color);
    }

    void bindDepthAsTexture(int slot = -1) const {
        GLenum stage = (slot >= 0) ? slot : 1;
        glActiveTexture(GLenum(GL_TEXTURE0 + stage));
        glBindTexture(GL_TEXTURE_CUBE_MAP, _depth);
    }
};

// SSBO for in-shader debug output. Check out ftl/main_ps.glsl for example usage.
// It is currently working on Windows only. Running it on Android crashes the driver.
#if defined(_DEBUG)
    #define DEBUG_SSBO_ENABLED 1
#else
    #define DEBUG_SSBO_ENABLED 0
#endif
struct DebugSSBO {
#if DEBUG_SSBO_ENABLED
    std::vector<float>                     buffer;
    mutable std::vector<float>             printed;
    int *                                  counter = nullptr;
    BufferObject<GL_SHADER_STORAGE_BUFFER> g;
#endif

    static constexpr bool isEnabled() {
#if DEBUG_SSBO_ENABLED
        return true;
#else
        return false;
#endif
    }

    ~DebugSSBO() { cleanup(); }

    void allocate(size_t n) {
#if DEBUG_SSBO_ENABLED
        cleanup();
        buffer.resize(n + 1);
        printed.resize(buffer.size());
        counter = (int *) &buffer[0];
        g.allocate(buffer.size(), buffer.data(), GL_STATIC_READ);
#else
        (void) n;
#endif
    }

    void bind(int slot = 15) const {
#if DEBUG_SSBO_ENABLED
        if (g) glBindBufferBase(GL_SHADER_STORAGE_BUFFER, slot, g);
#else
        (void) slot;
#endif
    }

    void cleanup() {
#if DEBUG_SSBO_ENABLED
        buffer.clear();
        printed.clear();
        counter = nullptr;
        g.cleanup();
#endif
    }

    void clearCounter() {
#if DEBUG_SSBO_ENABLED
        if (!counter) return;
        *counter = 0;
        g.update(counter, 0, 1);
#endif
    }

    void pullDataFromGPU() {
#if DEBUG_SSBO_ENABLED
        if (buffer.empty()) return;
        glMemoryBarrier(GL_ALL_BARRIER_BITS);
        g.getData(buffer.data(), 0, buffer.size());
#endif
    }

    void printLastResult() const;
};

struct FullScreenQuad {
    // vertex array
    GLuint                        va = 0;
    BufferObject<GL_ARRAY_BUFFER> vb;

    FullScreenQuad() {}

    ~FullScreenQuad() { cleanup(); }

    void allocate();

    void cleanup();

    void draw() const {
        GN_ASSERT(va);
        glBindVertexArray(va);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
};

class SimpleGlslProgram {
    GLuint _program = 0;

    // struct Uniform
    //{
    //    GLint location;
    //};
    // std::vector<Uniform> _uniforms;

public:
    // optional program name (for debug log)
    std::string name;

#ifdef _DEBUG
    std::string vsSource, psSource, csSource;
#endif

    GN_NO_COPY(SimpleGlslProgram);
    GN_NO_MOVE(SimpleGlslProgram);

    SimpleGlslProgram(const char * optionalProgramName = nullptr) {
        if (optionalProgramName) name = optionalProgramName;
    }

    ~SimpleGlslProgram() { cleanup(); }

    bool loadVsPs(const char * vscode, const char * pscode) {
#ifdef _DEBUG
        if (vscode) vsSource = vscode;
        if (pscode) psSource = pscode;
#endif
        cleanup();
        AutoShader vs = loadShaderFromString(vscode, 0, GL_VERTEX_SHADER, name.c_str());
        AutoShader ps = loadShaderFromString(pscode, 0, GL_FRAGMENT_SHADER, name.c_str());
        if ((vscode && !vs) || (pscode && !ps)) return false;
        _program = linkProgram({vs, ps}, name.c_str());
        return _program != 0;
    }

    bool loadCs(const char * code) {
#ifdef _DEBUG
        if (code) csSource = code;
#endif
        cleanup();
        AutoShader cs = loadShaderFromString(code, 0, GL_COMPUTE_SHADER, name.c_str());
        if (!cs) return false;
        _program = linkProgram({cs}, name.c_str());

        return _program != 0;
    }

    // void InitUniform(const char* name)
    //{
    //    auto loc = glGetUniformLocation(_program, name);
    //    if (-1 != loc) {
    //        _uniforms.push_back({ loc });
    //    }
    //}

    // template<typename T>
    // void UpdateUniform(size_t index, const T& value) const
    //{
    //    UpdateUniformValue(_uniforms[index].location, value);
    //}

    void use() const { glUseProgram(_program); }

    void cleanup() {
        //_uniforms.clear();
        if (_program) glDeleteProgram(_program), _program = 0;
    }

    GLint getUniformLocation(const char * name_) const { return glGetUniformLocation(_program, name_); }

    GLint getUniformBinding(const char * name_) const {
        auto loc = glGetUniformLocation(_program, name_);
        if (-1 == loc) return -1;
        GLint binding;
        glGetUniformiv(_program, loc, &binding);
        return binding;
    }

    operator GLuint() const { return _program; }
};

class SimpleUniform {
public:
    using Value = std::variant<int, unsigned int, float, Vector2f, Vector3f, Vector4f, Vector2i, Vector3i, Vector4i, Vector2u, Vector3u, Vector4u, Matrix33f,
                               Matrix44f, std::vector<float>>;

    Value value;

    SimpleUniform(std::string name): _name(name) {}

    template<typename T>
    SimpleUniform(std::string name, const T & v): value(v), _name(name) {}

    bool init(GLuint program) {
        if (program > 0) {
            _location = glGetUniformLocation(program, _name.c_str());
        } else {
            _location = -1;
        }
        return _location > -1;
    }

    void apply() const {
        if (_location < 0) return;
        std::visit(
            [&](auto && v) {
                using T = std::decay_t<decltype(v)>;
                if constexpr (std::is_same_v<T, int>)
                    glUniform1i(_location, v);
                else if constexpr (std::is_same_v<T, unsigned int>)
                    glUniform1ui(_location, v);
                else if constexpr (std::is_same_v<T, float>)
                    glUniform1f(_location, v);
                else if constexpr (std::is_same_v<T, Vector2f>)
                    glUniform2f(_location, v.x, v.y);
                else if constexpr (std::is_same_v<T, Vector3f>)
                    glUniform3f(_location, v.x, v.y, v.z);
                else if constexpr (std::is_same_v<T, Vector4f>)
                    glUniform4f(_location, v.x, v.y, v.z, v.w);
                else if constexpr (std::is_same_v<T, Vector2i>)
                    glUniform2i(_location, v.x, v.y);
                else if constexpr (std::is_same_v<T, Vector3i>)
                    glUniform3i(_location, v.x, v.y, v.z);
                else if constexpr (std::is_same_v<T, Vector4i>)
                    glUniform4i(_location, v.x, v.y, v.z, v.w);
                else if constexpr (std::is_same_v<T, Vector2u>)
                    glUniform2ui(_location, v.x, v.y);
                else if constexpr (std::is_same_v<T, Vector3u>)
                    glUniform3ui(_location, v.x, v.y, v.z);
                else if constexpr (std::is_same_v<T, Vector4u>)
                    glUniform4ui(_location, v.x, v.y, v.z, v.w);
                else if constexpr (std::is_same_v<T, Matrix33f>)
                    glUniformMatrix3fv(_location, 1, false, v.data());
                else if constexpr (std::is_same_v<T, Matrix44f>)
                    glUniformMatrix4fv(_location, 1, false, v.data());
                else if constexpr (std::is_same_v<T, const std::vector<float>>)
                    glUniform1fv(_location, (GLsizei) v.size(), v.data());
            },
            value);
    }

private:
    const std::string _name;
    GLint             _location = -1;
};

class SimpleTextureCopy {
    struct CopyProgram {
        SimpleGlslProgram program;
        GLint             tex0Binding = -1;
    };
    std::unordered_map<GLuint, CopyProgram> _programs; // key is texture target.
    FullScreenQuad                          _quad;
    GLuint                                  _sampler = 0;
    GLuint                                  _fbo     = 0;

public:
    GN_NO_COPY(SimpleTextureCopy);
    GN_NO_MOVE(SimpleTextureCopy);
    SimpleTextureCopy() {}
    ~SimpleTextureCopy() { cleanup(); }
    bool init();
    void cleanup();
    struct TextureSubResource {
        GLenum   target;
        GLuint   id;
        uint32_t level;
        uint32_t z; // layer index for 2d array texture.
        // TODO: uint32_t x, y, w, h;
    };
    void copy(const TextureSubResource & src, const TextureSubResource & dst);
    void copy(const TextureObject & src, uint32_t srcLevel, uint32_t srcZ, const TextureObject & dst, uint32_t dstLevel, uint32_t dstZ) {
        auto & s = src.getDesc();
        auto & d = dst.getDesc();
        copy({s.target, s.id, srcLevel, srcZ}, {d.target, d.id, dstLevel, dstZ});
    }
};

// -----------------------------------------------------------------------------
// For asynchronous timer (not time stamp) queries
struct GpuTimeElapsedQuery {

    std::string name;

    explicit GpuTimeElapsedQuery(std::string n = std::string("")): name(n) { _q.allocate(); }

    ~GpuTimeElapsedQuery() {}

    // returns duration in nanoseconds
    GLuint64 duration() const { return _result; }

    void start() { _q.begin(); }

    void stop();

    // Print stats to string
    std::string print() const;

    friend inline std::ostream & operator<<(std::ostream & s, const GpuTimeElapsedQuery & t) {
        s << t.print();
        return s;
    }

private:
    QueryObject<GL_TIME_ELAPSED> _q;
    uint64_t                     _result = 0;
};

// -----------------------------------------------------------------------------
// GPU time stamp query
class GpuTimestamps {
public:
    explicit GpuTimestamps(std::string name = std::string("")): _name(name) {}

    void start() {
        GN_ASSERT(!_started);
        if (!_started) {
            _started = true;
            _count   = 0;
            mark("start time");
        }
    }

    void stop() {
        GN_ASSERT(_started);
        if (_started) {
            mark("end time");
            _started = false;
        }
    }

    void mark(std::string name) {
        GN_ASSERT(_started);
        if (!_started) return;

        if (_count == _marks.size()) {
            _marks.emplace_back();
            _marks.back().name = name;
        }

        GN_ASSERT(_count < _marks.size());
        _marks[_count++].mark();
    }

    // Print stats of timestamps to string
    std::string print(const char * ident = nullptr) const;

private:
    struct Query {
        std::string               name;
        QueryObject<GL_TIMESTAMP> q;
        uint64_t                  result = 0;
        Query() { q.allocate(); }
        GN_NO_COPY(Query);
        GN_DEFAULT_MOVE(Query);
        void mark() {
            if (q.idle()) {
                q.mark();
            } else {
                q.getResult(result);
            }
        }
    };

    std::string        _name;
    std::vector<Query> _marks;
    size_t             _count   = 0;
    bool               _started = false;
};
} // namespace ogl
} // namespace GN
#include "pch.h"
#include <sstream>
#include <vector>

using namespace GN;
using namespace GN::ogl;

static GN::Logger * sLogger = GN::getLogger("GN.ogl");

// -----------------------------------------------------------------------------
//
static const char * shaderType2String(GLenum shaderType) {
    switch (shaderType) {
    case GL_VERTEX_SHADER:
        return "vertex";
    case GL_FRAGMENT_SHADER:
        return "fragment";
    case GL_COMPUTE_SHADER:
        return "compute";
    default:
        return "";
    }
}

// -----------------------------------------------------------------------------
static std::string addLineCount(const std::string & in) {
    std::stringstream ss;
    ss << "(  1) : ";
    int line = 1;
    for (auto ch : in) {
        if ('\n' == ch)
            ss << GN::str::format("\n(%3d) : ", ++line);
        else
            ss << ch;
    }
    return ss.str();
}

// -----------------------------------------------------------------------------
//
GLuint GN::ogl::loadShaderFromString(const char * source, size_t length, GLenum shaderType, const char * optionalFilename) {
    if (!source) return 0;
    const char * sources[] = {source};
    if (0 == length) length = strlen(source);
    GLint sizes[] = {(GLint) length};
    auto  shader  = glCreateShader(shaderType);
    glShaderSource(shader, 1, sources, sizes);
    glCompileShader(shader);

    // check for shader compile errors
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        glDeleteShader(shader);
        GN_ERROR(sLogger)
        ("\n================== Failed to compile %s shader '%s' ====================\n"
         "%s\n"
         "\n============================= GLSL shader source ===============================\n"
         "%s\n"
         "\n================================================================================\n",
         shaderType2String(shaderType), optionalFilename ? optionalFilename : "<no-name>", infoLog, addLineCount(source).c_str());
        return 0;
    }

    // done
    GN_ASSERT(shader);
    return shader;
}

// -----------------------------------------------------------------------------
//
GLuint GN::ogl::linkProgram(const GLuint * shaders, size_t count, const char * optionalProgramName) {
    auto program = glCreateProgram();
    for (size_t i = 0; i < count; ++i)
        if (shaders[i]) glAttachShader(program, shaders[i]);
    glLinkProgram(program);
    for (size_t i = 0; i < count; ++i)
        if (shaders[i]) glDetachShader(program, shaders[i]);
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        glDeleteProgram(program);
        GN_ERROR(sLogger)("Failed to link program %s:\n%s", optionalProgramName ? optionalProgramName : "", infoLog);
        return 0;
    }

    // Enable the following code to dump GL program binary to disk.
#if 0
    if (optionalProgramName) {
        std::string outfilename = std::string(optionalProgramName) + ".bin";
        std::ofstream fs;
        fs.open(outfilename);
        if (fs.good()) {
            std::vector<uint8_t> buffer(1024 * 1024 * 1024); // allocate 1MB buffer.
            GLsizei len;
            GLenum dummyFormat;
            GLCHK(glGetProgramBinary(program, (GLsizei)buffer.size(), &len, &dummyFormat, buffer.data()));
            fs.write((const char*)buffer.data(), len);
        }
    }
#endif

    // done
    GN_ASSERT(program);
    return program;
}

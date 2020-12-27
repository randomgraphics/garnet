#pragma once

#include <GL/glew.h>

namespace GN { namespace gfx
{
    class OGLDebugOutputARB
    {
        static const char * source2string( GLenum source )
        {
            switch( source )
            {
                case GL_DEBUG_SOURCE_API_ARB             : return "GL API";
                case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB   : return "Window System";
                case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB : return "Shader Compiler";
                case GL_DEBUG_SOURCE_THIRD_PARTY_ARB     : return "Third Party";
                case GL_DEBUG_SOURCE_APPLICATION_ARB     : return "Application";
                case GL_DEBUG_SOURCE_OTHER_ARB           : return "Other";
                default                                  : return "INVALID_SOURCE";
            }
        }

        static const char * type2string( GLenum type )
        {
            switch( type )
            {
                case GL_DEBUG_TYPE_ERROR_ARB               : return "Error";
                case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB : return "Deprecation";
                case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB  : return "Undefined Behavior";
                case GL_DEBUG_TYPE_PORTABILITY_ARB         : return "Portability";
                case GL_DEBUG_TYPE_PERFORMANCE_ARB         : return "Performance";
                case GL_DEBUG_TYPE_OTHER_ARB               : return "Other";
                default                                    : return "INVALID_TYPE";
            }
        }

        static const char * severity2string( GLenum severity )
        {
            switch( severity )
            {
                case GL_DEBUG_SEVERITY_HIGH_ARB   : return "High";
                case GL_DEBUG_SEVERITY_MEDIUM_ARB : return "Medium";
                case GL_DEBUG_SEVERITY_LOW_ARB    : return "Low";
                default                           : return "INVALID_SEVERITY";
            }
        }

        static void GLAPIENTRY messageCallback(
            GLenum source,
            GLenum type,
            GLuint id,
            GLenum severity,
            GLsizei, // length,
            const GLchar* message,
            const void *) // userparam
        {
            // Determine log level
            Logger::LogLevel logLevel;
            switch( type )
            {
                case GL_DEBUG_TYPE_ERROR_ARB              :
                case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB :
                    logLevel = Logger::ERROR_;
                    break;

                case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB:
                case GL_DEBUG_TYPE_PERFORMANCE_ARB        :
                case GL_DEBUG_TYPE_PORTABILITY            :
                    switch( severity )
                    {
                        case GL_DEBUG_SEVERITY_HIGH_ARB   :
                        case GL_DEBUG_SEVERITY_MEDIUM_ARB :
                            logLevel = Logger::WARN;
                            break;

                        case GL_DEBUG_SEVERITY_LOW_ARB    : // Performance warnings from redundant state changes
                            logLevel = Logger::VVERBOSE;
                            break;

                        default:
                            logLevel = Logger::ERROR_;
                            break;
                    }
                    break;

                case GL_DEBUG_TYPE_OTHER_ARB              :
                    switch( severity )
                    {
                        case GL_DEBUG_SEVERITY_HIGH_ARB   :
                            logLevel = Logger::ERROR_;
                            break;

                        case GL_DEBUG_SEVERITY_MEDIUM_ARB :
                            logLevel = Logger::WARN;
                            break;

                        case GL_DEBUG_SEVERITY_LOW_ARB    :
                            logLevel = Logger::VERBOSE;
                            break;

                        case GL_DEBUG_SEVERITY_NOTIFICATION:
                            logLevel = Logger::VVERBOSE;
                            break;

                        default:
                            logLevel = Logger::ERROR_;
                            break;
                    }
                    break;

                default:
                    // Invalid category
                    logLevel = Logger::ERROR_;
                    break;
            }

            static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu.OGL");
            GN_LOG(sLogger, logLevel)(
                "OpenGL debug output (id=[%d] source=[%s] type=[%s] severity=[%s]): %s",
                id, source2string(source), type2string(type), severity2string(severity), message );
        }

    public:

        static void enable()
        {
            if (!GLEW_ARB_debug_output) return;

            // setup the callback
            glDebugMessageCallbackARB( &messageCallback, NULL );
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);

            // enable all messages
            GN_OGL_CHECK( glDebugMessageControlARB(
                GL_DONT_CARE, // source
                GL_DONT_CARE, // type
                GL_DONT_CARE, // severity
                0, // count
                NULL, // ids
                GL_TRUE ) );
        }
    };
}}
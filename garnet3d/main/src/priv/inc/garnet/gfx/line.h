#ifndef __GN_GFX_LINE_H__
#define __GN_GFX_LINE_H__
// *****************************************************************************
/// \file
/// \brief   Colored line renderer
/// \author  chenli@@REDMOND (2009.5.4)
// *****************************************************************************

namespace GN { namespace gfx
{
    ///
    /// Colored line renderer
    ///
    class GN_API LineRenderer : public StdClass
    {
        GN_DECLARE_STDCLASS( LineRenderer, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        LineRenderer( Gpu & r ) : mGpu(r) { clear(); }
        virtual ~LineRenderer() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init();
        void quit();
    private:
        void clear() { mLines = 0; mBatchingModeEnabled = false; }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        ///
        /// Begin batching draw. Must be pair with batchingEnd().
        ///
        bool batchingBegin()
        {
            if( !mBatchingModeEnabled )
            {
                mBatchingModeEnabled = true;
                return true;
            }
            else
            {
                // Already in batching mode. Call batchingEnd() first.
                return false;
            }
        }

        ///
        /// End batching draw. Must be paired with batchingBegin().
        ///
        void batchingEnd()
        {
            if( mBatchingModeEnabled )
            {
                mBatchingModeEnabled = false;
                flush();
            }
        }

        ///
        /// draw line list
        ///
        void drawLines(
            const void *      positions, // 3D positions
            size_t            stride,    // position buffer stride. 0 for default stride (3 floats).
            size_t            numpoints,
            uint32            colorInRgba,
            const Matrix44f & transform );

        ///
        /// draw a wireframe box
        ///
        void drawBox(
            const Boxf      & box,
            uint32            colorInRgba,
            const Matrix44f & transform );

        // ********************************
        // private variables
        // ********************************
    private:

        struct LineVertex
        {
            Vector3f  pos;
            uint32    colorInRGBA;
            Matrix44f transform;
        };

        struct Line
        {
            LineVertex v0, v1;
        };

        enum
        {
            MAX_LINES = 256,
        };

        Gpu        & mGpu;
        GpuContext   mContext;
        Line       * mLines;
        Line       * mNextPendingLine;
        Line       * mNextFreeLine;
        bool         mBatchingModeEnabled;

        // ********************************
        // private functions
        // ********************************
    private:

        ///
        /// submit any pending line drawing requests to renderer
        ///
        void flush();
    };

    ///
    /// think line renderer
    ///
    class ThickLineRenderer : public StdClass
    {
        GN_DECLARE_STDCLASS( ThickLineRenderer, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        ThickLineRenderer();
        virtual ~ThickLineRenderer() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( Gpu & );
        void quit();
    private:
        void clear() { m_Drawing = false; }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        struct ThickLineVertex
        {
            float    x, y, z;
            float    u, v;
            uint32   color;
        };

        struct ThickLineParameters
        {
            Matrix44f worldview;          // matrix that transform vertex from object space to view space
            Matrix44f proj;               // matrix that transform vertex from view space to clip space
            float     width;              // line width
            bool      widthInScreenSpace; // true  : line width is in pixels in screen space
                                          // false : line width is in view space
        };

        bool drawBegin( const ThickLineParameters & parameters );
        void drawEnd();

        void line( const ThickLineVertex & v0, const ThickLineVertex & v1 );
        void line( float x1, float y1, float z1, float x2, float y2, float z2, uint32 abgr );
        void lineList( const ThickLineVertex * vertices, size_t numverts );

        // ********************************
        // private variables
        // ********************************
    private:

        struct PrivateParameters : public ThickLineParameters
        {
            Matrix44f wvp; // world * view * proj
            float screenWidth, screenHeight; // screen size in pixels
            float endPointHalfWidth, endPointHalfHeight; // size of line end point in clip space.
        };

        struct PrivateVertex
        {
            Vector4f position; // position in clip space
            uint32   color;
            float    u, v;
            uint32   _; // padding
        };

        struct EndPoint
        {
            float posl, posr, post, posb, posz, posw;
            float texl, texr, text, texb;
            uint32 color;

            PrivateVertex * topLeft( PrivateVertex * v );
            PrivateVertex * topRight( PrivateVertex * v );
            PrivateVertex * bottomLeft( PrivateVertex * v );
            PrivateVertex * bottomRight( PrivateVertex * v );
        };

        Gpu        * mGpu;
        GpuContext   mContext;

        bool m_Drawing;
        PrivateParameters m_Parameters;

        static const size_t MAX_VERTICES = 1024;
        PrivateVertex m_Vertices[MAX_VERTICES];
        uint32        m_NumVertices;

        // ********************************
        // private functions
        // ********************************
    private:

        void calcEndPoint(
            EndPoint              & endpoint,
            const ThickLineVertex & vertex );

        PrivateVertex * newPolygon6();

        void flush();
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_LINE_H__

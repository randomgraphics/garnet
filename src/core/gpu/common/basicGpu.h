#ifndef __GN_GPUCOMMON_BASICGPU_H__
#define __GN_GPUCOMMON_BASICGPU_H__
// *****************************************************************************
/// \file
/// \brief   Basic GPU class
/// \author  chenlee (2005.10.1)
// *****************************************************************************

#include <garnet/GNwin.h>

///
/// Rest-in-peace macro
///
#define GN_GPU_RIP GN::gfx::rip

namespace GN {
namespace gfx {
///
/// reset in peace...
///
void rip(const char * msg, ...);

///
/// basic GPU class
///
class BasicGpu : public Gpu, public StdClass {
    GN_DECLARE_STDCLASS(BasicGpu, StdClass);

    // ********************************
    // constructor/destructor
    // ********************************

    //@{
public:
    BasicGpu() { clear(); }
    virtual ~BasicGpu() {}
    //@}

    // ********************************
    // standard init / quit
    // ********************************

    //@{
public:
    bool init(const GpuOptions &);
    void quit();

private:
    void clear() {
        dispClear();
        contextClear();
        miscClear();
    }
    //@}

    // *****************************************************************************
    //
    /// \name                   Display Manager
    //
    // *****************************************************************************

    //@{

public:
    virtual const GpuOptions & getOptions() const { return mOptions; }
    virtual const DispDesc &   getDispDesc() const { return mDispDesc; }
    virtual GN::win::Window &  getRenderWindow() const {
        GN_ASSERT(mWindow);
        return *mWindow;
    }

protected:
    intptr_t getRenderWindowHandle() const { return mWindow ? mWindow->getWindowHandle() : 0; }

    void handleRenderWindowSizeMove();

private:
    bool dispInit(const GpuOptions &);
    void dispQuit();
    void dispClear() {
        mWindow     = 0;
        mOldMonitor = 0;
        mOldWindowSize.set(0, 0);
    }

private:
    GpuOptions        mOptions;
    DispDesc          mDispDesc;
    GN::win::Window * mWindow;
    intptr_t          mOldMonitor;
    Vector2<uint32_t> mOldWindowSize;

    //@}

    // *****************************************************************************
    //
    /// \name                   Caps Manager
    //
    // *****************************************************************************

    //@{

    //@}

    // *****************************************************************************
    //
    /// \name                   Resource Manager
    //
    // *****************************************************************************

    //@{

    //@}

    // *****************************************************************************
    //
    /// \name                   Context Manager
    //
    // *****************************************************************************

    //@{

public:
    virtual void                      bindContext(const GpuContext & c);
    virtual void                      rebindContext();
    virtual inline const GpuContext & getContext() const { return mContext; }

protected:
    virtual bool bindContextImpl(const GpuContext & context, bool skipDirtyCheck) = 0;

private:
    void contextClear() {
        mContextOk = false;
        mContext.clear();
    }

protected:
    GpuContext mContext;
    bool       mContextOk;

    //@}

    // *****************************************************************************
    //
    /// \name                   Draw Manager
    //
    // *****************************************************************************

    //@{

    void drawLines(uint32, const void *, uint32, uint32, uint32, const Matrix44f &, const Matrix44f &, const Matrix44f &) {
        static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu.common");
        GN_ERROR(sLogger)("obsolete functions. will be removed sooon.");
    }

    //@}

    // *****************************************************************************
    //
    /// \name                   Misc Manager
    //
    // *****************************************************************************

    //@{

public:
    virtual GpuSignals & getSignals() { return mSignals; }
    virtual void         getBackBufferContent(BackBufferContent &);
    virtual void         setUserData(const Guid & id, const void * data, uint32 length);
    virtual const void * getUserData(const Guid & id, uint32 * length) const;
    virtual bool         hasUserData(const Guid & id) const;
    virtual void         debugEnableParameterCheck(bool enable) { mParamCheckEnabled = enable; }
    virtual void         debugMarkBegin(const char *) {}
    virtual void         debugMarkEnd() {}
    virtual void         debugMarkSet(const char *) {}

public:
    bool paramCheckEnabled() const { return mParamCheckEnabled; }

private:
    void miscClear() {
        mParamCheckEnabled = GN_BUILD_DEBUG_ENABLED;
        mUserData.clear();
    }

private:
    typedef DynaArray<uint8> UserData;

    typedef Dictionary<Guid, UserData> UserDataMap;

    GpuSignals  mSignals;
    bool        mParamCheckEnabled;
    UserDataMap mUserData;

    //@}
};

///
/// render targets description.
///
/// Memory layout of this must match the render target fields defined in GpuContext.
///
struct RenderTargetDesc {
    /// color render targets
    StackArray<RenderTargetTexture, GpuContext::MAX_COLOR_RENDER_TARGETS, uint32> colortargets;

    /// depth stencil render target
    RenderTargetTexture depthstencil;

    /// check for invalid description.
    bool valid() const {
        for (uint32 i = 0; i < colortargets.size(); ++i) {
            if (!colortargets[i].texture) {
                GN_ERROR(GN::getLogger("GN.gfx"))("NULL color render targets in render target array is not allowed.");
                return false;
            }
        }

        return true;
    }

    /// equality check
    bool operator==(const RenderTargetDesc & rhs) const {
        if (colortargets.size() != rhs.colortargets.size()) return false;
        for (uint32 i = 0; i < colortargets.size(); ++i) {
            if (colortargets[i] != rhs.colortargets[i]) return false;
        }
        return depthstencil == rhs.depthstencil;
    }

    /// equality check
    bool operator!=(const RenderTargetDesc & rhs) const { return !operator==(rhs); }
};
} // namespace gfx
} // namespace GN

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GPUCOMMON_BASICGPU_H__

#if !defined(__GN_INSIDE_FX2_H__)
    #error "Include <garnet/GNfx2.h> instead."
#endif

namespace GN::fx2 {

/// Reusable device-bound GPU effect. Derived interfaces define typed recording
/// operations; there is no universal argument dictionary or implicit submission.
/// Keep resources alive through recorded work and copy immediate values during record().
struct Kernel : RCRT64 {
    GN_API GN_REGISTER_RUNTIME_TYPE(RCRT64);
    enum class Execution { RASTER, COMPUTE };
    /// The execution strategy is fixed by the concrete kernel, never selected implicitly.
    virtual Execution execution() const = 0;

protected:
    using RCRT64::RCRT64;
};

} // namespace GN::fx2

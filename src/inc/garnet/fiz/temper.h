#pragma once
#if !defined(__GN_INSIDE_FIZ_H__)
    #error "Do not include <garnet/fiz/temper.h> directly. Include <garnet/GNfiz.h> instead."
#endif

namespace GN::fiz {

/// Constitutive physical material properties governing contact, restitution, and compliance.
struct Temper {
    Scalar density        = 1000.0f; ///< Density in mass per unit volume (e.g. kg/m^3 or custom unit). Default: water density.
    Scalar restitution    = 0.0f;    ///< Coefficient of restitution [0.0 = completely inelastic, 1.0 = perfectly elastic].
    Scalar friction       = 0.5f;    ///< Coulomb friction coefficient [0.0 = frictionless, 1.0 = high traction].
    Scalar linearDamping  = 0.05f;   ///< Drag coefficient opposing linear velocity.
    Scalar angularDamping = 0.05f;   ///< Drag coefficient opposing rotational angular velocity.
    Scalar compliance     = 0.0f;    ///< XPBD inverse stiffness (0.0 = rigid, >0.0 = elastic compliance).

    /// Preset temper factories for convenience.
    static Temper rock() {
        Temper t;
        t.density     = 2500.0f;
        t.restitution = 0.1f;
        t.friction    = 0.8f;
        return t;
    }

    static Temper wood() {
        Temper t;
        t.density     = 700.0f;
        t.restitution = 0.3f;
        t.friction    = 0.6f;
        return t;
    }

    static Temper metal() {
        Temper t;
        t.density     = 7800.0f;
        t.restitution = 0.2f;
        t.friction    = 0.4f;
        return t;
    }

    static Temper rubber() {
        Temper t;
        t.density        = 1100.0f;
        t.restitution    = 0.85f;
        t.friction       = 0.9f;
        t.linearDamping  = 0.01f;
        t.angularDamping = 0.01f;
        return t;
    }

    static Temper ice() {
        Temper t;
        t.density     = 917.0f;
        t.restitution = 0.05f;
        t.friction    = 0.05f;
        return t;
    }

    static Temper bouncer() {
        Temper t;
        t.density        = 1000.0f;
        t.restitution    = 1.0f; // perfectly elastic
        t.friction       = 0.0f;
        t.linearDamping  = 0.0f;
        t.angularDamping = 0.0f;
        return t;
    }
};

} // namespace GN::fiz

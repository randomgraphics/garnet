// form.cpp — the engine's concrete Form implementation behind the sealed public interface.
// All structural state (hierarchy, transform, facet list) and its invariants live here;
// client code only ever sees the pure-virtual GN::e2::Form.

#include <garnet/GNengine2.h>

#include <cmath>

using namespace GN;
using namespace GN::e2;

namespace {

struct FormImpl : Form {
    GN_REGISTER_RUNTIME_TYPE(Form);

    FormImpl(Universe & u, const StrA & name): Form(TYPE_INFO(), u.generateUniqueIdentifier(), name) {}

    // Facets and children are ref-counted and may outlive this form: clear their
    // back-pointers so they do not dangle. A surviving child also loses its world
    // membership, since that membership was inherited through this form.
    ~FormImpl() override {
        for (auto & f : mFacets) {
            if (mWorld) f->leaveWorld(*mWorld);
            setFacetOwner(*f, nullptr);
        }
        for (auto & c : mChildren) {
            auto * impl = RuntimeType::cast<FormImpl>(c.get());
            GN_ASSERT(impl); // attach() admits only engine-created forms
            impl->mParent = nullptr;
            impl->setWorldRecursive(nullptr);
        }
    }

    World * world() const override { return mWorld; }

    Form * parent() const override { return mParent; }

    ArrayView<Ref<Form>> children() override { return {mChildren.data(), mChildren.size()}; }

    ArrayView<const Ref<Form>> children() const override { return {mChildren.data(), mChildren.size()}; }

    bool attach(Ref<Form> child) override {
        // The tree mutates internal state of its members (parent pointer, world propagation),
        // so only forms backed by this implementation can be attached.
        auto * impl = RuntimeType::cast<FormImpl>(child.get());
        if (!impl) GN_UNLIKELY return false;
        if (impl == this) GN_UNLIKELY return false;
        if (impl->mParent) GN_UNLIKELY return false;
        if (impl->mWorld && impl->mWorld != mWorld) GN_UNLIKELY return false;
        for (auto * p = mParent; p; p = p->mParent) {
            if (p == impl) GN_UNLIKELY return false;
        }
        impl->mParent = this;
        impl->setWorldRecursive(mWorld);
        mChildren.append(child);
        return true;
    }

    const WorldVector3 & position() const override { return mPosition; }

    void setPosition(const WorldVector3 & p) override { mPosition = p; }

    const Rotation & rotation() const override { return mRotation; }

    void setRotation(const Rotation & r) override { mRotation = r; }

    ArrayView<Ref<Facet>> facets() override { return {mFacets.data(), mFacets.size()}; }

    ArrayView<const Ref<Facet>> facets() const override { return {mFacets.data(), mFacets.size()}; }

    bool addFacet(Ref<Facet> facet) override {
        if (!facet) GN_UNLIKELY return false;
        if (facet->form()) GN_UNLIKELY return false;
        setFacetOwner(*facet, this);
        mFacets.append(facet);
        if (mWorld) facet->enterWorld(*mWorld);
        return true;
    }

    bool enterWorld(World & world) override {
        if (mWorld) GN_UNLIKELY return false;
        if (mParent) GN_UNLIKELY return false;
        setWorldRecursive(&world);
        return true;
    }

    void leaveWorld(World & world) override {
        if (mWorld == &world) setWorldRecursive(nullptr);
    }

    void live() override {
        for (auto & f : mFacets) f->live();
    }

private:
    World *               mWorld  = nullptr;
    FormImpl *            mParent = nullptr;
    ArrayBody<Ref<Form>>  mChildren;
    ArrayBody<Ref<Facet>> mFacets;
    WorldVector3          mPosition = {WorldLength(0), WorldLength(0), WorldLength(0)};
    Rotation              mRotation = Rotation(1.f, 0.f, 0.f, 0.f);

    // Propagates world membership through the form tree and notifies facets of the
    // transition. Facets see consistent state on both sides: on enter, mWorld is set before
    // the callback; on leave, it is cleared after. A world-to-world jump cannot happen —
    // attach() and enterWorld() both reject forms that already belong to a different world.
    void setWorldRecursive(World * world) {
        if (mWorld == world) return;
        if (mWorld) {
            for (auto & f : mFacets) f->leaveWorld(*mWorld);
        }
        mWorld = world;
        if (mWorld) {
            for (auto & f : mFacets) f->enterWorld(*mWorld);
        }
        for (auto & child : mChildren) {
            auto * impl = RuntimeType::cast<FormImpl>(child.get());
            GN_ASSERT(impl); // attach() admits only engine-created forms
            impl->setWorldRecursive(world);
        }
    }
};

// Rotate a world-unit vector in double precision: doubles represent integers up to 2^53
// exactly, so practical world coordinates survive the round trip; float would corrupt
// large int64 positions.
WorldVector3 rotatedBy(const Rotation & q, const WorldVector3 & v) {
    glm::dvec3 d = glm::dquat(q) * glm::dvec3((double) v.x.raw(), (double) v.y.raw(), (double) v.z.raw());
    return {WorldLength((int64_t) std::llround(d.x)), WorldLength((int64_t) std::llround(d.y)), WorldLength((int64_t) std::llround(d.z))};
}

} // namespace

namespace GN::e2 {

Ref<Form> Form::create(Universe & universe, const StrA & name) { return referenceTo(new FormImpl(universe, name)); }

WorldVector3 Form::worldPosition() const {
    auto * p = parent();
    return p ? p->worldPosition() + rotatedBy(p->worldRotation(), position()) : position();
}

Rotation Form::worldRotation() const {
    auto * p = parent();
    return p ? p->worldRotation() * rotation() : rotation();
}

} // namespace GN::e2

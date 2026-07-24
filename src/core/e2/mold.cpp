#include <garnet/GNengine2.h>

#include <utility>

using namespace GN;
using namespace GN::e2;

namespace {

struct MoldImpl : Mold {
    GN_REGISTER_RUNTIME_TYPE(Mold);

    MoldImpl(Universe & universe, const StrA & name, Factory factory)
        : Mold(TYPE_INFO(), universe.generateUniqueIdentifier(), name), mFactory(std::move(factory)) {}

    bool add(Ref<Mold> child, const StrA & childName) override {
        if (!child) GN_UNLIKELY return false;
        if (child.get() == this) GN_UNLIKELY return false;
        if (contains(child.get(), *this)) GN_UNLIKELY return false;
        mChildren.append(Child {child, childName});
        return true;
    }

    Ref<Form> cast(Universe & universe, const StrA & formName) const override {
        auto form = mFactory(CreateParameters {universe, formName});
        if (!form) GN_UNLIKELY return {};
        for (auto & child : mChildren) {
            auto childForm = child.mold->cast(universe, child.name);
            if (!form->attach(childForm)) GN_UNLIKELY return {};
        }
        return form;
    }

private:
    struct Child {
        Ref<Mold> mold;
        StrA      name;
    };

    Factory          mFactory;
    ArrayBody<Child> mChildren;

    static bool contains(const Mold * haystack, const Mold & needle) {
        if (haystack == &needle) return true;
        auto * impl = RuntimeType::cast<MoldImpl>(haystack);
        if (!impl) return false;
        for (auto & child : impl->mChildren) {
            if (contains(child.mold.get(), needle)) return true;
        }
        return false;
    }
};

} // namespace

namespace GN::e2 {

Ref<Mold> Mold::create(Universe & universe, const StrA & name, Factory factory) {
    if (!factory) GN_UNLIKELY return {};
    return referenceTo(new MoldImpl(universe, name, std::move(factory)));
}

} // namespace GN::e2

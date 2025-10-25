#include "../testCommon.h"

class BlobTest : public CxxTest::TestSuite {
    struct Element {
        inline static int count = 0; ///< number of Element instances
        inline static int ctor  = 0; ///< number of calls to ctor
        inline static int dtor  = 0; ///< number of calls to dtor
        inline static int cctor = 0; ///< number of calls to copy constructor
        inline static int cop   = 0; ///< number of calls to copy operator

        static void clear() {
            ctor  = 0;
            dtor  = 0;
            cctor = 0;
            cop   = 0;
        }

        bool constructed;
        int  i;

        Element(): constructed(true) {
            ++ctor;
            ++count;
        }

        Element(int i_): constructed(true), i(i_) {
            ++ctor;
            ++count;
        }

        Element(const Element & o): constructed(true), i(o.i) {
            if (!o.constructed) { GN_UNEXPECTED(); }

            ++cctor;
            ++count;
        }

        ~Element() {
            if (!constructed) { GN_UNEXPECTED(); }

            constructed = false;

            ++dtor;
            --count;
        }

        Element & operator=(const Element & o) {
            if (!constructed || !o.constructed) { GN_UNEXPECTED(); }

            i = o.i;

            ++cop;
            return *this;
        }
    };

public:
    void testBlobBasic() {
        using namespace GN;

        // Test basic blob functionality
        SimpleBlob<int> blob1(5);
        TS_ASSERT_EQUALS(5 * sizeof(int), blob1.size());
        TS_ASSERT(!blob1.empty());
        TS_ASSERT(blob1.data() != nullptr);

        // Test empty blob
        SimpleBlob<int> blob2(0);
        TS_ASSERT_EQUALS(0, blob2.size());
        TS_ASSERT(blob2.empty());
        TS_ASSERT_EQUALS(nullptr, blob2.data());

        // Test clear functionality
        blob1.clear();
        TS_ASSERT_EQUALS(0, blob1.size());
        TS_ASSERT(blob1.empty());
        TS_ASSERT_EQUALS(nullptr, blob1.data());
    }

    void testBlobWithData() {
        using namespace GN;

        int             data[] = {1, 2, 3, 4, 5};
        SimpleBlob<int> blob(5, data);

        TS_ASSERT_EQUALS(5 * sizeof(int), blob.size());
        TS_ASSERT(!blob.empty());

        // Test accessor
        auto accessor = blob.accessor<int>();
        TS_ASSERT_EQUALS(5, accessor.size());
        TS_ASSERT_EQUALS(1, accessor[0]);
        TS_ASSERT_EQUALS(2, accessor[1]);
        TS_ASSERT_EQUALS(3, accessor[2]);
        TS_ASSERT_EQUALS(4, accessor[3]);
        TS_ASSERT_EQUALS(5, accessor[4]);
    }

    void testBlobWithObjects() {
        using namespace GN;

        Element::clear();

        {
            Element data[] = {Element(1), Element(2), Element(3)};
            TS_ASSERT_EQUALS(3, Element::count);
            TS_ASSERT_EQUALS(3, Element::ctor);
            TS_ASSERT_EQUALS(0, Element::cctor);

            SimpleBlob<Element> blob(3, data);
            TS_ASSERT_EQUALS(6, Element::count); // 3 original + 3 copied
            TS_ASSERT_EQUALS(3, Element::ctor);
            TS_ASSERT_EQUALS(3, Element::cctor);
            TS_ASSERT_EQUALS(0, Element::dtor);

            TS_ASSERT_EQUALS(3 * sizeof(Element), blob.size());
            TS_ASSERT(!blob.empty());
        }

        // All elements should be destroyed when blob goes out of scope
        TS_ASSERT_EQUALS(0, Element::count);
        TS_ASSERT_EQUALS(6, Element::dtor); // 3 original + 3 from blob
    }

    void testBlobDefaultConstructed() {
        using namespace GN;

        Element::clear();

        {
            SimpleBlob<Element> blob(3); // Default construct 3 elements
            TS_ASSERT_EQUALS(3, Element::count);
            TS_ASSERT_EQUALS(3, Element::ctor);
            TS_ASSERT_EQUALS(0, Element::cctor);
            TS_ASSERT_EQUALS(0, Element::dtor);

            TS_ASSERT_EQUALS(3 * sizeof(Element), blob.size());
            TS_ASSERT(!blob.empty());
        }

        TS_ASSERT_EQUALS(0, Element::count);
        TS_ASSERT_EQUALS(3, Element::dtor);
    }
};

class DynaArrayBlobTest : public CxxTest::TestSuite {
    struct Element {
        inline static int count = 0;
        inline static int ctor  = 0;
        inline static int dtor  = 0;
        inline static int cctor = 0;
        inline static int cop   = 0;
        inline static int mctor = 0;
        inline static int mop   = 0;

        static void clear() {
            ctor  = 0;
            dtor  = 0;
            cctor = 0;
            cop   = 0;
            mctor = 0;
            mop   = 0;
        }

        bool constructed = true;
        int  i           = 0;

        Element() {
            ++ctor;
            ++count;
        }

        Element(int i_): i(i_) {
            ++ctor;
            ++count;
        }

        Element(const Element & o) {
            GN_VERIFY(o.constructed);
            i = o.i;
            ++cctor;
            ++count;
        }

        Element(Element && o) {
            GN_VERIFY(o.constructed);
            i   = o.i;
            o.i = 0;
            ++mctor;
            ++count;
        }

        ~Element() {
            GN_VERIFY(constructed);
            constructed = false;
            i           = 0;
            ++dtor;
            --count;
        }

        Element & operator=(Element && o) {
            GN_VERIFY(constructed && o.constructed);
            i   = o.i;
            o.i = 0;
            ++mop;
            return *this;
        }

        Element & operator=(const Element & o) {
            GN_VERIFY(constructed && o.constructed);
            i = o.i;
            ++cop;
            return *this;
        }
    };

public:
    void testDynaArrayBlobBasic() {
        using namespace GN;

        DynaArrayBlob<int> blob;
        TS_ASSERT_EQUALS(0, blob.size());
        TS_ASSERT(blob.empty());
        TS_ASSERT_EQUALS(nullptr, blob.data());
    }

    void testDynaArrayBlobResize() {
        using namespace GN;

        DynaArrayBlob<int> blob;

        // Test resize
        blob.resize(3);
        TS_ASSERT_EQUALS(3 * sizeof(int), blob.size());
        TS_ASSERT(!blob.empty());
        TS_ASSERT(blob.data() != nullptr);

        // Test resize to larger size
        blob.resize(5);
        TS_ASSERT_EQUALS(5 * sizeof(int), blob.size());
        TS_ASSERT(!blob.empty());

        // Test resize to smaller size
        blob.resize(2);
        TS_ASSERT_EQUALS(2 * sizeof(int), blob.size());
        TS_ASSERT(!blob.empty());

        // Test resize to zero
        blob.resize(0);
        TS_ASSERT_EQUALS(0, blob.size());
        TS_ASSERT(blob.empty());
    }

    void testDynaArrayBlobReserve() {
        using namespace GN;

        DynaArrayBlob<int> blob;

        // Test reserve
        blob.reserve(10);
        TS_ASSERT_EQUALS(0, blob.size());
        TS_ASSERT(blob.empty());

        // Test reserve with existing data
        blob.resize(3);
        TS_ASSERT_EQUALS(3 * sizeof(int), blob.size());

        blob.reserve(15);
        TS_ASSERT_EQUALS(3 * sizeof(int), blob.size()); // Size should not change
        TS_ASSERT(!blob.empty());
    }

    void testDynaArrayBlobAppend() {
        using namespace GN;

        DynaArrayBlob<int> blob;

        // Test append
        blob.append(1);
        TS_ASSERT_EQUALS(1 * sizeof(int), blob.size());
        TS_ASSERT(!blob.empty());

        blob.append(2);
        TS_ASSERT_EQUALS(2 * sizeof(int), blob.size());

        blob.append(3);
        TS_ASSERT_EQUALS(3 * sizeof(int), blob.size());

        // Test accessor
        auto accessor = blob.accessor<int>();
        TS_ASSERT_EQUALS(3, accessor.size());
        TS_ASSERT_EQUALS(1, accessor[0]);
        TS_ASSERT_EQUALS(2, accessor[1]);
        TS_ASSERT_EQUALS(3, accessor[2]);
    }

    void testDynaArrayBlobWithObjects() {
        using namespace GN;

        Element::clear();

        {
            DynaArrayBlob<Element> blob;

            // Test append with objects
            blob.append(Element(1));
            TS_ASSERT_EQUALS(1, Element::count);
            TS_ASSERT_EQUALS(1, Element::ctor);
            TS_ASSERT_EQUALS(0, Element::cctor);
            TS_ASSERT_EQUALS(0, Element::dtor);

            blob.append(Element(2));
            TS_ASSERT_EQUALS(2, Element::count);
            TS_ASSERT_EQUALS(2, Element::ctor);
            TS_ASSERT_EQUALS(0, Element::cctor);
            TS_ASSERT_EQUALS(0, Element::dtor);

            TS_ASSERT_EQUALS(2 * sizeof(Element), blob.size());
            TS_ASSERT(!blob.empty());
        }

        // All elements should be destroyed when blob goes out of scope
        TS_ASSERT_EQUALS(0, Element::count);
        TS_ASSERT_EQUALS(2, Element::dtor);
    }

    void testDynaArrayBlobResizeWithObjects() {
        using namespace GN;

        Element::clear();

        {
            DynaArrayBlob<Element> blob;

            // Test resize with objects
            blob.resize(3);
            TS_ASSERT_EQUALS(3, Element::count);
            TS_ASSERT_EQUALS(3, Element::ctor);
            TS_ASSERT_EQUALS(0, Element::cctor);
            TS_ASSERT_EQUALS(0, Element::dtor);

            // Test resize to larger size
            blob.resize(5);
            TS_ASSERT_EQUALS(5, Element::count);
            TS_ASSERT_EQUALS(5, Element::ctor);
            TS_ASSERT_EQUALS(0, Element::cctor);
            TS_ASSERT_EQUALS(0, Element::dtor);

            // Test resize to smaller size
            blob.resize(2);
            TS_ASSERT_EQUALS(2, Element::count);
            TS_ASSERT_EQUALS(5, Element::ctor);
            TS_ASSERT_EQUALS(0, Element::cctor);
            TS_ASSERT_EQUALS(3, Element::dtor);
        }

        TS_ASSERT_EQUALS(0, Element::count);
        TS_ASSERT_EQUALS(5, Element::dtor); // 2 remaining + 3 from resize down
    }

    void testDynaArrayBlobChaining() {
        using namespace GN;

        DynaArrayBlob<int> blob;

        // Test method chaining
        blob.reserve(10).resize(3).append(1).append(2).append(3);

        TS_ASSERT_EQUALS(3 * sizeof(int), blob.size());
        TS_ASSERT(!blob.empty());

        auto accessor = blob.accessor<int>();
        TS_ASSERT_EQUALS(3, accessor.size());
        TS_ASSERT_EQUALS(1, accessor[0]);
        TS_ASSERT_EQUALS(2, accessor[1]);
        TS_ASSERT_EQUALS(3, accessor[2]);
    }
};

class BlobEdgeCaseTest : public CxxTest::TestSuite {
public:
    void testBlobZeroSize() {
        using namespace GN;

        // Test SimpleBlob with zero size
        SimpleBlob<int> blob1(0);
        TS_ASSERT_EQUALS(0, blob1.size());
        TS_ASSERT(blob1.empty());
        TS_ASSERT_EQUALS(nullptr, blob1.data());

        // Test DynaArrayBlob with zero size
        DynaArrayBlob<int> blob2;
        blob2.resize(0);
        TS_ASSERT_EQUALS(0, blob2.size());
        TS_ASSERT(blob2.empty());
        TS_ASSERT_EQUALS(nullptr, blob2.data());
    }

    void testBlobAccessorEdgeCases() {
        using namespace GN;

        SimpleBlob<int> blob(3);
        auto            accessor = blob.accessor<int>();

        // Test accessor properties
        TS_ASSERT_EQUALS(3, accessor.size());
        TS_ASSERT(accessor.data() != nullptr);

        // Test with empty blob
        SimpleBlob<int> emptyBlob(0);
        auto            emptyAccessor = emptyBlob.accessor<int>();
        TS_ASSERT_EQUALS(0, emptyAccessor.size());
        TS_ASSERT_EQUALS(nullptr, emptyAccessor.data());
    }

    void testBlobClear() {
        using namespace GN;

        SimpleBlob<int> blob(5);
        TS_ASSERT_EQUALS(5 * sizeof(int), blob.size());
        TS_ASSERT(!blob.empty());

        blob.clear();
        TS_ASSERT_EQUALS(0, blob.size());
        TS_ASSERT(blob.empty());
        TS_ASSERT_EQUALS(nullptr, blob.data());

        // Test clear on already empty blob
        blob.clear();
        TS_ASSERT_EQUALS(0, blob.size());
        TS_ASSERT(blob.empty());
        TS_ASSERT_EQUALS(nullptr, blob.data());
    }

    void testBlobRefCounter() {
        using namespace GN;

        // Test that Blob inherits from RefCounter
        AutoRef<SimpleBlob<int>> blob = referenceTo(new SimpleBlob<int>(5));
        TS_ASSERT_EQUALS(1, blob->getref());

        AutoRef<SimpleBlob<int>> ref(blob);
        TS_ASSERT_EQUALS(2, blob->getref());

        // Test that blob is properly destroyed when ref goes out of scope
        ref.clear();
        TS_ASSERT_EQUALS(1, blob->getref());

        delete blob;
    }
};

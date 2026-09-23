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
            GN_REQUIRE(o.constructed);
            i = o.i;
            ++cctor;
            ++count;
        }

        Element(Element && o) {
            GN_REQUIRE(o.constructed);
            i   = o.i;
            o.i = 0;
            ++mctor;
            ++count;
        }

        ~Element() {
            GN_REQUIRE(constructed);
            constructed = false;
            i           = 0;
            ++dtor;
            --count;
        }

        Element & operator=(Element && o) {
            GN_REQUIRE(constructed && o.constructed);
            i   = o.i;
            o.i = 0;
            ++mop;
            return *this;
        }

        Element & operator=(const Element & o) {
            GN_REQUIRE(constructed && o.constructed);
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
            blob.reserve(2);

            // Test append with objects
            blob.append(Element(1));
            TS_ASSERT_EQUALS(1, Element::count);
            TS_ASSERT_EQUALS(1, Element::ctor);
            TS_ASSERT_EQUALS(1, Element::cctor);
            TS_ASSERT_EQUALS(1, Element::dtor);

            blob.append(Element(2));
            TS_ASSERT_EQUALS(2, Element::count);
            TS_ASSERT_EQUALS(2, Element::ctor);
            TS_ASSERT_EQUALS(2, Element::cctor);
            TS_ASSERT_EQUALS(2, Element::dtor);

            TS_ASSERT_EQUALS(2 * sizeof(Element), blob.size());
            TS_ASSERT(!blob.empty());
        }

        // All elements should be destroyed when blob goes out of scope
        TS_ASSERT_EQUALS(0, Element::count);
        TS_ASSERT_EQUALS(4, Element::dtor);
    }

    void testDynaArrayBlobResizeWithObjects() {
        using namespace GN;

        Element::clear();

        {
            DynaArrayBlob<Element> blob;
            blob.reserve(5);

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

        TS_ASSERT_EQUALS(6 * sizeof(int), blob.size());
        TS_ASSERT(!blob.empty());

        auto accessor = blob.accessor<int>();
        TS_ASSERT_EQUALS(6, accessor.size());
        TS_ASSERT_EQUALS(1, accessor[3]);
        TS_ASSERT_EQUALS(2, accessor[4]);
        TS_ASSERT_EQUALS(3, accessor[5]);
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
    }
};

class FixedBlobTest : public CxxTest::TestSuite {
    struct Element {
        static inline size_t count = 0;
        static inline size_t ctor  = 0;
        static inline size_t cctor = 0;
        static inline size_t dtor  = 0;
        static inline size_t cop   = 0;

        static void clear() {
            count = 0;
            ctor  = 0;
            cctor = 0;
            dtor  = 0;
            cop   = 0;
        }

        bool constructed;
        int  i = 0;

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

        Element & operator=(const Element & o) {
            if (!constructed || !o.constructed) { GN_UNEXPECTED(); }
            i = o.i;
            ++cop;
            return *this;
        }

        ~Element() {
            if (!constructed) { GN_UNEXPECTED(); }
            constructed = false;
            ++dtor;
            --count;
        }
    };

public:
    void testFixedBlobBasic() {
        using namespace GN;

        FixedBlob<int> pool(4);
        TS_ASSERT_EQUALS(4, pool.count());
        TS_ASSERT_EQUALS(4 * sizeof(int), pool.blobSize());
        TS_ASSERT_EQUALS(0, pool.pooledCount());

        int  values[4] = {10, 20, 30, 40};
        auto b1        = pool.allocate(values);
        TS_ASSERT(b1);
        TS_ASSERT_EQUALS(4 * sizeof(int), b1->size());
        TS_ASSERT_EQUALS(1, pool.pooledCount());

        auto acc1 = b1->accessor<int>();
        TS_ASSERT_EQUALS(10, acc1[0]);
        TS_ASSERT_EQUALS(20, acc1[1]);
        TS_ASSERT_EQUALS(30, acc1[2]);
        TS_ASSERT_EQUALS(40, acc1[3]);

        // b1 is active, so allocate creates a new blob
        auto b2 = pool.allocate();
        TS_ASSERT(b2);
        TS_ASSERT_EQUALS(2, pool.pooledCount());

        // Release b1 so its refcount returns to 1 (held only by pool)
        b1.clear();

        // Next allocation should reuse b1 instead of creating a 3rd blob
        int  newValues[4] = {100, 200, 300, 400};
        auto b3           = pool.allocate(newValues);
        TS_ASSERT(b3);
        TS_ASSERT_EQUALS(2, pool.pooledCount());

        auto acc3 = b3->accessor<int>();
        TS_ASSERT_EQUALS(100, acc3[0]);
        TS_ASSERT_EQUALS(200, acc3[1]);
        TS_ASSERT_EQUALS(300, acc3[2]);
        TS_ASSERT_EQUALS(400, acc3[3]);
    }

    void testFixedBlobWithObjects() {
        using namespace GN;

        Element::clear();

        {
            FixedBlob<Element> pool(2);
            TS_ASSERT_EQUALS(0, Element::count);

            Element data[2] = {Element(1), Element(2)};
            TS_ASSERT_EQUALS(2, Element::count);
            TS_ASSERT_EQUALS(2, Element::ctor);

            auto b1 = pool.allocate(data);
            TS_ASSERT_EQUALS(4, Element::count); // 2 data + 2 in b1
            TS_ASSERT_EQUALS(2, Element::cctor);

            // Release b1 (destructs elements and returns raw buffer to free list)
            b1.clear();
            TS_ASSERT_EQUALS(2, Element::count); // only data[2] alive

            // Allocate again, reusing b1's buffer from free list with data2
            Element data2[2] = {Element(10), Element(20)};
            auto    b2       = pool.allocate(data2);
            TS_ASSERT_EQUALS(1, pool.pooledCount());
            TS_ASSERT_EQUALS(1, pool.activeCount());
            TS_ASSERT_EQUALS(0, pool.freeCount());
            TS_ASSERT_EQUALS(6, Element::count); // data[2], data2[2], b2[2]
            TS_ASSERT_EQUALS(4, Element::cctor);

            b2.clear();
            TS_ASSERT_EQUALS(4, Element::count); // data[2], data2[2]
            TS_ASSERT_EQUALS(1, pool.freeCount());
            TS_ASSERT_EQUALS(0, pool.activeCount());
        }

        // After pool and local arrays destruction, all elements must be destructed
        TS_ASSERT_EQUALS(0, Element::count);
    }

    void testFixedBlobPoolDestroyedBeforeBlobs() {
        using namespace GN;

        Element::clear();
        AutoRef<Blob> b;
        {
            FixedBlob<Element> pool(2);
            Element            data[2] = {Element(5), Element(6)};
            b                          = pool.allocate(data);
            TS_ASSERT_EQUALS(4, Element::count); // 2 in data, 2 in b
            TS_ASSERT(b);
            TS_ASSERT_EQUALS(2 * sizeof(Element), b->size());
            // pool is destroyed here; should detach b and destruct its elements
        }
        // Elements in b are destructed when pool died
        TS_ASSERT_EQUALS(0, Element::count);
        TS_ASSERT_EQUALS(0, b->size());
        TS_ASSERT(nullptr == b->data());

        // Releasing b now should be completely safe
        b.clear();
        TS_ASSERT_EQUALS(0, Element::count);
    }
};

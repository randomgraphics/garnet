#include "../testCommon.h"

#if GN_GNUC
    #pragma GCC diagnostic ignored "-Warray-bounds" // need to disable this warning to test the out-of-bound access.
#endif

class StackArrayTest : public CxxTest::TestSuite {
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
    void testAppend() {
        using namespace GN;

        StackArray<int, 2> a;
        a.append(1);
        a.append(2);
        a.append(3); // should do nothing
        TS_ASSERT_EQUALS(2, a.size());
        TS_ASSERT_EQUALS(1, a.front());
        TS_ASSERT_EQUALS(2, a.back());

        Element::clear();
        StackArray<Element, 2> b;
        TS_ASSERT_EQUALS(0, Element::count);

        b.append(1);
        b.append(2);
        b.append(3); // should do nothing
        TS_ASSERT_EQUALS(2, Element::count);
    }

    void testInsert() {
        using namespace GN;
        StackArray<int, 4> a;
        a.insert(0, 1);   // 1 (insert at head)
        a.insert(1, 3);   // 1, 3 (insert at tail)
        a.insert(1, 2);   // 1, 2, 3 (insert at middle)
        a.insert(100, 0); // invalid position, should do nothing
        a.insert(3, 4);   // 1, 2, 3, 4 (insert at tail)
        a.insert(4, 5);   // already full, should do nothing
        TS_ASSERT_EQUALS(4, a.size());
        TS_ASSERT_EQUALS(1, a[0]);
        TS_ASSERT_EQUALS(2, a[1]);
        TS_ASSERT_EQUALS(3, a[2]);
        TS_ASSERT_EQUALS(4, a[3]);
    }

    void testInsert2() {
        using namespace GN;
        StackArray<Element, 4> a;
        a.insert(0, 1);   // 1 (insert at head)
        a.insert(1, 3);   // 1, 3 (insert at tail)
        a.insert(1, 2);   // 1, 2, 3 (insert at middle)
        a.insert(100, 0); // invalid position, should do nothing
        a.insert(3, 4);   // 1, 2, 3, 4 (insert at tail)
        a.insert(4, 5);   // already full, should do nothing
        TS_ASSERT_EQUALS(4, a.size());
    }

    void testErase() {
        using namespace GN;
        StackArray<int, 4> a;
        a.append(1);
        a.append(2);
        a.append(3);
        a.append(4);

        // erase middle: 1, 2, 4
        a.eraseIdx(2);
        TS_ASSERT_EQUALS(3, a.size());
        TS_ASSERT_EQUALS(4, a[2]);

        // erase head: 2, 4
        a.eraseIdx(0);
        TS_ASSERT_EQUALS(2, a.size());
        TS_ASSERT_EQUALS(2, a[0]);

        // erase tail: 2
        a.eraseIdx(1);
        TS_ASSERT_EQUALS(1, a.size());
        TS_ASSERT_EQUALS(2, a[0]);

        // invalid position
        a.eraseIdx(1);
        TS_ASSERT_EQUALS(1, a.size());
        TS_ASSERT_EQUALS(2, a[0]);

        // erase to empty
        a.eraseIdx(0);
        TS_ASSERT_EQUALS(0, a.size());

        // already empty
        a.eraseIdx(0);
        TS_ASSERT_EQUALS(0, a.size());
    }

    void testResize() {
        using namespace GN;
        StackArray<int, 4> a;

        a.resize(2);
        TS_ASSERT_EQUALS(2, a.size());

        a.resize(4);
        TS_ASSERT_EQUALS(4, a.size());

        a.resize(5);
        TS_ASSERT_EQUALS(4, a.size());

        a.resize(2);
        TS_ASSERT_EQUALS(2, a.size());
    }
};

class DynaArrayTest : public CxxTest::TestSuite {
    struct Element {
        inline static int count = 0; ///< number of Element instances
        inline static int ctor  = 0; ///< number of calls to ctor
        inline static int dtor  = 0; ///< number of calls to dtor
        inline static int cctor = 0; ///< number of calls to copy constructor
        inline static int cop   = 0; ///< number of calls to copy operator
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
    void testDefautlCtor() {
        using namespace GN;
        DynaArray<Element> a;

        TS_ASSERT(a.empty());
        TS_ASSERT_EQUALS((Element *) NULL, a.data());
        TS_ASSERT_EQUALS(0, a.size());
    }

    void testCtorWithCount() {
        using namespace GN;

        {
            DynaArray<Element> a(1);
            TS_ASSERT_EQUALS(1, a.size());
            TS_ASSERT_EQUALS(1, Element::count);
        }

        TS_ASSERT_EQUALS(0, Element::count);
    }

    void testCtorWithPtrAndCount() {
        using namespace GN;

        Element::clear();

        Element e[2];
        TS_ASSERT_EQUALS(2, Element::ctor);
        TS_ASSERT_EQUALS(0, Element::cctor);

        {
            DynaArray<Element> a(e, 2);
            TS_ASSERT_EQUALS(2, Element::cctor);
            TS_ASSERT_EQUALS(4, Element::count);
            TS_ASSERT_EQUALS(0, Element::dtor);
        }

        TS_ASSERT_EQUALS(2, Element::count);
        TS_ASSERT_EQUALS(2, Element::dtor);
    }

    void testCopyCtor() {
        using namespace GN;

        Element::clear();

        DynaArray<Element> a(2);

        TS_ASSERT_EQUALS(2, Element::count);
        TS_ASSERT_EQUALS(2, Element::ctor);
        TS_ASSERT_EQUALS(0, Element::cctor);
        TS_ASSERT_EQUALS(0, Element::dtor);
        TS_ASSERT_EQUALS(0, Element::cop);

        // 2 cctor for creating new array in b.
        // 2 dtor for deleteing old array in b.
        // 2 copy operators for copy c[0,1] to b[0,1]
        // 1 cctor for copy c[2] to b[2]
        DynaArray<Element> b(a);
        TS_ASSERT_EQUALS(4, Element::count);
        TS_ASSERT_EQUALS(2, Element::ctor);
        TS_ASSERT_EQUALS(2, Element::cctor);
        TS_ASSERT_EQUALS(0, Element::dtor);
        TS_ASSERT_EQUALS(0, Element::cop);
    }

    void testCopyOperator() {
        using namespace GN;

        Element::clear();

        DynaArray<Element> a(1);
        DynaArray<Element> b(2);
        DynaArray<Element> c(3);
        TS_ASSERT_EQUALS(6, Element::count);
        TS_ASSERT_EQUALS(6, Element::ctor);
        TS_ASSERT_EQUALS(0, Element::cctor);
        TS_ASSERT_EQUALS(0, Element::dtor);
        TS_ASSERT_EQUALS(0, Element::cop);

        // 2 cctor for creating new array in b.
        // 2 dtor for deleteing old array in b.
        // 2 copy operators for copy c[0,1] to b[0,1]
        // 1 cctor for copy c[2] to b[2]
        b = c;
        TS_ASSERT_EQUALS(7, Element::count);
        TS_ASSERT_EQUALS(6, Element::ctor);
        TS_ASSERT_EQUALS(1, Element::cctor);
        TS_ASSERT_EQUALS(2, Element::dtor);
        TS_ASSERT_EQUALS(2, Element::cop);

        // 2 dtor for b[1] and b[2]
        // 1 copy operator for copy a[0] to b[0]
        b = a;
        TS_ASSERT_EQUALS(5, Element::count);
        TS_ASSERT_EQUALS(6, Element::ctor);
        TS_ASSERT_EQUALS(1, Element::cctor);
        TS_ASSERT_EQUALS(4, Element::dtor);
        TS_ASSERT_EQUALS(3, Element::cop);
    }

    void testAppend() {
        using namespace GN;
        DynaArray<int> a;

        a.append(1);
        TS_ASSERT_EQUALS(1, a.size());
        TS_ASSERT_EQUALS(1, a.front());
        TS_ASSERT_EQUALS(1, a.back());

        a.append(3);
        TS_ASSERT_EQUALS(2, a.size());
        TS_ASSERT_EQUALS(1, a.front());
        TS_ASSERT_EQUALS(3, a.back());

        DynaArray<int> b;
        b.append(a);
        TS_ASSERT_EQUALS(2, b.size());
        TS_ASSERT_EQUALS(1, b.front());
        TS_ASSERT_EQUALS(3, b.back());
    }

    void testAppend2() {
        using namespace GN;
        DynaArray<Element> a;
        Element            b;
        Element            c[2];

        TS_ASSERT_EQUALS(3, Element::count);

        a.append(b);
        TS_ASSERT_EQUALS(4, Element::count);

        a.append(c, 2);
        TS_ASSERT_EQUALS(6, Element::count);
    }

    void testBeginEnd() {
        using namespace GN;
        DynaArray<int> a;

        a.append(1);
        a.append(2);

        int * b = a.begin();
        int * e = a.end();

        TS_ASSERT_EQUALS(1, *b);
        TS_ASSERT_EQUALS(2, e - b);
    }

    void testInsert() {
        using namespace GN;
        DynaArray<int> a;

        a.insert(0, 3);   // 3
        a.insert(0, 1);   // 1, 3
        a.insert(1, 2);   // 1, 2, 3
        a.insert(100, 4); // should do nothing
        a.insert(3, 4);   // 1, 2, 3, 4
        a.insert(4, 5);   // 1, 2, 3, 4, 5

        TS_ASSERT_EQUALS(5, a.size());
        TS_ASSERT_EQUALS(1, a[0]);
        TS_ASSERT_EQUALS(2, a[1]);
        TS_ASSERT_EQUALS(3, a[2]);
        TS_ASSERT_EQUALS(4, a[3]);
        TS_ASSERT_EQUALS(5, a[4]);
    }

    void testErase() {
        using namespace GN;
        DynaArray<int> a;

        a.append(1);
        a.append(2);
        a.append(3);
        a.append(4);
        a.append(5);

        a.eraseIdx(100); // should do nothing
        TS_ASSERT_EQUALS(5, a.size());
        TS_ASSERT_EQUALS(5, a[4]);

        a.eraseIdx(4); // 1, 2, 3, 4
        TS_ASSERT_EQUALS(4, a.size());
        TS_ASSERT_EQUALS(4, a[3]);

        a.eraseIdx(3); // 1, 2, 3
        TS_ASSERT_EQUALS(3, a.size());
        TS_ASSERT_EQUALS(3, a[2]);

        a.eraseIdx(1); // 1, 3
        TS_ASSERT_EQUALS(2, a.size());
        TS_ASSERT_EQUALS(1, a[0]);
        TS_ASSERT_EQUALS(3, a[1]);

        a.eraseIdx(0); // 3
        TS_ASSERT_EQUALS(1, a.size());
        TS_ASSERT_EQUALS(3, a[0]);

        a.eraseIdx(0);
        TS_ASSERT_EQUALS(0, a.size());

        a.eraseIdx(0); // this should do nothing
    }

    void testPopBack() {
        using namespace GN;
        DynaArray<int> a;
        a.append(1);
        a.append(2);
        a.popBack();
        TS_ASSERT_EQUALS(1, a.size());
        TS_ASSERT_EQUALS(1, a.back());

        a.popBack();
        TS_ASSERT(a.empty());

        a.popBack(); // should do nothing
    }

    void testEraseAndPopback2() {
        // make sure erased item is destructed

        using namespace GN;

        Element::clear();

        DynaArray<Element> a(3);
        TS_ASSERT_EQUALS(3, Element::count);
        TS_ASSERT_EQUALS(3, Element::ctor);
        TS_ASSERT_EQUALS(0, Element::cctor);
        TS_ASSERT_EQUALS(0, Element::dtor);
        TS_ASSERT_EQUALS(0, Element::cop);

        // 2 cop for moving a[1,2] to a[0,1]
        // 1 dtor for a[2]
        a.eraseIdx(0);
        TS_ASSERT_EQUALS(2, Element::count);
        TS_ASSERT_EQUALS(3, Element::ctor);
        TS_ASSERT_EQUALS(0, Element::cctor);
        TS_ASSERT_EQUALS(1, Element::dtor);
        TS_ASSERT_EQUALS(2, Element::cop);

        // 1 dtor for a[1]
        a.popBack();
        TS_ASSERT_EQUALS(1, Element::count);
        TS_ASSERT_EQUALS(3, Element::ctor);
        TS_ASSERT_EQUALS(0, Element::cctor);
        TS_ASSERT_EQUALS(2, Element::dtor);
        TS_ASSERT_EQUALS(2, Element::cop);
    }

    void testResize() {
        // make sure erased item is destructed
        using namespace GN;
        Element::clear();

        DynaArray<Element> a(2);
        TS_ASSERT_EQUALS(2, Element::count);

        a.resize(3);
        TS_ASSERT_EQUALS(3, Element::count);

        a.resize(4);
        TS_ASSERT_EQUALS(4, Element::count);

        a.resize(1);
        TS_ASSERT_EQUALS(1, Element::count);
    }

    void testMove() {
        using namespace GN;
        Element::clear();

        DynaArray<Element> a;
        a.append(2);
        TS_ASSERT_EQUALS(1, Element::count);
        TS_ASSERT_EQUALS(1, Element::mctor);
        TS_ASSERT_EQUALS(0, Element::mop);

        DynaArray<Element> b = std::move(a);
        TS_ASSERT_EQUALS(0, a.size());
        TS_ASSERT_EQUALS(1, b.size());
        TS_ASSERT_EQUALS(1, Element::count);
        TS_ASSERT_EQUALS(1, Element::mctor);
        TS_ASSERT_EQUALS(0, Element::mop);
    }

    void testCopy2() {
        using namespace GN;
        Element::clear();

        struct Stuff : public RefCounter {
            int * _i;
            Stuff(int i) {
                _i  = new int;
                *_i = i;
            }
            ~Stuff() { delete _i; }
        };

        DynaArray<AutoRef<Stuff>> a;
        a.append(AutoRef<Stuff> {new Stuff(0)});
        a.append(AutoRef<Stuff> {new Stuff(1)});

        DynaArray<AutoRef<Stuff>> b;
        a = b;
        a.clear();
        b.clear();
    }

    void testReserve() {
        using namespace GN;
        Element::clear();

        {

            DynaArray<Element> a;

            a.resize(2);
            TS_ASSERT_EQUALS(2, a.size());
            TS_ASSERT_EQUALS(2, Element::count);
            TS_ASSERT_EQUALS(2, Element::ctor);

            // reserve should not modify array size
            a.reserve(1000);
            TS_ASSERT_EQUALS(2, a.size());
            TS_ASSERT_EQUALS(2, Element::count);
            TS_ASSERT_EQUALS(2, Element::ctor);
            TS_ASSERT_EQUALS(2, Element::mctor);
            TS_ASSERT_EQUALS(2, Element::dtor);
        }

        TS_ASSERT_EQUALS(0, Element::count);
        TS_ASSERT_EQUALS(4, Element::dtor);
    }

    // Some basic smoke test of Blob class

    void testBlobBasic() {
        using namespace GN;

        // Test default constructor
        Blob<int> a;
        TS_ASSERT(a.empty());
        TS_ASSERT_EQUALS(0, a.size());
        TS_ASSERT_EQUALS(nullptr, a.data());

        // Test constructor with count
        Blob<int> b(5);
        TS_ASSERT(!b.empty());
        TS_ASSERT_EQUALS(5, b.size());
        TS_ASSERT(a.data() != b.data());

        // Test constructor with count and data
        int       data[] = {1, 2, 3, 4, 5};
        Blob<int> c(5, data);
        TS_ASSERT_EQUALS(5, c.size());
        TS_ASSERT_EQUALS(1, c[0]);
        TS_ASSERT_EQUALS(5, c[4]);

        // Test constructor with count and value
        Blob<int> d(3, 42);
        TS_ASSERT_EQUALS(3, d.size());
        TS_ASSERT_EQUALS(42, d[0]);
        TS_ASSERT_EQUALS(42, d[1]);
        TS_ASSERT_EQUALS(42, d[2]);
    }

    void testBlobMove() {
        using namespace GN;

        // Test move constructor
        Blob<int> a(3, 10);
        TS_ASSERT_EQUALS(3, a.size());
        TS_ASSERT_EQUALS(10, a[0]);

        Blob<int> b = std::move(a);
        TS_ASSERT_EQUALS(3, b.size());
        TS_ASSERT_EQUALS(10, b[0]);
        TS_ASSERT(a.empty());
        TS_ASSERT_EQUALS(0, a.size());

        // Test move assignment
        Blob<int> c(2, 20);
        c = std::move(b);
        TS_ASSERT_EQUALS(3, c.size());
        TS_ASSERT_EQUALS(10, c[0]);
        TS_ASSERT(b.empty());
    }

    void testBlobAccess() {
        using namespace GN;

        Blob<int> a(4);
        a[0] = 1;
        a[1] = 2;
        a[2] = 3;
        a[3] = 4;

        // Test operator[]
        TS_ASSERT_EQUALS(1, a[0]);
        TS_ASSERT_EQUALS(2, a[1]);
        TS_ASSERT_EQUALS(3, a[2]);
        TS_ASSERT_EQUALS(4, a[3]);

        // Test at()
        TS_ASSERT_EQUALS(1, a.at(0));
        TS_ASSERT_EQUALS(4, a.at(3));

        // Test front/back
        TS_ASSERT_EQUALS(1, a.front());
        TS_ASSERT_EQUALS(4, a.back());

        // Test data access
        int * data = a.data();
        TS_ASSERT_EQUALS(1, data[0]);
        TS_ASSERT_EQUALS(4, data[3]);
    }

    void testBlobIterators() {
        using namespace GN;

        Blob<int> a(3);
        a[0] = 10;
        a[1] = 20;
        a[2] = 30;

        // Test begin/end
        int sum = 0;
        for (auto it = a.begin(); it != a.end(); ++it) { sum += *it; }
        TS_ASSERT_EQUALS(60, sum);

        // Test const iterators
        const Blob<int> & constA = a;
        sum                      = 0;
        for (auto it = constA.cbegin(); it != constA.cend(); ++it) { sum += *it; }
        TS_ASSERT_EQUALS(60, sum);

        // Test range-based for
        sum = 0;
        for (int val : a) { sum += val; }
        TS_ASSERT_EQUALS(60, sum);
    }

    void testBlobAttach() {
        using namespace GN;

        int * rawData = (int *) HeapMemory::alignedAlloc(3 * sizeof(int), 16);
        rawData[0]    = 100;
        rawData[1]    = 200;
        rawData[2]    = 300;

        Blob<int> a;
        a.attachTo(3, rawData);

        TS_ASSERT_EQUALS(3, a.size());
        TS_ASSERT_EQUALS(100, a[0]);
        TS_ASSERT_EQUALS(200, a[1]);
        TS_ASSERT_EQUALS(300, a[2]);

        // Note: attachTo takes ownership, so we don't delete rawData
        // The Blob destructor will handle cleanup
    }

    void testBlobMoveTo() {
        using namespace GN;

        Blob<int> a(4);
        a[0] = 1;
        a[1] = 2;
        a[2] = 3;
        a[3] = 4;

        // Move to char array (different type)
        Blob<char> b = a.moveTo<char>();

        // Original blob should be empty
        TS_ASSERT(a.empty());
        TS_ASSERT_EQUALS(0, a.size());

        // New blob should have the data
        TS_ASSERT_EQUALS(16, b.size()); // 4 ints * 4 bytes = 16 chars
    }

    void testBlobClear() {
        using namespace GN;

        Blob<int> a(3, 42);
        TS_ASSERT_EQUALS(3, a.size());
        TS_ASSERT_EQUALS(42, a[0]);

        a.clear();
        TS_ASSERT(a.empty());
        TS_ASSERT_EQUALS(0, a.size());
        TS_ASSERT_EQUALS(nullptr, a.data());
    }
};
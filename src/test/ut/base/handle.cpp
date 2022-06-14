#include "../testCommon.h"

class HandleTest : public CxxTest::TestSuite {
    struct TestItem {
        int * ptr;

        TestItem(): ptr(new int[100]) {}

        ~TestItem() { delete[] ptr; }
    };

public:
    void testCtor() {
        using namespace GN;

        HandleManager<TestItem> hm;

        size_t h = hm.newHandle();
        TS_ASSERT(h);

        hm.remove(h);

        TS_ASSERT(hm.empty());
    }

    void test1() {
        GN::HandleManager<int> hm;

        TS_ASSERT(hm.empty());
        TS_ASSERT_EQUALS(hm.size(), 0);
        TS_ASSERT_EQUALS(hm.first(), 0);
        TS_ASSERT_EQUALS(hm.next(0), 0);

        size_t h1 = hm.add(1);
        size_t h2 = hm.add(2);
        size_t h3 = hm.add(3);
        size_t h4 = hm.add(4);
        size_t h5 = hm.add(5);

        TS_ASSERT(!hm.empty());
        TS_ASSERT_EQUALS(hm.size(), 5);

        TS_ASSERT_EQUALS(hm.first(), h1);
        TS_ASSERT_EQUALS(hm.next(h1), h2);
        TS_ASSERT_EQUALS(hm.next(h2), h3);
        TS_ASSERT_EQUALS(hm.next(h3), h4);
        TS_ASSERT_EQUALS(hm.next(h4), h5);
        TS_ASSERT_EQUALS(hm.next(h5), 0);
        TS_ASSERT_EQUALS(hm.next(0), 0);
        TS_ASSERT_EQUALS(hm.next(100), 0);

        TS_ASSERT_EQUALS(hm[h1], 1);
        TS_ASSERT_EQUALS(hm[h2], 2);
        TS_ASSERT_EQUALS(hm[h3], 3);

        hm.remove(0);
        hm.remove(h1);
        TS_ASSERT_EQUALS(hm.size(), 4);
        TS_ASSERT_EQUALS(hm.first(), h2);
        TS_ASSERT_EQUALS(hm.next(h2), h3);

        hm.remove(h3);
        TS_ASSERT_EQUALS(hm.size(), 3);
        TS_ASSERT_EQUALS(hm.next(h2), h4);
        TS_ASSERT_EQUALS(hm.next(h4), h5);

        hm.remove(h5);
        TS_ASSERT_EQUALS(hm.size(), 2);
        TS_ASSERT_EQUALS(hm.next(h4), 0);

        // find
        TS_ASSERT_EQUALS(hm.find(1), 0);
        TS_ASSERT_EQUALS(hm.find(2), h2);

        // re-add
        h1 = hm.add(1);
        TS_ASSERT(h1);

        hm.clear();
        TS_ASSERT(hm.empty());
        TS_ASSERT_EQUALS(hm.size(), 0);
        TS_ASSERT_EQUALS(hm.first(), 0);
        TS_ASSERT_EQUALS(hm.next(0), 0);
    }

    void testCapacity() {
        GN::HandleManager<int> hm;
        hm.reserve(1);
        TS_ASSERT_LESS_EQUALS(1, hm.capacity());
        hm.reserve(1000);
        TS_ASSERT_LESS_EQUALS(1000, hm.capacity());
    }
};

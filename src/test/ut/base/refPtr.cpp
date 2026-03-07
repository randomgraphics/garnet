#include "../testCommon.h"
#include <atomic>
#include <thread>
#include <vector>

class RefPtrTest : public CxxTest::TestSuite {
    struct MyRefObj : public GN::RefCounter {};
    typedef GN::AutoRef<MyRefObj> MyRefObjPtr;

    struct MyRefObj2 : public MyRefObj {};
    typedef GN::AutoRef<MyRefObj2> MyRefObj2Ptr;

    struct MyStruct {
        MyRefObjPtr p;
    };

public:
    void testCopyCtor() {
        MyRefObj2Ptr p2 = GN::referenceTo(new MyRefObj2);
        TS_ASSERT_EQUALS(p2->getref(), 1);

        MyRefObjPtr p1(p2);
        TS_ASSERT_EQUALS(p1->getref(), 2);

        MyRefObjPtr p3(p1);
        TS_ASSERT_EQUALS(p3->getref(), 3);
    }

    void testAssignment() {
        MyRefObjPtr p2 = GN::referenceTo(new MyRefObj2);
        TS_ASSERT_EQUALS(p2->getref(), 1);

        MyRefObjPtr p1a, p1b;

        p1a = p2;
        TS_ASSERT_EQUALS(p1a->getref(), 2);

        p1b = p1a;
        TS_ASSERT_EQUALS(p1b->getref(), 3);
    }

    void testWeakRef1() {
        struct Ref2 : public GN::RefCounter {};

        GN::AutoRef<Ref2> p(new Ref2());
        TS_ASSERT(1 == p->getref());

        GN::WeakRef<Ref2> w1(p);
        GN::WeakRef<Ref2> w2(w1);
        GN::WeakRef<Ref2> w3;

        // weak reference should not affect strong reference counter
        TS_ASSERT(1 == p->getref());

        // test assignment
        w3 = w1;
        TS_ASSERT(w1);
        TS_ASSERT(w2);
        TS_ASSERT(w3);
        TS_ASSERT(w1 == w2);
        TS_ASSERT(w1 == w3);
        TS_ASSERT(w2 == w3);

        p = NULL;

        TS_ASSERT(!w1);
        TS_ASSERT(!w2);
        TS_ASSERT(!w3);
    }

    void testMultithreadAutoRefWeakRef() {
        struct Ref2 : public GN::RefCounter {};

        const int         numThreads = 8;
        const int         loopsPerTh = 500;
        std::atomic<int>  errs {0};
        std::atomic<int>  exitedCount {0};
        std::atomic<bool> releaseStrong {false};
        std::atomic<bool> cleared {false};

        GN::AutoRef<Ref2> strong(new Ref2());
        Ref2 *            raw = strong.get();
        TS_ASSERT(raw && strong->getref() == 1);

        std::vector<std::thread> threads;

        for (int t = 0; t < numThreads; ++t) {
            threads.emplace_back([raw, &releaseStrong, &cleared, &exitedCount, &errs, loopsPerTh, numThreads]() {
                GN::WeakRef<Ref2> w(raw);
                for (int i = 0; i < loopsPerTh; ++i) {
                    if (releaseStrong.load(std::memory_order_acquire)) break;
                    auto p = w.promote();
                    if (!p && !w.empty()) errs.fetch_add(1, std::memory_order_relaxed);
                    GN::WeakRef<Ref2> w2(w);
                    auto              p2 = w2.promote();
                    if (!p2 && !w2.empty()) errs.fetch_add(1, std::memory_order_relaxed);
                }
                exitedCount.fetch_add(1, std::memory_order_release);
                while (!cleared.load(std::memory_order_acquire)) {}
                if (!w.empty()) errs.fetch_add(1, std::memory_order_relaxed);
                auto dead = w.promote();
                if (dead) errs.fetch_add(1, std::memory_order_relaxed);
            });
        }

        releaseStrong.store(true, std::memory_order_release);
        while (exitedCount.load(std::memory_order_acquire) != numThreads) {}
        strong.clear();
        cleared.store(true, std::memory_order_release);

        for (auto & th : threads) th.join();

        TS_ASSERT_EQUALS(0, errs.load());
    }

    void testMultithreadAutoRefCopyRelease() {
        struct Ref2 : public GN::RefCounter {};

        const int        numThreads = 8;
        const int        copies     = 200;
        std::atomic<int> errs {0};

        GN::AutoRef<Ref2> shared(new Ref2());
        Ref2 *            raw = shared.get();

        std::vector<std::thread> threads;
        for (int t = 0; t < numThreads; ++t) {
            threads.emplace_back([raw, &errs, copies]() {
                for (int i = 0; i < copies; ++i) {
                    GN::AutoRef<Ref2> p(raw);
                    if (!p || p->getref() < 1) errs.fetch_add(1, std::memory_order_relaxed);
                    GN::WeakRef<Ref2> w(p);
                    auto              p2 = w.promote();
                    if (!p2) errs.fetch_add(1, std::memory_order_relaxed);
                }
            });
        }

        for (auto & th : threads) th.join();
        shared.clear();

        TS_ASSERT_EQUALS(0, errs.load());
    }
};

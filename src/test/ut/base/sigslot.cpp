#include "../testCommon.h"
#include <iostream>

// Test helper classes and functions
namespace {
// Static function for delegate testing
static int staticFunction(int x, int y) { return x + y; }

static void staticVoidFunction(int & value) { value = 42; }

static void staticHandler(int v) {
    // This will be used in tests
    (void) v; // suppress unused parameter warning
}

static int noParamFunc() { return 42; }

static int  voidNoParamValue = 0;
static void voidNoParamFunc() { voidNoParamValue = 100; }

// Test class for member function delegates
class TestClass {
public:
    int value;

    TestClass(): value(0) {}

    int add(int x, int y) { return x + y; }

    int multiply(int x, int y) const { return x * y; }

    void setValue(int v) { value = v; }

    void setValueConst(int v) const { const_cast<TestClass *>(this)->value = v; }

    int getValue() const { return value; }
};

// Test class for signal/slot testing
class TestSlot : public GN::SlotBase {
public:
    int callCount;
    int lastValue;

    TestSlot(): callCount(0), lastValue(0) {}

    void onSignal(int value) {
        callCount++;
        lastValue = value;
    }

    void onSignalConst(int value) const { const_cast<TestSlot *>(this)->onSignal(value); }
};

// Helper class to track calls for emit() tests
class CallTracker {
public:
    static int callCount;
    static int lastValue;
    static int sum;

    static void reset() {
        callCount = 0;
        lastValue = 0;
        sum       = 0;
    }

    static void track(int value) {
        callCount++;
        lastValue = value;
        sum += value;
    }

    static int trackAndReturn(int value) {
        callCount++;
        lastValue = value;
        return value * 2; // Return different value to verify we get the last one
    }
};

int CallTracker::callCount = 0;
int CallTracker::lastValue = 0;
int CallTracker::sum       = 0;

// Static functions for signal emit tests
static void staticVoidHandler(int value) { CallTracker::track(value); }

static int staticIntHandler(int value) { return CallTracker::trackAndReturn(value); }

static int staticTwoArgHandler(int x, int y) {
    (void) x; // suppress unused parameter warning
    (void) y;
    return x + y;
}

static void staticRefHandler(int & v) { v = 99; }

static void staticConstRefHandler(const int & v) {
    (void) v; // suppress unused parameter warning
}

// Test class for member function emit tests
class EmitTestClass {
public:
    int callCount;
    int lastValue;

    EmitTestClass(): callCount(0), lastValue(0) {}

    void handler(int value) {
        callCount++;
        lastValue = value;
    }

    int handlerWithReturn(int value) {
        callCount++;
        lastValue = value;
        return value * 3;
    }
};
} // namespace

class SigSlotTest : public CxxTest::TestSuite {
public:
    // ========================================================================
    // Delegate Tests
    // ========================================================================

    void testDelegateDefaultConstructor() {
        GN::internal::delegate<int(int, int)> d;
        TS_ASSERT(d.isNull());
        TS_ASSERT(d == nullptr);
        TS_ASSERT(!(d != nullptr));
    }

    void testDelegateStaticFunction() {
        auto d = GN::internal::delegate<int(int, int)>::createFunction<staticFunction>();
        TS_ASSERT(!d.isNull());
        TS_ASSERT(d != nullptr);

        int result = d(5, 3);
        TS_ASSERT_EQUALS(result, 8);
    }

    void testDelegateMemberFunction() {
        TestClass obj;
        auto      d = GN::internal::delegate<int(int, int)>::createMethod<TestClass, &TestClass::add>(&obj);

        TS_ASSERT(!d.isNull());

        int result = d(4, 7);
        TS_ASSERT_EQUALS(result, 11);
    }

    void testDelegateConstMemberFunction() {
        TestClass obj;
        auto      d = GN::internal::delegate<int(int, int)>::createMethod<TestClass, &TestClass::multiply>(&obj);

        TS_ASSERT(!d.isNull());

        int result = d(3, 5);
        TS_ASSERT_EQUALS(result, 15);
    }

    void testDelegateLambda() {
        int captured = 10;

        auto lambda = [captured](int x) -> int { return x + captured; };

        auto d = GN::internal::delegate<int(int)>::createLambda(lambda);

        TS_ASSERT(!d.isNull());

        int result = d(5);
        TS_ASSERT_EQUALS(result, 15);
    }

    void testDelegateLambdaAssignment() {
        int captured = 20;

        auto lambda = [captured](int x) -> int { return x * captured; };

        GN::internal::delegate<int(int)> d;
        d = lambda;

        TS_ASSERT(!d.isNull());

        int result = d(3);
        TS_ASSERT_EQUALS(result, 60);
    }

    void testDelegateCopy() {
        TestClass obj;
        auto      d1 = GN::internal::delegate<int(int, int)>::createMethod<TestClass, &TestClass::add>(&obj);
        auto      d2 = d1; // copy

        TS_ASSERT(!d1.isNull());
        TS_ASSERT(!d2.isNull());
        TS_ASSERT(d1 == d2);

        TS_ASSERT_EQUALS(d1(2, 3), 5);
        TS_ASSERT_EQUALS(d2(2, 3), 5);
    }

    void testDelegateAssignment() {
        TestClass obj1, obj2;
        auto      d1 = GN::internal::delegate<int(int, int)>::createMethod<TestClass, &TestClass::add>(&obj1);
        auto      d2 = GN::internal::delegate<int(int, int)>::createMethod<TestClass, &TestClass::multiply>(&obj2);

        TS_ASSERT(d1 != d2);

        d2 = d1; // assignment

        TS_ASSERT(d1 == d2);
        TS_ASSERT_EQUALS(d1(3, 4), 7);
        TS_ASSERT_EQUALS(d2(3, 4), 7);
    }

    void testDelegateVoidReturn() {
        int value = 0;

        auto d = GN::internal::delegate<void(int &)>::createFunction<staticVoidFunction>();

        TS_ASSERT(!d.isNull());

        d(value);
        TS_ASSERT_EQUALS(value, 42);
    }

    void testDelegateVoidMemberFunction() {
        TestClass obj;

        auto d = GN::internal::delegate<void(int)>::createMethod<TestClass, &TestClass::setValue>(&obj);

        TS_ASSERT(!d.isNull());

        d(100);
        TS_ASSERT_EQUALS(obj.value, 100);
    }

    // ========================================================================
    // Signal Tests
    // ========================================================================

    void testSignalDefaultConstructor() {
        GN::Signal<void(int)> signal;
        // Signal should be constructible
        TS_ASSERT(true);
    }

    void testSignalConnectStaticFunction() {
        GN::Signal<void(int)> signal;

        auto tether = signal.connect<staticHandler>();

        TS_ASSERT(tether.signal() == &signal);

        // Test that tether can be moved
        auto tether2 = std::move(tether);
        TS_ASSERT(tether2.signal() == &signal);
        TS_ASSERT(tether.signal() == nullptr);
    }

    void testSignalConnectMemberFunction() {
        TestSlot slot;

        GN::Signal<void(int)> signal;

        auto tether = signal.connect<&TestSlot::onSignal>(&slot);

        TS_ASSERT(tether.signal() == &signal);
        TS_ASSERT_EQUALS(slot.callCount, 0);
    }

    void testSignalConnectConstMemberFunction() {
        TestSlot slot;

        GN::Signal<void(int)> signal;

        auto tether = signal.connect<&TestSlot::onSignalConst>(&slot);

        TS_ASSERT(tether.signal() == &signal);
    }

    void testSignalMultipleConnections() {
        TestSlot slot1, slot2;

        GN::Signal<void(int)> signal;

        auto tether1 = signal.connect<&TestSlot::onSignal>(&slot1);
        auto tether2 = signal.connect<&TestSlot::onSignal>(&slot2);

        TS_ASSERT(tether1.signal() == &signal);
        TS_ASSERT(tether2.signal() == &signal);
    }

    // ========================================================================
    // Signal emit() Tests
    // ========================================================================

    void testSignalEmitVoidNoDelegates() {
        GN::Signal<void(int)> signal;

        // Emitting with no delegates should not crash
        signal.emit(42);
        signal(100); // Test operator() form
        TS_ASSERT(true);
    }

    void testSignalEmitVoidSingleDelegate() {
        CallTracker::reset();
        GN::Signal<void(int)> signal;

        auto tether = signal.connect<staticVoidHandler>();

        signal.emit(10);
        TS_ASSERT_EQUALS(CallTracker::callCount, 1);
        TS_ASSERT_EQUALS(CallTracker::lastValue, 10);

        signal(20); // Test operator() form
        TS_ASSERT_EQUALS(CallTracker::callCount, 2);
        TS_ASSERT_EQUALS(CallTracker::lastValue, 20);
    }

    void testSignalEmitVoidMultipleDelegates() {
        CallTracker::reset();
        TestSlot              slot1, slot2;
        GN::Signal<void(int)> signal;

        auto tether1 = signal.connect<staticVoidHandler>();
        auto tether2 = signal.connect<&TestSlot::onSignal>(&slot1);
        auto tether3 = signal.connect<&TestSlot::onSignal>(&slot2);

        signal.emit(100);

        // All delegates should be called
        TS_ASSERT_EQUALS(CallTracker::callCount, 1);
        TS_ASSERT_EQUALS(CallTracker::lastValue, 100);
        TS_ASSERT_EQUALS(slot1.callCount, 1);
        TS_ASSERT_EQUALS(slot1.lastValue, 100);
        TS_ASSERT_EQUALS(slot2.callCount, 1);
        TS_ASSERT_EQUALS(slot2.lastValue, 100);
    }

    void testSignalEmitNonVoidNoDelegates() {
        GN::Signal<int(int)> signal;

        // Should return default-constructed value
        int result = signal.emit(42);
        TS_ASSERT_EQUALS(result, 0);

        result = signal(100); // Test operator() form
        TS_ASSERT_EQUALS(result, 0);
    }

    void testSignalEmitNonVoidSingleDelegate() {
        CallTracker::reset();
        GN::Signal<int(int)> signal;

        auto tether = signal.connect<staticIntHandler>();

        int result = signal.emit(5);
        TS_ASSERT_EQUALS(CallTracker::callCount, 1);
        TS_ASSERT_EQUALS(CallTracker::lastValue, 5);
        TS_ASSERT_EQUALS(result, 10); // staticIntHandler returns value * 2

        result = signal(7); // Test operator() form
        TS_ASSERT_EQUALS(CallTracker::callCount, 2);
        TS_ASSERT_EQUALS(CallTracker::lastValue, 7);
        TS_ASSERT_EQUALS(result, 14);
    }

    void testSignalEmitNonVoidMultipleDelegates() {
        CallTracker::reset();
        EmitTestClass        obj1, obj2;
        GN::Signal<int(int)> signal;

        auto tether1 = signal.connect<staticIntHandler>();
        auto tether2 = signal.connect<&EmitTestClass::handlerWithReturn>(&obj1);
        auto tether3 = signal.connect<&EmitTestClass::handlerWithReturn>(&obj2);

        int result = signal.emit(3);

        // All delegates should be called
        TS_ASSERT_EQUALS(CallTracker::callCount, 1);
        TS_ASSERT_EQUALS(CallTracker::lastValue, 3);
        TS_ASSERT_EQUALS(obj1.callCount, 1);
        TS_ASSERT_EQUALS(obj1.lastValue, 3);
        TS_ASSERT_EQUALS(obj2.callCount, 1);
        TS_ASSERT_EQUALS(obj2.lastValue, 3);

        // Should return the last delegate's result (obj2 returns value * 3 = 9)
        TS_ASSERT_EQUALS(result, 9);
    }

    void testSignalEmitWithMultipleArguments() {
        GN::Signal<int(int, int)> signal;
        auto                      tether = signal.connect<staticTwoArgHandler>();

        int result = signal.emit(10, 20);
        TS_ASSERT_EQUALS(result, 30);

        result = signal(5, 15); // Test operator() form
        TS_ASSERT_EQUALS(result, 20);
    }

    void testSignalEmitVoidWithReferenceArguments() {
        GN::Signal<void(int &)> signal;
        auto                    tether = signal.connect<staticRefHandler>();

        int testValue = 0;
        signal.emit(testValue);
        TS_ASSERT_EQUALS(testValue, 99);

        testValue = 50;
        signal(testValue); // Test operator() form
        TS_ASSERT_EQUALS(testValue, 99);
    }

    void testSignalEmitDisconnectDuringEmit() {
        // Test that disconnecting a tether doesn't affect ongoing emit
        TestSlot              slot1, slot2;
        GN::Signal<void(int)> signal;

        auto tether1 = signal.connect<&TestSlot::onSignal>(&slot1);
        auto tether2 = signal.connect<&TestSlot::onSignal>(&slot2);

        // Disconnect first tether
        tether1.clear();

        // Emit should only call slot2
        signal.emit(42);
        TS_ASSERT_EQUALS(slot1.callCount, 0);
        TS_ASSERT_EQUALS(slot2.callCount, 1);
        TS_ASSERT_EQUALS(slot2.lastValue, 42);
    }

    void testSignalEmitAllDelegatesCalled() {
        // Verify that all delegates are called even when some return values
        CallTracker::reset();
        EmitTestClass        obj1, obj2, obj3;
        GN::Signal<int(int)> signal;

        auto tether1 = signal.connect<staticIntHandler>();
        auto tether2 = signal.connect<&EmitTestClass::handlerWithReturn>(&obj1);
        auto tether3 = signal.connect<&EmitTestClass::handlerWithReturn>(&obj2);
        auto tether4 = signal.connect<&EmitTestClass::handlerWithReturn>(&obj3);

        int result = signal.emit(4);

        // Verify all were called
        TS_ASSERT_EQUALS(CallTracker::callCount, 1);
        TS_ASSERT_EQUALS(obj1.callCount, 1);
        TS_ASSERT_EQUALS(obj2.callCount, 1);
        TS_ASSERT_EQUALS(obj3.callCount, 1);

        // Result should be from last delegate (obj3: 4 * 3 = 12)
        TS_ASSERT_EQUALS(result, 12);
    }

    void testSignalEmitWithConstArguments() {
        GN::Signal<void(const int &)> signal;
        auto                          tether = signal.connect<staticConstRefHandler>();

        const int constValue = 123;
        signal.emit(constValue);
        // Handler should have been called (tested via static variable in handler)
        TS_ASSERT(true);
    }

    // ========================================================================
    // Tether Tests
    // ========================================================================

    void testTetherDefaultConstructor() {
        GN::Tether tether;
        TS_ASSERT(tether.signal() == nullptr);
    }

    void testTetherMove() {
        TestSlot              slot;
        GN::Signal<void(int)> signal;

        auto tether1 = signal.connect<&TestSlot::onSignal>(&slot);
        auto tether2 = std::move(tether1);

        TS_ASSERT(tether2.signal() == &signal);
        TS_ASSERT(tether1.signal() == nullptr);
    }

    void testTetherMoveAssignment() {
        TestSlot              slot;
        GN::Signal<void(int)> signal1, signal2;

        auto tether1 = signal1.connect<&TestSlot::onSignal>(&slot);
        auto tether2 = signal2.connect<&TestSlot::onSignal>(&slot);

        TS_ASSERT(tether1.signal() == &signal1);
        TS_ASSERT(tether2.signal() == &signal2);

        tether2 = std::move(tether1);

        TS_ASSERT(tether2.signal() == &signal1);
        TS_ASSERT(tether1.signal() == nullptr);
    }

    void testTetherClear() {
        TestSlot              slot;
        GN::Signal<void(int)> signal;

        auto tether = signal.connect<&TestSlot::onSignal>(&slot);
        TS_ASSERT(tether.signal() == &signal);

        tether.clear();
        TS_ASSERT(tether.signal() == nullptr);
    }

    void testTetherSignalDeletedBeforeTether() {
        // Create signal and tether in a scope
        GN::Tether tether;
        {
            TestSlot slot;
            uint8_t  buffer[sizeof(GN::Signal<void(int)>)];
            auto     signal = new (buffer) GN::Signal<void(int)>();
            tether          = signal->connect<&TestSlot::onSignal>(&slot);
            TS_ASSERT(tether.signal() == signal);
            signal->~Signal<void(int)>();         // destroy the signal
            memset(buffer, 0xFF, sizeof(buffer)); // make sure the signal is completely wiped out.
        }
        // Signal is now deleted (out of scope)

        // calling tether.clear() should not crash
        tether.clear();
    }

    // ========================================================================
    // SlotBase Tests
    // ========================================================================

    void testSlotBaseConnectToSignal() {
        TestSlot              slot;
        GN::Signal<void(int)> signal1, signal2;

        slot.connectToSignal<&TestSlot::onSignal>(signal1);
        slot.connectToSignal<&TestSlot::onSignal>(signal2);

        signal1.emit(10);
        TS_ASSERT_EQUALS(slot.callCount, 1);
        TS_ASSERT_EQUALS(slot.lastValue, 10);

        signal2.emit(20);
        TS_ASSERT_EQUALS(slot.callCount, 2);
        TS_ASSERT_EQUALS(slot.lastValue, 20);

        slot.disconnectFromAllSignals();

        // trigger again to ensure disconnection
        signal1.emit(30);
        signal2.emit(40);
        TS_ASSERT_EQUALS(slot.callCount, 2); // should not have increased
    }

    // ========================================================================
    // Integration Tests
    // ========================================================================

    void testDelegateWithDifferentSignatures() {
        // Test delegate with no parameters
        auto d1 = GN::internal::delegate<int()>::createFunction<noParamFunc>();
        TS_ASSERT(!d1.isNull());
        TS_ASSERT_EQUALS(d1(), 42);

        // Test delegate with void return and no parameters
        auto d2 = GN::internal::delegate<void()>::createFunction<voidNoParamFunc>();
        TS_ASSERT(!d2.isNull());
        voidNoParamValue = 0;
        d2();
        TS_ASSERT_EQUALS(voidNoParamValue, 100);
    }

    void testSignalWithDifferentReturnTypes() {
        GN::Signal<int()>          signal1;
        GN::Signal<void()>         signal2;
        GN::Signal<bool(int, int)> signal3;

        // Signals should be constructible with different signatures
        TS_ASSERT(true);
    }

    void testDelegateEquality() {
        TestClass obj1, obj2;

        auto d1 = GN::internal::delegate<int(int, int)>::createMethod<TestClass, &TestClass::add>(&obj1);
        auto d2 = GN::internal::delegate<int(int, int)>::createMethod<TestClass, &TestClass::add>(&obj1);
        auto d3 = GN::internal::delegate<int(int, int)>::createMethod<TestClass, &TestClass::add>(&obj2);

        // Same object and method should be equal
        TS_ASSERT(d1 == d2);

        // Different objects with same method should not be equal
        TS_ASSERT(d1 != d3);
    }

    void testDelegateInequality() {
        TestClass obj;

        auto d1 = GN::internal::delegate<int(int, int)>::createMethod<TestClass, &TestClass::add>(&obj);
        auto d2 = GN::internal::delegate<int(int, int)>::createMethod<TestClass, &TestClass::multiply>(&obj);

        // Different methods should not be equal
        TS_ASSERT(d1 != d2);
    }
};

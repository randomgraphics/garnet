/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/ErrorPrinter.h>

int main() {
 return CxxTest::ErrorPrinter().run();
}
#include "D:/chen/gamedev/myproj/garnet3d/new/core/src/test/base/assert.cpp"

static AssertTest suite_AssertTest;

static CxxTest::List Tests_AssertTest = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_AssertTest( "D:/chen/gamedev/myproj/garnet3d/new/core/src/test/base/assert.cpp", 3, "AssertTest", suite_AssertTest, Tests_AssertTest );

static class TestDescription_AssertTest_testSuccess : public CxxTest::RealTestDescription {
public:
 TestDescription_AssertTest_testSuccess() : CxxTest::RealTestDescription( Tests_AssertTest, suiteDescription_AssertTest, 6, "testSuccess" ) {}
 void runTest() { suite_AssertTest.testSuccess(); }
} testDescription_AssertTest_testSuccess;

static class TestDescription_AssertTest_TestFailure : public CxxTest::RealTestDescription {
public:
 TestDescription_AssertTest_TestFailure() : CxxTest::RealTestDescription( Tests_AssertTest, suiteDescription_AssertTest, 11, "TestFailure" ) {}
 void runTest() { suite_AssertTest.TestFailure(); }
} testDescription_AssertTest_TestFailure;

#include "D:/chen/gamedev/myproj/garnet3d/new/core/src/test/base/functor.cpp"

static FunctorTest suite_FunctorTest;

static CxxTest::List Tests_FunctorTest = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_FunctorTest( "D:/chen/gamedev/myproj/garnet3d/new/core/src/test/base/functor.cpp", 93, "FunctorTest", suite_FunctorTest, Tests_FunctorTest );

static class TestDescription_FunctorTest_testFunctor : public CxxTest::RealTestDescription {
public:
 TestDescription_FunctorTest_testFunctor() : CxxTest::RealTestDescription( Tests_FunctorTest, suiteDescription_FunctorTest, 96, "testFunctor" ) {}
 void runTest() { suite_FunctorTest.testFunctor(); }
} testDescription_FunctorTest_testFunctor;

static class TestDescription_FunctorTest_testSigslot : public CxxTest::RealTestDescription {
public:
 TestDescription_FunctorTest_testSigslot() : CxxTest::RealTestDescription( Tests_FunctorTest, suiteDescription_FunctorTest, 183, "testSigslot" ) {}
 void runTest() { suite_FunctorTest.testSigslot(); }
} testDescription_FunctorTest_testSigslot;

#include "D:/chen/gamedev/myproj/garnet3d/new/core/src/test/base/image.cpp"

static ImageTest suite_ImageTest;

static CxxTest::List Tests_ImageTest = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_ImageTest( "D:/chen/gamedev/myproj/garnet3d/new/core/src/test/base/image.cpp", 7, "ImageTest", suite_ImageTest, Tests_ImageTest );

static class TestDescription_ImageTest_testInvalidPNG : public CxxTest::RealTestDescription {
public:
 TestDescription_ImageTest_testInvalidPNG() : CxxTest::RealTestDescription( Tests_ImageTest, suiteDescription_ImageTest, 10, "testInvalidPNG" ) {}
 void runTest() { suite_ImageTest.testInvalidPNG(); }
} testDescription_ImageTest_testInvalidPNG;

static class TestDescription_ImageTest_testInvalidJPG : public CxxTest::RealTestDescription {
public:
 TestDescription_ImageTest_testInvalidJPG() : CxxTest::RealTestDescription( Tests_ImageTest, suiteDescription_ImageTest, 26, "testInvalidJPG" ) {}
 void runTest() { suite_ImageTest.testInvalidJPG(); }
} testDescription_ImageTest_testInvalidJPG;

#include "D:/chen/gamedev/myproj/garnet3d/new/core/src/test/base/log.cpp"

static LogTest suite_LogTest;

static CxxTest::List Tests_LogTest = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_LogTest( "D:/chen/gamedev/myproj/garnet3d/new/core/src/test/base/log.cpp", 3, "LogTest", suite_LogTest, Tests_LogTest );

static class TestDescription_LogTest_testLogMacro : public CxxTest::RealTestDescription {
public:
 TestDescription_LogTest_testLogMacro() : CxxTest::RealTestDescription( Tests_LogTest, suiteDescription_LogTest, 6, "testLogMacro" ) {}
 void runTest() { suite_LogTest.testLogMacro(); }
} testDescription_LogTest_testLogMacro;

#include "D:/chen/gamedev/myproj/garnet3d/new/core/src/test/base/math.cpp"

static MathTest suite_MathTest;

static CxxTest::List Tests_MathTest = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_MathTest( "D:/chen/gamedev/myproj/garnet3d/new/core/src/test/base/math.cpp", 48, "MathTest", suite_MathTest, Tests_MathTest );

static class TestDescription_MathTest_testVector : public CxxTest::RealTestDescription {
public:
 TestDescription_MathTest_testVector() : CxxTest::RealTestDescription( Tests_MathTest, suiteDescription_MathTest, 51, "testVector" ) {}
 void runTest() { suite_MathTest.testVector(); }
} testDescription_MathTest_testVector;

#include "D:/chen/gamedev/myproj/garnet3d/new/core/src/test/base/str.cpp"

static StringTest suite_StringTest;

static CxxTest::List Tests_StringTest = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_StringTest( "D:/chen/gamedev/myproj/garnet3d/new/core/src/test/base/str.cpp", 7, "StringTest", suite_StringTest, Tests_StringTest );

static class TestDescription_StringTest_testCtor : public CxxTest::RealTestDescription {
public:
 TestDescription_StringTest_testCtor() : CxxTest::RealTestDescription( Tests_StringTest, suiteDescription_StringTest, 11, "testCtor" ) {}
 void runTest() { suite_StringTest.testCtor(); }
} testDescription_StringTest_testCtor;

static class TestDescription_StringTest_testMethod : public CxxTest::RealTestDescription {
public:
 TestDescription_StringTest_testMethod() : CxxTest::RealTestDescription( Tests_StringTest, suiteDescription_StringTest, 40, "testMethod" ) {}
 void runTest() { suite_StringTest.testMethod(); }
} testDescription_StringTest_testMethod;

#include <cxxtest/Root.cpp>

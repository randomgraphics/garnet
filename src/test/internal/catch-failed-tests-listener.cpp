/**
 * Catch2 event listener that prints the names of all failed/skipped tests at
 * the end of the test run.
 */

#include <catch2/reporters/catch_reporter_event_listener.hpp>
#include <catch2/reporters/catch_reporter_registrars.hpp>
#include <catch2/catch_test_case_info.hpp>
#include <iostream>
#include <string>
#include <vector>

namespace {

class FailedTestsListener : public Catch::EventListenerBase {
public:
    static std::string getDescription() { return "Prints names of all failed/skipped tests at the end of the run"; }

    explicit FailedTestsListener(Catch::IConfig const * config): EventListenerBase(config) {}

    void testRunStarting(Catch::TestRunInfo const &) override {
        m_failedNames.clear();
        m_skippedNames.clear();
    }

    void testCaseStarting(Catch::TestCaseInfo const & t) override { std::cout << "\n----- Starting test: " << t.name << " -----\n" << std::flush; }

    void testCaseEnded(Catch::TestCaseStats const & testCaseStats) override {
        if (testCaseStats.totals.assertions.total() > 0 && !testCaseStats.totals.assertions.allOk()) {
            m_failedNames.push_back(testCaseStats.testInfo->name);
        } else if (testCaseStats.totals.testCases.skipped > 0) {
            m_skippedNames.push_back(testCaseStats.testInfo->name);
        }
    }

    // Called for tests skipped due to test-run abort (not SKIP() macro).
    void skipTest(Catch::TestCaseInfo const & testInfo) override { m_skippedNames.push_back(testInfo.name); }

    void testRunEnded(Catch::TestRunStats const &) override {
        if (!m_skippedNames.empty()) {
            std::cout << "\n--- Skipped tests ---\n";
            for (auto const & name : m_skippedNames) std::cout << "  " << name << "\n";
            std::cout << "---------------------\n";
        }
        if (!m_failedNames.empty()) {
            std::cout << "\n--- Failed tests ---\n";
            for (auto const & name : m_failedNames) std::cout << "  " << name << "\n";
            std::cout << "--------------------\n";
        }
        std::cout << std::flush;
    }

private:
    std::vector<std::string> m_failedNames;
    std::vector<std::string> m_skippedNames;
};

} // namespace

CATCH_REGISTER_LISTENER(FailedTestsListener)

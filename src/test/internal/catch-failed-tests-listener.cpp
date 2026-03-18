/**
 * Catch2 event listener that prints the names of all failed tests at the end
 * of the test run.
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
    static std::string getDescription() { return "Prints names of all failed tests at the end of the run"; }

    explicit FailedTestsListener(Catch::IConfig const * config): EventListenerBase(config) {}

    void testRunStarting(Catch::TestRunInfo const &) override { m_failedNames.clear(); }

    void testCaseEnded(Catch::TestCaseStats const & testCaseStats) override {
        if (testCaseStats.totals.assertions.total() > 0 && !testCaseStats.totals.assertions.allOk()) { m_failedNames.push_back(testCaseStats.testInfo->name); }
    }

    void testRunEnded(Catch::TestRunStats const &) override {
        if (m_failedNames.empty()) return;
        std::cout << "\n--- Failed tests ---\n";
        for (auto const & name : m_failedNames) std::cout << "  " << name << "\n";
        std::cout << "--------------------\n" << std::flush;
    }

private:
    std::vector<std::string> m_failedNames;
};

} // namespace

CATCH_REGISTER_LISTENER(FailedTestsListener)

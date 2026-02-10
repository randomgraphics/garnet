#include "../testCommon.h"
#include <map>
#include <unordered_map>
#include <chrono>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <random>

using namespace GN;

// Helper function to format time
static std::string formatTime(double seconds) {
    if (seconds < 0.000001) {
        return std::to_string(seconds * 1000000000) + " ns";
    } else if (seconds < 0.001) {
        return std::to_string(seconds * 1000000) + " us";
    } else if (seconds < 1.0) {
        return std::to_string(seconds * 1000) + " ms";
    } else {
        return std::to_string(seconds) + " s";
    }
}

// Helper function to measure time
template<typename Func>
double measureTime(Func && func, int iterations = 1) {
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        func();
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    return duration.count() / 1e9 / iterations; // Return time in seconds per iteration
}

class TestDictPerf : public CxxTest::TestSuite {
public:
    static const int TEST_SIZE = 10000;
    static const int WARMUP_ITERATIONS = 3;
    static const int BENCHMARK_ITERATIONS = 5;

    // Generate test data
    std::vector<int> generateIntKeys(int count) {
        std::vector<int> keys;
        keys.reserve(count);
        for (int i = 0; i < count; ++i) {
            keys.push_back(i);
        }
        // Shuffle to avoid ordered insertion
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(keys.begin(), keys.end(), g);
        return keys;
    }

    std::vector<std::string> generateStringKeys(int count) {
        std::vector<std::string> keys;
        keys.reserve(count);
        for (int i = 0; i < count; ++i) {
            std::ostringstream oss;
            oss << "key_" << i << "_" << (i * 7 + 13);
            keys.push_back(oss.str());
        }
        // Shuffle to avoid ordered insertion
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(keys.begin(), keys.end(), g);
        return keys;
    }

    // Test insert performance
    template<typename DictType, typename KeyType>
    double testInsert(const std::vector<KeyType> & keys) {
        DictType dict;
        return measureTime([&]() {
            for (const auto & key : keys) {
                dict[key] = {}; // insert default value
            }
        });
    }

    // Test find performance for GN::Dictionary
    template<typename KeyType>
    std::pair<double, int> testFindDict(const Dictionary<KeyType, int> & dict, const std::vector<KeyType> & keys) {
        int sum = 0;
        double time = measureTime([&]() {
            for (const auto & key : keys) {
                if (dict.find(key) != nullptr) {
                    sum += 1;
                }
            }
        });
        return std::make_pair(time, sum);
    }

    // Test find performance for std containers
    template<typename DictType, typename KeyType>
    std::pair<double, int> testFindStd(const DictType & dict, const std::vector<KeyType> & keys) {
        int sum = 0;
        double time = measureTime([&]() {
            for (const auto & key : keys) {
                auto it = dict.find(key);
                if (it != dict.end()) {
                    sum += 1;
                }
            }
        });
        return std::make_pair(time, sum);
    }

    // Test iteration performance for GN::Dictionary
    template<typename KeyType>
    std::pair<double, size_t> testIterateDict(const Dictionary<KeyType, int> & dict) {
        size_t count = 0;
        double time = measureTime([&]() {
            for (auto it = dict.begin(); it != dict.end(); ++it) {
                count += 1;
                // Access key/value to make it more realistic
                auto k = it->key();
                auto v = it->value();
                GN_UNUSED_PARAM(k);
                GN_UNUSED_PARAM(v);
            }
        });
        return std::make_pair(time, count);
    }

    // Test iteration performance for std containers
    template<typename DictType>
    std::pair<double, size_t> testIterateStd(const DictType & dict) {
        size_t count = 0;
        double time = measureTime([&]() {
            for (auto it = dict.begin(); it != dict.end(); ++it) {
                count += 1;
                // Access key/value to make it more realistic
                auto k = it->first;
                auto v = it->second;
                GN_UNUSED_PARAM(k);
                GN_UNUSED_PARAM(v);
            }
        });
        return std::make_pair(time, count);
    }

    // Test remove performance for GN::Dictionary
    template<typename KeyType>
    double testRemoveDict(Dictionary<KeyType, int> & dict, const std::vector<KeyType> & keys) {
        return measureTime([&]() {
            for (const auto & key : keys) {
                dict.remove(key);
            }
        });
    }

    // Test remove performance for std containers
    template<typename DictType, typename KeyType>
    double testRemoveStd(DictType & dict, const std::vector<KeyType> & keys) {
        return measureTime([&]() {
            for (const auto & key : keys) {
                dict.erase(key);
            }
        });
    }

    // Benchmark a specific operation (returns time only)
    template<typename Func>
    double benchmark(Func && func) {
        // Warmup
        for (int i = 0; i < WARMUP_ITERATIONS; ++i) {
            func();
        }
        // Actual benchmark
        double total = 0.0;
        for (int i = 0; i < BENCHMARK_ITERATIONS; ++i) {
            total += func();
        }
        return total / BENCHMARK_ITERATIONS;
    }

    // Benchmark a specific operation (returns pair of time and value)
    template<typename Func>
    std::pair<double, typename std::result_of<Func()>::type::second_type> benchmarkWithValue(Func && func) {
        // Warmup
        for (int i = 0; i < WARMUP_ITERATIONS; ++i) {
            func();
        }
        // Actual benchmark
        double total = 0.0;
        typename std::result_of<Func()>::type::second_type value = 0;
        for (int i = 0; i < BENCHMARK_ITERATIONS; ++i) {
            auto result = func();
            total += result.first;
            value = result.second; // Use the last value
        }
        return std::make_pair(total / BENCHMARK_ITERATIONS, value);
    }

    void testIntKeyInsert() {
        auto keys = generateIntKeys(TEST_SIZE);

        double gnDictTime = benchmark([&]() { return testInsert<Dictionary<int, int>>(keys); });
        double stdMapTime = benchmark([&]() { return testInsert<std::map<int, int>>(keys); });
        double stdUnorderedMapTime = benchmark([&]() { return testInsert<std::unordered_map<int, int>>(keys); });

        printf("\n=== Integer Key Insert Performance (size: %d) ===\n", TEST_SIZE);
        printf("GN::Dictionary:        %s\n", formatTime(gnDictTime).c_str());
        printf("std::map:              %s\n", formatTime(stdMapTime).c_str());
        printf("std::unordered_map:    %s\n", formatTime(stdUnorderedMapTime).c_str());
        printf("GN::Dictionary vs std::map: %.2fx\n", stdMapTime / gnDictTime);
        printf("GN::Dictionary vs std::unordered_map: %.2fx\n", stdUnorderedMapTime / gnDictTime);
    }

    void testIntKeyFind() {
        auto keys = generateIntKeys(TEST_SIZE);

        Dictionary<int, int> gnDict;
        std::map<int, int> stdMap;
        std::unordered_map<int, int> stdUnorderedMap;

        for (const auto & key : keys) {
            gnDict[key] = key;
            stdMap[key] = key;
            stdUnorderedMap[key] = key;
        }

        auto gnDictResult = benchmarkWithValue([&]() { return testFindDict(gnDict, keys); });
        auto stdMapResult = benchmarkWithValue([&]() { return testFindStd(stdMap, keys); });
        auto stdUnorderedMapResult = benchmarkWithValue([&]() { return testFindStd(stdUnorderedMap, keys); });

        printf("\n=== Integer Key Find Performance (size: %d) ===\n", TEST_SIZE);
        printf("GN::Dictionary:        %s (sum: %d)\n", formatTime(gnDictResult.first).c_str(), gnDictResult.second);
        printf("std::map:              %s (sum: %d)\n", formatTime(stdMapResult.first).c_str(), stdMapResult.second);
        printf("std::unordered_map:    %s (sum: %d)\n", formatTime(stdUnorderedMapResult.first).c_str(), stdUnorderedMapResult.second);
        printf("GN::Dictionary vs std::map: %.2fx\n", stdMapResult.first / gnDictResult.first);
        printf("GN::Dictionary vs std::unordered_map: %.2fx\n", stdUnorderedMapResult.first / gnDictResult.first);
    }

    void testIntKeyIterate() {
        auto keys = generateIntKeys(TEST_SIZE);

        Dictionary<int, int> gnDict;
        std::map<int, int> stdMap;
        std::unordered_map<int, int> stdUnorderedMap;

        for (const auto & key : keys) {
            gnDict[key] = key;
            stdMap[key] = key;
            stdUnorderedMap[key] = key;
        }

        auto gnDictResult = benchmarkWithValue([&]() { return testIterateDict(gnDict); });
        auto stdMapResult = benchmarkWithValue([&]() { return testIterateStd(stdMap); });
        auto stdUnorderedMapResult = benchmarkWithValue([&]() { return testIterateStd(stdUnorderedMap); });

        printf("\n=== Integer Key Iterate Performance (size: %d) ===\n", TEST_SIZE);
        printf("GN::Dictionary:        %s (count: %zu)\n", formatTime(gnDictResult.first).c_str(), gnDictResult.second);
        printf("std::map:              %s (count: %zu)\n", formatTime(stdMapResult.first).c_str(), stdMapResult.second);
        printf("std::unordered_map:    %s (count: %zu)\n", formatTime(stdUnorderedMapResult.first).c_str(), stdUnorderedMapResult.second);
        printf("GN::Dictionary vs std::map: %.2fx\n", stdMapResult.first / gnDictResult.first);
        printf("GN::Dictionary vs std::unordered_map: %.2fx\n", stdUnorderedMapResult.first / gnDictResult.first);
    }

    void testIntKeyRemove() {
        auto keys = generateIntKeys(TEST_SIZE);

        double gnDictTime = benchmark([&]() {
            Dictionary<int, int> dict;
            for (const auto & key : keys) {
                dict[key] = key;
            }
            return testRemoveDict(dict, keys);
        });

        double stdMapTime = benchmark([&]() {
            std::map<int, int> dict;
            for (const auto & key : keys) {
                dict[key] = key;
            }
            return testRemoveStd(dict, keys);
        });

        double stdUnorderedMapTime = benchmark([&]() {
            std::unordered_map<int, int> dict;
            for (const auto & key : keys) {
                dict[key] = key;
            }
            return testRemoveStd(dict, keys);
        });

        printf("\n=== Integer Key Remove Performance (size: %d) ===\n", TEST_SIZE);
        printf("GN::Dictionary:        %s\n", formatTime(gnDictTime).c_str());
        printf("std::map:              %s\n", formatTime(stdMapTime).c_str());
        printf("std::unordered_map:    %s\n", formatTime(stdUnorderedMapTime).c_str());
        printf("GN::Dictionary vs std::map: %.2fx\n", stdMapTime / gnDictTime);
        printf("GN::Dictionary vs std::unordered_map: %.2fx\n", stdUnorderedMapTime / gnDictTime);
    }

    void testStringKeyInsert() {
        auto keys = generateStringKeys(TEST_SIZE);

        double gnDictTime = benchmark([&]() { return testInsert<Dictionary<std::string, int>>(keys); });
        double stdMapTime = benchmark([&]() { return testInsert<std::map<std::string, int>>(keys); });
        double stdUnorderedMapTime = benchmark([&]() { return testInsert<std::unordered_map<std::string, int>>(keys); });

        printf("\n=== String Key Insert Performance (size: %d) ===\n", TEST_SIZE);
        printf("GN::Dictionary:        %s\n", formatTime(gnDictTime).c_str());
        printf("std::map:              %s\n", formatTime(stdMapTime).c_str());
        printf("std::unordered_map:    %s\n", formatTime(stdUnorderedMapTime).c_str());
        printf("GN::Dictionary vs std::map: %.2fx\n", stdMapTime / gnDictTime);
        printf("GN::Dictionary vs std::unordered_map: %.2fx\n", stdUnorderedMapTime / gnDictTime);
    }

    void testStringKeyFind() {
        auto keys = generateStringKeys(TEST_SIZE);

        Dictionary<std::string, int> gnDict;
        std::map<std::string, int> stdMap;
        std::unordered_map<std::string, int> stdUnorderedMap;

        for (const auto & key : keys) {
            gnDict[key] = 1;
            stdMap[key] = 1;
            stdUnorderedMap[key] = 1;
        }

        auto gnDictResult = benchmarkWithValue([&]() { return testFindDict(gnDict, keys); });
        auto stdMapResult = benchmarkWithValue([&]() { return testFindStd(stdMap, keys); });
        auto stdUnorderedMapResult = benchmarkWithValue([&]() { return testFindStd(stdUnorderedMap, keys); });

        printf("\n=== String Key Find Performance (size: %d) ===\n", TEST_SIZE);
        printf("GN::Dictionary:        %s (sum: %d)\n", formatTime(gnDictResult.first).c_str(), gnDictResult.second);
        printf("std::map:              %s (sum: %d)\n", formatTime(stdMapResult.first).c_str(), stdMapResult.second);
        printf("std::unordered_map:    %s (sum: %d)\n", formatTime(stdUnorderedMapResult.first).c_str(), stdUnorderedMapResult.second);
        printf("GN::Dictionary vs std::map: %.2fx\n", stdMapResult.first / gnDictResult.first);
        printf("GN::Dictionary vs std::unordered_map: %.2fx\n", stdUnorderedMapResult.first / gnDictResult.first);
    }

    void testStringKeyIterate() {
        auto keys = generateStringKeys(TEST_SIZE);

        Dictionary<std::string, int> gnDict;
        std::map<std::string, int> stdMap;
        std::unordered_map<std::string, int> stdUnorderedMap;

        for (const auto & key : keys) {
            gnDict[key] = 1;
            stdMap[key] = 1;
            stdUnorderedMap[key] = 1;
        }

        auto gnDictResult = benchmarkWithValue([&]() { return testIterateDict(gnDict); });
        auto stdMapResult = benchmarkWithValue([&]() { return testIterateStd(stdMap); });
        auto stdUnorderedMapResult = benchmarkWithValue([&]() { return testIterateStd(stdUnorderedMap); });

        printf("\n=== String Key Iterate Performance (size: %d) ===\n", TEST_SIZE);
        printf("GN::Dictionary:        %s (count: %zu)\n", formatTime(gnDictResult.first).c_str(), gnDictResult.second);
        printf("std::map:              %s (count: %zu)\n", formatTime(stdMapResult.first).c_str(), stdMapResult.second);
        printf("std::unordered_map:    %s (count: %zu)\n", formatTime(stdUnorderedMapResult.first).c_str(), stdUnorderedMapResult.second);
        printf("GN::Dictionary vs std::map: %.2fx\n", stdMapResult.first / gnDictResult.first);
        printf("GN::Dictionary vs std::unordered_map: %.2fx\n", stdUnorderedMapResult.first / gnDictResult.first);
    }

    void testStringKeyRemove() {
        auto keys = generateStringKeys(TEST_SIZE);

        double gnDictTime = benchmark([&]() {
            Dictionary<std::string, int> dict;
            for (const auto & key : keys) {
                dict[key] = 1;
            }
            return testRemoveDict(dict, keys);
        });

        double stdMapTime = benchmark([&]() {
            std::map<std::string, int> dict;
            for (const auto & key : keys) {
                dict[key] = 1;
            }
            return testRemoveStd(dict, keys);
        });

        double stdUnorderedMapTime = benchmark([&]() {
            std::unordered_map<std::string, int> dict;
            for (const auto & key : keys) {
                dict[key] = 1;
            }
            return testRemoveStd(dict, keys);
        });

        printf("\n=== String Key Remove Performance (size: %d) ===\n", TEST_SIZE);
        printf("GN::Dictionary:        %s\n", formatTime(gnDictTime).c_str());
        printf("std::map:              %s\n", formatTime(stdMapTime).c_str());
        printf("std::unordered_map:    %s\n", formatTime(stdUnorderedMapTime).c_str());
        printf("GN::Dictionary vs std::map: %.2fx\n", stdMapTime / gnDictTime);
        printf("GN::Dictionary vs std::unordered_map: %.2fx\n", stdUnorderedMapTime / gnDictTime);
    }
};

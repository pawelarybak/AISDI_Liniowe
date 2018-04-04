#include <cstddef>
#include <cstdlib>
#include <string>
#include <iostream>
#include <map>
#include <ctime>

#include "Vector.h"
#include "LinkedList.h"

namespace {

    using Stats = struct Stats {
        std::map<std::string, long long> time {
                {"10_insert_begin", 0},
                {"10_insert_middle", 0},
                {"10_insert_end", 0},
                {"100_insert_begin", 0},
                {"100_insert_middle", 0},
                {"100_insert_end", 0},
                {"1000_insert_begin", 0},
                {"1000_insert_middle", 0},
                {"1000_insert_end", 0},
                {"10000_insert_begin", 0},
                {"10000_insert_middle", 0},
                {"10000_insert_end", 0},
                {"100000_insert_begin", 0},
                {"100000_insert_middle", 0},
                {"100000_insert_end", 0},
        };
        long executions = 0L;

        void print_results() {
            for (auto& pair : time) {
                std::cout << pair.first << "= " << float(pair.second) / executions << std::endl;
            }
        }
    };

    template<typename T>
    using LinearCollection = aisdi::Vector<T>;

    std::clock_t test_begin(size_t size) {
        LinearCollection<int> collection;
        std::clock_t ts, tf;
        for (unsigned int i = 0; i < size; ++i) {
            collection.append(i);
        }
        auto it = collection.begin();
        ts = std::clock();
        collection.insert(it, 103);
        tf = std::clock();
        return tf - ts;
    }

    std::clock_t test_middle(size_t size) {
        LinearCollection<int> collection;
        std::clock_t ts, tf;
        for (unsigned int i = 0; i < size; ++i) {
            collection.append(i);
        }
        auto it = collection.begin() + collection.getSize() / 2;
        ts = std::clock();
        collection.insert(it, 103);
        tf = std::clock();
        return tf - ts;
    }

    std::clock_t test_end(size_t size) {
        LinearCollection<int> collection;
        std::clock_t ts, tf;
        for (unsigned int i = 0; i < size; ++i) {
            collection.append(i);
        }
        auto it = collection.end();
        ts = std::clock();
        collection.insert(it, 103);
        tf = std::clock();
        return tf - ts;
    }

    void perfomTest(Stats& statistics) {
        statistics.executions += 1;
        std::cout << "Iteration: " << statistics.executions << std::endl;
        std::vector<int> sizes{10, 100, 1000, 10000, 100000};

        for (int size : sizes) {
            statistics.time[std::to_string(size) + "_insert_begin"] += test_begin(size);
            statistics.time[std::to_string(size) + "_insert_middle"] += test_middle(size);
            statistics.time[std::to_string(size) + "_insert_end"] += test_end(size);
        }
    }
}

int main(int argc, char** argv)
{
    Stats stats;
    const std::size_t repeatCount = argc > 1 ? std::atoll(argv[1]) : 1;
    std::cout << "Testing for " << repeatCount << " iterations..." << std::endl;
    for (std::size_t i = 0; i < repeatCount; ++i) {
        perfomTest(stats);
    }
    stats.print_results();
    return 0;
}

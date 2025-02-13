#ifndef SHAREDRESULTS_HPP
#define SHAREDRESULTS_HPP

#define SHARED_RESULTS_SIZE 1000000
#include <atomic>
#include <array>

class SharedResults
{
public:
    SharedResults() : arr{} {}

    uint64_t getValue(InfInt key) {
        if (key >= SHARED_RESULTS_SIZE) return 0;
        return arr[key.toUnsignedLong()];
    }

    bool setValue(InfInt key, uint64_t value) {
        if (key >= SHARED_RESULTS_SIZE) return false;
        std::atomic<uint64_t> expected(value); // value in array is 0 <==> value not set yet
        return expected.compare_exchange_weak(arr[key.toUnsignedLong()], value);
    }
private:
    std::array<uint64_t, SHARED_RESULTS_SIZE> arr;
};


#endif
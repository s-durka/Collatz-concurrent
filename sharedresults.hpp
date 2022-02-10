#ifndef SHAREDRESULTS_HPP
#define SHAREDRESULTS_HPP

#define SHARED_RESULTS_SIZE 1000000
#include <atomic>

class SharedResults
{
public:
    SharedResults() : arr{} {
        printf("new shared results\n");
        print(2);
        for (int i = 0; i < 3; i++)
            printf("--%lu, ", arr[i]);
//        for (int i = 0; i < SHARED_RESULTS_SIZE; i++) {
//            arr[i] = std::atomic<uint64_t>(-1);
//        }
    }
//    ~SharedResults() =default;
//        delete[] arr;

    void print(uint64_t n) {
        for (int i = 0; i < n; i++)
            printf("%lu, ", arr[i]);
        printf("\n");
    }

    uint64_t getValue(size_t key) {
//        printf("get value\n");
//        printf("key = %lu\n", key);
//        printf("arr[key] = %lu\n", arr[key]);
//        print(4);
        return arr[key];
    }

    bool setValue(size_t key, uint64_t value) {
        if (key >= SHARED_RESULTS_SIZE) return false;
        std::atomic<uint64_t> expected(value); // value in array is 0 <==> value not set yet
        return expected.compare_exchange_weak(arr[key], value);
    }
private:
//    std::atomic<uint64_t>* arr;
    std::array<uint64_t, SHARED_RESULTS_SIZE> arr;
//    std::atomic<uint64_t> arr2[SHARED_RESULTS_SIZE];
};


#endif
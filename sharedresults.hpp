#ifndef SHAREDRESULTS_HPP
#define SHAREDRESULTS_HPP

#define SHARED_RESULTS_SIZE 1000000

class SharedResults
{
//TODO
public:
    SharedResults() {
        arr = new std::atomic<uint64_t>[SHARED_RESULTS_SIZE]{};
        for (int i = 0; i < SHARED_RESULTS_SIZE; i++) {
            arr[i] = -1;
        }
    }
    ~SharedResults() {
        delete[] arr;
    }

    uint64_t getValue(size_t key) {
        return arr[key].load();
    }

    void setValue(size_t key, uint64_t value) {
        uint64_t expected = -1;
        arr[key].compare_exchange_weak(expected, value);
    }
private:
    std::atomic<uint64_t>* arr;

};

#endif
#include <utility>
#include <deque>
#include <future>
#include <list>
#include <inttypes.h>
#include "teams.hpp"
#include "contest.hpp"
#include "collatz.hpp"

void TeamNewThreads::insertCollatz(ContestResult & result, uint64_t i, uint64_t idx, InfInt const & n,
                                            std::queue<uint64_t>& readyToJoin, std::mutex& mutex,
                                            std::condition_variable& cond, rtimers::cxx11::ThreadedTimer& thTimer,
                                            rtimers::cxx11::DefaultTimer& soloTimer2) {
    thTimer.scopedStart();
    soloTimer2.scopedStart();
    result[idx] = calcCollatz(n);
    std::unique_lock<std::mutex> lock(mutex);
    readyToJoin.push(i);
    lock.unlock();
    cond.notify_all();
}
ContestResult TeamNewThreads::runContestImpl(ContestInput const & contestInput)
{
    ContestResult result;
    result.resize(contestInput.size());
    uint64_t idx = 0;
    std::queue<uint64_t> readyToJoin;
    std::condition_variable cond;
    std::mutex cond_mutex; // mutex for the condition variable cond

    rtimers::cxx11::DefaultTimer soloTimer("CalcCollatzSoloTimer");
    rtimers::cxx11::ThreadedTimer thTimer("NewThreadsTimer");
    rtimers::cxx11::DefaultTimer soloTimer2("CalcCollatzSoloTimer2");


    uint32_t maxThreads = this->getSize();
//    printf("MAX THREADS = %d\n", maxThreads);
    std::thread threads[maxThreads];
    uint64_t i = 0;
    uint64_t waitingThreads = 0;
    for(InfInt const & singleInput : contestInput)
    {
        auto scopedStartStop = soloTimer.scopedStart();
        if (i < maxThreads) {
            threads[i] = createThread([&result, i, idx, &singleInput, &readyToJoin, &cond_mutex, &cond, &thTimer, &soloTimer2]
                    { insertCollatz(result, i, idx, singleInput, readyToJoin, cond_mutex, cond, thTimer, soloTimer2); });
        } else {
            std::unique_lock<std::mutex> lock(cond_mutex);
            waitingThreads++;
            cond.wait(lock, [&readyToJoin]{ return !readyToJoin.empty(); });
            uint64_t j = readyToJoin.front();
            threads[j].join();
            waitingThreads--;
            readyToJoin.pop();
            threads[j] = createThread([&result, j, idx, &singleInput, &readyToJoin, &cond_mutex, &cond, &thTimer, &soloTimer2]
                                      { insertCollatz(result, j, idx, singleInput, readyToJoin, cond_mutex, cond, thTimer, soloTimer2); });
        }
        ++idx;
        ++i;
    }
//    printf("IIIIIIIIIIIIIIII\n");
//    while (activeThreads > 0) {
////        printf("XXXXXXXXXXXXX %" PRIu64 "\n", activeThreads);
//        std::unique_lock<std::mutex> lock(cond_mutex);
//        cond.wait(lock, [&readyToJoin]{ return !readyToJoin.empty(); });
//        uint64_t k = readyToJoin.front();
////        printf("LALALA %" PRIu64 "\n", k);
//        threads[readyToJoin.front()].join();
//        activeThreads--;
//        readyToJoin.pop();
//    }
    uint32_t min;
    if (maxThreads > i) min = i;
    else min = maxThreads;
    for (int k = 0; k < min; k++) {
        threads[k].join();
    }
    return result;
}

void TeamConstThreads::insertCollatz(ContestResult & result,
                                     ContestInput const & input,
                                     size_t threadsNum,
                                     size_t myNum,
                                     rtimers::cxx11::ThreadedTimer& thTimer) {

    for (size_t i = myNum; i < input.size(); i+= threadsNum) {
        thTimer.scopedStart();
//        printf("Thread %zu - calcCollatz[%zu];\n", myNum, i);
        result[i] = calcCollatz(input[i]);
    }
}

ContestResult TeamConstThreads::runContestImpl(ContestInput const & contestInput)
{
    ContestResult r;
    size_t threadsNum = this->getSize();
    printf("getSize = %u", this->getSize());
    r.resize(contestInput.size());
    rtimers::cxx11::DefaultTimer soloTimer("CalcCollatzSoloTimer");
    rtimers::cxx11::ThreadedTimer thTimer("NewThreadsTimer");
    std::thread threads[threadsNum];
    for (size_t thIndex = 0; thIndex < threadsNum; thIndex++) {
        printf("THREAD %zu CREATED\n", thIndex);
        threads[thIndex] = createThread([&threadsNum, &r, &contestInput, thIndex, &thTimer ]
                { insertCollatz(r, contestInput, threadsNum, thIndex, thTimer); });
    }
    for (size_t thIndex = 0; thIndex < threadsNum; thIndex++) {
        threads[thIndex].join();
    }
    return r;
}

ContestResult TeamPool::runContest(ContestInput const & contestInput)
{
    ContestResult r;
    //TODO
    return r;
}

ContestResult TeamNewProcesses::runContest(ContestInput const & contestInput)
{
    ContestResult r;
    //TODO
    return r;
}

ContestResult TeamConstProcesses::runContest(ContestInput const & contestInput)
{
    ContestResult r;
    //TODO
    return r;
}

ContestResult TeamAsync::runContest(ContestInput const & contestInput)
{
    ContestResult r;
    //TODO
    return r;
}

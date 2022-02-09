//
// Created by stas on 08.02.2022.
//

#include <utility>
#include <deque>
#include <future>

#include "teams.hpp"
#include "contest.hpp"
#include "collatz.hpp"

void f(int n, rtimers::cxx11::ThreadedTimer& thTimer) {
    auto scopedStartStop2 = thTimer.scopedStart();

    std::this_thread::sleep_for(std::chrono::seconds(1));
//    int x = 99;
//    auto scopedStartStop = soloTimer.scopedStart();
    calcCollatz(n);
//    for (int j = 0; j<1000; j++) {
//        x = x*100000;
//        x = x % 7;
//        x = x*x*x;
//    }
}
int main() {
    std::thread threads[10];
    rtimers::cxx11::ThreadedTimer thTimer("thread");
    rtimers::cxx11::DefaultTimer soloTimer("solo");
    rtimers::cxx11::DefaultTimer totalTimer("Total");
    auto totalStartStop = totalTimer.scopedStart();
    for (int i = 0; i < 5; i++) {
        auto scopedStartStop = soloTimer.scopedStart();

        int n = 1234567;
        calcCollatz(n);
        std::this_thread::sleep_for(std::chrono::seconds(1));


//        threads[i] = std::thread{ [n, &thTimer]{f(n, thTimer); }};
//        soloTimer.getStats();
    }
//    for (int i = 0; i < 5; i++)
//        threads[i].join();
    return 0;
}
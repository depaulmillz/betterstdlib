#include <jthread.hh>
#include <cassert>
#include <iostream>
#include <atomic>

int main() {
    std::atomic<int> i(0);

    auto t = betterstd::jthread([&i](int val) {
        i += val;
        std::cerr << i << std::endl;
    }, 2);

    t.join();

    assert(i == 2);

    i.store(0);

    auto s = new betterstd::jthread([&i](int val) {
        i += val;
        std::cerr << i << std::endl;
    }, 2);

    delete s;

    assert(i == 2);

    return 0;
}
#include <lthread.hh>
#include <cassert>
#include <iostream>

int main() {
    std::atomic<int> i(0);
    auto t = betterstd::lthread([&i](int val) {
        i += val;
        std::cerr << i << std::endl;
    }, 2);

    while (i < 10);

    t.join();

    assert(i >= 10);

    i.store(0);

    auto s = new betterstd::lthread([&i](int val) {
        i += val;
        std::cerr << i << std::endl;
    }, 2);

    while (i < 10);

    delete s;

    assert(i >= 10);

    return 0;
}
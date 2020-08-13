#include <threadpool.hh>
#include <cassert>

int f() {
    return 3;
}

int main() {

    using namespace betterstd;


    threadpool pool(2);

    for (int i = 0; i < 5; i++)
        pool.add_task(f);
    auto fut = pool.add_task(f);
    assert(fut.get() == 3);
    while (pool.queueSize() > 0);

    return 0;

}
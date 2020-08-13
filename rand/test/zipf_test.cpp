#include <zipf.hh>

int main() {

    const double zetan = betterstd::zeta(0.99, 10000);

    while (betterstd::rand_zipf(10000, zetan, 0.99) != 1)
        ;

    while (betterstd::rand_zipf(10000, zetan, 0.99) == 1)
        ;

    return 0;
}
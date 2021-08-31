#include "a.h"
#include "core/nope.h"
#include   "core/b.h"
#include "core/c.h"
#include "core/yet/d.h"
#include "extra/e.h"

#include <iostream>

void print_a() {
    std::cout << "a\n";
    print_b();
}

int main() {
    std::cout << "---- trivial ----\n";
    print_a();
    print_b();
    print_c();
    print_d();
    print_e();
}

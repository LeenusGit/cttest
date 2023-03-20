#include <cttest.h>

cttest::Suite suite{"Global Suite"};

using namespace cttest;

int main() {
    suite.summary();
    return suite.passed() ? 0 : 1;
}

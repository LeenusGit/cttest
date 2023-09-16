#include <cttest.h>

#include "ut_suite.h"
// #include "staticmap.h"

// auto test1 = []{
//     return cttest::expect(1, cttest::LessThan{}, 1);
// };

// constexpr auto suite = [] {
//     cttest::Suite res{"suite"};
//     res.addTest(test1);
//     return res;
// };

auto test = [] {
    return (
        ut_suite().run()
        // ut_StaticMap().run()
    );
};


// static_assert(test());

int main() {
    return test() ? 0 : 1;
}

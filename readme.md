# CTTEST - Run your unit tests at compile-time

## Example Usage:

```c++
#include <cttest.h>

int main() {

    int const rc = ctt::test_all(

        [] {
            int ans = 0;
            CTT_CHECK(ctt::expect_eq(ans, 0));
            CTT_CHECK(ctt::expect_ne(ans, 1));

            CTT_CHECK(ctt::expect_true(ans == 0));
            CTT_CHECK(ctt::expect_false(ans != 0));

            CTT_CHECK(ctt::expect_less(ans, 1));
            CTT_CHECK(ctt::expect_greater(ans, -1));

            CTT_CHECK(ctt::expect_less_equal(ans, 0));
            CTT_CHECK(ctt::expect_greater_equal(ans, 0));

            return ctt::passed;
        }
        
        ,[] {
            int ans = 0;
            CTT_CHECK(ctt::expect_eq(ans, 42));
            return ctt::passed;
        }
    );

    return rc;
}

```
Possible output:

```
FAILED: test/ut_simple.p/ut_simple.cpp.o 
clang++-17 -Itest/ut_simple.p -Itest -I../../test -I../../subprojects/fmt-10.1.1/include -Isrc -I../../src -fcolor-diagnostics -D_FILE_OFFSET_BITS=64 -Wall -Winvalid-pch -O0 -g -std=c++26 -DFMT_SHARED -MD -MQ test/ut_simple.p/ut_simple.cpp.o -MF test/ut_simple.p/ut_simple.cpp.o.d -o test/ut_simple.p/ut_simple.cpp.o -c ../../test/ut_simple.cpp
In file included from ../../test/ut_simple.cpp:1:
../../src/cttest.h:155:23: error: static assertion failed due to requirement '!err': 
	in file:(../../test/ut_simple.cpp:26)
		expected {0} == {42}
  155 |         static_assert(not err, err.value());
      |                       ^~~~~~~
../../src/cttest.h:169:31: note: in instantiation of function template specialization 'ctt::test<(lambda at ../../test/ut_simple.cpp:24:10)>' requested here
  169 |     int const passed = (... + test(funcs));
      |                               ^
../../test/ut_simple.cpp:5:25: note: in instantiation of function template specialization 'ctt::test_all<(lambda at ../../test/ut_simple.cpp:7:9), (lambda at ../../test/ut_simple.cpp:24:10)>' requested here
    5 |     int const rc = ctt::test_all(
      |                         ^
1 error generated.
ninja: build stopped: subcommand failed.
```

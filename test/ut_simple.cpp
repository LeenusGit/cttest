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

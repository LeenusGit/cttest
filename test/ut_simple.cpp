#include <cttest.h>

int main() {

    int const rc = ctt::test(
        [] {
            int ans = 0;
            CTT_CHECK(ctt::expect_less(ans, 111));
            CTT_CHECK(ctt::expect_less(ans, 11));
            CTT_CHECK(ctt::expect_less(ans, 1));

            return ctt::passed;
        },
        
        [] {
            int ans = 0;
            CTT_CHECK(ctt::expect_less(ans, 111));
            CTT_CHECK(ctt::expect_less(ans, 11));
            CTT_CHECK(ctt::expect_less(ans, 1));

            return ctt::passed;
        },

        [] {
            int ans = 42;
            CTT_CHECK(ctt::expect_true(ans == 42));
            return ctt::passed;
        }
    
    );

    return rc;
}

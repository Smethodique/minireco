#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

void test_hello(void) {
    CU_ASSERT(1 + 1 == 2);
}

int main() {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("Hello_Suite", 0, 0);
    CU_add_test(suite, "test_hello", test_hello);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return 0;
}
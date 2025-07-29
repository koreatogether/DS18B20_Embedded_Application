#include <unity.h> // 기존 "../test/unit/unity.h" 대신 시스템 Unity 사용

void test_addition(void)
{
    TEST_ASSERT_EQUAL_INT(4, 2 + 2);
}

void setUp(void) {}
void tearDown(void) {}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_addition);
    return UNITY_END();
}

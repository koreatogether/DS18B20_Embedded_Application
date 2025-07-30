#include <unity.h>
#include "mocks/MockLongRunStabilityManager.h"
#include <string>

MockLongRunStabilityManager *stabilityManager;

void setUp(void)
{
    stabilityManager = new MockLongRunStabilityManager();
}

void tearDown(void)
{
    delete stabilityManager;
}

void test_long_run_stability_no_error_no_leak()
{
    // Given: 1시간(3600초) 동안 정상 동작, 메모리 안정적
    stabilityManager->start(2048, 16);
    for (int i = 0; i < 3600; ++i)
    {
        stabilityManager->simulateIteration(2048 - (i % 3)); // 약간의 변동만
    }
    stabilityManager->stop();
    LongRunStabilityResult result = stabilityManager->analyze();
    TEST_ASSERT_EQUAL_STRING("LongRunStabilityTest", result.testName.c_str());
    TEST_ASSERT_EQUAL(3600, result.totalIterations);
    TEST_ASSERT_EQUAL(0, result.errorCount);
    TEST_ASSERT_EQUAL(0, result.memoryLeakCount);
    TEST_ASSERT_TRUE(result.passed);
}

void test_long_run_stability_with_errors()
{
    // Given: 1시간 중 10회 에러 발생
    stabilityManager->start(2048, 16);
    for (int i = 1; i <= 3600; ++i)
    {
        bool error = (i % 360 == 0);
        stabilityManager->simulateIteration(2048 - (i % 3), error);
    }
    stabilityManager->stop();
    LongRunStabilityResult result = stabilityManager->analyze();
    TEST_ASSERT_EQUAL(10, result.errorCount);
    TEST_ASSERT_FALSE(result.passed);
}

void test_long_run_stability_with_leaks()
{
    // Given: 1시간 중 5회 메모리 누수(20바이트 이상 감소)
    stabilityManager->start(2048, 16);
    int mem = 2048;
    for (int i = 0; i < 3600; ++i)
    {
        if (i % 700 == 0 && i != 0)
            mem -= 20; // 누수 발생
        stabilityManager->simulateIteration(mem - (i % 2));
    }
    stabilityManager->stop();
    LongRunStabilityResult result = stabilityManager->analyze();
    TEST_ASSERT_EQUAL(5, result.memoryLeakCount);
    TEST_ASSERT_FALSE(result.passed);
}

void test_long_run_stability_short_run()
{
    // Given: 10분(600초)만 실행
    stabilityManager->start(2048, 16);
    for (int i = 0; i < 600; ++i)
    {
        stabilityManager->simulateIteration(2048 - (i % 2));
    }
    stabilityManager->stop();
    LongRunStabilityResult result = stabilityManager->analyze();
    TEST_ASSERT_EQUAL(600, result.totalIterations);
    TEST_ASSERT_TRUE(result.passed);
}

void test_long_run_stability_summary_format()
{
    // Given: 정상 시나리오
    stabilityManager->start(2048, 16);
    for (int i = 0; i < 100; ++i)
    {
        stabilityManager->simulateIteration(2048 - (i % 2));
    }
    stabilityManager->stop();
    LongRunStabilityResult result = stabilityManager->analyze();
    TEST_ASSERT_TRUE(result.summary.find("Duration:") != std::string::npos);
    TEST_ASSERT_TRUE(result.summary.find("Errors:") != std::string::npos);
    TEST_ASSERT_TRUE(result.summary.find("Leaks:") != std::string::npos);
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_long_run_stability_no_error_no_leak);
    RUN_TEST(test_long_run_stability_with_errors);
    RUN_TEST(test_long_run_stability_with_leaks);
    RUN_TEST(test_long_run_stability_short_run);
    RUN_TEST(test_long_run_stability_summary_format);
    return UNITY_END();
}

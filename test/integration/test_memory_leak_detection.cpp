#include <unity.h>
#include "../mocks/MockMemoryLeakDetector.h"
#include <cstring> // C++ 표준에 맞게 cstring 사용

// 테스트 전역 변수
MockMemoryLeakDetector *detector;

void setUp(void)
{
    detector = new MockMemoryLeakDetector();
}

void tearDown(void)
{
    delete detector;
}

/**
 * @brief 기본 메모리 누수 탐지 기능 테스트
 */
void test_basic_memory_leak_detection()
{
    // Given: 초기 메모리 상태 설정
    detector->setMockFreeMemory(2000);
    detector->startLeakDetection("BasicLeakTest");

    // When: 메모리가 점진적으로 감소하는 시나리오
    for (int i = 0; i < 10; i++)
    {
        detector->setMockFreeMemory(2000 - (i * 15)); // 15바이트씩 감소
        detector->recordTestIteration(i + 1, 2000 - (i * 15));
    }

    // Then: 메모리 누수가 탐지되어야 함
    MemoryLeakAnalysis analysis = detector->analyzeLeakPattern();

    TEST_ASSERT_TRUE(analysis.leakDetected);
    TEST_ASSERT_TRUE(analysis.suspiciousDecreaseCount >= 3);
    TEST_ASSERT_TRUE(analysis.maxMemoryDecrease >= 10);
    TEST_ASSERT_TRUE(analysis.averageMemoryTrend < 0);
    // null termination 보장 확인 (정적 분석기 오탐 방지)
    TEST_ASSERT_EQUAL_CHAR('\0', analysis.summary[sizeof(analysis.summary) - 1]);
    // null-termination 보장 후 안전하게 strnlen 사용
    TEST_ASSERT_GREATER_THAN(0, strnlen(analysis.summary, sizeof(analysis.summary)));
}

/**
 * @brief 정상적인 메모리 사용 패턴 테스트 (누수 없음)
 */
void test_normal_memory_usage_no_leak()
{
    // Given: 초기 메모리 상태 설정
    detector->setMockFreeMemory(2000);
    detector->startLeakDetection("NormalUsageTest");

    // When: 메모리가 안정적인 시나리오 (약간의 변동은 있지만 전체적으로 안정)
    int memoryValues[] = {2000, 1995, 2000, 1998, 2000, 1997, 2001, 1999, 2000, 2002};
    for (int i = 0; i < 10; i++)
    {
        detector->setMockFreeMemory(memoryValues[i]);
        detector->recordTestIteration(i + 1, memoryValues[i]);
    }

    // Then: 메모리 누수가 탐지되지 않아야 함
    MemoryLeakAnalysis analysis = detector->analyzeLeakPattern();

    TEST_ASSERT_FALSE(analysis.leakDetected);
    TEST_ASSERT_TRUE(analysis.maxMemoryDecrease <= 10);           // 작은 변동만 있음
    TEST_ASSERT_GREATER_THAN(-1.0f, analysis.averageMemoryTrend); // 심각한 감소 추세 없음
}

/**
 * @brief 메모리 스트레스 테스트 시뮬레이션
 */
void test_memory_stress_simulation()
{
    // Given & When: 스트레스 테스트 시뮬레이션 실행
    detector->simulateMemoryStress(20, 5); // 20회 반복, 회당 5바이트 감소

    // Then: 적절한 스냅샷이 기록되고 누수가 탐지되어야 함
    TEST_ASSERT_GREATER_THAN(10, detector->getSnapshotCount());

    MemoryLeakAnalysis analysis = detector->analyzeLeakPattern();
    TEST_ASSERT_TRUE(analysis.leakDetected);
    TEST_ASSERT_GREATER_THAN(50, analysis.maxMemoryDecrease); // 상당한 메모리 감소
}

/**
 * @brief 메모리 복구 시나리오 테스트
 */
void test_memory_recovery_scenario()
{
    // Given: 메모리 누수 상황 생성
    detector->simulateMemoryStress(10, 10); // 10회 반복, 회당 10바이트 감소

    // When: 메모리 복구 시뮬레이션
    detector->simulateMemoryRecovery(80); // 80바이트 복구

    // Then: 복구 스냅샷이 기록되어야 함
    TEST_ASSERT_GREATER_THAN(10, detector->getSnapshotCount());

    // 마지막 스냅샷이 복구를 반영해야 함
    MemorySnapshotFixed lastSnapshot = detector->getSnapshot(detector->getSnapshotCount() - 1);
    TEST_ASSERT_EQUAL_STRING("MEMORY_RECOVERY", lastSnapshot.eventType);
}

/**
 * @brief 임계값 기반 누수 탐지 테스트
 */
void test_threshold_based_leak_detection()
{
    // Given: 초기 상태 설정
    detector->setMockFreeMemory(2000);
    detector->startLeakDetection("ThresholdTest");

    // When: 작은 메모리 감소 (임계값 미만)
    for (int i = 0; i < 5; i++)
    {
        detector->setMockFreeMemory(2000 - (i * 2)); // 2바이트씩 소량 감소
        detector->recordTestIteration(i + 1, 2000 - (i * 2));
    }

    // Then: 임계값 미만이므로 누수 탐지되지 않아야 함
    bool leakDetected = detector->detectMemoryLeak(10, 3); // 10바이트 임계값, 3회 감소
    TEST_ASSERT_FALSE(leakDetected);

    // When: 큰 메모리 감소 (임계값 초과)
    for (int i = 5; i < 10; i++)
    {
        detector->setMockFreeMemory(2000 - (i * 15)); // 15바이트씩 대량 감소
        detector->recordTestIteration(i + 1, 2000 - (i * 15));
    }

    // Then: 임계값 초과로 누수 탐지되어야 함
    leakDetected = detector->detectMemoryLeak(10, 3);
    TEST_ASSERT_TRUE(leakDetected);
}

/**
 * @brief 스냅샷 히스토리 관리 테스트
 */
void test_snapshot_history_management()
{
    // Given: 최대 히스토리 크기 제한 설정
    detector->setMaxHistorySize(5);

    // When: 제한보다 많은 스냅샷 기록
    for (int i = 0; i < 10; i++)
    {
        detector->setMockFreeMemory(2000 - i);
        detector->recordCurrentMemory("TEST_SNAPSHOT", "Overflow test");
    }

    // Then: 최대 크기를 넘지 않아야 함
    TEST_ASSERT_EQUAL(5, detector->getSnapshotCount());

    // 가장 오래된 데이터는 제거되고 최신 데이터만 유지되어야 함
    MemorySnapshotFixed firstSnapshot = detector->getSnapshot(0);
    TEST_ASSERT_EQUAL(1995, firstSnapshot.freeBytes); // 마지막 5개 중 첫 번째
}

/**
 * @brief 리포트 생성 기능 테스트
 */
void test_leak_report_generation()
{
    // Given: 테스트 데이터 준비
    detector->simulateMemoryStress(5, 8);

    // When: 리포트 생성
    char reportBuffer[1024];
    detector->generateLeakReport(reportBuffer, sizeof(reportBuffer));

    // Then: 리포트에 필수 정보가 포함되어야 함
    TEST_ASSERT_TRUE(strstr(reportBuffer, "Memory Leak Detection Report") != NULL);
    TEST_ASSERT_TRUE(strstr(reportBuffer, "Memory_Stress_Test") != NULL);
    TEST_ASSERT_TRUE(strstr(reportBuffer, "Total Snapshots") != NULL);
    TEST_ASSERT_TRUE(strstr(reportBuffer, "Baseline Memory") != NULL);
}

/**
 * @brief 메모리 히스토리 클리어 기능 테스트
 */
void test_memory_history_clear()
{
    // Given: 여러 스냅샷 기록
    detector->simulateMemoryStress(5, 5);
    TEST_ASSERT_GREATER_THAN(0, detector->getSnapshotCount());

    // When: 히스토리 클리어
    detector->clearHistory();

    // Then: 모든 스냅샷이 제거되어야 함
    TEST_ASSERT_EQUAL(0, detector->getSnapshotCount());
}

/**
 * @brief Mock 시간 설정 기능 테스트
 */
void test_mock_time_functionality()
{
    // Given: Mock 시간 설정
    unsigned long testTime = 12345;
    detector->setMockCurrentTime(testTime);

    // When: 스냅샷 기록
    detector->recordCurrentMemory("TIME_TEST", "Mock time test");

    // Then: 설정된 시간이 스냅샷에 반영되어야 함
    MemorySnapshotFixed snapshot = detector->getSnapshot(0);
    TEST_ASSERT_EQUAL(testTime, snapshot.timestamp);
}

/**
 * @brief 엣지 케이스: 데이터 부족 시나리오 테스트
 */
void test_insufficient_data_scenario()
{
    // Given: 스냅샷 데이터가 거의 없는 상황
    detector->recordCurrentMemory("SINGLE_SNAPSHOT", "Only one data point");

    // When: 분석 시도
    MemoryLeakAnalysis analysis = detector->analyzeLeakPattern();

    // Then: 안전하게 처리되어야 함
    TEST_ASSERT_FALSE(analysis.leakDetected);
    TEST_ASSERT_TRUE(strstr(analysis.summary, "Insufficient data") != NULL);
    TEST_ASSERT_EQUAL(1, analysis.totalSnapshots);
}

// 메인 테스트 실행 함수
void runMemoryLeakDetectionTests()
{
    RUN_TEST(test_basic_memory_leak_detection);
    RUN_TEST(test_normal_memory_usage_no_leak);
    RUN_TEST(test_memory_stress_simulation);
    RUN_TEST(test_memory_recovery_scenario);
    RUN_TEST(test_threshold_based_leak_detection);
    RUN_TEST(test_snapshot_history_management);
    RUN_TEST(test_leak_report_generation);
    RUN_TEST(test_memory_history_clear);
    RUN_TEST(test_mock_time_functionality);
    RUN_TEST(test_insufficient_data_scenario);
}

#ifndef UNITY_INCLUDE_EXEC_TIME
// Unity 프레임워크 메인 함수 (독립 실행용)
int main(void)
{
    UNITY_BEGIN();
    runMemoryLeakDetectionTests();
    return UNITY_END();
}
#endif

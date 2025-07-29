#include <unity.h>
#include "mocks/MockMemoryMonitorService.h"
#include "mocks/MockMemoryTracker.h"
#include "mocks/MockHal.h"
#include <memory>
#include <vector>
#include <string>

// Test Objects for Memory Leak Detection
MockMemoryTracker *memoryTracker;
MockMemoryMonitorService *memoryService;
MockHal *mockHal;

// Memory leak detection constants
const int LEAK_DETECTION_ITERATIONS = 100;
const int MEMORY_TOLERANCE_BYTES = 50; // 허용 가능한 메모리 변동량 (바이트)

void setUp(void)
{
    // Mock 객체들 초기화
    mockHal = new MockHal();
    memoryTracker = new MockMemoryTracker();
    memoryService = new MockMemoryMonitorService(1000);

    // 초기 메모리 상태 설정
    mockHal->setFreeMemory(2048); // 2KB 초기 메모리
    memoryTracker->clearSnapshots();
}

void tearDown(void)
{
    delete memoryService;
    delete memoryTracker;
    delete mockHal;
}

// ===== Memory Leak Detection Tests =====

void test_memory_tracker_snapshot_creation_no_leak()
{
    // Given: 초기 메모리 상태
    uint16_t initialMemory = 2048;
    mockHal->setFreeMemory(initialMemory);

    // When: 스냅샷 생성을 여러 번 반복
    for (int i = 0; i < LEAK_DETECTION_ITERATIONS; i++)
    {
        memoryTracker->takeSnapshot("leak_test_" + std::to_string(i));
        // 메모리 상태는 변화 없음을 시뮬레이션
        mockHal->setFreeMemory(initialMemory);
    }

    // Then: 메모리 사용량이 증가하지 않아야 함
    std::vector<MemorySnapshot> snapshots = memoryTracker->getSnapshots();
    TEST_ASSERT_EQUAL(LEAK_DETECTION_ITERATIONS, snapshots.size());

    // 첫 번째와 마지막 스냅샷의 메모리 차이 확인
    uint16_t memoryDifference = snapshots[0].freeMemory - snapshots[snapshots.size() - 1].freeMemory;
    TEST_ASSERT_TRUE(memoryDifference <= MEMORY_TOLERANCE_BYTES);
}

void test_memory_tracker_detects_memory_leak()
{
    // Given: 초기 메모리 상태
    uint16_t initialMemory = 2048;
    uint16_t leakPerIteration = 10; // 매 반복마다 10바이트씩 누수
    mockHal->setFreeMemory(initialMemory);

    // When: 메모리 누수를 시뮬레이션하며 스냅샷 생성
    for (int i = 0; i < LEAK_DETECTION_ITERATIONS; i++)
    {
        memoryTracker->takeSnapshot("leak_simulation_" + std::to_string(i));
        // 메모리 누수 시뮬레이션: 매번 메모리가 감소
        uint16_t currentMemory = initialMemory - (leakPerIteration * (i + 1));
        mockHal->setFreeMemory(currentMemory);
    }

    // Then: 메모리 누수가 탐지되어야 함
    std::vector<MemorySnapshot> snapshots = memoryTracker->getSnapshots();
    TEST_ASSERT_EQUAL(LEAK_DETECTION_ITERATIONS, snapshots.size());

    // 첫 번째와 마지막 스냅샷의 메모리 차이 확인
    uint16_t totalLeak = snapshots[0].freeMemory - snapshots[snapshots.size() - 1].freeMemory;
    uint16_t expectedLeak = leakPerIteration * LEAK_DETECTION_ITERATIONS;

    TEST_ASSERT_EQUAL(expectedLeak, totalLeak);
    TEST_ASSERT_TRUE(totalLeak > MEMORY_TOLERANCE_BYTES);
}

void test_memory_service_repeated_operations_no_leak()
{
    // Given: 메모리 서비스 초기화
    uint16_t initialMemory = 2048;
    memoryService->setFreeMemoryBytes(initialMemory);

    // When: 동일한 메모리 연산을 반복 수행
    std::string operation_result;
    for (int i = 0; i < LEAK_DETECTION_ITERATIONS; i++)
    {
        operation_result = memoryService->getFreeMemory();
        operation_result = memoryService->getStructMemoryInfo();
        operation_result = memoryService->stressTestMemory();

        // 메모리 상태가 안정적임을 시뮬레이션
        memoryService->setFreeMemoryBytes(initialMemory);
    }

    // Then: 메모리 누수 없이 연산이 완료되어야 함
    uint16_t finalMemory = memoryService->getFreeMemoryBytes();
    TEST_ASSERT_EQUAL(initialMemory, finalMemory);
}

void test_memory_tracker_leak_threshold_detection()
{
    // Given: 메모리 누수 임계값 설정
    uint16_t initialMemory = 2048;
    uint16_t leakThreshold = 200; // 200바이트 임계값
    mockHal->setFreeMemory(initialMemory);

    // When: 임계값을 초과하는 메모리 누수 시뮬레이션
    for (int i = 0; i < 30; i++)
    {
        memoryTracker->takeSnapshot("threshold_test_" + std::to_string(i));
        uint16_t currentMemory = initialMemory - (i * 8); // 8바이트씩 감소
        mockHal->setFreeMemory(currentMemory);
    }

    // Then: 메모리 누수가 임계값을 초과했는지 확인
    std::vector<MemorySnapshot> snapshots = memoryTracker->getSnapshots();
    uint16_t totalLeak = snapshots[0].freeMemory - snapshots[snapshots.size() - 1].freeMemory;

    TEST_ASSERT_TRUE(totalLeak > leakThreshold);
    TEST_ASSERT_EQUAL(29 * 8, totalLeak); // 29번 반복 * 8바이트 = 232바이트 누수
}

void test_memory_pattern_analysis_for_leak_detection()
{
    // Given: 메모리 패턴 분석을 위한 데이터 수집
    uint16_t baseMemory = 2048;
    std::vector<uint16_t> memoryPattern;

    // When: 다양한 메모리 패턴 생성 (정상, 누수, 회복)
    for (int i = 0; i < 50; i++)
    {
        std::string snapshotName = "pattern_" + std::to_string(i);

        uint16_t currentMemory;
        if (i < 10)
        {
            // 정상 패턴: 메모리 안정
            currentMemory = baseMemory;
        }
        else if (i < 30)
        {
            // 누수 패턴: 메모리 감소
            currentMemory = baseMemory - ((i - 10) * 5);
        }
        else
        {
            // 회복 패턴: 메모리 회복
            currentMemory = baseMemory - 100 + ((i - 30) * 2);
        }

        mockHal->setFreeMemory(currentMemory);
        memoryTracker->takeSnapshot(snapshotName);
        memoryPattern.push_back(currentMemory);
    }

    // Then: 메모리 패턴 분석 결과 검증
    std::vector<MemorySnapshot> snapshots = memoryTracker->getSnapshots();
    TEST_ASSERT_EQUAL(50, snapshots.size());

    // 누수 구간 확인 (10~29번째 스냅샷)
    uint16_t leakStartMemory = snapshots[10].freeMemory;
    uint16_t leakEndMemory = snapshots[29].freeMemory;
    TEST_ASSERT_TRUE(leakStartMemory > leakEndMemory);

    // 회복 구간 확인 (30~49번째 스냅샷)
    uint16_t recoveryStartMemory = snapshots[30].freeMemory;
    uint16_t recoveryEndMemory = snapshots[49].freeMemory;
    TEST_ASSERT_TRUE(recoveryEndMemory > recoveryStartMemory);
}

void test_memory_leak_report_generation()
{
    // Given: 메모리 누수가 있는 상황 시뮬레이션
    uint16_t initialMemory = 2048;
    mockHal->setFreeMemory(initialMemory);

    // When: 누수를 포함한 스냅샷들 생성
    for (int i = 0; i < 20; i++)
    {
        memoryTracker->takeSnapshot("report_test_" + std::to_string(i));
        uint16_t currentMemory = initialMemory - (i * 15); // 15바이트씩 누수
        mockHal->setFreeMemory(currentMemory);
    }

    // 리포트 생성
    std::string report = memoryTracker->generateMemoryReport();

    // Then: 리포트에 누수 정보가 포함되어야 함
    TEST_ASSERT_TRUE(report.find("Memory Usage Report") != std::string::npos);
    TEST_ASSERT_TRUE(report.find("Snapshots: 20") != std::string::npos);

    // 총 누수량 확인 (19 * 15 = 285바이트)
    std::vector<MemorySnapshot> snapshots = memoryTracker->getSnapshots();
    uint16_t totalLeak = snapshots[0].freeMemory - snapshots[snapshots.size() - 1].freeMemory;
    TEST_ASSERT_EQUAL(285, totalLeak);
}

// ===== Memory Leak Detection Test Runner =====

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    // Memory Leak Detection Tests
    RUN_TEST(test_memory_tracker_snapshot_creation_no_leak);
    RUN_TEST(test_memory_tracker_detects_memory_leak);
    RUN_TEST(test_memory_service_repeated_operations_no_leak);
    RUN_TEST(test_memory_tracker_leak_threshold_detection);
    RUN_TEST(test_memory_pattern_analysis_for_leak_detection);
    RUN_TEST(test_memory_leak_report_generation);

    return UNITY_END();
}

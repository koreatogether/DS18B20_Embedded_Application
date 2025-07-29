#include <unity.h>
#include "mocks/MockMemoryMonitorService.h"
#include "mocks/MockSerialCommandHandler.h"
#include "mocks/MockMemoryTracker.h"
#include <memory>
#include <string>

// Test Objects
MockMemoryMonitorService *memoryService;
MockSerialCommandHandler *commandHandler;
std::shared_ptr<MockMemoryMonitorService> memoryServicePtr;

void setUp(void)
{
    // 각 테스트 전에 Mock 객체들을 새로 생성
    memoryService = new MockMemoryMonitorService(1000); // 1초 간격
    memoryServicePtr = std::shared_ptr<MockMemoryMonitorService>(memoryService);
    commandHandler = new MockSerialCommandHandler(memoryServicePtr);
}

void tearDown(void)
{
    // 각 테스트 후에 객체들을 정리
    delete commandHandler;
    // memoryService는 shared_ptr에 의해 자동으로 삭제됨
}

// ===== MemoryMonitorService Tests =====

void test_memory_service_initialization()
{
    // Given: MockMemoryMonitorService가 초기화됨

    // When: 초기화 직후 상태 확인

    // Then: 서비스가 정상적으로 생성되어야 함
    TEST_ASSERT_NOT_NULL(memoryService);
}

void test_get_free_memory_returns_csv_format()
{
    // Given: Mock이 1024 바이트의 free memory를 반환하도록 설정
    memoryService->setFreeMemoryBytes(1024);
    memoryService->setCurrentMillis(5000); // 시간 설정

    // When: getFreeMemory() 메서드 호출
    std::string result = memoryService->getFreeMemory();

    // Then: 타임스탬프가 포함된 CSV 형식으로 결과가 반환되어야 함
    TEST_ASSERT_EQUAL_STRING("5000,FREE_MEMORY,1024", result.c_str());
}

void test_get_structure_analysis_returns_markdown_format()
{
    // Given: Mock이 특정 메모리 구조 정보를 반환하도록 설정
    std::string expectedInfo = "Heap End: 0x200, Stack Pointer: 0x300";
    memoryService->setMemoryStructureInfo(expectedInfo);
    memoryService->setCurrentMillis(3000); // 시간 설정

    // When: getStructureAnalysis() 메서드 호출
    std::string result = memoryService->getStructureAnalysis();

    // Then: 타임스탬프가 포함된 Markdown 형식으로 결과가 반환되어야 함
    std::string expected = "| 3000 | MEMORY_STRUCTURE | " + expectedInfo + " |";
    TEST_ASSERT_EQUAL_STRING(expected.c_str(), result.c_str());
}

void test_toggle_monitoring_changes_status()
{
    // Given: 초기 상태에서 모니터링이 활성화되어 있음
    memoryService->setCurrentMillis(4000); // 시간 설정

    // When: toggleMonitoring()을 한 번 호출
    std::string result1 = memoryService->toggleMonitoring();

    // Then: 모니터링이 비활성화되어야 함 (타임스탬프 포함)
    TEST_ASSERT_EQUAL_STRING("| 4000 | MONITORING_STATUS | DISABLED |", result1.c_str());

    // When: toggleMonitoring()을 다시 호출
    std::string result2 = memoryService->toggleMonitoring();

    // Then: 모니터링이 다시 활성화되어야 함
    TEST_ASSERT_EQUAL_STRING("| 4000 | MONITORING_STATUS | ENABLED |", result2.c_str());
}

void test_periodic_check_respects_interval()
{
    // Given: Mock 시간을 0으로 설정하고 모니터링이 활성화된 상태
    memoryService->setCurrentMillis(0);
    memoryService->setFreeMemoryBytes(512);

    // When: 첫 번째 periodicCheck() 호출 (간격 미달)
    memoryService->periodicCheck();

    // Then: 출력이 발생하지 않아야 함 (아직 간격이 지나지 않음)
    TEST_ASSERT_EQUAL_size_t(0, memoryService->getPrintedMessages().size());

    // When: 시간을 1000ms(1초)로 변경하고 다시 호출
    memoryService->setCurrentMillis(1000);
    memoryService->periodicCheck();

    // Then: 이제 출력이 발생해야 함 (향상된 주기적 로그 포맷)
    TEST_ASSERT_EQUAL_size_t(1, memoryService->getPrintedMessages().size());
    std::string logMessage = memoryService->getPrintedMessages()[0];
    TEST_ASSERT_TRUE(logMessage.find("PERIODIC_CHECK") != std::string::npos);
    TEST_ASSERT_TRUE(logMessage.find("1000") != std::string::npos); // 타임스탬프
}

void test_periodic_check_when_monitoring_disabled()
{
    // Given: 모니터링을 비활성화
    memoryService->toggleMonitoring();     // ENABLED -> DISABLED
    memoryService->setCurrentMillis(2000); // 충분한 시간 경과
    memoryService->clearPrintedMessages();

    // When: periodicCheck() 호출
    memoryService->periodicCheck();

    // Then: 모니터링이 비활성화되어 있으므로 출력이 발생하지 않아야 함
    TEST_ASSERT_EQUAL_size_t(0, memoryService->getPrintedMessages().size());
}

void test_get_runtime_analysis_performs_stress_test()
{
    // Given: MemoryMonitorService 객체가 준비되고 Mock 설정
    memoryService->setCurrentMillis(6000);
    memoryService->setFreeMemoryBytes(2048); // 초기/최종 free memory

    // When: getRuntimeAnalysis() 메서드 호출
    std::string result = memoryService->getRuntimeAnalysis();

    // Then: 런타임 분석 결과가 CSV 형식으로 반환되어야 함
    // 결과에는 초기, 스트레스, 최종, 차이값이 포함되어야 함
    TEST_ASSERT_TRUE(result.find("RUNTIME_INITIAL") != std::string::npos);
    TEST_ASSERT_TRUE(result.find("RUNTIME_STRESS") != std::string::npos);
    TEST_ASSERT_TRUE(result.find("RUNTIME_FINAL") != std::string::npos);
    TEST_ASSERT_TRUE(result.find("RUNTIME_DIFF") != std::string::npos);
    TEST_ASSERT_TRUE(result.find("6000") != std::string::npos); // 타임스탬프
}

// ===== SerialCommandHandler Tests =====

void test_command_handler_initialization()
{
    // Given: MockSerialCommandHandler가 초기화됨

    // When: 초기화 직후 상태 확인

    // Then: 핸들러가 정상적으로 생성되어야 함
    TEST_ASSERT_NOT_NULL(commandHandler);
}

void test_help_command_returns_help_message()
{
    // Given: CommandHandler가 준비됨

    // When: "help" 명령어 처리
    std::string result = commandHandler->processCommand("help");

    // Then: 도움말 메시지가 반환되어야 함
    TEST_ASSERT_TRUE(result.find("Available commands:") != std::string::npos);
    TEST_ASSERT_TRUE(result.find("memory") != std::string::npos);
}

void test_memory_command_calls_memory_analyzer()
{
    // Given: 메모리 분석기가 특정 값을 반환하도록 설정
    memoryService->setFreeMemoryBytes(2048);
    memoryService->setCurrentMillis(7000); // 시간 설정

    // When: "memory" 명령어 처리
    std::string result = commandHandler->processCommand("memory");

    // Then: 메모리 분석기의 결과가 반환되어야 함 (타임스탬프 포함)
    TEST_ASSERT_EQUAL_STRING("7000,FREE_MEMORY,2048", result.c_str());
}

void test_unknown_command_returns_error_message()
{
    // Given: CommandHandler가 준비됨

    // When: 알 수 없는 명령어 처리
    std::string result = commandHandler->processCommand("unknown_command");

    // Then: 오류 메시지가 반환되어야 함
    TEST_ASSERT_TRUE(result.find("Unknown command:") != std::string::npos);
    TEST_ASSERT_TRUE(result.find("unknown_command") != std::string::npos);
}

void test_memory_toggle_command()
{
    // Given: CommandHandler가 준비됨
    memoryService->setCurrentMillis(8000); // 시간 설정

    // When: "memory toggle" 명령어 처리
    std::string result = commandHandler->processCommand("memory toggle");

    // Then: 모니터링 상태 변경 결과가 반환되어야 함 (타임스탬프 포함)
    TEST_ASSERT_EQUAL_STRING("| 8000 | MONITORING_STATUS | DISABLED |", result.c_str());
}

// ===== Memory Tracker Tests =====

void test_memory_tracker_interface()
{
    // Given: MockMemoryTracker 생성
    MockMemoryTracker tracker;

    // Initially no snapshots
    TEST_ASSERT_EQUAL(0, tracker.getSnapshotCount());

    // When: Record snapshot
    tracker.recordSnapshot(2000, 500, "TEST", "Test snapshot");

    // Then: Should have one snapshot
    TEST_ASSERT_EQUAL(1, tracker.getSnapshotCount());

    // When: Clear history
    tracker.clearHistory();

    // Then: Should have no snapshots
    TEST_ASSERT_EQUAL(0, tracker.getSnapshotCount());
}

void test_memory_tracker_report_generation()
{
    // Given: MockMemoryTracker with data
    MockMemoryTracker tracker;

    // When: Add multiple snapshots
    tracker.addMockSnapshot(1000, 2000, 500, "TEST1"); // Free: 2000, Used: 500
    tracker.addMockSnapshot(2000, 1800, 700, "TEST2"); // Free: 1800, Used: 700
    tracker.addMockSnapshot(3000, 1900, 600, "TEST3"); // Free: 1900, Used: 600

    MemoryReport report = tracker.generateReport();

    // Then: Report should contain correct data
    TEST_ASSERT_EQUAL(3, report.totalSnapshots);
    TEST_ASSERT_EQUAL(1800, report.minFreeBytes); // Minimum free memory
    TEST_ASSERT_EQUAL(2000, report.maxFreeBytes); // Maximum free memory
    TEST_ASSERT_EQUAL(1900, report.avgFreeBytes); // Average free memory (2000+1800+1900)/3 = 1900
}

void test_memory_tracker_csv_export()
{
    // Given: MockMemoryTracker with data
    MockMemoryTracker tracker;
    tracker.addMockSnapshot(1000, 2000, 500, "TEST1");
    tracker.addMockSnapshot(2000, 1800, 700, "TEST2");

    // When: Export to CSV
    std::string csv = tracker.exportToCsv();

    // Then: Should contain header and data
    TEST_ASSERT_TRUE(csv.find("Timestamp,FreeBytes,UsedBytes,EventType,Description") != std::string::npos);
    TEST_ASSERT_TRUE(csv.find("1000,2000,500,TEST1") != std::string::npos);
    TEST_ASSERT_TRUE(csv.find("2000,1800,700,TEST2") != std::string::npos);
}

void test_memory_tracker_markdown_export()
{
    // Given: MockMemoryTracker with data
    MockMemoryTracker tracker;
    tracker.addMockSnapshot(1000, 2000, 500, "TEST1");
    tracker.addMockSnapshot(2000, 1800, 700, "TEST2");

    // When: Export to Markdown
    std::string markdown = tracker.exportReportToMarkdown();

    // Then: Should contain markdown format
    TEST_ASSERT_TRUE(markdown.find("# Memory Tracking Report") != std::string::npos);
    TEST_ASSERT_TRUE(markdown.find("| Timestamp | Free Bytes") != std::string::npos);
    TEST_ASSERT_TRUE(markdown.find("| 1000 | 2000") != std::string::npos);
    TEST_ASSERT_TRUE(markdown.find("| 2000 | 1800") != std::string::npos);
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    // MemoryMonitorService Tests
    RUN_TEST(test_memory_service_initialization);
    RUN_TEST(test_get_free_memory_returns_csv_format);
    RUN_TEST(test_get_structure_analysis_returns_markdown_format);
    RUN_TEST(test_toggle_monitoring_changes_status);
    RUN_TEST(test_periodic_check_respects_interval);
    RUN_TEST(test_periodic_check_when_monitoring_disabled);
    RUN_TEST(test_get_runtime_analysis_performs_stress_test);

    // SerialCommandHandler Tests
    RUN_TEST(test_command_handler_initialization);
    RUN_TEST(test_help_command_returns_help_message);
    RUN_TEST(test_memory_command_calls_memory_analyzer);
    RUN_TEST(test_unknown_command_returns_error_message);
    RUN_TEST(test_memory_toggle_command);

    // Memory Tracker Tests
    RUN_TEST(test_memory_tracker_interface);
    RUN_TEST(test_memory_tracker_report_generation);
    RUN_TEST(test_memory_tracker_csv_export);
    RUN_TEST(test_memory_tracker_markdown_export);

    return UNITY_END();
}

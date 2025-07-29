#include <unity.h>
#include "mocks/MockMemoryMonitorService.h"
#include "mocks/MockSerialCommandHandler.h"
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

    // When: getFreeMemory() 메서드 호출
    std::string result = memoryService->getFreeMemory();

    // Then: CSV 형식으로 결과가 반환되어야 함
    TEST_ASSERT_EQUAL_STRING("type,FREE_MEMORY,value,1024", result.c_str());
}

void test_get_structure_analysis_returns_markdown_format()
{
    // Given: Mock이 특정 메모리 구조 정보를 반환하도록 설정
    std::string expectedInfo = "Heap End: 0x200, Stack Pointer: 0x300";
    memoryService->setMemoryStructureInfo(expectedInfo);

    // When: getStructureAnalysis() 메서드 호출
    std::string result = memoryService->getStructureAnalysis();

    // Then: Markdown 형식으로 결과가 반환되어야 함
    std::string expected = "| MEMORY_STRUCTURE | " + expectedInfo + " |";
    TEST_ASSERT_EQUAL_STRING(expected.c_str(), result.c_str());
}

void test_toggle_monitoring_changes_status()
{
    // Given: 초기 상태에서 모니터링이 활성화되어 있음

    // When: toggleMonitoring()을 한 번 호출
    std::string result1 = memoryService->toggleMonitoring();

    // Then: 모니터링이 비활성화되어야 함
    TEST_ASSERT_EQUAL_STRING("| MONITORING_STATUS | DISABLED |", result1.c_str());

    // When: toggleMonitoring()을 다시 호출
    std::string result2 = memoryService->toggleMonitoring();

    // Then: 모니터링이 다시 활성화되어야 함
    TEST_ASSERT_EQUAL_STRING("| MONITORING_STATUS | ENABLED |", result2.c_str());
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

    // Then: 이제 출력이 발생해야 함
    TEST_ASSERT_EQUAL_size_t(1, memoryService->getPrintedMessages().size());
    TEST_ASSERT_EQUAL_STRING("type,FREE_MEMORY,value,512",
                             memoryService->getPrintedMessages()[0].c_str());
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

void test_get_runtime_analysis_returns_not_implemented()
{
    // Given: MemoryMonitorService 객체가 준비됨

    // When: getRuntimeAnalysis() 메서드 호출
    std::string result = memoryService->getRuntimeAnalysis();

    // Then: "Not Implemented" 메시지가 Markdown 형식으로 반환되어야 함
    TEST_ASSERT_EQUAL_STRING("| RUNTIME_TEST | Not Implemented |", result.c_str());
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

    // When: "memory" 명령어 처리
    std::string result = commandHandler->processCommand("memory");

    // Then: 메모리 분석기의 결과가 반환되어야 함
    TEST_ASSERT_EQUAL_STRING("type,FREE_MEMORY,value,2048", result.c_str());
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

    // When: "memory toggle" 명령어 처리
    std::string result = commandHandler->processCommand("memory toggle");

    // Then: 모니터링 상태 변경 결과가 반환되어야 함
    TEST_ASSERT_EQUAL_STRING("| MONITORING_STATUS | DISABLED |", result.c_str());
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
    RUN_TEST(test_get_runtime_analysis_returns_not_implemented);

    // SerialCommandHandler Tests
    RUN_TEST(test_command_handler_initialization);
    RUN_TEST(test_help_command_returns_help_message);
    RUN_TEST(test_memory_command_calls_memory_analyzer);
    RUN_TEST(test_unknown_command_returns_error_message);
    RUN_TEST(test_memory_toggle_command);

    return UNITY_END();
}

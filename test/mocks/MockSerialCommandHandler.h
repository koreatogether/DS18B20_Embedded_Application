#ifndef MOCK_SERIAL_COMMAND_HANDLER_H
#define MOCK_SERIAL_COMMAND_HANDLER_H

#include "../../src/infrastructure/ICommandProcessor.h"
#include "MockMemoryMonitorService.h"
#include <string>
#include <memory>
#include <map>

/**
 * @class MockSerialCommandHandler
 * @brief ICommandProcessor 인터페이스의 완전한 Header-Only Mock 구현체
 * @details 실제 SerialCommandHandler의 모든 비즈니스 로직을 포함하되,
 *          의존성은 Mock으로 대체한 완전 독립적인 테스트용 구현
 */
class MockSerialCommandHandler : public ICommandProcessor
{
private:
    std::shared_ptr<MockMemoryMonitorService> _memoryAnalyzer;
    std::map<std::string, std::string> _responses; // 테스트용 응답 저장

public:
    MockSerialCommandHandler(std::shared_ptr<MockMemoryMonitorService> memoryAnalyzer)
        : _memoryAnalyzer(memoryAnalyzer) {}

    // ICommandProcessor 인터페이스 구현 (실제 비즈니스 로직과 동일)
    std::string processCommand(const std::string &command) override
    {
        std::string response;

        if (command == "help" || command == "menu")
        {
            response = getHelpMessage();
        }
        else if (command == "memory")
        {
            response = _memoryAnalyzer->getFreeMemory();
        }
        else if (command == "memory structure" || command == "mem struct")
        {
            response = _memoryAnalyzer->getStructureAnalysis();
        }
        else if (command == "memory runtime" || command == "mem runtime")
        {
            response = _memoryAnalyzer->getRuntimeAnalysis();
        }
        else if (command == "memory toggle" || command == "mem toggle")
        {
            response = _memoryAnalyzer->toggleMonitoring();
        }
        else if (command == "status")
        {
            response = "System Status: Running";
        }
        else
        {
            response = "Unknown command: " + command + ". Type 'help' for available commands.";
        }

        // 테스트 검증을 위해 응답 저장
        _responses[command] = response;
        return response;
    }

    // Test Helper Methods
    std::string getLastResponse(const std::string &command) const
    {
        auto it = _responses.find(command);
        return (it != _responses.end()) ? it->second : "";
    }

    void clearResponses() { _responses.clear(); }

private:
    std::string getHelpMessage()
    {
        return "Available commands:\n"
               "  help, menu          - Show this help message\n"
               "  memory              - Show current free memory\n"
               "  memory structure    - Show memory structure info\n"
               "  memory runtime      - Show runtime memory analysis\n"
               "  memory toggle       - Toggle memory monitoring\n"
               "  status              - Show system status";
    }
};

#endif // MOCK_SERIAL_COMMAND_HANDLER_H

#ifndef MOCK_SERIAL_COMMAND_HANDLER_H
#define MOCK_SERIAL_COMMAND_HANDLER_H

#include "../../src/infrastructure/ICommandProcessor.h"
#include "MockMemoryMonitorService.h"
#include <string>
#include <memory>
#include <map>
#include <functional>

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
        using Handler = std::function<std::string()>;
        static const std::map<std::string, Handler> handlers = {
            {"help", [this]()
             { return getHelpMessage(); }},
            {"menu", [this]()
             { return getHelpMessage(); }},
            {"memory", [this]()
             { return _memoryAnalyzer->getFreeMemory(); }},
            {"memory structure", [this]()
             { return _memoryAnalyzer->getStructureAnalysis(); }},
            {"mem struct", [this]()
             { return _memoryAnalyzer->getStructureAnalysis(); }},
            {"memory runtime", [this]()
             { return _memoryAnalyzer->getRuntimeAnalysis(); }},
            {"mem runtime", [this]()
             { return _memoryAnalyzer->getRuntimeAnalysis(); }},
            {"memory toggle", [this]()
             { return _memoryAnalyzer->toggleMonitoring(); }},
            {"mem toggle", [this]()
             { return _memoryAnalyzer->toggleMonitoring(); }},
            {"status", []()
             { return std::string("System Status: Running"); }}};
        std::string response;
        auto it = handlers.find(command);
        if (it != handlers.end())
        {
            response = it->second();
        }
        else
        {
            response = "Unknown command: " + command + ". Type 'help' for available commands.";
        }
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

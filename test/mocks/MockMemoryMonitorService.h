#ifndef MOCK_MEMORY_MONITOR_SERVICE_H
#define MOCK_MEMORY_MONITOR_SERVICE_H

#include "../../src/application/IMemoryAnalyzer.h"
#include <string>
#include <sstream>
#include <vector>

/**
 * @class MockMemoryMonitorService
 * @brief IMemoryAnalyzer 인터페이스의 완전한 Header-Only Mock 구현체
 * @details 실제 MemoryMonitorService의 모든 비즈니스 로직을 포함하되,
 *          하드웨어 의존성은 Mock으로 대체한 완전 독립적인 테스트용 구현
 */
class MockMemoryMonitorService : public IMemoryAnalyzer
{
private:
    // Mock 하드웨어 상태
    unsigned long _currentMillis;
    int _freeMemoryBytes;
    std::string _memoryStructureInfo;
    std::vector<std::string> _printedMessages;

    // 서비스 상태 (실제 구현과 동일)
    unsigned long _lastCheckTime;
    const unsigned long _interval;
    bool _isMonitoringEnabled;

public:
    MockMemoryMonitorService(unsigned long interval = 1000)
        : _currentMillis(0),
          _freeMemoryBytes(1024),
          _memoryStructureInfo("Heap End: 0x200, Stack Pointer: 0x300"),
          _lastCheckTime(0),
          _interval(interval),
          _isMonitoringEnabled(true) {}

    // Test Control Methods - 테스트에서 Mock 상태를 제어하기 위함
    void setCurrentMillis(unsigned long millis) { _currentMillis = millis; }
    void setFreeMemoryBytes(int bytes) { _freeMemoryBytes = bytes; }
    void setMemoryStructureInfo(const std::string &info) { _memoryStructureInfo = info; }
    const std::vector<std::string> &getPrintedMessages() const { return _printedMessages; }
    void clearPrintedMessages() { _printedMessages.clear(); }

    // IMemoryAnalyzer 인터페이스 구현 (실제 비즈니스 로직과 동일)
    void periodicCheck() override
    {
        if (_isMonitoringEnabled && (_currentMillis - _lastCheckTime >= _interval))
        {
            _lastCheckTime = _currentMillis;
            std::string message = getFreeMemory();
            _printedMessages.push_back(message);
        }
    }

    std::string getRuntimeAnalysis() override
    {
        return formatAsMarkdown("RUNTIME_TEST", "Not Implemented");
    }

    std::string getStructureAnalysis() override
    {
        return formatAsMarkdown("MEMORY_STRUCTURE", _memoryStructureInfo);
    }

    std::string getFreeMemory() override
    {
        return formatAsCsv("FREE_MEMORY", _freeMemoryBytes);
    }

    std::string toggleMonitoring() override
    {
        _isMonitoringEnabled = !_isMonitoringEnabled;
        std::string status = _isMonitoringEnabled ? "ENABLED" : "DISABLED";
        return formatAsMarkdown("MONITORING_STATUS", status);
    }

private:
    // 실제 구현과 동일한 헬퍼 메서드들
    std::string formatAsCsv(const std::string &type, int value)
    {
        std::ostringstream oss;
        oss << "type," << type << ",value," << value;
        return oss.str();
    }

    std::string formatAsMarkdown(const std::string &type, const std::string &value)
    {
        std::ostringstream oss;
        oss << "| " << type << " | " << value << " |";
        return oss.str();
    }
};

#endif // MOCK_MEMORY_MONITOR_SERVICE_H

#include "MemoryMonitorService.h"
#include <sstream>
#include <iomanip>

MemoryMonitorService::MemoryMonitorService(std::shared_ptr<IHal> hal, unsigned long interval)
    : _hal(hal), _lastCheckTime(0), _interval(interval), _isMonitoringEnabled(true)
{
    // 생성자에서 직접 Serial 출력을 하지 않고, 초기화 로직만 담당
}

void MemoryMonitorService::periodicCheck()
{
    if (_isMonitoringEnabled && (_hal->millis() - _lastCheckTime >= _interval))
    {
        _lastCheckTime = _hal->millis();
        _hal->print(getFreeMemory());
    }
}

std::string MemoryMonitorService::getRuntimeAnalysis()
{
    // 런타임/스트레스 테스트는 아직 구현되지 않음
    return formatAsMarkdown("RUNTIME_TEST", "Not Implemented");
}

std::string MemoryMonitorService::getStructureAnalysis()
{
    std::string info = _hal->getMemoryStructureInfo();
    return formatAsMarkdown("MEMORY_STRUCTURE", info);
}

std::string MemoryMonitorService::getFreeMemory()
{
    int freeBytes = _hal->getFreeMemoryBytes();
    return formatAsCsv("FREE_MEMORY", freeBytes);
}

std::string MemoryMonitorService::toggleMonitoring()
{
    _isMonitoringEnabled = !_isMonitoringEnabled;
    std::string status = _isMonitoringEnabled ? "ENABLED" : "DISABLED";
    return formatAsMarkdown("MONITORING_STATUS", status);
}

std::string MemoryMonitorService::formatAsCsv(const std::string &type, int value)
{
    std::ostringstream oss;
    oss << "type," << type << ",value," << value;
    return oss.str();
}

std::string MemoryMonitorService::formatAsMarkdown(const std::string &type, const std::string &value)
{
    std::ostringstream oss;
    oss << "| " << type << " | " << value << " |";
    return oss.str();
}

#include "MemoryMonitorService.h"
#include <sstream>
#include <iomanip>
#include <vector>

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

        // 주기적 메모리 모니터링 로그 생성
        std::string timestamp = std::to_string(_hal->millis());
        std::string freeMemLog = getFreeMemory();
        std::string structLog = getStructureAnalysis();

        // CSV 형식으로 타임스탬프와 함께 로그 출력
        std::string periodicLog = formatAsCsv("PERIODIC_CHECK", _hal->millis()) +
                                  "," + freeMemLog + "," + structLog;
        _hal->print(periodicLog);
    }
}

std::string MemoryMonitorService::getRuntimeAnalysis()
{
    // 런타임 메모리 분석: 동적 메모리 할당/해제 테스트
    int initialFree = _hal->getFreeMemoryBytes();

    // 스트레스 테스트: 임시 메모리 할당 후 해제
    std::vector<int> testData;
    testData.reserve(100); // 100개 int 예약

    for (int i = 0; i < 100; i++)
    {
        testData.push_back(i);
    }

    int stressFree = _hal->getFreeMemoryBytes();
    testData.clear(); // 메모리 해제

    int finalFree = _hal->getFreeMemoryBytes();

    // 결과를 CSV 형식으로 반환
    std::ostringstream result;
    result << formatAsCsv("RUNTIME_INITIAL", initialFree) << ","
           << formatAsCsv("RUNTIME_STRESS", stressFree) << ","
           << formatAsCsv("RUNTIME_FINAL", finalFree) << ","
           << formatAsCsv("RUNTIME_DIFF", finalFree - initialFree);

    return result.str();
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
    // CSV 헤더: timestamp,type,value
    oss << _hal->millis() << "," << type << "," << value;
    return oss.str();
}

std::string MemoryMonitorService::formatAsMarkdown(const std::string &type, const std::string &value)
{
    std::ostringstream oss;
    // Markdown 테이블 형식: | 시간 | 타입 | 값 |
    oss << "| " << _hal->millis() << " | " << type << " | " << value << " |";
    return oss.str();
}

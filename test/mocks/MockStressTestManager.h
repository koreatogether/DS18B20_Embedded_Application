#ifndef MOCK_STRESS_TEST_MANAGER_H
#define MOCK_STRESS_TEST_MANAGER_H

#include <string>
#include <vector>
#include <cstring>
#include <cstdio>

// 스트레스 테스트 결과 구조체
struct StressTestResult
{
    std::string testName;
    int durationMs;
    int operationsPerformed;
    int memoryUsedBytes;
    int peakMemoryUsage;
    int errorCount;
    bool testPassed;
    double operationsPerSecond;
    double memoryEfficiency;
    std::string performanceRating;

    StressTestResult() : testName(""), durationMs(0), operationsPerformed(0),
                         memoryUsedBytes(0), peakMemoryUsage(0), errorCount(0),
                         testPassed(false), operationsPerSecond(0.0),
                         memoryEfficiency(0.0), performanceRating("UNKNOWN") {}
};

// 메모리 압박 시나리오 구조체
struct MemoryStressScenario
{
    std::string scenarioName;
    int allocationSize;
    int allocationCount;
    int deallocateAfter;
    int expectedMemoryDrop;
    bool enableFragmentation;

    MemoryStressScenario() : scenarioName(""), allocationSize(0), allocationCount(0),
                             deallocateAfter(0), expectedMemoryDrop(0), enableFragmentation(false) {}
};

// CPU 부하 시나리오 구조체
struct CpuStressScenario
{
    std::string scenarioName;
    int cpuIntensiveLoops;
    int mathCalculations;
    int stringOperations;
    int expectedDurationMs;
    double targetCpuUsage;

    CpuStressScenario() : scenarioName(""), cpuIntensiveLoops(0), mathCalculations(0),
                          stringOperations(0), expectedDurationMs(0), targetCpuUsage(0.0) {}
};

// I/O 스트레스 시나리오 구조체
struct IoStressScenario
{
    std::string scenarioName;
    int serialWriteOperations;
    int serialReadOperations;
    int sensorReadOperations;
    int expectedLatencyMs;
    bool simulateErrors;

    IoStressScenario() : scenarioName(""), serialWriteOperations(0), serialReadOperations(0),
                         sensorReadOperations(0), expectedLatencyMs(0), simulateErrors(false) {}
};

class MockStressTestManager
{
private:
    std::vector<StressTestResult> testResults;
    int currentMemoryBytes;
    int initialMemoryBytes;
    int currentTimeMs;
    bool testingEnabled;

    // 고정 크기 배열로 시나리오 저장 (STL 없이)
    static const int MAX_SCENARIOS = 10;
    MemoryStressScenario memoryScenarios[MAX_SCENARIOS];
    CpuStressScenario cpuScenarios[MAX_SCENARIOS];
    IoStressScenario ioScenarios[MAX_SCENARIOS];
    int memoryScenarioCount;
    int cpuScenarioCount;
    int ioScenarioCount;

public:
    inline MockStressTestManager() : currentMemoryBytes(2048), initialMemoryBytes(2048),
                                     currentTimeMs(0), testingEnabled(true),
                                     memoryScenarioCount(0), cpuScenarioCount(0), ioScenarioCount(0)
    {

        // 기본 메모리 스트레스 시나리오 초기화
        addMemoryStressScenario("Small_Allocations", 32, 50, 25, 1600, false);
        addMemoryStressScenario("Large_Allocations", 256, 8, 4, 2048, false);
        addMemoryStressScenario("Fragmentation_Test", 64, 30, 15, 1920, true);

        // 기본 CPU 스트레스 시나리오 초기화
        addCpuStressScenario("Math_Intensive", 10000, 5000, 1000, 2000, 80.0);
        addCpuStressScenario("String_Processing", 5000, 2000, 8000, 3000, 70.0);
        addCpuStressScenario("Mixed_Operations", 7500, 3500, 4500, 2500, 85.0);

        // 기본 I/O 스트레스 시나리오 초기화
        addIoStressScenario("Serial_Flood", 1000, 500, 0, 5000, false);
        addIoStressScenario("Sensor_Burst", 0, 0, 200, 3000, false);
        addIoStressScenario("Error_Simulation", 300, 150, 50, 4000, true);
    }

    // 메모리 스트레스 시나리오 추가
    inline void addMemoryStressScenario(const std::string &name, int allocSize,
                                        int allocCount, int deallocAfter,
                                        int expectedDrop, bool fragmentation)
    {
        if (memoryScenarioCount < MAX_SCENARIOS)
        {
            MemoryStressScenario &scenario = memoryScenarios[memoryScenarioCount++];
            scenario.scenarioName = name;
            scenario.allocationSize = allocSize;
            scenario.allocationCount = allocCount;
            scenario.deallocateAfter = deallocAfter;
            scenario.expectedMemoryDrop = expectedDrop;
            scenario.enableFragmentation = fragmentation;
        }
    }

    // CPU 스트레스 시나리오 추가
    inline void addCpuStressScenario(const std::string &name, int loops,
                                     int mathCalcs, int stringOps,
                                     int expectedDuration, double targetCpu)
    {
        if (cpuScenarioCount < MAX_SCENARIOS)
        {
            CpuStressScenario &scenario = cpuScenarios[cpuScenarioCount++];
            scenario.scenarioName = name;
            scenario.cpuIntensiveLoops = loops;
            scenario.mathCalculations = mathCalcs;
            scenario.stringOperations = stringOps;
            scenario.expectedDurationMs = expectedDuration;
            scenario.targetCpuUsage = targetCpu;
        }
    }

    // I/O 스트레스 시나리오 추가
    inline void addIoStressScenario(const std::string &name, int writes,
                                    int reads, int sensorReads,
                                    int expectedLatency, bool errors)
    {
        if (ioScenarioCount < MAX_SCENARIOS)
        {
            IoStressScenario &scenario = ioScenarios[ioScenarioCount++];
            scenario.scenarioName = name;
            scenario.serialWriteOperations = writes;
            scenario.serialReadOperations = reads;
            scenario.sensorReadOperations = sensorReads;
            scenario.expectedLatencyMs = expectedLatency;
            scenario.simulateErrors = errors;
        }
    }

    // 메모리 스트레스 테스트 실행
    inline StressTestResult runMemoryStressTest(int scenarioIndex)
    {
        StressTestResult result;

        if (scenarioIndex < 0 || scenarioIndex >= memoryScenarioCount)
        {
            result.testName = "Invalid_Memory_Scenario";
            result.testPassed = false;
            return result;
        }

        const MemoryStressScenario &scenario = memoryScenarios[scenarioIndex];
        result.testName = "Memory_" + scenario.scenarioName;

        int startTime = currentTimeMs;
        int startMemory = currentMemoryBytes;
        int peakUsage = currentMemoryBytes;

        // 메모리 할당 시뮬레이션
        for (int i = 0; i < scenario.allocationCount; i++)
        {
            currentMemoryBytes -= scenario.allocationSize;
            if (currentMemoryBytes < peakUsage)
            {
                peakUsage = currentMemoryBytes;
            }

            // 부분적 해제 시뮬레이션
            if (i >= scenario.deallocateAfter && i % 2 == 0)
            {
                currentMemoryBytes += scenario.allocationSize / 2; // 일부만 복구
            }

            currentTimeMs += 10; // 각 할당마다 10ms 소요
        }

        // 단편화 시뮬레이션 (추가 메모리 손실)
        if (scenario.enableFragmentation)
        {
            currentMemoryBytes -= 100; // 단편화로 인한 추가 손실
        }

        result.durationMs = currentTimeMs - startTime;
        result.operationsPerformed = scenario.allocationCount;
        result.memoryUsedBytes = startMemory - currentMemoryBytes;
        result.peakMemoryUsage = startMemory - peakUsage;
        result.errorCount = (currentMemoryBytes < 100) ? 1 : 0; // 메모리 부족 시 에러

        // 성능 메트릭 계산
        result.operationsPerSecond = (result.durationMs > 0) ? (result.operationsPerformed * 1000.0) / result.durationMs : 0.0;
        result.memoryEfficiency = (result.memoryUsedBytes > 0) ? (double)result.operationsPerformed / result.memoryUsedBytes : 0.0;

        // 테스트 통과 조건 확인
        result.testPassed = (result.memoryUsedBytes <= scenario.expectedMemoryDrop) &&
                            (result.errorCount == 0);

        // 성능 등급 결정
        if (result.operationsPerSecond > 100)
        {
            result.performanceRating = "EXCELLENT";
        }
        else if (result.operationsPerSecond > 50)
        {
            result.performanceRating = "GOOD";
        }
        else if (result.operationsPerSecond > 20)
        {
            result.performanceRating = "FAIR";
        }
        else
        {
            result.performanceRating = "POOR";
        }

        testResults.push_back(result);
        return result;
    }

    // CPU 스트레스 테스트 실행
    inline StressTestResult runCpuStressTest(int scenarioIndex)
    {
        StressTestResult result;

        if (scenarioIndex < 0 || scenarioIndex >= cpuScenarioCount)
        {
            result.testName = "Invalid_CPU_Scenario";
            result.testPassed = false;
            return result;
        }

        const CpuStressScenario &scenario = cpuScenarios[scenarioIndex];
        result.testName = "CPU_" + scenario.scenarioName;

        int startTime = currentTimeMs;
        int operations = 0;

        // CPU 집약적 연산 시뮬레이션
        for (int i = 0; i < scenario.cpuIntensiveLoops; i++)
        {
            operations++;
            currentTimeMs += 1; // 각 루프마다 1ms
        }

        // 수학 계산 시뮬레이션
        for (int i = 0; i < scenario.mathCalculations; i++)
        {
            operations++;
            currentTimeMs += 2; // 수학 계산은 더 오래 걸림
        }

        // 문자열 처리 시뮬레이션
        for (int i = 0; i < scenario.stringOperations; i++)
        {
            operations++;
            currentTimeMs += 3; // 문자열 처리가 가장 오래 걸림
        }

        result.durationMs = currentTimeMs - startTime;
        result.operationsPerformed = operations;
        result.memoryUsedBytes = operations / 10; // CPU 연산도 약간의 메모리 사용
        result.peakMemoryUsage = result.memoryUsedBytes;
        result.errorCount = 0; // CPU 테스트에서는 일반적으로 에러 없음

        // 성능 메트릭 계산
        result.operationsPerSecond = (result.durationMs > 0) ? (result.operationsPerformed * 1000.0) / result.durationMs : 0.0;
        result.memoryEfficiency = (result.memoryUsedBytes > 0) ? (double)result.operationsPerformed / result.memoryUsedBytes : 100.0;

        // 테스트 통과 조건 (예상 시간 대비)
        int timeTolerance = scenario.expectedDurationMs * 20 / 100; // 20% 여유
        result.testPassed = (result.durationMs <= (scenario.expectedDurationMs + timeTolerance));

        // 성능 등급 결정 (처리량 기준)
        if (result.operationsPerSecond > 1000)
        {
            result.performanceRating = "EXCELLENT";
        }
        else if (result.operationsPerSecond > 500)
        {
            result.performanceRating = "GOOD";
        }
        else if (result.operationsPerSecond > 200)
        {
            result.performanceRating = "FAIR";
        }
        else
        {
            result.performanceRating = "POOR";
        }

        testResults.push_back(result);
        return result;
    }

    // I/O 스트레스 테스트 실행
    inline StressTestResult runIoStressTest(int scenarioIndex)
    {
        StressTestResult result;

        if (scenarioIndex < 0 || scenarioIndex >= ioScenarioCount)
        {
            result.testName = "Invalid_IO_Scenario";
            result.testPassed = false;
            return result;
        }

        const IoStressScenario &scenario = ioScenarios[scenarioIndex];
        result.testName = "IO_" + scenario.scenarioName;

        int startTime = currentTimeMs;
        int operations = 0;
        int errors = 0;

        // 시리얼 쓰기 시뮬레이션
        for (int i = 0; i < scenario.serialWriteOperations; i++)
        {
            operations++;
            currentTimeMs += 5; // 시리얼 쓰기는 5ms
            if (scenario.simulateErrors && i % 50 == 0)
            {
                errors++; // 50회마다 에러 발생
            }
        }

        // 시리얼 읽기 시뮬레이션
        for (int i = 0; i < scenario.serialReadOperations; i++)
        {
            operations++;
            currentTimeMs += 8; // 시리얼 읽기는 8ms
            if (scenario.simulateErrors && i % 30 == 0)
            {
                errors++; // 30회마다 에러 발생
            }
        }

        // 센서 읽기 시뮬레이션
        for (int i = 0; i < scenario.sensorReadOperations; i++)
        {
            operations++;
            currentTimeMs += 15; // 센서 읽기는 15ms (DS18B20은 느림)
            if (scenario.simulateErrors && i % 20 == 0)
            {
                errors++; // 20회마다 에러 발생
            }
        }

        result.durationMs = currentTimeMs - startTime;
        result.operationsPerformed = operations;
        result.memoryUsedBytes = operations / 5; // I/O 연산은 약간의 버퍼 메모리 사용
        result.peakMemoryUsage = result.memoryUsedBytes;
        result.errorCount = errors;

        // 성능 메트릭 계산
        result.operationsPerSecond = (result.durationMs > 0) ? (result.operationsPerformed * 1000.0) / result.durationMs : 0.0;
        result.memoryEfficiency = (result.memoryUsedBytes > 0) ? (double)result.operationsPerformed / result.memoryUsedBytes : 100.0;

        // 테스트 통과 조건 (예상 지연시간 및 에러율)
        int latencyTolerance = scenario.expectedLatencyMs * 30 / 100; // 30% 여유
        double errorRate = (operations > 0) ? (double)errors / operations : 0.0;
        result.testPassed = (result.durationMs <= (scenario.expectedLatencyMs + latencyTolerance)) &&
                            (errorRate <= 0.1); // 에러율 10% 이하

        // 성능 등급 결정 (처리량 및 에러율 기준)
        if (result.operationsPerSecond > 100 && errorRate < 0.02)
        {
            result.performanceRating = "EXCELLENT";
        }
        else if (result.operationsPerSecond > 50 && errorRate < 0.05)
        {
            result.performanceRating = "GOOD";
        }
        else if (result.operationsPerSecond > 20 && errorRate < 0.10)
        {
            result.performanceRating = "FAIR";
        }
        else
        {
            result.performanceRating = "POOR";
        }

        testResults.push_back(result);
        return result;
    }

    // 종합 스트레스 테스트 실행
    inline std::string runComprehensiveStressTest()
    {
        if (!testingEnabled)
        {
            return "Stress testing is disabled";
        }

        testResults.clear();
        currentMemoryBytes = initialMemoryBytes;
        currentTimeMs = 0;

        // 모든 메모리 시나리오 실행
        for (int i = 0; i < memoryScenarioCount; i++)
        {
            runMemoryStressTest(i);
        }

        // 모든 CPU 시나리오 실행
        for (int i = 0; i < cpuScenarioCount; i++)
        {
            runCpuStressTest(i);
        }

        // 모든 I/O 시나리오 실행
        for (int i = 0; i < ioScenarioCount; i++)
        {
            runIoStressTest(i);
        }

        return generateComprehensiveReport();
    }

    // 종합 리포트 생성
    inline std::string generateComprehensiveReport()
    {
        std::string report = "=== COMPREHENSIVE STRESS TEST REPORT ===\n";

        int totalTests = testResults.size();
        int passedTests = 0;
        int totalOperations = 0;
        int totalMemoryUsed = 0;
        int totalErrors = 0;
        double totalDuration = 0;

        // 통계 계산
        for (const auto &result : testResults)
        {
            if (result.testPassed)
                passedTests++;
            totalOperations += result.operationsPerformed;
            totalMemoryUsed += result.memoryUsedBytes;
            totalErrors += result.errorCount;
            totalDuration += result.durationMs;
        }

        // 종합 통계
        char buffer[1024];
        snprintf(buffer, sizeof(buffer),
                 "Summary:\n"
                 "- Total Tests: %d\n"
                 "- Passed: %d (%.1f%%)\n"
                 "- Failed: %d\n"
                 "- Total Operations: %d\n"
                 "- Total Memory Used: %d bytes\n"
                 "- Total Errors: %d\n"
                 "- Total Duration: %.0f ms\n"
                 "- Overall Ops/sec: %.2f\n\n",
                 totalTests, passedTests,
                 (totalTests > 0) ? (passedTests * 100.0 / totalTests) : 0.0,
                 totalTests - passedTests, totalOperations, totalMemoryUsed,
                 totalErrors, totalDuration,
                 (totalDuration > 0) ? (totalOperations * 1000.0 / totalDuration) : 0.0);

        report += buffer;

        // 개별 테스트 결과
        report += "Individual Test Results:\n";
        for (const auto &result : testResults)
        {
            snprintf(buffer, sizeof(buffer),
                     "- %s: %s (%.0f ops/sec, %s)\n",
                     result.testName.c_str(),
                     result.testPassed ? "PASS" : "FAIL",
                     result.operationsPerSecond,
                     result.performanceRating.c_str());
            report += buffer;
        }

        // 시스템 건강성 평가
        report += "\nSystem Health Assessment:\n";
        double passRate = (totalTests > 0) ? (passedTests * 100.0 / totalTests) : 0.0;
        double errorRate = (totalOperations > 0) ? (totalErrors * 100.0 / totalOperations) : 0.0;

        if (passRate >= 90 && errorRate <= 2)
        {
            report += "Status: EXCELLENT - System handles stress very well\n";
        }
        else if (passRate >= 75 && errorRate <= 5)
        {
            report += "Status: GOOD - System performs adequately under stress\n";
        }
        else if (passRate >= 50 && errorRate <= 10)
        {
            report += "Status: FAIR - System shows some stress symptoms\n";
        }
        else
        {
            report += "Status: POOR - System struggles under stress\n";
        }

        return report;
    }

    // 특정 부하 수준에서 시스템 안정성 테스트
    inline StressTestResult runSystemStabilityTest(int loadLevel)
    {
        StressTestResult result;
        result.testName = "System_Stability_Load_" + std::to_string(loadLevel);

        int startTime = currentTimeMs;
        int operations = 0;
        int errors = 0;

        // 부하 수준에 따른 연산 수 결정
        int baseOperations = 1000;
        int totalOperations = baseOperations * loadLevel;

        for (int i = 0; i < totalOperations; i++)
        {
            operations++;

            // 메모리 압박 시뮬레이션
            if (i % 100 == 0)
            {
                currentMemoryBytes -= 10;
                if (currentMemoryBytes < 100)
                {
                    errors++;
                    currentMemoryBytes = 100; // 최소 메모리 유지
                }
            }

            // CPU 부하 시뮬레이션
            currentTimeMs += (loadLevel > 5) ? 2 : 1;

            // 간헐적 메모리 복구
            if (i % 500 == 0)
            {
                currentMemoryBytes += 50; // 일부 메모리 복구
            }
        }

        result.durationMs = currentTimeMs - startTime;
        result.operationsPerformed = operations;
        result.memoryUsedBytes = initialMemoryBytes - currentMemoryBytes;
        result.peakMemoryUsage = result.memoryUsedBytes;
        result.errorCount = errors;

        // 성능 메트릭
        result.operationsPerSecond = (result.durationMs > 0) ? (operations * 1000.0) / result.durationMs : 0.0;
        result.memoryEfficiency = (result.memoryUsedBytes > 0) ? (double)operations / result.memoryUsedBytes : 100.0;

        // 안정성 평가
        double errorRate = (operations > 0) ? (double)errors / operations : 0.0;
        result.testPassed = (errorRate <= 0.05) && (currentMemoryBytes > 200);

        if (errorRate < 0.01 && result.operationsPerSecond > 100)
        {
            result.performanceRating = "EXCELLENT";
        }
        else if (errorRate < 0.03 && result.operationsPerSecond > 50)
        {
            result.performanceRating = "GOOD";
        }
        else if (errorRate < 0.05 && result.operationsPerSecond > 20)
        {
            result.performanceRating = "FAIR";
        }
        else
        {
            result.performanceRating = "POOR";
        }

        testResults.push_back(result);
        return result;
    }

    // 테스트 설정 및 상태 관리
    inline void setCurrentMemory(int bytes) { currentMemoryBytes = bytes; }
    inline void setCurrentTime(int timeMs) { currentTimeMs = timeMs; }
    inline void enableTesting(bool enabled) { testingEnabled = enabled; }
    inline void resetToInitialState()
    {
        currentMemoryBytes = initialMemoryBytes;
        currentTimeMs = 0;
        testResults.clear();
    }

    // 상태 조회
    inline int getCurrentMemory() const { return currentMemoryBytes; }
    inline int getCurrentTime() const { return currentTimeMs; }
    inline bool isTestingEnabled() const { return testingEnabled; }
    inline int getTestResultCount() const { return testResults.size(); }
    inline const std::vector<StressTestResult> &getTestResults() const { return testResults; }

    // 시나리오 개수 조회
    inline int getMemoryScenarioCount() const { return memoryScenarioCount; }
    inline int getCpuScenarioCount() const { return cpuScenarioCount; }
    inline int getIoScenarioCount() const { return ioScenarioCount; }
};

#endif // MOCK_STRESS_TEST_MANAGER_H

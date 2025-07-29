#include "SerialCommandHandler.h"
#include <sstream>

SerialCommandHandler::SerialCommandHandler(std::shared_ptr<IMemoryAnalyzer> memoryAnalyzer)
    : _memoryAnalyzer(memoryAnalyzer) {}

SerialCommandHandler::~SerialCommandHandler() {}

std::string SerialCommandHandler::processCommand(const std::string &command)
{
    if (command == "help" || command == "menu")
    {
        return handleHelp();
    }
    else if (command == "memtest")
    {
        return _memoryAnalyzer->getRuntimeAnalysis();
    }
    else if (command == "memstress")
    {
        // memstress는 getRuntimeAnalysis와 동일한 기능을 호출하도록 임시 연결
        return _memoryAnalyzer->getRuntimeAnalysis();
    }
    else if (command == "memstruct")
    {
        return _memoryAnalyzer->getStructureAnalysis();
    }
    else if (command == "memfree")
    {
        return _memoryAnalyzer->getFreeMemory();
    }
    else if (command == "memtoggle")
    {
        return _memoryAnalyzer->toggleMonitoring();
    }
    else
    {
        return "Unknown command: " + command;
    }
}

std::string SerialCommandHandler::handleHelp()
{
    std::ostringstream oss;
    oss << "--- Command Menu ---\n";
    oss << "memtest   : Run memory runtime analysis.\n";
    oss << "memstress : Run memory stress test (same as memtest).\n";
    oss << "memstruct : Show memory structure analysis.\n";
    oss << "memfree   : Show current free memory.\n";
    oss << "memtoggle : Toggle periodic memory monitoring.\n";
    oss << "help/menu : Show this menu.\n";
    oss << "--------------------";
    return oss.str();
}

std::string SerialCommandHandler::handleMemTest()
{
    return _memoryAnalyzer->getRuntimeAnalysis();
}

std::string SerialCommandHandler::handleMemStress()
{
    return _memoryAnalyzer->getRuntimeAnalysis();
}

std::string SerialCommandHandler::handleMemStruct()
{
    return _memoryAnalyzer->getStructureAnalysis();
}

std::string SerialCommandHandler::handleMemFree()
{
    return _memoryAnalyzer->getFreeMemory();
}

std::string SerialCommandHandler::handleMemToggle()
{
    return _memoryAnalyzer->toggleMonitoring();
}

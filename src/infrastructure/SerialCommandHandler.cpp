#include "SerialCommandHandler.h"
#include <sstream>

SerialCommandHandler::SerialCommandHandler() {}
SerialCommandHandler::~SerialCommandHandler() {}

std::string SerialCommandHandler::processCommand(const std::string &command)
{
    if (command == "help" || command == "menu")
    {
        return handleHelp();
    }
    else if (command == "memtest")
    {
        return handleMemTest();
    }
    else if (command == "memstress")
    {
        return handleMemStress();
    }
    else if (command == "memstruct")
    {
        return handleMemStruct();
    }
    else if (command == "memfree")
    {
        return handleMemFree();
    }
    else if (command == "memtoggle")
    {
        return handleMemToggle();
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
    oss << "memtest   : Run memory stress test.\n";
    oss << "memstress : (Not Implemented) Run memory stress test.\n";
    oss << "memstruct : (Not Implemented) Show memory structure.\n";
    oss << "memfree   : (Not Implemented) Show free memory.\n";
    oss << "memtoggle : (Not Implemented) Toggle memory monitoring.\n";
    oss << "help/menu : Show this menu.\n";
    oss << "--------------------";
    return oss.str();
}

std::string SerialCommandHandler::handleMemTest()
{
    // TODO: 실제 메모리 테스트 로직 연동
    return "[OK] memtest command received. (Not implemented)";
}

std::string SerialCommandHandler::handleMemStress()
{
    // TODO: 실제 메모리 스트레스 테스트 로직 연동
    return "[OK] memstress command received. (Not implemented)";
}

std::string SerialCommandHandler::handleMemStruct()
{
    // TODO: 실제 메모리 구조 분석 로직 연동
    return "[OK] memstruct command received. (Not implemented)";
}

std::string SerialCommandHandler::handleMemFree()
{
    // TODO: 실제 가용 메모리 출력 로직 연동
    return "[OK] memfree command received. (Not implemented)";
}

std::string SerialCommandHandler::handleMemToggle()
{
    // TODO: 실제 메모리 모니터링 토글 로직 연동
    return "[OK] memtoggle command received. (Not implemented)";
}

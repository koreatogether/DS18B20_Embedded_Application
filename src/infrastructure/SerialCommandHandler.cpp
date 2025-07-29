#include "SerialCommandHandler.h"
#include <sstream>

SerialCommandHandler::SerialCommandHandler() {}
SerialCommandHandler::~SerialCommandHandler() {}

std::string SerialCommandHandler::processCommand(const std::string &command)
{
    if (command == "memtest")
    {
        return handleMemTest();
    }
    else if (command == "help")
    {
        return handleHelp();
    }
    else
    {
        return "Unknown command: " + command;
    }
}

std::string SerialCommandHandler::handleMemTest()
{
    // TODO: 실제 메모리 테스트 로직 연동
    return "[memtest] Memory test executed.";
}

std::string SerialCommandHandler::handleHelp()
{
    std::ostringstream oss;
    oss << "Available commands:\n";
    oss << "  memtest   - Run memory test\n";
    oss << "  help      - Show this help message\n";
    // ... 기타 명령어 설명 추가
    return oss.str();
}

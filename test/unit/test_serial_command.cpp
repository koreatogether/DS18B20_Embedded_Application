#include "infrastructure/SerialCommandHandler.h"
#include <string>
#include <unity.h>

// 테스트할 SerialCommandHandler 인스턴스
SerialCommandHandler commandHandler;

void setUp(void)
{
    // 각 테스트 전에 실행되는 설정 (필요 시 사용)
}

void tearDown(void)
{
    // 각 테스트 후에 실행되는 정리 (필요 시 사용)
}

void test_command_help()
{
    const char *expected = "--- Command Menu ---\n"
                           "memtest   : Run memory stress test.\n"
                           "memstress : (Not Implemented) Run memory stress test.\n"
                           "memstruct : (Not Implemented) Show memory structure.\n"
                           "memfree   : (Not Implemented) Show free memory.\n"
                           "memtoggle : (Not Implemented) Toggle memory monitoring.\n"
                           "help/menu : Show this menu.\n"
                           "--------------------";
    std::string response = commandHandler.processCommand("help");
    TEST_ASSERT_EQUAL_STRING(expected, response.c_str());

    response = commandHandler.processCommand("menu");
    TEST_ASSERT_EQUAL_STRING(expected, response.c_str());
}

void test_command_memtest()
{
    std::string response = commandHandler.processCommand("memtest");
    TEST_ASSERT_EQUAL_STRING("[OK] memtest command received. (Not implemented)", response.c_str());
}

void test_command_memstress()
{
    std::string response = commandHandler.processCommand("memstress");
    TEST_ASSERT_EQUAL_STRING("[OK] memstress command received. (Not implemented)", response.c_str());
}

void test_command_memstruct()
{
    std::string response = commandHandler.processCommand("memstruct");
    TEST_ASSERT_EQUAL_STRING("[OK] memstruct command received. (Not implemented)", response.c_str());
}

void test_command_memfree()
{
    std::string response = commandHandler.processCommand("memfree");
    TEST_ASSERT_EQUAL_STRING("[OK] memfree command received. (Not implemented)", response.c_str());
}

void test_command_memtoggle()
{
    std::string response = commandHandler.processCommand("memtoggle");
    TEST_ASSERT_EQUAL_STRING("[OK] memtoggle command received. (Not implemented)", response.c_str());
}

void test_command_unknown()
{
    std::string command = "unknown_command";
    std::string response = commandHandler.processCommand(command);
    std::string expected = "Unknown command: " + command;
    TEST_ASSERT_EQUAL_STRING(expected.c_str(), response.c_str());
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_command_help);
    RUN_TEST(test_command_memtest);
    RUN_TEST(test_command_memstress);
    RUN_TEST(test_command_memstruct);
    RUN_TEST(test_command_memfree);
    RUN_TEST(test_command_memtoggle);
    RUN_TEST(test_command_unknown);
    return UNITY_END();
}

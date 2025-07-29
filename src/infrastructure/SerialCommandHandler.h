#ifndef SERIALCOMMANDHANDLER_H
#define SERIALCOMMANDHANDLER_H

#include "ICommandProcessor.h"
#include <string>

/**
 * @brief 시리얼 명령어 처리 구현체 (Infrastructure Layer)
 *        - ICommandProcessor 인터페이스 구현
 *        - 시리얼 입력 명령어 파싱 및 응답 처리
 */
class SerialCommandHandler : public ICommandProcessor
{
public:
    SerialCommandHandler();
    virtual ~SerialCommandHandler();

    /**
     * @brief 명령어 문자열을 파싱 및 실행
     * @param command 입력 명령어(예: "memtest", "help", ...)
     * @return 처리 결과 메시지(응답 문자열)
     */
    std::string processCommand(const std::string &command) override;

private:
    // 명령어별 처리 메서드 예시
    std::string handleMemTest();
    std::string handleHelp();
    // ... 기타 명령어 핸들러 추가
};

#endif // SERIALCOMMANDHANDLER_H

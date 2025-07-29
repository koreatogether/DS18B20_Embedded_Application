#include <Arduino.h>
#include "infrastructure/ICommandProcessor.h"
#include "infrastructure/SerialCommandHandler.h"
#include <memory>

// ICommandProcessor 인터페이스를 가리키는 스마트 포인터 선언
std::unique_ptr<ICommandProcessor> commandProcessor;

// put function declarations here:
int myFunction(int, int);

void setup()
{
  // 시리얼 통신을 115200 속도로 초기화
  Serial.begin(115200);
  // 시리얼 포트가 연결될 때까지 대기
  while (!Serial)
    ;

  // SerialCommandHandler 객체를 생성하고 스마트 포인터에 할당
  commandProcessor = std::make_unique<SerialCommandHandler>();

  Serial.println("\n========================================");
  Serial.println(" DS18B20 Embedded System Initialized");
  Serial.println("========================================");
  Serial.println("Type 'help' or 'menu' for a list of commands.");

  // put your setup code here, to run once:
  int result = myFunction(2, 3);
}

void loop()
{
  // 시리얼 버퍼에 수신된 데이터가 있는지 확인
  if (Serial.available() > 0)
  {
    // 개행 문자('\n')를 만날 때까지 문자열을 읽어옴
    String commandStr = Serial.readStringUntil('\n');

    // 앞뒤 공백 제거
    commandStr.trim();

    // 읽어온 명령어가 비어있지 않다면 처리
    if (commandStr.length() > 0)
    {
      Serial.print("> ");
      Serial.println(commandStr);

      // Arduino String을 std::string으로 변환하여 명령어 처리기에 전달
      std::string response = commandProcessor->processCommand(commandStr.c_str());

      // 처리 결과를 시리얼 모니터에 출력
      Serial.println(response.c_str());
    }
  }
}

// put function definitions here:
int myFunction(int x, int y)
{
  return x + y;
}

#include <gtest/gtest.h>
#include "TemperatureSensorManager.h"

// 1. ITemperatureSensor 인터페이스 정의 (별도 테스트 필요시 확장)

// 2. TemperatureSensorManager 클래스 생성/기본값
TEST(Domain_TemperatureSensorManager, Construction_Defaults)
{
    TemperatureSensorManager mgr;
    EXPECT_EQ(mgr.getSensorCount(), 0u) << "초기 센서 개수는 0이어야 함";
}

// 3. 센서 자동 탐지 및 주소 관리 로직
TEST(Domain_TemperatureSensorManager, SensorAddress_NormalAndError)
{
    TemperatureSensorManager mgr;
    std::vector<std::vector<uint8_t>> addrs = {
        {0x28,0xFF,0x1C,0x60,0x91,0x16,0x03,0x5C}
    };
    mgr._test_setSensorAddresses(addrs);
    uint8_t addr[8] = {0};
    // 정상 케이스: 주소 반환
    EXPECT_TRUE(mgr.getSensorAddress(0, addr));
    for (size_t i = 0; i < 8; ++i) EXPECT_EQ(addr[i], addrs[0][i]);
    // 예외 케이스: 인덱스 초과
    EXPECT_FALSE(mgr.getSensorAddress(1, addr));
}

TEST(Domain_TemperatureSensorManager, SensorCount_Normal)
{
    TemperatureSensorManager mgr;
    EXPECT_EQ(mgr.getSensorCount(), 0u);
    mgr._test_setSensorAddresses({{1,2,3,4,5,6,7,8},{9,10,11,12,13,14,15,16}});
    EXPECT_EQ(mgr.getSensorCount(), 2u);
}

// 4. 센서별 온도 측정/수집/갱신 메서드
TEST(Domain_TemperatureSensorManager, ReadTemperature_ErrorCase)
{
    TemperatureSensorManager mgr;
    mgr._test_setSensorAddresses({{1,2,3,4,5,6,7,8}});
    // 현재는 항상 -127.0 반환 (예외 케이스)
    EXPECT_FLOAT_EQ(mgr.readTemperature(0), -127.0f);
    EXPECT_FLOAT_EQ(mgr.readTemperature(1), -127.0f);
}

// 5. 센서별 사용자 데이터(ID) 저장/조회/중복 체크
TEST(Domain_TemperatureSensorManager, SensorId_NormalAndError)
{
    TemperatureSensorManager mgr;
    // 2개 센서 주소/ID 강제 주입
    std::vector<std::vector<uint8_t>> addrs = {
        {0x28,0xFF,0x1C,0x60,0x91,0x16,0x03,0x5C},
        {0x28,0xFF,0x2D,0xA0,0x91,0x16,0x04,0x6D}
    };
    mgr._test_setSensorAddresses(addrs);
    mgr._test_setSensorIds({"", ""});

    // 정상 케이스: ID 설정/조회
    EXPECT_TRUE(mgr.setSensorId(0, "A"));
    EXPECT_TRUE(mgr.setSensorId(1, "B"));
    EXPECT_EQ(mgr.getSensorId(0), "A");
    EXPECT_EQ(mgr.getSensorId(1), "B");

    // 예외 케이스: 인덱스 범위 초과
    EXPECT_FALSE(mgr.setSensorId(2, "C"));
    EXPECT_EQ(mgr.getSensorId(2), "");

    // 정상/예외 케이스: ID 중복 체크
    EXPECT_FALSE(mgr.isIdDuplicated("C"));
    EXPECT_TRUE(mgr.isIdDuplicated("A"));
    EXPECT_TRUE(mgr.isIdDuplicated("B"));
}

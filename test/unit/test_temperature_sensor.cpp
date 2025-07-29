#include <gtest/gtest.h>
#include "TemperatureSensorManager.h"

TEST(TemperatureSensorManagerTest, ConstructionAndDefaults)
{
    TemperatureSensorManager mgr;
    EXPECT_EQ(mgr.getSensorCount(), 0u);
}

TEST(TemperatureSensorManagerTest, SetAndGetSensorId)
{
    TemperatureSensorManager mgr;
    // 임의로 내부 벡터 크기 조정 (실제 discoverSensors 이후 동작 가정)
    // 실제 구현에서는 discoverSensors()가 벡터를 채움
    // 여기서는 테스트 목적으로 private 멤버 접근이 필요하므로 friend 선언 또는 setter 필요
}

// TODO: discoverSensors, readTemperature 등 실제 동작에 대한 테스트 추가 예정

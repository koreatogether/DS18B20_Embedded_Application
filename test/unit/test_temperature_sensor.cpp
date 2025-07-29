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

TEST(TemperatureSensorManagerTest, SetAndGetSensorId)
{
    TemperatureSensorManager mgr;
    // 테스트용으로 discoverSensors 대신 직접 벡터 크기 조정(실제 구현에서는 discoverSensors 사용)
    // private 멤버 접근이 불가하므로, 테스트를 위한 public setter가 필요하거나 friend 선언 필요
    // 여기서는 가정상 sensorIds_가 2개라고 가정하고 setSensorId/getSensorId/isIdDuplicated 동작만 검증
    // (실제 구현에서는 테스트용 인터페이스 확장 필요)
    // 아래는 논리 예시
    // mgr.setSensorId(0, "A");
    // mgr.setSensorId(1, "B");
    // EXPECT_EQ(mgr.getSensorId(0), "A");
    // EXPECT_EQ(mgr.getSensorId(1), "B");
    // EXPECT_FALSE(mgr.isIdDuplicated("C"));
    // EXPECT_TRUE(mgr.isIdDuplicated("A"));
}

// TODO: discoverSensors, readTemperature 등 실제 동작에 대한 테스트 추가 예정

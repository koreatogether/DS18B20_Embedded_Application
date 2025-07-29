#pragma once

#include "ITemperatureSensor.h"

// 최대 센서 개수 정의 (STL 없이 고정 배열 사용)
#define MAX_MOCK_SENSORS 10

/**
 * @brief ITemperatureSensor 인터페이스를 구현한 Mock 객체
 *
 * STL 라이브러리를 사용하지 않는 순수 목업 구현
 * 단위 테스트에서 실제 DS18B20 센서 하드웨어 없이도
 * TemperatureSensorManager를 테스트할 수 있도록 합니다.
 */
class MockTemperatureSensor : public ITemperatureSensor
{
public:
    MockTemperatureSensor();
    virtual ~MockTemperatureSensor() = default;

    // 테스트에서 설정할 수 있는 메서드들
    void setMockSensorCount(int count);
    void setMockTemperature(int index, float temperature);
    void setMockUserData(int index, int userData);
    void clearMockData();

    // ITemperatureSensor 인터페이스 구현
    int getSensorCount() const override;
    float getTemperatureC(int index) const override;
    int getUserData(int index) const override;
    void setUserData(int index, int userData) override;
    bool isIdUsed(int id) const override;

private:
    int sensorCount_;
    float temperatures_[MAX_MOCK_SENSORS];
    int userDataArray_[MAX_MOCK_SENSORS];
    bool userDataSet_[MAX_MOCK_SENSORS]; // 사용자 데이터 설정 여부 추적
};

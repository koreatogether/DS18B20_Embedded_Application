#pragma once

#include "../../src/domain/ITemperatureSensor.h"
#include <vector>
#include <unordered_map>
#include <string>

/**
 * @brief ITemperatureSensor 인터페이스를 구현한 Mock 객체
 *
 * 단위 테스트에서 실제 DS18B20 센서 하드웨어 없이도
 * TemperatureSensorManager를 테스트할 수 있도록 합니다.
 */
class MockTemperatureSensor : public ITemperatureSensor
{
public:
    MockTemperatureSensor() = default;
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
    int sensorCount_ = 0;
    std::vector<float> temperatures_;
    std::unordered_map<int, int> userDataMap_; // index -> userData
};

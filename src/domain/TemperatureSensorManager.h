#pragma once
#include <vector>
#include <string>
#include <cstdint>
#include "ITemperatureSensor.h"

// DS18B20 센서 관리 도메인 매니저
// 여러 센서의 주소, ID, 온도값을 관리하며, 센서 자동 탐지 및 예외 처리 기능 제공
class TemperatureSensorManager
{
public:
    TemperatureSensorManager();
    ~TemperatureSensorManager();

    void discoverSensors(); // 센서 자동 탐지
    size_t getSensorCount() const;
    bool getSensorAddress(size_t index, uint8_t *address) const;
    float readTemperature(size_t index);
    bool setSensorId(size_t index, const std::string &id);
    std::string getSensorId(size_t index) const;
    bool isIdDuplicated(const std::string &id) const;
    // 기타 예외/에러 처리 메서드 선언 예정

    // [TEST ONLY] 테스트를 위한 센서 주소/ID 강제 주입 메서드
    void _test_setSensorAddresses(const std::vector<std::vector<uint8_t>>& addrs) { sensorAddresses_ = addrs; sensorIds_.resize(addrs.size()); }
    void _test_setSensorIds(const std::vector<std::string>& ids) { sensorIds_ = ids; }

private:
    std::vector<std::vector<uint8_t>> sensorAddresses_; // 8바이트 주소
    std::vector<std::string> sensorIds_;
    ITemperatureSensor *sensorDriver_; // 의존성 주입
};

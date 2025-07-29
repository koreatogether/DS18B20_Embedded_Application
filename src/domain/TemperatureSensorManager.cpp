#include "TemperatureSensorManager.h"

TemperatureSensorManager::TemperatureSensorManager()
    : sensorDriver_(nullptr)
{
    // TODO: 의존성 주입 방식으로 sensorDriver_ 할당 예정
}

TemperatureSensorManager::~TemperatureSensorManager() {}

void TemperatureSensorManager::discoverSensors()
{
    // TODO: 센서 자동 탐지 로직 구현 예정
}

size_t TemperatureSensorManager::getSensorCount() const
{
    return sensorAddresses_.size();
}

bool TemperatureSensorManager::getSensorAddress(size_t index, uint8_t *address) const
{
    if (index >= sensorAddresses_.size())
        return false;
    for (size_t i = 0; i < 8; ++i)
        address[i] = sensorAddresses_[index][i];
    return true;
}

float TemperatureSensorManager::readTemperature(size_t index)
{
    // TODO: 센서 드라이버를 통해 온도값 읽기 구현 예정
    return -127.0f;
}

bool TemperatureSensorManager::setSensorId(size_t index, const std::string &id)
{
    if (index >= sensorIds_.size())
        return false;
    sensorIds_[index] = id;
    return true;
}

std::string TemperatureSensorManager::getSensorId(size_t index) const
{
    if (index >= sensorIds_.size())
        return "";
    return sensorIds_[index];
}

bool TemperatureSensorManager::isIdDuplicated(const std::string &id) const
{
    for (const auto &existingId : sensorIds_)
    {
        if (existingId == id)
            return true;
    }
    return false;
}

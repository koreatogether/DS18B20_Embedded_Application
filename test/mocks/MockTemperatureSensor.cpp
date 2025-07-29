#include "MockTemperatureSensor.h"

// 테스트에서 설정할 수 있는 메서드들
void MockTemperatureSensor::setMockSensorCount(int count)
{
    sensorCount_ = count;
    temperatures_.resize(count, 0.0f); // 기본값 0도로 초기화
}

void MockTemperatureSensor::setMockTemperature(int index, float temperature)
{
    if (index >= 0 && index < static_cast<int>(temperatures_.size()))
    {
        temperatures_[index] = temperature;
    }
}

void MockTemperatureSensor::setMockUserData(int index, int userData)
{
    userDataMap_[index] = userData;
}

void MockTemperatureSensor::clearMockData()
{
    sensorCount_ = 0;
    temperatures_.clear();
    userDataMap_.clear();
}

// ITemperatureSensor 인터페이스 구현
int MockTemperatureSensor::getSensorCount() const
{
    return sensorCount_;
}

float MockTemperatureSensor::getTemperatureC(int index) const
{
    if (index < 0 || index >= static_cast<int>(temperatures_.size()))
    {
        return -127.0f; // 에러값 반환 (DS18B20 에러값)
    }
    return temperatures_[index];
}

int MockTemperatureSensor::getUserData(int index) const
{
    auto it = userDataMap_.find(index);
    if (it != userDataMap_.end())
    {
        return it->second;
    }
    return 0; // 기본값 반환
}

void MockTemperatureSensor::setUserData(int index, int userData)
{
    if (index >= 0 && index < sensorCount_)
    {
        userDataMap_[index] = userData;
    }
}

bool MockTemperatureSensor::isIdUsed(int id) const
{
    for (const auto &pair : userDataMap_)
    {
        if (pair.second == id)
        {
            return true;
        }
    }
    return false;
}

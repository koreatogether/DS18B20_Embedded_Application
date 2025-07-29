#include "MockTemperatureSensor.h"

MockTemperatureSensor::MockTemperatureSensor()
    : sensorCount_(0)
{
    // 배열 초기화
    for (int i = 0; i < MAX_MOCK_SENSORS; i++)
    {
        temperatures_[i] = 0.0f;
        userDataArray_[i] = 0;
        userDataSet_[i] = false;
    }
}

// 테스트에서 설정할 수 있는 메서드들
void MockTemperatureSensor::setMockSensorCount(int count)
{
    if (count >= 0 && count <= MAX_MOCK_SENSORS)
    {
        sensorCount_ = count;
        // 새로 설정된 센서들은 기본값으로 초기화
        for (int i = 0; i < count; i++)
        {
            temperatures_[i] = 0.0f;
            userDataArray_[i] = 0;
            userDataSet_[i] = false;
        }
    }
}

void MockTemperatureSensor::setMockTemperature(int index, float temperature)
{
    if (index >= 0 && index < sensorCount_)
    {
        temperatures_[index] = temperature;
    }
}

void MockTemperatureSensor::setMockUserData(int index, int userData)
{
    if (index >= 0 && index < sensorCount_)
    {
        userDataArray_[index] = userData;
        userDataSet_[index] = true;
    }
}

void MockTemperatureSensor::clearMockData()
{
    sensorCount_ = 0;
    for (int i = 0; i < MAX_MOCK_SENSORS; i++)
    {
        temperatures_[i] = 0.0f;
        userDataArray_[i] = 0;
        userDataSet_[i] = false;
    }
}

// ITemperatureSensor 인터페이스 구현
int MockTemperatureSensor::getSensorCount() const
{
    return sensorCount_;
}

float MockTemperatureSensor::getTemperatureC(int index) const
{
    if (index < 0 || index >= sensorCount_)
    {
        return -127.0f; // 에러값 반환 (DS18B20 에러값)
    }
    return temperatures_[index];
}

int MockTemperatureSensor::getUserData(int index) const
{
    if (index >= 0 && index < sensorCount_ && userDataSet_[index])
    {
        return userDataArray_[index];
    }
    return 0; // 기본값 반환
}

void MockTemperatureSensor::setUserData(int index, int userData)
{
    if (index >= 0 && index < sensorCount_)
    {
        userDataArray_[index] = userData;
        userDataSet_[index] = true;
    }
}

bool MockTemperatureSensor::isIdUsed(int id) const
{
    for (int i = 0; i < sensorCount_; i++)
    {
        if (userDataSet_[i] && userDataArray_[i] == id)
        {
            return true;
        }
    }
    return false;
}

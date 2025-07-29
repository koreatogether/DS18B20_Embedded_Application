#include "MockTemperatureSensorManager.h"

MockTemperatureSensorManager::MockTemperatureSensorManager()
    : sensorCount_(0), lastErrorCode_(MockSensorErrorCode::SUCCESS)
{
    // 배열 초기화
    for (int i = 0; i < MAX_MOCK_SENSORS; i++)
    {
        // 센서 주소 초기화
        for (int j = 0; j < 8; j++)
        {
            sensorAddresses_[i][j] = 0x00;
        }

        // ID 초기화
        sensorIds_[i][0] = '\0';
        sensorIdSet_[i] = false;

        // 온도 및 연결 상태 초기화
        temperatures_[i] = 0.0f;
        sensorConnected_[i] = true;
    }
}

void MockTemperatureSensorManager::discoverSensors()
{
    // 목업에서는 미리 설정된 센서 개수를 그대로 사용
    clearLastError();
}

int MockTemperatureSensorManager::getSensorCount() const
{
    return sensorCount_;
}

bool MockTemperatureSensorManager::getSensorAddress(int index, unsigned char *address) const
{
    if (!isValidIndex(index) || address == nullptr)
    {
        return false;
    }

    for (int i = 0; i < 8; i++)
    {
        address[i] = sensorAddresses_[index][i];
    }
    return true;
}

float MockTemperatureSensorManager::readTemperature(int index)
{
    if (!isValidIndex(index))
    {
        setLastError(MockSensorErrorCode::INDEX_OUT_OF_RANGE);
        return -127.0f;
    }

    if (!sensorConnected_[index])
    {
        setLastError(MockSensorErrorCode::SENSOR_DISCONNECTED);
        return -127.0f;
    }

    clearLastError();
    return temperatures_[index];
}

bool MockTemperatureSensorManager::setSensorId(int index, const char *id)
{
    if (!isValidIndex(index) || id == nullptr)
    {
        return false;
    }

    if (strlen_mock(id) > MAX_ID_LENGTH)
    {
        return false;
    }

    // ID 중복 체크
    if (isIdDuplicated(id))
    {
        return false;
    }

    strcpy_mock(sensorIds_[index], id);
    sensorIdSet_[index] = true;
    return true;
}

const char *MockTemperatureSensorManager::getSensorId(int index) const
{
    if (!isValidIndex(index) || !sensorIdSet_[index])
    {
        return nullptr;
    }
    return sensorIds_[index];
}

bool MockTemperatureSensorManager::isIdDuplicated(const char *id) const
{
    if (id == nullptr)
    {
        return false;
    }

    for (int i = 0; i < sensorCount_; i++)
    {
        if (sensorIdSet_[i] && strcmp_mock(sensorIds_[i], id))
        {
            return true;
        }
    }
    return false;
}

bool MockTemperatureSensorManager::isTemperatureValid(float temperature) const
{
    return temperature > -55.0f && temperature < 125.0f;
}

MockSensorErrorCode MockTemperatureSensorManager::getLastErrorCode() const
{
    return lastErrorCode_;
}

const char *MockTemperatureSensorManager::getErrorMessage(MockSensorErrorCode errorCode) const
{
    switch (errorCode)
    {
    case MockSensorErrorCode::SUCCESS:
        return "Success";
    case MockSensorErrorCode::SENSOR_NOT_FOUND:
        return "Sensor not found";
    case MockSensorErrorCode::INDEX_OUT_OF_RANGE:
        return "Index out of range";
    case MockSensorErrorCode::TEMPERATURE_READ_ERROR:
        return "Temperature read error";
    case MockSensorErrorCode::SENSOR_DISCONNECTED:
        return "Sensor disconnected";
    case MockSensorErrorCode::INVALID_CRC:
        return "Invalid CRC";
    case MockSensorErrorCode::POWER_PARASITE_ERROR:
        return "Power/Parasite error";
    default:
        return "Unknown error";
    }
}

bool MockTemperatureSensorManager::isSensorConnected(int index) const
{
    if (!isValidIndex(index))
    {
        return false;
    }
    return sensorConnected_[index];
}

void MockTemperatureSensorManager::clearLastError()
{
    lastErrorCode_ = MockSensorErrorCode::SUCCESS;
}

// 목업 테스트를 위한 설정 메서드들
void MockTemperatureSensorManager::setMockSensorCount(int count)
{
    if (count >= 0 && count <= MAX_MOCK_SENSORS)
    {
        sensorCount_ = count;

        // 새로 추가된 센서들의 기본 주소 설정
        for (int i = 0; i < count; i++)
        {
            sensorAddresses_[i][0] = 0x28; // DS18B20 family code
            for (int j = 1; j < 7; j++)
            {
                sensorAddresses_[i][j] = static_cast<unsigned char>(i + j); // 임의의 주소
            }
            sensorAddresses_[i][7] = 0xFF; // CRC (임의)
        }
    }
}

void MockTemperatureSensorManager::setMockTemperature(int index, float temperature)
{
    if (isValidIndex(index))
    {
        temperatures_[index] = temperature;
    }
}

void MockTemperatureSensorManager::setMockSensorConnected(int index, bool connected)
{
    if (isValidIndex(index))
    {
        sensorConnected_[index] = connected;
    }
}

void MockTemperatureSensorManager::setMockErrorCode(MockSensorErrorCode errorCode)
{
    lastErrorCode_ = errorCode;
}

// 내부 메서드들
void MockTemperatureSensorManager::setLastError(MockSensorErrorCode errorCode)
{
    lastErrorCode_ = errorCode;
}

bool MockTemperatureSensorManager::isValidIndex(int index) const
{
    return index >= 0 && index < sensorCount_;
}

int MockTemperatureSensorManager::strlen_mock(const char *str) const
{
    if (str == nullptr)
    {
        return 0;
    }

    int len = 0;
    while (str[len] != '\0' && len < MAX_ID_LENGTH)
    {
        len++;
    }
    return len;
}

void MockTemperatureSensorManager::strcpy_mock(char *dest, const char *src)
{
    if (dest == nullptr || src == nullptr)
    {
        return;
    }

    int i = 0;
    while (src[i] != '\0' && i < MAX_ID_LENGTH)
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

bool MockTemperatureSensorManager::strcmp_mock(const char *str1, const char *str2) const
{
    if (str1 == nullptr || str2 == nullptr)
    {
        return false;
    }

    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0' && i < MAX_ID_LENGTH)
    {
        if (str1[i] != str2[i])
        {
            return false;
        }
        i++;
    }
    return str1[i] == str2[i];
}

#pragma once

// 최대 센서 개수 정의
#define MAX_MOCK_SENSORS 10
#define MAX_ID_LENGTH 20

// DS18B20 센서 에러 코드 정의 (목업용)
enum class MockSensorErrorCode
{
    SUCCESS = 0,
    SENSOR_NOT_FOUND = -1,
    INDEX_OUT_OF_RANGE = -2,
    TEMPERATURE_READ_ERROR = -3,
    SENSOR_DISCONNECTED = -4,
    INVALID_CRC = -5,
    POWER_PARASITE_ERROR = -6
};

// TemperatureSensorManager의 목업 구현
// STL 없이 고정 배열만 사용하는 순수 목업
class MockTemperatureSensorManager
{
public:
    MockTemperatureSensorManager();
    ~MockTemperatureSensorManager() = default;

    void discoverSensors(); // 센서 자동 탐지 (목업)
    int getSensorCount() const;
    bool getSensorAddress(int index, unsigned char *address) const;
    float readTemperature(int index);
    bool setSensorId(int index, const char *id);
    const char *getSensorId(int index) const;
    bool isIdDuplicated(const char *id) const;

    // 에러 처리 및 예외 상황 관리 메서드
    bool isTemperatureValid(float temperature) const;
    MockSensorErrorCode getLastErrorCode() const;
    const char *getErrorMessage(MockSensorErrorCode errorCode) const;
    bool isSensorConnected(int index) const;
    void clearLastError();

    // 목업 테스트를 위한 설정 메서드들
    void setMockSensorCount(int count);
    void setMockTemperature(int index, float temperature);
    void setMockSensorConnected(int index, bool connected);
    void setMockErrorCode(MockSensorErrorCode errorCode);

private:
    int sensorCount_;
    unsigned char sensorAddresses_[MAX_MOCK_SENSORS][8]; // 8바이트 주소
    char sensorIds_[MAX_MOCK_SENSORS][MAX_ID_LENGTH + 1];
    bool sensorIdSet_[MAX_MOCK_SENSORS];
    float temperatures_[MAX_MOCK_SENSORS];
    bool sensorConnected_[MAX_MOCK_SENSORS];
    MockSensorErrorCode lastErrorCode_;

    // 내부 메서드
    void setLastError(MockSensorErrorCode errorCode);
    bool isValidIndex(int index) const;
    int strlen_mock(const char *str) const;
    void strcpy_mock(char *dest, const char *src);
    bool strcmp_mock(const char *str1, const char *str2) const;
};

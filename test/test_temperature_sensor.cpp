#include <unity.h>
#include "domain/TemperatureSensorManager.h"

void setUp(void)
{
    // 각 테스트 전에 실행
}

void tearDown(void)
{
    // 각 테스트 후에 실행
}

// -----------------------------
// 2. TemperatureSensorManager 클래스 생성/기본값
// -----------------------------
void test_temperatureSensorManager_construction_defaults(void)
{
    TemperatureSensorManager mgr;
    TEST_ASSERT_EQUAL_UINT32(0, mgr.getSensorCount());
}

// -----------------------------
// 3. 센서 자동 탐지 및 주소 관리 로직
// -----------------------------
void test_temperatureSensorManager_sensorAddress_normal(void)
{
    TemperatureSensorManager mgr;
    std::vector<std::vector<uint8_t>> addrs = {
        {0x28, 0xFF, 0x1C, 0x60, 0x91, 0x16, 0x03, 0x5C}};
    mgr._test_setSensorAddresses(addrs);
    uint8_t addr[8] = {0};
    TEST_ASSERT_TRUE(mgr.getSensorAddress(0, addr));
    for (size_t i = 0; i < 8; ++i)
        TEST_ASSERT_EQUAL_UINT8(addrs[0][i], addr[i]);
}

void test_temperatureSensorManager_sensorAddress_error_outOfRange(void)
{
    TemperatureSensorManager mgr;
    std::vector<std::vector<uint8_t>> addrs = {
        {0x28, 0xFF, 0x1C, 0x60, 0x91, 0x16, 0x03, 0x5C}};
    mgr._test_setSensorAddresses(addrs);
    uint8_t addr[8] = {0};
    TEST_ASSERT_FALSE(mgr.getSensorAddress(1, addr));
}

void test_temperatureSensorManager_sensorCount_normal(void)
{
    TemperatureSensorManager mgr;
    TEST_ASSERT_EQUAL_UINT32(0, mgr.getSensorCount());
    mgr._test_setSensorAddresses({{1, 2, 3, 4, 5, 6, 7, 8}, {9, 10, 11, 12, 13, 14, 15, 16}});
    TEST_ASSERT_EQUAL_UINT32(2, mgr.getSensorCount());
}

// -----------------------------
// 4. 센서별 온도 측정/수집/갱신 메서드
// -----------------------------
void test_temperatureSensorManager_readTemperature_errorCase(void)
{
    TemperatureSensorManager mgr;
    mgr._test_setSensorAddresses({{1, 2, 3, 4, 5, 6, 7, 8}});
    TEST_ASSERT_FLOAT_WITHIN(0.01f, -127.0f, mgr.readTemperature(0));
    TEST_ASSERT_FLOAT_WITHIN(0.01f, -127.0f, mgr.readTemperature(1));
}

// -----------------------------
// 5. 센서별 사용자 데이터(ID) 저장/조회/중복 체크
// -----------------------------
void test_temperatureSensorManager_sensorId_normal(void)
{
    TemperatureSensorManager mgr;
    std::vector<std::vector<uint8_t>> addrs = {
        {0x28, 0xFF, 0x1C, 0x60, 0x91, 0x16, 0x03, 0x5C},
        {0x28, 0xFF, 0x2D, 0xA0, 0x91, 0x16, 0x04, 0x6D}};
    mgr._test_setSensorAddresses(addrs);
    mgr._test_setSensorIds({"", ""});
    TEST_ASSERT_TRUE(mgr.setSensorId(0, "A"));
    TEST_ASSERT_TRUE(mgr.setSensorId(1, "B"));
    TEST_ASSERT_EQUAL_STRING("A", mgr.getSensorId(0).c_str());
    TEST_ASSERT_EQUAL_STRING("B", mgr.getSensorId(1).c_str());
}

void test_temperatureSensorManager_sensorId_error_outOfRange(void)
{
    TemperatureSensorManager mgr;
    std::vector<std::vector<uint8_t>> addrs = {
        {0x28, 0xFF, 0x1C, 0x60, 0x91, 0x16, 0x03, 0x5C},
        {0x28, 0xFF, 0x2D, 0xA0, 0x91, 0x16, 0x04, 0x6D}};
    mgr._test_setSensorAddresses(addrs);
    mgr._test_setSensorIds({"", ""});
    TEST_ASSERT_FALSE(mgr.setSensorId(2, "C"));
    TEST_ASSERT_EQUAL_STRING("", mgr.getSensorId(2).c_str());
}

void test_temperatureSensorManager_sensorId_duplicateCheck(void)
{
    TemperatureSensorManager mgr;
    std::vector<std::vector<uint8_t>> addrs = {
        {0x28, 0xFF, 0x1C, 0x60, 0x91, 0x16, 0x03, 0x5C},
        {0x28, 0xFF, 0x2D, 0xA0, 0x91, 0x16, 0x04, 0x6D}};
    mgr._test_setSensorAddresses(addrs);
    mgr._test_setSensorIds({"A", "B"});
    TEST_ASSERT_FALSE(mgr.isIdDuplicated("C"));
    TEST_ASSERT_TRUE(mgr.isIdDuplicated("A"));
    TEST_ASSERT_TRUE(mgr.isIdDuplicated("B"));
}

// -----------------------------
// 6. 에러 처리(-127.0 등) 및 예외 상황 관리
// -----------------------------
void test_temperatureSensorManager_errorHandling_temperatureValidation(void)
{
    TemperatureSensorManager mgr;

    // 정상 온도값
    TEST_ASSERT_TRUE(mgr.isTemperatureValid(20.5f));
    TEST_ASSERT_TRUE(mgr.isTemperatureValid(0.0f));
    TEST_ASSERT_TRUE(mgr.isTemperatureValid(100.0f));

    // 에러 온도값
    TEST_ASSERT_FALSE(mgr.isTemperatureValid(-127.0f));
    TEST_ASSERT_FALSE(mgr.isTemperatureValid(-128.0f));
    TEST_ASSERT_FALSE(mgr.isTemperatureValid(125.0f));
    TEST_ASSERT_FALSE(mgr.isTemperatureValid(126.0f));
}

void test_temperatureSensorManager_errorHandling_errorCodes(void)
{
    TemperatureSensorManager mgr;

    // 초기 상태는 SUCCESS
    TEST_ASSERT_EQUAL_INT32((int32_t)SensorErrorCode::SUCCESS, (int32_t)mgr.getLastErrorCode());

    // 인덱스 범위 초과 시 에러 코드 설정
    mgr.getSensorAddress(999, nullptr);
    TEST_ASSERT_EQUAL_INT32((int32_t)SensorErrorCode::INDEX_OUT_OF_RANGE, (int32_t)mgr.getLastErrorCode());

    // 에러 클리어
    mgr.clearLastError();
    TEST_ASSERT_EQUAL_INT32((int32_t)SensorErrorCode::SUCCESS, (int32_t)mgr.getLastErrorCode());

    // 테스트용 에러 설정
    mgr._test_setLastError(SensorErrorCode::SENSOR_DISCONNECTED);
    TEST_ASSERT_EQUAL_INT32((int32_t)SensorErrorCode::SENSOR_DISCONNECTED, (int32_t)mgr.getLastErrorCode());
}

void test_temperatureSensorManager_errorHandling_errorMessages(void)
{
    TemperatureSensorManager mgr;

    TEST_ASSERT_EQUAL_STRING("Success", mgr.getErrorMessage(SensorErrorCode::SUCCESS).c_str());
    TEST_ASSERT_EQUAL_STRING("Index out of range", mgr.getErrorMessage(SensorErrorCode::INDEX_OUT_OF_RANGE).c_str());
    TEST_ASSERT_EQUAL_STRING("Sensor disconnected", mgr.getErrorMessage(SensorErrorCode::SENSOR_DISCONNECTED).c_str());
    TEST_ASSERT_EQUAL_STRING("Temperature read error", mgr.getErrorMessage(SensorErrorCode::TEMPERATURE_READ_ERROR).c_str());
}

void test_temperatureSensorManager_errorHandling_sensorConnection(void)
{
    TemperatureSensorManager mgr;
    std::vector<std::vector<uint8_t>> addrs = {
        {0x28, 0xFF, 0x1C, 0x60, 0x91, 0x16, 0x03, 0x5C}};
    mgr._test_setSensorAddresses(addrs);

    // 현재 구현에서는 항상 false 반환 (시뮬레이션)
    TEST_ASSERT_FALSE(mgr.isSensorConnected(0));
    TEST_ASSERT_FALSE(mgr.isSensorConnected(999)); // 인덱스 범위 초과
}

int main(void)
{
    UNITY_BEGIN();

    // 2. TemperatureSensorManager 클래스 생성/기본값
    RUN_TEST(test_temperatureSensorManager_construction_defaults);

    // 3. 센서 자동 탐지 및 주소 관리 로직
    RUN_TEST(test_temperatureSensorManager_sensorAddress_normal);
    RUN_TEST(test_temperatureSensorManager_sensorAddress_error_outOfRange);
    RUN_TEST(test_temperatureSensorManager_sensorCount_normal);

    // 4. 센서별 온도 측정/수집/갱신 메서드
    RUN_TEST(test_temperatureSensorManager_readTemperature_errorCase);

    // 5. 센서별 사용자 데이터(ID) 저장/조회/중복 체크
    RUN_TEST(test_temperatureSensorManager_sensorId_normal);
    RUN_TEST(test_temperatureSensorManager_sensorId_error_outOfRange);
    RUN_TEST(test_temperatureSensorManager_sensorId_duplicateCheck);

    // 6. 에러 처리 및 예외 상황 관리
    RUN_TEST(test_temperatureSensorManager_errorHandling_temperatureValidation);
    RUN_TEST(test_temperatureSensorManager_errorHandling_errorCodes);
    RUN_TEST(test_temperatureSensorManager_errorHandling_errorMessages);
    RUN_TEST(test_temperatureSensorManager_errorHandling_sensorConnection);

    return UNITY_END();
}

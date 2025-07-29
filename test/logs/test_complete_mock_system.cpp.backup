#include <unity.h>
#include "mocks\MockTemperatureSensor.h"
#include "mocks\MockTemperatureSensorManager.h"

// MockTemperatureSensor 테스트
void test_mock_sensor_count()
{
    MockTemperatureSensor mock;
    mock.setMockSensorCount(3);
    TEST_ASSERT_EQUAL(3, mock.getSensorCount());
}

void test_mock_temperature_set_get()
{
    MockTemperatureSensor mock;
    mock.setMockSensorCount(2);
    mock.setMockTemperature(0, 12.5f);
    mock.setMockTemperature(1, 25.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 12.5f, mock.getTemperatureC(0));
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 25.0f, mock.getTemperatureC(1));
}

void test_mock_user_data()
{
    MockTemperatureSensor mock;
    mock.setMockSensorCount(1);
    mock.setMockUserData(0, 42);
    TEST_ASSERT_EQUAL(42, mock.getUserData(0));
    mock.setUserData(0, 99);
    TEST_ASSERT_EQUAL(99, mock.getUserData(0));
}

void test_mock_is_id_used()
{
    MockTemperatureSensor mock;
    mock.setMockSensorCount(2);
    mock.setMockUserData(0, 11);
    mock.setMockUserData(1, 22);
    TEST_ASSERT_TRUE(mock.isIdUsed(11));
    TEST_ASSERT_TRUE(mock.isIdUsed(22));
    TEST_ASSERT_FALSE(mock.isIdUsed(33));
}

void test_mock_boundary_conditions()
{
    MockTemperatureSensor mock;
    mock.setMockSensorCount(2);

    // 범위 밖 인덱스 테스트
    TEST_ASSERT_FLOAT_WITHIN(0.01f, -127.0f, mock.getTemperatureC(-1));
    TEST_ASSERT_FLOAT_WITHIN(0.01f, -127.0f, mock.getTemperatureC(2));

    // 범위 밖 설정 테스트 (에러가 발생하지 않아야 함)
    mock.setMockTemperature(-1, 100.0f);
    mock.setMockTemperature(2, 100.0f);
}

// MockTemperatureSensorManager 테스트
void test_manager_sensor_count()
{
    MockTemperatureSensorManager manager;
    manager.setMockSensorCount(3);
    TEST_ASSERT_EQUAL(3, manager.getSensorCount());
}

void test_manager_temperature_reading()
{
    MockTemperatureSensorManager manager;
    manager.setMockSensorCount(2);
    manager.setMockTemperature(0, 23.5f);
    manager.setMockTemperature(1, -10.2f);

    TEST_ASSERT_FLOAT_WITHIN(0.01f, 23.5f, manager.readTemperature(0));
    TEST_ASSERT_FLOAT_WITHIN(0.01f, -10.2f, manager.readTemperature(1));
}

void test_manager_sensor_id()
{
    MockTemperatureSensorManager manager;
    manager.setMockSensorCount(2);

    TEST_ASSERT_TRUE(manager.setSensorId(0, "sensor_1"));
    TEST_ASSERT_TRUE(manager.setSensorId(1, "sensor_2"));

    const char *id0 = manager.getSensorId(0);
    const char *id1 = manager.getSensorId(1);

    TEST_ASSERT_NOT_NULL(id0);
    TEST_ASSERT_NOT_NULL(id1);

    // 문자열 비교 (직접 구현)
    bool id0_match = true;
    const char *expected0 = "sensor_1";
    for (int i = 0; expected0[i] != '\0'; i++)
    {
        if (id0[i] != expected0[i])
        {
            id0_match = false;
            break;
        }
    }
    TEST_ASSERT_TRUE(id0_match);
}

void test_manager_id_duplication()
{
    MockTemperatureSensorManager manager;
    manager.setMockSensorCount(3);

    TEST_ASSERT_TRUE(manager.setSensorId(0, "unique_id"));
    TEST_ASSERT_FALSE(manager.setSensorId(1, "unique_id")); // 중복이므로 실패해야 함
    TEST_ASSERT_TRUE(manager.isIdDuplicated("unique_id"));
    TEST_ASSERT_FALSE(manager.isIdDuplicated("non_existent_id"));
}

void test_manager_error_handling()
{
    MockTemperatureSensorManager manager;
    manager.setMockSensorCount(1);

    // 연결되지 않은 센서 테스트
    manager.setMockSensorConnected(0, false);
    float temp = manager.readTemperature(0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, -127.0f, temp);
    TEST_ASSERT_EQUAL((int)MockSensorErrorCode::SENSOR_DISCONNECTED, (int)manager.getLastErrorCode());

    // 연결 복구 후 테스트
    manager.setMockSensorConnected(0, true);
    manager.setMockTemperature(0, 25.0f);
    temp = manager.readTemperature(0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 25.0f, temp);
    TEST_ASSERT_EQUAL((int)MockSensorErrorCode::SUCCESS, (int)manager.getLastErrorCode());
}

void test_manager_temperature_validation()
{
    MockTemperatureSensorManager manager;

    TEST_ASSERT_TRUE(manager.isTemperatureValid(25.0f));
    TEST_ASSERT_TRUE(manager.isTemperatureValid(-54.9f));
    TEST_ASSERT_TRUE(manager.isTemperatureValid(124.9f));
    TEST_ASSERT_FALSE(manager.isTemperatureValid(-55.1f));
    TEST_ASSERT_FALSE(manager.isTemperatureValid(125.1f));
}

void setUp(void) {}
void tearDown(void) {}

int main(void)
{
    UNITY_BEGIN();

    // MockTemperatureSensor 테스트
    RUN_TEST(test_mock_sensor_count);
    RUN_TEST(test_mock_temperature_set_get);
    RUN_TEST(test_mock_user_data);
    RUN_TEST(test_mock_is_id_used);
    RUN_TEST(test_mock_boundary_conditions);

    // MockTemperatureSensorManager 테스트
    RUN_TEST(test_manager_sensor_count);
    RUN_TEST(test_manager_temperature_reading);
    RUN_TEST(test_manager_sensor_id);
    RUN_TEST(test_manager_id_duplication);
    RUN_TEST(test_manager_error_handling);
    RUN_TEST(test_manager_temperature_validation);

    return UNITY_END();
}

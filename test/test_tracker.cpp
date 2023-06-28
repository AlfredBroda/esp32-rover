
#include <esp32-rover.cpp>
#include <unity.h>

Tracker track;

void setUp(void) {
    // set stuff up here

    track = Tracker(1, 2, 3, 4, IMU());
}

void tearDown(void) {
    // clean stuff up here
}

void test_calculator_addition(void) {
    TEST_ASSERT_EQUAL(M_FORWARD, track.setMotorState(M_FORWARD));
}

void test_calculator_subtraction(void) {
    TEST_ASSERT_EQUAL(20, calc.sub(23, 3));
}

void test_calculator_multiplication(void) {
    TEST_ASSERT_EQUAL(50, calc.mul(25, 2));
}

void test_calculator_division(void) {
    TEST_ASSERT_EQUAL(32, calc.div(100, 3));
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_calculator_addition);
    RUN_TEST(test_calculator_subtraction);
    RUN_TEST(test_calculator_multiplication);
    RUN_TEST(test_calculator_division);
    UNITY_END();

    return 0;
}

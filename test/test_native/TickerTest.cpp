#include "StubDisplay.h"
#include <display/DisplayRoutine.h>
#include <unity.h>

void test_ticker(void) {
    test::StubDisplay display = test::StubDisplay();
    DisplayRoutine::Ticker ticker = DisplayRoutine::Ticker(display);
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_ticker);
    UNITY_END();

    return 0;
}
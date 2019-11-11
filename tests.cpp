#include "Arduino.h"

#include "tests.hpp"

#define BREAK true
#define TEST_POWMOD true

bool verify_powmod(uint32_t expected, uint32_t actual, uint32_t base, uint32_t power, uint32_t mod)
{
    if (expected != actual)
    {
        Serial.println("FAILED CASE:");
        Serial.print("Base: ");
        Serial.print(base);
        Serial.print(" Power: ");
        Serial.print(power);
        Serial.print(" Mod: ");
        Serial.println(mod);
        Serial.print("Expected: ");
        Serial.print(expected);
        Serial.print(" Actual: ");
        Serial.println(actual);
        return false;
    }
    return true;
}

void test_powmod()
{
    const uint32_t power = (uint32_t)pow(2.0, 32.0) - 1;
    const uint32_t mod = (uint32_t)pow(2.0, 31.0) - 1;
    for (uint32_t i = 0; i < (uint32_t)pow(2.0, 32.0) - 1; i++)
    {
        uint32_t expected = (uint32_t)pow(i, power) % mod;
        uint32_t actual = powmod(i, power, mod);
        if (verify_powmod(expected, actual, i, power, mod) && BREAK)
        {
            break;
        }
    }
    Serial.println("TESTS ENDED");
}

void tests_run()
{
    Serial.println("TESTS COMMENSING");
    #if(TEST_POWMOD)
        test_powmod();
    #endif
}
/*
	Basic Blink LED Example
*/
#include "Arduino.h"

#include "tests.hpp"

#define ARDUINO_MODE_PIN 13

const uint32_t serverPublicKey = 7;
const uint32_t serverPrivateKey = 27103;
const uint32_t serverModulus = 95477;
const uint32_t clientPublicKey = 11;
const uint32_t clientPrivateKey = 38291;
const uint32_t clientModulus = 84823;

uint32_t d;
uint32_t n;
uint32_t e;
uint32_t m;

int main(){
	
	setup();

	#if(!TEST_MODE)
		while(true)
		{
			if (Serial.available())
			{
				// Read from computer input
			}
			if (Serial3.available())
			{
				// Read from Arduino input
			}	
		}
	#else
		tests_run();
	#endif
	
	Serial.flush();
	Serial3.flush();
	return 0;
}

void setup()
{
	init();
	Serial.begin(9600);
	Serial3.begin(9600);

	pinMode(ARDUINO_MODE_PIN, INPUT);

	// Determine if Arduino is configured to be a server or client

	if (digitalRead(ARDUINO_MODE_PIN) == HIGH)
	{
		d = serverPrivateKey;
		n = serverModulus;
		e = clientPublicKey;
		m = clientModulus;
	}
	else
	{
		d = clientPrivateKey;
		n = clientModulus;
		e = serverPublicKey;
		m = serverModulus;
	}
}

/**
 * Writes an uint32_t to Serial3 , starting from the least - significant
 * and finishing with the most significant byte .
 */
void uint32_to_serial3 (uint32_t num)
{
	Serial3.write((char) (num >> 0));
	Serial3.write((char) (num >> 8));
	Serial3.write((char) (num >> 16));
	Serial3.write((char) (num >> 24));
}
/**
 * Reads an uint32_t from Serial3 , starting from the least - significant
 * and finishing with the most significant byte .
 */
uint32_t uint32_from_serial3()
{
	uint32_t num = 0;
	num = num | ((uint32_t) Serial3.read()) << 0;
	num = num | ((uint32_t) Serial3.read()) << 8;
	num = num | ((uint32_t) Serial3.read()) << 16;
	num = num | ((uint32_t) Serial3.read()) << 24;
	return num;
}

uint32_t powmod(uint32_t base, uint32_t power, uint32_t mod)
{
	base  = base % mod;
	uint32_t ans = 0;
	while (power != 0)
	{
		if (power & 1)
		{
			// Perform multiplication step
			uint32_t n = 0;
			while ((1 << n) < base)
			{
				if ((1 << n) & base)
				{
					uint32_t x = base;
					for (uint32_t i = 0; i < n; i++)
					{
						x = (2*x) % mod;
					}
					ans = (x + ans) % mod;
				}
				n++;
			}
		}
		power /= 2;
	}

	return ans;
}
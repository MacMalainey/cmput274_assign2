/*
	CMPUT 274 Assignment #2 Part 1
	Mackenzie Malainey - 1570494
	Lora Ma - STUDENT ID #
*/
#include "Arduino.h"

#include "tests.hpp"

#define ARDUINO_MODE_PIN 13

// Predefined server and client keys

const uint32_t serverPublicKey = 7;
const uint32_t serverPrivateKey = 27103;
const uint32_t serverModulus = 95477;
const uint32_t clientPublicKey = 11;
const uint32_t clientPrivateKey = 38291;
const uint32_t clientModulus = 84823;

// This Arduino's RSA encryption / decryption information

uint32_t d;
uint32_t n;
uint32_t e;
uint32_t m;

int main(){

	setup();

	#if(!TEST_MODE)
		while(true)
		{
			if (Serial.available() > 0)
			{
				// Read from computer input
				uint32_t read_input = Serial.read();

				// Encrypt byte
				uint32_t encrypted = powmod(read_input, serverPublicKey, serverModulus);

				// Send to Arduino
				Serial3.write(encrypted);
			}
			if (Serial3.available() > 0)
			{
				// Read from Arduino input
				uint32_t read_input = Serial3.read();

				// Decrypt byte
				decrypted = powmod(read_input, serverPrivateKey, serverModulus);

				// Send to Computer
				Serial.write(decrypted);
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
	// Initialize arduino and serial modules, as well as pins
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
 * Description:
 * Writes an uint32_t to Serial3, starting from the least - significant bit
 * and finishing with the most significant byte.
 * (FUNCTION PROVIDED FROM ASSIGNMENT INFORMATION)
 *
 * Arguments:
 * num (uint32_t): 32 bit unsigned integer to print to serial3
 */
void uint32_to_serial3 (uint32_t num)
{
	Serial3.write((char) (num >> 0));
	Serial3.write((char) (num >> 8));
	Serial3.write((char) (num >> 16));
	Serial3.write((char) (num >> 24));
}
/**
 * Description:
 * Reads an uint32_t from Serial3, starting from the least - significant
 * and finishing with the most significant byte.
 * (FUNCTION PROVIDED FROM ASSIGNMENT INFORMATION)
 *
 * Returns:
 * num (uint32_t): 32 bit unsigned integer read from Serial3
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

/**
 * Description:
 * Performs fast modular exponentiation (formula: ((base)^power) % mod)
 *
 * Arguments:
 * base (uint32_t): base for exponentation
 * power (uint32_t): exponent for exponentation
 * mod (uint32_t): number to perform modulus around
 *
 * Returns:
 * ans (uint32_t): Result of the expression ((base)^power) % mod
 */
uint32_t powmod(uint32_t base, uint32_t power, uint32_t mod)
{
	base  = base % mod;
	uint32_t ans = 0;
	while (power != 0)
	{
		if (power & 1)
		{
			// Perform modulus multiplication
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

/*
	CMPUT 274 Assignment #2 Part 1
	Mackenzie Malainey - 1570494
	Lora Ma - 1570935
*/
#include "Arduino.h"

#include "test.hpp"

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

uint32_t debug_d;


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

		debug_d = clientPrivateKey;
	}
	else
	{
		d = clientPrivateKey;
		n = clientModulus;
		e = serverPublicKey;
		m = serverModulus;
	}
}


uint32_t mulmod(uint32_t a, uint32_t b, uint32_t m)
{
  uint32_t n = 1;
  uint32_t ans = 0;
  uint32_t last_n = 1;
  b = b % m;
  while (n <= a)
  {
    if ((n & a) > 0)
    {
      while (last_n < n)
      {
        b = (2*b) % m;
        last_n <<= 1;
      }
      ans = (ans + b) % m;
    }
    n <<= 1;
  }
  return ans;
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
  uint32_t ans = 1;
  uint32_t pow_x = base % mod;

  while (power > 0) {
    if (power & 1 == 1) {
      ans = mulmod(pow_x, ans, mod);
    }

    pow_x = mulmod(pow_x, pow_x, mod);

    power >>= 1;
  }

  return ans;
}

// computes the value x^pow mod m ("x to the power of pow" mod m)
uint32_t powmod2(uint32_t x, uint32_t pow, uint32_t m) {
  // you will make these uint32_t types
  // for the final submission
  uint64_t ans = 1;
  uint64_t pow_x = x;

  // NOTE: in the full assignment you will have to
  // replace the 64-bit types with an algorithm that
  // performs multiplication modulo a 31-bit number while
  // only using 32-bit types.

  while (pow > 0) {
    if (pow&1 == 1) {
      // will replace the following line with a "mulmod" call
      // discussed on Nov 7 (see also the worksheet posted then)
      ans = (ans*pow_x)%m;
    }

    // as well as this line (i.e. call mulmod instead)
    pow_x = (pow_x*pow_x)%m;

    pow >>= 1; // divides by 2
  }

  return ans;
}

int main(){

	setup();

	while(true)
	{
		if (Serial.available() > 0)
		{
			// Read from computer input
			char input = Serial.read();

			// Encrypt byte
			if (input == '\r' ) {
				Serial.println();
				uint32_t encryptedR = powmod('\r', e, m);
				uint32_to_serial3(encryptedR);
				uint32_t encryptedN = powmod('\n', e, m);
				uint32_to_serial3(encryptedN);
			} else {
				Serial.print(input);
				uint32_t encrypted = powmod(input, e, m);
				uint32_to_serial3(encrypted);
			}
		}

		if (Serial3.available() > 0) {
			uint32_t read_input = uint32_from_serial3();
			char decrypted = (char)powmod(read_input, d, n);
			Serial.print(decrypted);
		}
	}

	Serial.flush();
	Serial3.flush();
	return 0;
}

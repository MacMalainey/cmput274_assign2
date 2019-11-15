#include <Arduino.h>

#include "test.hpp"

#if(FALSE)
// forward declaration of powmod, or else the test function below
// doesn't know about it
uint32_t powmod(uint32_t x, uint32_t pow, uint32_t m);

uint32_t mulmod(uint32_t num1, uint32_t num2, uint32_t mod);

// calls powmod to compute x^pow mod m, and compares it with ans
// prints a statement if the test failed
void powmodTestOnce(uint32_t x, uint32_t pow, uint32_t m, uint32_t ans) {
  Serial.print("Testing: ");
  Serial.print(x);
  Serial.print(" ");
  Serial.print(pow);
  Serial.print(" ");
  Serial.print(m);
  Serial.print("... ");

  uint32_t computed = powmod(x, pow, m);
  if (ans != computed) {
    Serial.println("failed!");
    Serial.print("Expected: ");
    Serial.println(ans);
    Serial.print("Got: ");
    Serial.println(computed);
  }
  else {
    Serial.println("OK!");
  }
}

void mulmodTestOnce(uint32_t num1, uint32_t num2, uint32_t m, uint32_t ans)
{
  Serial.print("Testing: ");
  Serial.print(num1);
  Serial.print(" ");
  Serial.print(num2);
  Serial.print(" ");
  Serial.print(m);
  Serial.print("... ");

  uint32_t computed = mulmod(num1, num2, m);
  if (ans != computed) {
    Serial.println("failed!");
    Serial.print("Expected: ");
    Serial.println(ans);
    Serial.print("Got: ");
    Serial.println(computed);
  }
  else {
    Serial.println("OK!");
  }
}

void mulmodTest() {
  // you can generate examples by calling, say, pow(2, 15, 37) in Python
  // to compute 2^(15) mod 37
  Serial.println("Beginning mulmod tests...");
  // mulmodTestOnce(1, 1, 3, 1);
  // mulmodTestOnce(0, 10, 10, 0);
  // mulmodTestOnce(1, 5, 1, 0);
  mulmodTestOnce(5, 20, 37, 26);
  mulmodTestOnce(12345, 12345, 123456789, 28942236);
  mulmodTestOnce(123456, 123456, 123456789, 56198889);
  Serial.println("Done tests");
}

void powmodTest() {
  // you can generate examples by calling, say, pow(2, 15, 37) in Python
  // to compute 2^(15) mod 37
  Serial.println("Beginning powmod tests...");
  powmodTestOnce(1, 5, 6, 1);
  powmodTestOnce(0, 10, 10, 0);
  powmodTestOnce(1, 5, 1, 0);
  powmodTestOnce(2, 15, 37, 23);
  powmodTestOnce(2, 12345, 123456789, 92652722);
  powmodTestOnce(2, 123456, 123456789, 68532166);
  powmodTestOnce(2, 1234567, 123456789, 30787427);
  powmodTestOnce(123, 123456789, 1234567890, 353558493);
  Serial.println("Done tests");
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

// computes the value x^pow mod m ("x to the power of pow" mod m)
uint32_t powmod(uint32_t x, uint32_t pow, uint32_t m) {
  // you will make these uint32_t types
  // for the final submission
  uint32_t ans = 1;
  uint32_t pow_x = x % m;

  // NOTE: in the full assignment you will have to
  // replace the 64-bit types with an algorithm that
  // performs multiplication modulo a 31-bit number while
  // only using 32-bit types.

  while (pow > 0) {
    if (pow&1 == 1) {
      // will replace the following line with a "mulmod" call
      // discussed on Nov 7 (see also the worksheet posted then)
      ans = mulmod(pow_x, ans, m);
    }

    // as well as this line (i.e. call mulmod instead)
    pow_x = mulmod(pow_x, pow_x, m);

    pow >>= 1; // divides by 2
  }

  return ans;
}

// /**
//  * Description:
//  * Performs fast modular exponentiation (formula: ((base)^power) % mod)
//  *
//  * Arguments:
//  * base (uint32_t): base for exponentation
//  * power (uint32_t): exponent for exponentation
//  * mod (uint32_t): number to perform modulus around
//  *
//  * Returns:
//  * ans (uint32_t): Result of the expression ((base)^power) % mod
//  */
// uint32_t powmod(uint32_t base, uint32_t power, uint32_t mod)
// {
// 	base  = base % mod;
// 	uint32_t ans = 0;
// 	while (power > 0)
// 	{
// 		if (power & 1 == 1)
// 		{
// 			// Perform modulus multiplication
// 			mulmod(base, base, mod);
// 		}
// 		power >>= 1;
// 	}

// 	return ans;
// }

#endif

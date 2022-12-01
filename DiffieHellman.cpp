#include <stdio.h>
#include <string.h>
#include <math.h>
#include <cstdlib>
#include <iostream>

using namespace std;

long long fast_mod_expo(long long base, long long power, int mod){
  long long result = 1;
  while (power > 0)
  {
    if (power % 2 == 1)
      result = (result * base) % mod;
    power = power >> 1;
    base = (base * base) % mod;
  }
  return result;
}

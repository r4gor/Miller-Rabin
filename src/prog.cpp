#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random.hpp>
#include <iostream>

using namespace boost::multiprecision;
using namespace boost::multiprecision::literals;
using namespace boost::random;

using uint512 = number<cpp_int_backend<512, 512, unsigned_magnitude, unchecked, void>>;
using uint1024 = number<cpp_int_backend<1024, 1024, unsigned_magnitude, unchecked, void>>;

// Computes (a^p) % n while also checking for compositeness
uint512 power(uint512 a, uint512 p, uint512 n, bool *isProbablyPrime) {
  if (p == 0)
    return 1;

  // Recursive call: compute a^(p/2) % n
  uint512 x = power(a, p/2, n, isProbablyPrime);
  uint512 result = uint512((uint1024(x) * uint1024(x)) % uint1024(n));

  // If we find a nontrivial square root of 1 modulo n, n is composite
  if (result == 1 && x != 1 && x != n-1 )
    *isProbablyPrime = false;

  // If the exponent is odd, multiply by a and reduce modulo n
  if (p % 2 == 1)
    result = uint512((uint1024(a) * result) % uint1024(n));

  return result;
}

// Function: probabilistic primality test using Fermat-like check
bool primalityTest(uint512 n) {
  mt19937 mt(clock());

  // Generate a random base 'a' in the range [2, n-1]
  uniform_int_distribution<uint512> ui(uint512(2), n-1);
  uint512 a = ui(mt);

  bool isProbablyPrime = true;

  uint512 result = power(a, n-1, n, &isProbablyPrime);
  if (result != 1 || !isProbablyPrime)
    return false;
  else
    return true;
}

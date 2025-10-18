#include <string.h>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/miller_rabin.hpp>
#include <boost/random.hpp>
#include "prog.cpp"

using namespace boost::multiprecision;
using namespace boost::multiprecision::literals;
using uint512 = number<cpp_int_backend<512, 512, unsigned_magnitude, unchecked, void>>;
using uint1024 = number<cpp_int_backend<1024, 1024, unsigned_magnitude, unchecked, void>>;
using namespace boost::random;

// Struct to store results of primality tests for a number
struct Result {
  uint512 num;
  bool isPrime;
  float correctness[4];
};

int main() {

  const int numbers_to_generate = 100;
  const int tries_per_num = 2000;
  Result results[numbers_to_generate];

  // Loop over each random number
  for(int i = 0; i < numbers_to_generate; i++) {
    bool isPrime_tries_1 = 1;
    bool isPrime_tries_5 = 1;
    bool isPrime_tries_10 = 1;
    bool isPrime_tries_25 = 1;

    // Gen number
    mt19937 mt2(clock());
    mt19937 mt3(clock());

    uniform_int_distribution<long> ui(2, 10000);

    uint512 randomNum = uint512(ui(mt2));

    // Result given by Boost library with 0.25^25 probability of error
    bool Boost_PrimalityTest = miller_rabin_test(randomNum, 25, mt3);


    int tries_per_confidence[tries_per_num][4];
    float mean_per_confidence[4];

    // Repeat testing to get statistical confidence
    for(int k = 0; k < tries_per_num; k++) {

      isPrime_tries_1 = primalityTest(randomNum);

      for(int j = 0; j < 5; j++) {
        if (!primalityTest(randomNum)) {
          isPrime_tries_5 = false;
          break;
        }
      }

      for(int j = 0; j < 10; j++) {
        if (!primalityTest(randomNum)) {
          isPrime_tries_10 = false;
          break;
        }
      }

      for(int j = 0; j < 25; j++) {
        if (!primalityTest(randomNum)) {
          isPrime_tries_25 = false;
          break;
        }
      }

      // Compare results with Boost's Miller-Rabin test and store 1 if they match, 0 otherwise
      tries_per_confidence[k][0] = isPrime_tries_1 == Boost_PrimalityTest ? 1 : 0;
      tries_per_confidence[k][1] = isPrime_tries_5 == Boost_PrimalityTest ? 1 : 0;
      tries_per_confidence[k][2] = isPrime_tries_10 == Boost_PrimalityTest ? 1 : 0;
      tries_per_confidence[k][3] = isPrime_tries_25 == Boost_PrimalityTest ? 1 : 0;
    }


    // Calculate mean correctness for each confidence level
    for(int j = 0; j < 4; j++) {
      mean_per_confidence[j] = 0.f;
      for(int k = 0; k < tries_per_num; k++) {
        mean_per_confidence[j] += float(tries_per_confidence[k][j]);
      }
      mean_per_confidence[j] /= float(tries_per_num);
    }

    results[i].num = randomNum;
    results[i].isPrime = Boost_PrimalityTest;
    for(int j=0; j<4; j++)
      results[i].correctness[j] = mean_per_confidence[j];

    if (i % 10 == 0) {
      printf("iteration %3d/%d\n", i, numbers_to_generate);
    }
  }

  // Print results for all numbers
  std::cout << std::endl;
  std::cout << "number   | prime | 1 try | 5 tries | 10 tries | 25 tries" << std::endl;
  std::cout << "---------------------------------------------------------" << std::endl;
  for(int i = 0; i < numbers_to_generate; i++) {
    Result r = results[i];
    std::string num = boost::lexical_cast<std::string>(r.num);
    printf("%-8s |   %1d   | %.3f | %.3f   | %.3f    | %.3f   \n",
      num.c_str(), r.isPrime, r.correctness[0], r.correctness[1], r.correctness[2], r.correctness[3]);
  }
}

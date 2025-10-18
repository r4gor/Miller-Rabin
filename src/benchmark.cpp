#include <boost/multiprecision/cpp_int.hpp>
#include <benchmark/benchmark.h>
#include "prog.cpp"

using namespace boost::multiprecision;
using namespace boost::multiprecision::literals;
using uint512 = number<cpp_int_backend<512, 512, unsigned_magnitude, unchecked, void>>;

// Benchmark function to test performance of primalityTest
static void BM_NotPrimes(benchmark::State& state) {
    long exp = state.range(0);
    uint512 n = 1;
    n <<= exp;

    for (auto _ :state)
        benchmark::DoNotOptimize(primalityTest(n));
    state.SetComplexityN(exp);
}

BENCHMARK(BM_NotPrimes)
    ->DenseRange(8, 504, 8)
    ->Complexity();

BENCHMARK_MAIN();

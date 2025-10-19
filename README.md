# Miller-Rabin Primality Test on 512-bit Integers

Course challenge at Politecnico di Milano (October 2025), by **Raul Agolli** and **Gioele Guaiumi**.
📄 **[Report](report/report.pdf)** · 📓 **[Notebook](PrimalityTest.ipynb)**

A Miller-Rabin-style probabilistic primality test for 512-bit numbers in C++ with Boost.Multiprecision: a Fermat check that also flags nontrivial square roots of 1 during modular exponentiation.
After *k* rounds the chance of passing a composite is at most 4<sup>-k</sup>.

- `src/prog.cpp`: the test (modular exponentiation on 512/1024-bit fixed-width integers, random witnesses).
- `src/test.cpp`: checks 20 known 512-bit primes.
- `src/test2.cpp`: compares results on random 512-bit numbers with Boost's own `miller_rabin_test`.
- `src/benchmark.cpp`: Google Benchmark runs; the notebook plots mean and standard deviation per input size.

## Run

```bash
sudo apt-get install libboost-all-dev
cd src
g++ test.cpp -o test && ./test
g++ test2.cpp -o test2 && ./test2
# benchmark: build google/benchmark first, see the notebook
g++ benchmark.cpp -O2 -std=c++11 -lbenchmark -lpthread -o bench && ./bench --benchmark_time_unit=us
```

The notebook is designed for Colab: it installs Boost and Google Benchmark, writes the sources and runs everything.

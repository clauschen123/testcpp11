// parallel-fibonacci.cpp
// compile with: /EHsc
#include <windows.h>
#include <ppl.h>
#include <concurrent_vector.h>
#include <array>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>

//using namespace concurrency;
//using namespace std;

// Calls the provided work function and returns the number of milliseconds 
// that it takes to call that function.
namespace ppl_ {
    template <class Function>
    __int64 time_call(Function&& f)
    {
        __int64 begin = GetTickCount();
        f();
        return GetTickCount() - begin;
    }

    // Computes the nth Fibonacci number.
    int fibonacci(int n)
    {
        if (n < 2)
            return n;
        return fibonacci(n - 1) + fibonacci(n - 2);
    }

    void main()
    {
        __int64 elapsed;

        std::array<int, 4> a = { 24, 26, 41, 42 };
        std::vector<std::tuple<int, int>> results1;
        elapsed = time_call([&]
        {
            std::for_each(begin(a), end(a), [&](int n) {
                results1.push_back(std::make_tuple(n, fibonacci(n)));
            });
        });
        std::wcout << L"serial time: " << elapsed << L" ms" << std::endl;

        concurrency::concurrent_vector<std::tuple<int, int>> results2;
        // Use the parallel_for_each algorithm to perform the same task.
        elapsed = time_call([&]
        {
            concurrency::parallel_for_each(begin(a), end(a), [&](int n) {
                results2.push_back(std::make_tuple(n, fibonacci(n)));
            });

            // Because parallel_for_each acts concurrently, the results do not 
            // have a pre-determined order. Sort the concurrent_vector object
            // so that the results match the serial version.
            std::sort(begin(results2), std::end(results2));
        });
        std::wcout << L"parallel time: " << elapsed << L" ms" << std::endl << std::endl;

        // Print the results.
        std::for_each(std::begin(results2), std::end(results2), [](std::tuple<int, int>& pair) {
            std::wcout << L"fib(" << std::get<0>(pair) << L"): " << std::get<1>(pair) << std::endl;
        });
    }
}
void ppl_main()
{
    ppl_::main();
}
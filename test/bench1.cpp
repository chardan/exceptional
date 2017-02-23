
//          Copyright Jesse Williamson 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/* 
Benchmark a "normal" function and/or try-catch block, without our library. 
*/

#include "bench_lib.hpp"

#include <set>
#include <map>
#include <string>
#include <chrono>
#include <iostream>

using namespace std;

using timings_type = map<string, map<int, chrono::steady_clock::duration>>;

int std_except_handler(std::exception& e)
{
 std::cerr << "exception caught: " << e.what() << '\n';
 return 0;
}

/* Touch a volatile variable declared in another compilation unit. The /hope/ is
that the compiler won't actually see it... */
int f(const int i) noexcept
{
 return attempt_bench_data_to_touch ^= static_cast<bool>(i);
}

auto test_plain_call(const int i) noexcept
{
 return f(i);
}

/* Exception handler is present, but f() will never actually throw:
	- presumably, the compiler can optimize this...: */
auto test_plain_call_with_exception_handler(const int i)
{
 try
  {
	return f(i);
  }
 catch(std::exception& e)
  {
	return std_except_handler(e);
  }
}

// throws when value is 0:
int g(const int i)
{
 if(0 == i)
  throw std::exception();

 return f(i);
}

auto test_plain_call_throw(const int i)
{
 try
  {
	return g(i);
  }
 catch(std::exception& e)
  {
	return std_except_handler(e);
  }
}

template <typename FnT>
auto test(FnT& f, const unsigned int ncalls)
{
 using namespace std;

 auto start = chrono::steady_clock::now();

 // The idea here is that touching the external, volatile variable can't be optimized away:
 for(unsigned int i = 0; ncalls > i; ++i)
  f(i);

 return chrono::steady_clock::now() - start;
}

template <typename FnT>
auto time_test(const int& round, FnT& f, const unsigned int ncalls = 10'000'000)
{
 return make_pair(round, test(f, ncalls));
}

int main()
{
 timings_type timings;

 for(int round = 1; 5 >= round; ++round)
  {
	std::cout << "* round " << round << ":\n";

	timings["test_plain_call"].insert(time_test(round, test_plain_call));
	timings["test_plain_call_with_exception_handler"].insert(time_test(round, test_plain_call_with_exception_handler));
	timings["test_plain_call_throw"].insert(time_test(round, test_plain_call_throw));
  }

 for(const auto& timing : timings)
  {
	cout << "==== " << timing.first << ":\n";

	const auto& test = timing.second;

	for(const auto& round : test)
	 {
		cout << " " << round.first << ":\n";
		cout << chrono::duration <double, milli> (round.second).count() << " ms" << endl; 
	 }
  }
}

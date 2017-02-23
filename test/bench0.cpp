
//          Copyright Jesse Williamson 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/* Please note that many things need to change about this benchmark for it to be
meaningful. */

#include "exceptional.hpp"

#include <chrono>
#include <iostream>

using std::cout;

int std_except_handler(std::exception& e)
{
 std::cerr << "exception caught: " << e.what() << '\n';
 return 0;
}

int f(const int i)
{
 static int nums[] = { 1, 2, 3, 4, 5 };
 static int n = 0;

 return n++, n %= sizeof(nums), n * i;
}

auto test_plain_call(const int i)
{
 return f(i);
}

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

auto test_attempt_call(const int i)
{
 static auto fn = exceptional::attempt(f, std_except_handler);

 return *(fn(i));
}

// throws when value is 0:
int g(const int i)
{
 static int nums[] = { 1, 2, 3, 4, 5 };
 static int n = 0;

 if(0 == i)
  throw std::exception();

 return n++, n %= sizeof(nums), n * i;
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

auto test_attempt_call_throw(const int i)
{
 static auto fn = exceptional::attempt(g, std_except_handler);

 return *(fn(i));
}

template <typename FnT>
void test(const std::string& name, FnT& f)
{
  using namespace std;

 auto start = chrono::steady_clock::now();

 for(unsigned int i = 0; 10000000 > i; ++i)
  std::cerr << f(i) << '\n';

 auto end = chrono::steady_clock::now();
 
 auto diff = end - start; 

 cout << "==== " << name << ":\n";
 cout << chrono::duration <double, milli> (diff).count() << " ms" << endl; 
}

int main()
{
 for(int round = 1; 5 >= round; ++round)
  {
	std::cout << "* round " << round << ":\n";

	test("test_plain_call", test_plain_call);
	test("test_plain_call_with_exception_handler", test_plain_call_with_exception_handler);
	test("test_attempt_call", test_attempt_call);
	
	test("test_plain_call_throw", test_plain_call_throw);
	test("test_attempt_call_throw", test_attempt_call_throw);
  }
}

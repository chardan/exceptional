
//          Copyright Jesse Williamson 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/*
Possible GCC bug. Works as expected with clang. 

This is not-quite-minimal, but does point out the verified bug; discussion here:
https://gcc.gnu.org/bugzilla/show_bug.cgi?id=79249
*/

#include <iostream>
#include <functional>

template <typename FnT, typename HandlerT0>
auto attempt(FnT f, HandlerT0 h)
{
 return [f, h](auto&& ...xs) 
        {
                try
                {
			return f(std::forward<decltype(xs)>(xs)...);
                }
                catch(std::exception& e) 
                {
			return h(e);
                }
        };
}

int std_except_handler(std::exception& e)
{
 std::cerr << "exception caught: " << e.what() << '\n';
 return 0;
}

// More complex behavior does not seem to matter here, however
// removing "noexcept" results in GCC generating a program that works as expected:
int f_ident(const int i) noexcept
{
 return i;
}

// Even though this is not actually referenced by the other code, removing 
// it causes GCC to generate a program that works as expected (the presence of
// "static" seems to make no difference).
auto test_attempt_call_with_exception_handler(const int i)
{
 static auto fn_ = attempt(f_ident, std_except_handler);
 return fn_(i);
}

// throws when value is 0:
int g(const int i)
{
 if(0 == i)
  throw std::exception();

 return f_ident(i);
}

int main()
{
 auto f = attempt(g, std_except_handler);

 f(0);
}

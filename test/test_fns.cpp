
//          Copyright Jesse Williamson 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#define CATCH_CONFIG_MAIN       // tell Catch to handle the interface
#include "catch.hpp"

#include "exceptional.hpp"

#include <string>
#include <sstream>
#include <utility>
#include <cstring>
#include <iostream>
#include <stdexcept>

/* Test function that maybe-throws: */
std::string maybe_throw(const std::string& s, const float f)
{
 std::ostringstream os;
 os << s << ": " << f;

 if("throw" == s)
  throw std::runtime_error("exception thrown");

 return os.str();
}

/* Test function that takes no parameters: */
std::string void_fn()
{
 return "hello from void_fn()";
}

/* Test function that takes one parameter: */
std::string f_one(const std::string& s)
{
 return std::string("Hello, ") + s;
}

/* Variadic test function: */
std::string concat()
{
 return std::string();
}

template <class ...XS>
std::string concat(const std::string& s, const XS& ...xs)
{
 return s + concat(xs...);
}

/* A handler that returns the same type as the function: */
std::string handler(std::exception& e)
{
 return std::string("value from handler(): ") + e.what();
}

/* A handler that returns a converting type: */
const char *convert_handler(std::exception&)
{
 static const char *s = "Hello from convert_handler()";
 return s;
}

/* A handler that returns nothing: */
void void_handler(std::exception& e)
{
 std::string s = std::string("void handler() called, caught: ") + e.what();
 INFO(s);
}

TEST_CASE("test the test functions", "[test_basis]")
{
 SECTION("check maybe_throw") {
        REQUIRE_NOTHROW(maybe_throw("Zorble", 1.0));
        REQUIRE_THROWS_AS(maybe_throw("throw", 1.0), std::exception);

        REQUIRE("Zorble: 1" == maybe_throw("Zorble", 1.0));
 } 

 SECTION("test void_fn") {
        REQUIRE("hello from void_fn()" == void_fn());
 }

 SECTION("test f_one") {
        REQUIRE("Hello, f_one()" == f_one("f_one()"));
 }

 SECTION("test concat") {
        REQUIRE("" == concat(""));
        REQUIRE("a" == concat("a"));
        REQUIRE("ab" == concat("a", "b"));
        REQUIRE("abc" == concat("a", "b", "c"));
        REQUIRE("abc" == concat("ab", "c"));
        REQUIRE("abc" == concat("a", "bc"));
        REQUIRE("abcdefg" == concat("a", "b", "c", "d", "e", "f", "g"));
 }

 SECTION("test handlers") {
        std::runtime_error re("identity");

        REQUIRE("value from handler(): identity" == handler(re));

        REQUIRE(0 == strcmp("Hello from convert_handler()", convert_handler(re)));

        REQUIRE(1 == (void_handler(re), 1));       // just make sure we can call it
 }
}

SCENARIO("test attempt", "[test_attempt]")
{
 using std::cout;

 GIVEN("an attempt wrapper with no handler")
 {
        auto opt0 = exceptional::attempt(maybe_throw);

        WHEN("the wrapper-function is called")
        {
              auto val0 = opt0("hello", 2.0);

               THEN("there should be a value sent back")
               {
                        REQUIRE(val0);
                        REQUIRE("hello: 2" == *val0);
               }
        }
        AND_WHEN("the wrapper-function is asked to throw")
        {
                THEN("the exception should not be caught")
                {
                        REQUIRE_THROWS_AS(opt0("throw", 1.0), std::exception);
                }
        }
 }

 GIVEN("an attempt wrapper and handler")
 { 
        auto opt0 = exceptional::attempt(maybe_throw, handler);

        WHEN("the wrapper-function is called")
        {
               auto val0 = opt0("hello", 2.0);

               THEN("there should be a value sent back")
               {
                        REQUIRE(val0);
                        REQUIRE("hello: 2" == *val0);
               }
        }
        AND_WHEN("the wrapper-function is asked to throw")
        {
                auto val1 = opt0("throw", 2.5);

                THEN("there should also be a value")
                {
                        REQUIRE(val1);
                        REQUIRE("value from handler(): exception thrown" == *val1);
                }
        }
 }

 GIVEN("an attempt wrapper and a void-handler")
 {
        auto opt0 = exceptional::attempt(maybe_throw, void_handler);

        WHEN("the wrapper-function is asked to throw")
        {
                auto val0 = opt0("throw", 32.2);

                THEN("we should not get back a value")
                {
                        REQUIRE(!val0);
                }
        }
 }
}

/* JFW: TODO: these all follow pretty much the same pattern, should be possible
to squish them... */
SCENARIO("test attempt n-ary calls", "[test_attempt_arity]")
{
 GIVEN("a nullary function")
 {
        auto opt1 = exceptional::attempt(void_fn, handler);

        WHEN("we call it")
        {
                auto val1 = opt1();

                THEN("we should get a value back")
                {
                        REQUIRE(val1);
                        REQUIRE("hello from void_fn()" == *val1);
                }
        }
 }

 GIVEN("an unary function")
 {
        auto opt2 = exceptional::attempt(f_one, handler);

        WHEN("we call it")
        {
                auto val2 = opt2("marmot");
        
                THEN("we should get a value back")
                {
                        REQUIRE(val2);
                        REQUIRE("Hello, marmot" == *val2);
                }
        }
 }

 GIVEN("a variadic function")
 {
        auto strmeow = exceptional::attempt(*concat<std::string, std::string>, handler);

        WHEN("we call it")
        {
                auto out = strmeow("hello", "there", "world");

                THEN("we should see the result")
                {
                        REQUIRE(out);
                        REQUIRE("hellothereworld" == *out);
                }
        }
 }
}

SCENARIO("test attempt multiple/nested calls", "[test_attempt_multiple]")
{
 GIVEN("multiple attempt functions")
 {
        auto fn0 = exceptional::attempt(f_one, handler);
        auto fn1 = exceptional::attempt(f_one, handler);

        WHEN("we call them")
        {
                auto r0 = fn0("hi");
                auto r1 = fn1("there");

                THEN("an exception should not stop the program")
                {
                        REQUIRE(true);
                }
        }
 }
} 
/*
JFW: TODO: Other Callable entities are going to take work-- lambdas don't have
the same type traits as std::function, for example, and boost::function_types doesn't handle
them (and perhaps can't?): 
SCENARIO("test attempt lambda calls", "[test_attempt_lambda]")
{
 GIVEN("a lambda function")
 {
        auto fn = [](const std::string s) { return s + ", world!"; };

        auto afn = attempt::attempt(fn, handler);

        afn(std::string("hi"));

        WHEN("we call it")
        {
                auto val = afn("Hello");

                THEN("we should get a value back")
                {
                        REQUIRE(val);
                        REQUIRE("Hello, world!" == *val);
                }
        }
 }

 GIVEN("a lambda handler")
 {
 }
}
*/

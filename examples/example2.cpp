
/* 

attempt<> is an alternative interface for exceptions which translates exceptions
into optional values. This is useful when it is undesirable to leak certain
exceptions, when a function would be mired with repetetive try-catch blocks, or
when exceptions need to be swallowed (rare, but for example some FFI boundaries).
Another use is in providing function-level exception checked boundaries, so that
the types of exceptions checked are guaranteed at the level of each function call
entry.

Basic usage of attempt<>:

	- define a function (your "happy path")
	- define one or more exception handlers

In this example, we'll show how to use multiple exception handlers to take different
actions depending on the kind of exception caught.

*/

#include "exceptional.hpp"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include <experimental/optional>

using std::cout;
using std::string;
using std::vector;
using std::experimental::optional;

// A "dummy" type representing a user exception:
struct some_user_exception	{};

// Here is the function that we want to use. It opens a file and reads the first line:
string read_first_line(const string& fname);

void handle_POD_exception(int& e);
void handle_read_error(std::exception& e);
void handle_runtime_error(std::runtime_error& e);
void handle_user_exception(some_user_exception& e);

int main(int argc, char *argv[])
{
 using exceptional::attempt;

 std::cout << "Important: this example can feel confusing-- it throws different exception types based on runtime behavior.\n";

 vector<string> args(1 + argv, argc + argv);

 if(1 != args.size())
  {
	cout << "example0 \n";
	return 1;
  }

 auto first_line_reader = attempt(read_first_line, 
				  handle_POD_exception, handle_user_exception, handle_runtime_error, handle_read_error);

// JFW: ...this lambda function currently segfaults...
 optional<string> first_line = first_line_reader(args[0]);

 if(first_line)
  cout << "The first line is:\n" << *first_line << '\n';
}

string read_first_line(const string& fname)
{
 std::ifstream infile(fname);

 if(!infile)
  throw std::runtime_error("unable to open input file");

 string result;

 getline(infile, result);

 // ...quite a contrivance... 
 if(10 < result.length())
  throw 5; // JFW

 if(20 < result.length())
  throw some_user_exception(); // JFW

 return result;
}

void handle_read_error(std::exception& e)
{
 std::cerr << "Error (handle_read_error()): " << e.what() << '\n';
}

void handle_runtime_error(std::runtime_error& e)
{
 std::cerr << "*** handle_runtime_error1(): std::runtime_error: Error:" << e.what();
}

void handle_POD_exception(int& e)
{
 std::cout << "Ex: " << e << '\n';
 std::cerr << "Error (handle_POD_exception()): caught POD exception\n";
}

void handle_user_exception(some_user_exception& e)
{
 std::cout << "sizeof(e): " << sizeof(e) << '\n';
 std::cerr << "Error (handle_user_exception()): caught user exception\n";
}

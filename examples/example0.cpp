
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
	- define an exception handler

In this example, a handler which does not produce a return value when an exception is caught
is used. Instead, it logs a message to the error console and ensures that there is no return
value. This kind of handler is useful for situations in which default construction of the
attempted function's return type would either be valid data or have no meaning.

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

// Here is the function that we want to use. It opens a file and reads the first line:
string read_first_line(const string& fname);

// Here's what we will do if we catch a std::exception:
void report_read_error(std::exception& e);

int main(int argc, char *argv[])
try
{
 vector<string> args(1 + argv, argc + argv);

 if(1 != args.size())
  {
	cout << "example0 <filename>\n";
	return 1;
  }

 auto first_line_reader = exceptional::attempt(read_first_line, report_read_error);

 optional<string> first_line = first_line_reader(args[0]);

 /* ...if an exception was thrown, the handler was called and we do not have output: */
 if(first_line)
  cout << "The first line is:\n" << *first_line << '\n';
 else
  cout << "Failed to read the first line!\n";
}
catch(...)
{
 std::cerr << "Unhandled exception.\n";
}

string read_first_line(const string& fname)
{
 std::ifstream infile(fname);

 if(!infile)
  throw std::runtime_error("unable to open input file");

 string result;

 getline(infile, result);

 return result;
}

void report_read_error(std::exception& e)
{
 std::cerr << "Oops! " << e.what() << '\n';
}

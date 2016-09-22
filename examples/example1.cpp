
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

In this example, an exception handler which guarantees a return value of the same
type as the attempted function is shown. This is useful in a situation when a
return type should always be produced.

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
string handle_read_error(std::exception& e);

int main(int argc, char *argv[])
{
 vector<string> args(1 + argv, argc + argv);

 if(1 != args.size())
  {
	cout << "example0 <filename>\n";
	return 1;
  }

 auto first_line_reader = exceptional::attempt(read_first_line, handle_read_error);

 optional<string> first_line = first_line_reader(args[0]);

 /* ...we should always have output: */
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

 return result;
}

string handle_read_error(std::exception& e)
{
 std::cerr << "Error (caught by handle_read_error()): " << e.what() << '\n';

 return "This is a default value that shows up when there's an error.";
}

#include "exceptional.hpp"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

struct user_exception {};

std::string do_something(const std::string& cmd)
{
 if("ok" == cmd)
  return "* success (no exception)";

 if("exception" == cmd)
  throw std::runtime_error(cmd);

 if("user-exception" == cmd)
  throw user_exception();

 throw std::runtime_error("unknown command");
}

// Handle a std::exception, return failure with no output:
void handle_ex(std::exception& e)
{
 std::cout << "* caught std::exception: " << e.what() << '\n';
}

// Handle a user_exception, produce some output:
std::string handle_user_ex(user_exception&)
{
 std::cout << "* caught user_exception; rewriting output" << '\n';

 return "I decided to return a valid result!";
}

auto demo(const std::string& cmd)
{ 
 using exceptional::attempt;

 auto checked_demo = attempt(do_something, handle_ex, handle_user_ex);

 auto result = checked_demo(cmd);

 return result ? *result : "function failed (no output)";
}

int main(int argc, char *argv[])
{
 std::vector<std::string> args(1 + argv, argc + argv);

 if(1 != args.size())
  {
	std::cout << "example0 [ok|exception|user-exception]\n";
	return 1;
  }

 std::cout << demo(args[0]) << '\n';
}

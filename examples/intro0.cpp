
//          Copyright Jesse Williamson 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/* Example with traditional exception handling: */

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

struct user_exception {};

auto demo(const std::string& cmd)
{
 using namespace std;

 try
  {
	if("ok" == cmd)
	 return "* success (no exception)";

	if("exception" == cmd)
	 throw std::runtime_error(cmd);

	if("user-exception" == cmd)
	 throw user_exception();
  }
 catch(std::exception& e)
  {
	std::cout << "* caught std::exception: " << e.what() << '\n';
  }
 catch(user_exception& e)
  {
	std::cout << "* caught user_exception" << '\n';
  }

 // Unknown command:
 throw std::runtime_error("unknown command");
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

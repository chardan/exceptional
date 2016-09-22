
/* Canonical Lippincott Function, adapted from ("http://cppsecrets.blogspot.com/2013/12/using-lippincott-function-for.html"). */

#include <iostream>

struct MyException1 {};
struct MyException2 {};

enum class foo_Result : int { FOO_UNKNOWN, FOO_OK, FOO_ERROR1, FOO_ERROR2 };

struct foo
{
 static int DoThing(const std::string& cmd)
 {
	if("throw1" == cmd)
	 throw MyException1();

	if("throw2" == cmd)
	 throw MyException2();

	if("throw_unknown" == cmd)
	 throw std::runtime_error("unknown exception (by user request!)");

	if("ok" != cmd)
	 throw std::runtime_error("oops, bad command");

	std::cout << "Hello, World!\n";

	return 0;
 }
};

foo_Result lippincott()
{
    try
    {
        throw;
    }
    catch (const MyException1&)
    {
	std::cerr << "Caught MyException1\n";
        return foo_Result::FOO_ERROR1;
    }
    catch (const MyException2&)
    {
	std::cerr << "Caught MyException2\n";
        return foo_Result::FOO_ERROR2;
    }
    catch (...)
    {
	std::cerr << "Caught unknown exception\n";
        return foo_Result::FOO_UNKNOWN;
    }
}
 
foo_Result foo_dothing(const std::string cmd)
{
    try
    {
        foo::DoThing(cmd);
        return foo_Result::FOO_OK;
    }
    catch (...)
    {
        return lippincott();
    }
}

int main(int argc, char *argv[])
{
 if(1 == argc)
  {
	std::cerr << "usage: " << argv[0] << ": [ok|throw1|throw2|throw_unknown]\n";
	return 1;
  }

 return static_cast<int>(foo_dothing(argv[1]));
}

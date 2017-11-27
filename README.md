# exceptional

## A C++ tool for function-oriented exception handling. 
Map your exception handling into functions! 
Let all your dreams come true! 
Amaze your friends, confound your enemies!

* Easily compose checked functions, with return and exception types deduced from function signatures:
```
// Given:
string read_first_line(const string&);
string handle_read_error(exception&);

// Construct a new function:
auto checked = exceptional::attempt(read_first_line, handle_read_error);

// Usage:
auto first_line = checked("input.txt");

// ...we now have an optional value back:
return first_line ? *first_line : "function failed (no output)";
```

attempt() is variadic, you may pass in multiple handlers (ie. "attempt(f, h0, h1, h2)").

## Dependencies:
---

* C++14. It is likely possible to port the library to C++11.
	- The implementation currently uses some experimental C++17 goodies; you'll need a standard
	library that supports those.

	- The earliest versions of this used Boost.Preprocessor and MPL to more-or-less
	work in C++98, but frankly the result wasn't satisfying. If there's actual demand,
	it could be revisted.

* There are limitations with GCC in C++14 mode, hopefully they will be resolved:
	- https://gcc.gnu.org/bugzilla/show_bug.cgi?id=79249

* Dependencies:
	- Boost function_traits.

* Dependencies for tests:
	- Catch.
	- Rapidcheck is not yet needed, but hopefully will be soon.

* If you want to build the tests:

	mkdir deps && cd deps
	git clone https://github.com/emil-e/rapidcheck.git
	git clone https://github.com/philsquared/Catch.git

	...I can probably be talked in to making these submodules.

## Limitations and Notes:

* Limitation: An exception handler taking void should generate "catch(...)". If that's too 
problematic, a type tag (attempt::catch_all()) is fine. (I thinking investigating void_t or something similar may be a good start.) I
think this is probably the most significant feature missing right now.

## Contributing:

* There's plenty of work remaining-- please see the TODO file. 

## ACKNOWLEDGEMENTS:
---

For invaluable involvement and feedback, I'd especially like to thank:
Marshall Clow, Pablo Helpern, Erich Keane, Lisa Lippincott, Thiago Maciera, Eric Niebler, Josh Triplett.

I'm very appreciative of my employer, SUSE, for allowing me to spend time on this project during Hack Week.


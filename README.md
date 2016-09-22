
#exceptional

##A C++ tool for function-oriented exception handling. Map your exception handling into functions!

```
// Given:
string read_first_line(const string&);
string handle_read_error(std::exception&);

// Construct a new function:
auto checked = attempt(read_first_line, handle_read_error);

// Usage:
auto first_line = checked("input.txt");

return first_line ? *first_line : "function failed (no output)";
```

# Dependencies:
---

* C++14. It is likely possible to port the library to C++11.
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

## Limitations, TODO, Suggested Projects:

* Limitation: An exception handler taking void should generate "catch(...)". If that's too 
problematic, a type tag (attempt::catch_all()) is fine. (I thinking investigating void_t or something similar may be a good start.) I
think this is probably the most significant feature missing right now.

* Possible bug: checking conversion on return types is decide whether or not to ignore them seemed cute, but now I think I
* More examples would be nice. 

* Replace boost::function_traits with the standard library type_traits, or something without an external dependency.

had it right the first time: just check for void. That way, the compiler can report as an error any rejection of
return type-- right now, results are ignored when that code is activated, and I think the only appropriate type for
that is void. (detail.hpp, Maybe<... enable_if_t... is_constructible>>). Opinions?

* Policy classes should control behavior of catching. For example, there's no reason to be married to std::optional<>.
	- Ideally, the existing Maybe monad could return whatever the user wanted.

* It would be nice to multiple exception types in one handler. For example, a signature:
	void handler(std::exception&, user_exception&);
...should be invoked to catch either a thrown std::exception& or user_exception&.

* Improvements to the build system are welcomed; I'm not that cmake saavy.
	- the library itself should remain header-only, if possible.

* RapidCheck ALL THE THINGS! :-)
	- RapidCheck is a QuickCheck style tester for C++.

* Any Callable type should be acceptable (although by-reference sounds pretty dangerous!)-- including functions
  that are moved into the closure.

* It may be that the general case of finding a function signature automatically isn't direclty possible (eg. overloaded
  functions). Find out if this is the case.

* Limitation: It would be nice to be able to take lambda functions. Unfortunately, the mechanisms I'm using can't deduce lambda type signatures (perhaps because they aren't named objects, and can't have their address taken).
	- perhaps a lambda-wrapper workaround is possible?

* Improved benchmarks, more scenarios, etc..
	The initial benchmark is pretty quick and dirty. My early measurements are encouraging, showing surprisingly comprable times for
	attempt-enabled functions and raw functions when optimization is on. I wasn't able to include GCC at this time.

	Some improvements suggested so far include (and are at least partially implemented):
		- split the test functions into different modules, to avoid effects from
		compiler optimization, especially inlining (Lippincott).
		- --force-no-inline (Keane).
		- write to a volatile variable in a separate compilation unit; the compiler probably can't see it (Helpern).
		- separate measurment of the overhead of optional, lambda, and n-depth nesting

	...that said, I'm of course most interested in how the compiler /can/ optimize it! Nonetheless, I'd like to get a handle on the overhead of using this method, itself. Improvements welcomed.

# ACKNOWLEDGEMENTS:
---

For invaluable involvement and feedback, I'd especially like to thank:
Marshall Clow, Pablo Helpern, Erich Keane, Lisa Lippincott, Thiago Maciera, Eric Niebler, Josh Triplett.

I'm very appreciative of my employer, SUSE, for allowing me to spend time on this project during Hack Week.


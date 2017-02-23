
#          Copyright Jesse Williamson 2017.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

//CXX=g++-6 --std=c++14
CXX=clang -std=c++14 -lstdc++
CXXFLAGS=-Wall -Wextra -pedantic

CXXINC=-I../include -I../deps/Catch/include

CXX_CC=$(CXX) $(CXXFLAGS) -ggdb

all:
	$(CXX_CC) -o intro0 intro0.cpp $(CXXINC)
	$(CXX_CC) -o intro1 intro1.cpp $(CXXINC)

clean:
	rm -fv example0 example1 example2 intro0 intro1 lippincott0

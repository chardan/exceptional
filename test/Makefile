
#          Copyright Jesse Williamson 2017.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)


CXX_CLANG=clang --std=c++1y -lstdc++ 
CXX_GCC=g++ --std=c++14 

CXX=$(CXX_CLANG) 
#CXX=$(CXX_GCC)

BENCH_CXX_CC=$(CXX_CLANG) -O0
#BENCH_CXX_CC=$(CXX_GCC) -O0

CXXFLAGS=-Wall -Wextra -pedantic 

CXXINC=-I../include -I../deps/Catch/include

CXX_CC=$(CXX) $(CXXFLAGS)

all:	unit bench bug

unit:
	$(CXX_CC) -o ./test_fns ./test_fns.cpp $(CXXINC)

bench:
	$(CXX_CC) -c -o ./bench_lib.o ./bench_lib.cpp 

	$(BENCH_CXX_CC) -O0 -o ./bench0-O0 ./bench0.cpp $(CXXINC)

	$(BENCH_CXX_CC) -O0 -o ./bench1-O0 ./bench1.cpp ./bench_lib.o
	$(BENCH_CXX_CC) -O0 -o ./bench2-O0 ./bench2.cpp ./bench_lib.o $(CXXINC)

	$(BENCH_CXX_CC) -O3 -o ./bench1-O3 ./bench1.cpp ./bench_lib.o
	$(BENCH_CXX_CC) -O3 -o ./bench2-O3 ./bench2.cpp ./bench_lib.o $(CXXINC)

bug:
	$(CXX_GCC) -o ./minimal_break-gcc ./minimal_break.cpp $(CXXINC)
	$(CXX_CLANG) -o ./minimal_break-clang ./minimal_break.cpp $(CXXINC)

clean:
	rm -fv bench0 bench1 bench2 test_fns bench_lib.o
	rm -rf bench0-O0 bench1-O0 bench2-O0 bench1-O3 bench2-O3
	rm -fv minimal_break-gcc minimal_break-clang bench_lib_gcc.o bench_lib.clang.o 


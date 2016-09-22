//CXX=g++-6 --std=c++14
CXX=clang -std=c++14 -lstdc++
CXXFLAGS=-Wall -Wextra -pedantic

CXXINC=-I../include -I../deps/Catch/include

CXX_CC=$(CXX) $(CXXFLAGS) -ggdb

all:
	$(CXX_CC) -o intro0 intro0.cpp $(CXXINC)
	$(CXX_CC) -o intro1 intro1.cpp $(CXXINC)

	$(CXX_CC) -o example0 example0.cpp $(CXXINC)
	$(CXX_CC) -o example1 example1.cpp $(CXXINC)
	$(CXX_CC) -o example2 example2.cpp $(CXXINC)

	$(CXX_CC) -o lippincott0 lippincott0.cpp $(CXXINC)

clean:
	rm -fv example0 example1 example2 intro0 intro1 lippincott0

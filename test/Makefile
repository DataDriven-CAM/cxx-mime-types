
all: LDFLAGS= -ldl -pthread
all: main
	g++ $(LDFLAGS) -o main $(wildcard build/src/*.o)


main: CXXFLAGS= -O3 -pthread -std=c++2a -I../include -I../node_modules/rapidjson/include -I../node_modules/doctest/doctest
main: LDFLAGS= 
main: src/main.cpp
	g++ $(CXXFLAGS) -c -o build/src/main.o src/main.cpp

clean:
	cd node_modules/zlib && $(MAKE) clean
	cd node_modules/openssl && $(MAKE) clean


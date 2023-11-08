
all: LDFLAGS= -L../cpp_modules/json-thresher/ -ljsonthresher -ldl -pthread
all: build/src/main.o
	$(CXX) $(LDFLAGS) -o main $(wildcard build/src/*.o)


build/src/main.o: CXXFLAGS= -O3 -pthread -std=c++2a -I../include -I../cpp_modules/json-thresher/include -I../cpp_modules/fmt/dist/include -I../cpp_modules/doctest/doctest -MMD
build/src/main.o: LDFLAGS= 
build/src/main.o: src/main.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o build/src/main.o src/main.cpp

clean:
	cd node_modules/zlib && $(MAKE) clean
	cd node_modules/openssl && $(MAKE) clean


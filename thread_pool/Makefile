.PHONY: test clean

run: test
	./test

test: main.cpp thread_pool.hpp
	g++ -std=c++11 main.cpp -o test


clean:
	rm -rf test *.o

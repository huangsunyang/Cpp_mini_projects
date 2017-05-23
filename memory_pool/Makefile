.PHONY: clean

run: main
	./main

main: main.cpp StackAlloc.hpp
	g++ -std=c++11 main.cpp -o main

clean:
	rm -rf *.o main


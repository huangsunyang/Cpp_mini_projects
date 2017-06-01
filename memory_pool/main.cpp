#include <iostream>
#include <cassert>
#include <ctime>
#include <vector>
#include <stack>
#include "MemoryPool.hpp"
#include "StackAlloc.hpp"

#define ELEMS 1000000
#define REPS 100


template<typename T>
double calc_time() {
    clock_t start;

    StackAlloc<int, T> stack;
    start = clock();

    // frequently push and pop elements on the stack
    for (int j = 0; j < REPS; j++) {
        assert(stack.empty());

        for (int i = 0; i < ELEMS; i++) {
            stack.push(i);
        }
        for (int i = 0; i < ELEMS; i++) {
            stack.pop();
        }
    }

    clock_t now = clock();
    return double(now - start) / CLOCKS_PER_SEC;
}


int main() {
    double time_used;
    clock_t start;

    // use default allocator
    time_used = calc_time<std::allocator<int>>();
    std::cout << "Default Allocator Time: ";
    std::cout << time_used << '\n' << std::endl; 

    // use customized memory pool
    time_used = calc_time<MemoryPool<int>>();
    std::cout << "Using Memory Pool Time: ";
    std::cout << time_used << '\n' << std::endl;

    // use vector
    start = clock();
    std::vector<int> vec;
    for (int j = 0; j < REPS; j++) {
        assert(vec.empty());

        for (int i = 0; i < ELEMS; i++) {
            vec.push_back(i);
        }
        for (int i = 0; i < ELEMS; i++) {
            vec.pop_back();
        }
    }
    std::cout << "Using vector Time: ";
    std::cout << double(clock() - start) / CLOCKS_PER_SEC << std::endl;
}




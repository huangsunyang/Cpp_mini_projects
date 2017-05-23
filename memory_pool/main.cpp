#include <iostream>
#include <cassert>
#include <ctime>
#include <vector>

//#include "MemoryPool.hpp"
#include "StackAlloc.hpp"

#define ELEMS 1000000
#define REPS 100

int main() {
    clock_t start, now;
    double time_used;

    //using stl default allocator
    StackAlloc<int, std::allocator<int>> stackDefault;
    start = clock();

    for (int j = 0; j < REPS; j++) {
        assert(stackDefault.empty());
        for (int i = 0; i < ELEMS; i++) {
            stackDefault.push(i);
        }
        for (int i = 0; i < ELEMS; i++) {
            stackDefault.pop();
        }
    }

    now = clock();
    time_used = double(now - start) / CLOCKS_PER_SEC;

    std::cout << "Default Allocator Time: ";
    std::cout << time_used << '\n' << std::endl;

    //using custom memory pool
   // StackAlloc<int, MemoryPool<int>> stackPool;
   // start = clock();

   // for (int j = 0; j < REPS; j++) {
   //     assert(stackPool.empty());
   //     for (int i = 0; i < ELEMS; i++) {
   //         stackPool.push(i);
   //     }
   //     for (int i = 0; i < ELEMS; i++) {
   //         stackPool.pop();
   //     }
   // }

   // now = clock();
   // time_used = double(now - start) / CLOCKS_PER_SEC;

   // std::cout << "Memory Pool Allocator Time: ";
   // std::cout << time_used << '\n' << std::endl;

    return 0;
}




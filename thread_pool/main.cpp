//
//  main.cpp
//  thread_pool
//
//  Created by huangsunyang on 5/20/2017.
//  Copyright © 2017年 huangsunyang. All rights reserved.
//

#include "thread_pool.hpp"

int main(int argc, const char * argv[]) {
    ThreadPool pool(4);
    std::vector<std::future<std::string>> results;
    
    for (int i = 0; i < 8; i++) {
        results.emplace_back(
            pool.enqueue([i] {
                std::cout << "hello" << i << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(16 - i * 2));
                std::cout << "world" << i << std::endl;
                return std::string("---thread") + std::to_string(i) + std::string( "finished---");
            })
        );
    }
    
    for (auto && result: results)
        std::cout << result.get() << ' ';
    std::cout << std::endl;
    return 0;
}

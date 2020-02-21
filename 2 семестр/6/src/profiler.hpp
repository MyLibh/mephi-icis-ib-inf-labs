#pragma once

#include <chrono>

template<typename _Func>
void profile(_Func&& func)
{
    auto t1 = std::chrono::high_resolution_clock::now();
    func();
    auto t2 = std::chrono::high_resolution_clock::now();

    std::cout << "Function took " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << "us" << std::endl;
}
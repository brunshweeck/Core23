//
// Created by Brunshweeck on 11/09/2023 17:23.
// Copyright (c) 2023
//

//using namespace core;
#include <atomic>
#include <Windows.h>

int main() {
    volatile LONG lg = 123;
    auto a = InterlockedCompareExchange(&lg, 321, 123);
    auto b = InterlockedCompareExchange(&lg, 213, 123);
    return 0;
}

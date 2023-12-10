//
// Created by Brunshweeck on 11/09/2023 17:23.
// Copyright (c) 2023
//

#include <core/Complex.h>
#include <core/util/PriorityQueue.h>
#include <core/util/ArrayList.h>
#include <queue>

using namespace core;
using namespace core::util;

CORE_ALIAS(U, native::Unsafe);

auto main(gint   /*argc*/, const char * /*argv*/[], const char * /*env*/[]) -> gint {
    std::priority_queue<int> p = {};
    p.push(2);
    p.push(1);
    p.push(3);
    p.push(4);
    p.push(2);
    p.push(25);
    p.push(4);
    p.push(10);
    p.push(1);
    p.push(2);
    p.push(3);
    p.push(5);
    p.push(8);
    p.push(9);
    p.push(9);
    p.push(9);
    p.push(9);
    return 0;
}

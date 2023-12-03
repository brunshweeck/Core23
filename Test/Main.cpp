//
// Created by Brunshweeck on 11/09/2023 17:23.
// Copyright (c) 2023
//

#include <core/util/ArrayList.h>
#include <core/util/LinkedList.h>
#include <core/Complex.h>

using namespace core;
using namespace core::util;

void Main(const ReferenceArray<String> &args);

int main(int argc, const char *argv[], const char *env[]) {
    ReferenceArray<String> ra(argc);
    for (int i = 0; i < argc; ++i) {
        ra.set(i, argv[i]);
    }
    Main(ra);
    return 0;
}


void Main(const ReferenceArray<String> &args) {
    LinkedList<Integer> ll = {};
    ArrayList<Integer> al = {};
    for (int i = 0; i < 100; ++i) {
        ll.add(100 - i);
        al.add(100 - i);
    }
}

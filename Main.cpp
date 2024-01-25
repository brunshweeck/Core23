//
// Created by Brunshweeck on 11/09/2023 17:23.
// Copyright (c) 2023
//

#include <iostream>
#include <core/io/File.h>

using namespace core;
using namespace util;
using namespace charset;
using namespace native;
using namespace io;
using namespace charset;
using namespace std;

namespace core {
    namespace util {

    }
}

void println(const Object &obj) {
    String const str = String::valueOf(obj) + "\n"_S;
    CharArray const ca = str.chars();
    wcout << (wchar_t *) &ca[0];
    fflush(stdout);
    fflush(stderr);
    fflush(stdin);
}

gint main() noexcept(false) {
    Charset const& cs = Charset::forName("windows-1252"_S, Charset::defaultCharset());
    auto &res = cs.encode("les élèves de ma mère mangent du maïs"_S);
    println(res);
    return 0;
}

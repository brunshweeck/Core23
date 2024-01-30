//
// Created by Brunshweeck on 11/09/2023 17:23.
// Copyright (c) 2023
//

#include <core/io/File.h>
#include <core/time/Chrono.h>
#include <core/Character.h>
#include <core/foreign/CIterator.h>
#include <iostream>

using namespace core;
using namespace util;
using namespace time;
using namespace function;
using namespace foreign;

template<class T>
void println(T &&obj) {
    String const str = String::valueOf(obj) + "\n"_S;
    CharArray const ca = str.chars();
    std::wcout << (wchar_t *) &ca[0];
    fflush(stdout);
}

class A : public Object {
    Character c[100];
public:
    A() : c{
            1, 2, 3, 4, 5, 6, 7, 8, 9, 0,
            1, 2, 3, 4, 5, 6, 7, 8, 9, 0,
            1, 2, 3, 4, 5, 6, 7, 8, 9, 0,
            1, 2, 3, 4, 5, 6, 7, 8, 9, 0,
            1, 2, 3, 4, 5, 6, 7, 8, 9, 0,
            1, 2, 3, 4, 5, 6, 7, 8, 9, 0,
            1, 2, 3, 4, 5, 6, 7, 8, 9, 0,
            1, 2, 3, 4, 5, 6, 7, 8, 9, 0,
            1, 2, 3, 4, 5, 6, 7, 8, 9, 0,
            1, 2, 3, 4, 5, 6, 7, 8, 9, 0,
    } {}

    CIterator<Character> begin() {
        A &This = *this;
        return CIterator<Character>(
                *this,
                BiPredicate<Object &, Object>::from([](Object &This, Object const &fields) -> gbool {
                    LongArray const &attr = (const LongArray &) fields;
                    A &a = (A &) This;
                    return attr[0] < sizeof(a.c) / sizeof(Character);
                }),
                BiFunction<Object &, Object &, Character &>::from([](Object &This, Object &fields) -> Character & {
                    LongArray &attr = (LongArray &) fields;
                    A &a = (A &) This;
                    return a.c[attr[1] = attr[0]++];
                }),
                LongArray::of(0 /*cursor*/, -1 /*last index*/)
        );
    }

    CIterator<Character> end() {
        return CIterator<Character>(*this);
    }
};

gint main() noexcept(false) {
    Chrono const chrono = Chrono(true);

    A a = {};
    gint j = 0;
    for (auto &c: a) {
        println(Character::valueOf(c + 65));
        j += 1;
    }

    println(j);

    println("Duration: "_S + Duration::ofMillis(chrono.elapsedTime()).toString());
    return 0;
}


class Any {
    class Value {
    public:
        virtual ~Value() = default;
    };

    template<class T>
    class SpecialValue: public Value {
    private:
        const T &t;

    public:
        CORE_EXPLICIT SpecialValue(const T &t) : t(t) {}
    };

    template<>
    class SpecialValue<String>: public Value {
    private:
        String t;

    public:
        CORE_EXPLICIT SpecialValue(const String &t) : t(t) {}
    };

    Value *v = null;

    template<class T>
    CORE_EXPLICIT Any(T &&t) {
        v = new SpecialValue<T>(Unsafe::forwardInstance<t>());
    }

    CORE_EXPLICIT Any(const Object &o) {
        v = new SpecialValue<Object>(o);
    }

    CORE_EXPLICIT Any(const String &o) {
        v = new SpecialValue<const String &>(o);
    }
};

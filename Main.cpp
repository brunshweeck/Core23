//
// Created by Brunshweeck on 11/09/2023 17:23.
// Copyright (c) 2023
//

#include <core/util/TreeMap.h>
#include <core/Complex.h>
#include <core/Byte.h>
#include <core/Character.h>
#include <core/util/HashSet.h>
#include <core/util/TreeSet.h>
#include <core/native/BooleanArray.h>
#include <core/native/StringArray.h>
#include <core/Enum.h>

using namespace core;
using namespace core::util;

CORE_ALIAS(U, native::Unsafe);

gint main() {
    Complex c = 2 + 5J;
    HashSet<Complex> hashSet = {};
    hashSet.addAll(hashSet);
    TreeSet<Complex> treeSet = {};
    String strings[50] = {};
    StringArray array = StringArray::fromAddress((glong) strings, 50);
    StringArray array1 = StringArray::of("", "hello world", "hello my mother");
    StringArray array2 = StringArray::copyOf(strings);
    Class<String>::isAssignable<String>();
    Enum<Character::UnicodeBlock> ub = (Character::UnicodeBlock) 59;
    String str = ub.toString();
    return 0;
}

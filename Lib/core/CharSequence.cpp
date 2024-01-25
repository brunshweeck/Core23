//
// Created by T.N.Brunshweeck on 05/01/2024.
//

#include "CharSequence.h"
#include <core/Math.h>

namespace core {
    gbool CharSequence::isEmpty() const {
        return length() == 0;
    }

    gint CharSequence::compare(const CharSequence &cs1, const CharSequence &cs2) {
        if (&cs1 == &cs2) {
            return 0;
        }

        for (gint i = 0, len = Math::min(cs1.length(), cs2.length()); i < len; i++) {
            char a = cs1.charAt(i);
            char b = cs2.charAt(i);
            if (a != b) {
                return a - b;
            }
        }

        return cs1.length() - cs2.length();
    }
} // core
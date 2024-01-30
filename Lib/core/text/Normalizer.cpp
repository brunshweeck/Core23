//
// Created by T.N.Brunshweeck on 26/01/2024.
//

#include "Normalizer.h"

namespace core {
    namespace text {
        String Normalizer::normalize(const CharSequence &src, Normalizer::Form form) {
            return normalize(src.toString(), form);
        }

        gbool Normalizer::isNormalized(const CharSequence &src, Normalizer::Form form) {
            return isNormalized(src.toString(), form);
        }

        String Normalizer::normalize(const String &src, Normalizer::Form form) {
            CORE_IGNORE(form);
            return src;
        }

        gbool Normalizer::isNormalized(const String &src, Normalizer::Form form) {
            CORE_IGNORE(src);
            CORE_IGNORE(form);
            return true;
        }
    } // text
} // core

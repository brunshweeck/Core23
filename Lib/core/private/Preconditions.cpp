//
// Created by T.N.Brunshweeck on 24/11/2023.
//

#include "Preconditions.h"
#include <core/IndexException.h>

namespace core {
    namespace util {
        gint Preconditions::checkIndex(gint index, gint length) {
            if (index < 0 || index >= length)
                IndexException("Index " + String::valueOf(index) + " out of bounds for length "
                               + String::valueOf(length)).throws(__trace("core.util.Preconditions"));
            return index;
        }

        gint Preconditions::checkIndexFromRange(gint startIndex, gint endIndex, gint length) {
            if (startIndex < 0 || endIndex < 0 || endIndex < startIndex || endIndex > length)
                IndexException("Range [" + String::valueOf(startIndex) + ", " + String::valueOf(endIndex) +
                               ") out of bounds for length " + String::valueOf(length))
                        .throws(__trace("core.util.Preconditions"));
            return startIndex;
        }

        gint Preconditions::checkIndexFromSize(gint startIndex, gint size, gint length) {
            if ((length | startIndex | size) < 0 || size > length - startIndex) {
                String str = String::valueOf(startIndex);
                IndexException(
                        "Range [" + str + ", " + str + " + " + String::valueOf(size) + ") out of bounds for length " +
                        String::valueOf(length)).throws(__trace("core.util.Preconditions"));
            }
            return startIndex;
        }

        glong Preconditions::checkIndex(glong index, glong length) {
            if (index < 0 || index >= length)
                IndexException("Index " + String::valueOf(index) + " out of bounds for length "
                               + String::valueOf(length)).throws(__trace("core.util.Preconditions"));
            return index;
        }

        glong Preconditions::checkIndexFromRange(glong startIndex, glong endIndex, glong length) {
            if (startIndex < 0 || endIndex < 0 || endIndex < startIndex || endIndex > length)
                IndexException("Range [" + String::valueOf(startIndex) + ", " + String::valueOf(endIndex) +
                               ") out of bounds for length " + String::valueOf(length))
                        .throws(__trace("core.util.Preconditions"));
            return startIndex;
        }

        glong Preconditions::checkIndexFromSize(glong startIndex, glong size, glong length) {
            if ((length | startIndex | size) < 0 || size > length - startIndex) {
                String str = String::valueOf(startIndex);
                IndexException(
                        "Range [" + str + ", " + str + " + " + String::valueOf(size) + ") out of bounds for length " +
                        String::valueOf(length)).throws(__trace("core.util.Preconditions"));
            }
            return startIndex;
        }
        
        gint Preconditions::checkIndexForAdding(gint index, gint length) {
            if (index < 0 || index > length)
                IndexException("Index " + String::valueOf(index)
                               + " out of adding bounds for length "
                               + String::valueOf(length)).throws(__trace("core.util.Preconditions"));
            return index;
        }

        glong Preconditions::checkIndexForAdding(glong index, glong length) {
            if (index < 0 || index > length)
                IndexException("Index " + String::valueOf(index)
                               + " out of adding bounds for length "
                               + String::valueOf(length)).throws(__trace("core.util.Preconditions"));
            return index;
        }
    } // util
} // core
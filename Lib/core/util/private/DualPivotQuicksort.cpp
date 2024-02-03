//
// Created by T.N.Brunshweeck on 02/02/2024.
//

#include "DualPivotQuicksort.h"

namespace core {
    namespace util {
        gint DualPivotQuicksort::depth(gint parallelism, gint size) {
            gint depth = 0;

            while ((parallelism >>= 3) > 0 && (size >>= 2) > 0) {
                depth -= 2;
            }
            return depth;
        }

        gint DualPivotQuicksort::sort(IntArray &a, gint parallelism, gint low, gint high) {}
    } // util
} // core

//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#include "FileSystem.h"
#include <core/io/File.h>

namespace core {
    namespace io {

        gint FileSystem::prefixLength0(const File &f) {
            return f.prefixLength;
        }

        gbool FileSystem::checkAttributes(const File &f, gint attributes) const {
            return (this->attributes(f) & attributes) == attributes;
        }
    }
} // core
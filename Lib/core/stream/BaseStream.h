//
// Created by T.N.Brunshweeck on 31/01/2024.
//

#ifndef CORE23_BASESTREAM_H
#define CORE23_BASESTREAM_H

#include <core/AutoClosable.h>

namespace core {
    namespace stream {

        template<class T, class S>
        class BaseStream: public Object, public AutoClosable {

        };

    } // stream
} // core

#endif //CORE23_BASESTREAM_H

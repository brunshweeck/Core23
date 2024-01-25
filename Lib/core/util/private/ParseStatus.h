//
// Created by T.N.Brunshweeck on 24/01/2024.
//

#ifndef CORE23_PARSESTATUS_H
#define CORE23_PARSESTATUS_H

#include <core/util/Locale.h>

namespace core {
    namespace util {

        class ParseStatus CORE_FINAL : public Object {
        public:
            int length;
            int errIndex;
            String errMessage;

            ParseStatus();

            void reset();

            gbool isError() const;

            int errorIndex() const;

            int parseLength() const;

            String errorMessage() const;
        };

    } // util
} // core

#endif //CORE23_PARSESTATUS_H

//
// Created by T.N.Brunshweeck on 24/01/2024.
//

#include "ParseStatus.h"

namespace core {
    namespace util {
        ParseStatus::ParseStatus() : length(0), errIndex(-1) {}

        void ParseStatus::reset() {
            length = 0;
            errIndex = -1;
            errMessage = u""_S;
        }

        gbool ParseStatus::isError() const {
            return (errIndex >= 0);
        }

        int ParseStatus::errorIndex() const {
            return errIndex;
        }

        int ParseStatus::parseLength() const {
            return length;
        }

        String ParseStatus::errorMessage() const {
            return errMessage;
        }
    } // util
} // core

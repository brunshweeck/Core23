//
// Created by T.N.Brunshweeck on 09/01/2024.
//

#ifndef CORE23_CODERMALFUNCTIONERROR_H
#define CORE23_CODERMALFUNCTIONERROR_H

#include <core/Error.h>

namespace core {
    namespace charset {


        /**
         * Error thrown when the <b style="color: orange;">decodeLoop</b> method of
         * a <b style="color: orange;">CharsetDecoder</b>, or the <b style="color: orange;">encodeLoop</b> method of
         * a <b style="color: orange;">CharsetEncoder</b>, throws an unexpected exception.
         */
        class CoderMalfunctionError : public Error {
        public:
            /**
             * Initializes an instance of this class.
             *
             * @param  cause
             *         The unexpected exception that was thrown
             */
            CORE_EXPLICIT CoderMalfunctionError(const Exception &cause);

            Object &clone() const override;

        private:
            void raise() && override;
        };

    } // charset
} // core

#endif //CORE23_CODERMALFUNCTIONERROR_H

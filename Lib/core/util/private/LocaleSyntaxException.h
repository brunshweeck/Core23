//
// Created by T.N.Brunshweeck on 24/01/2024.
//

#ifndef CORE23_LOCALESYNTAXEXCEPTION_H
#define CORE23_LOCALESYNTAXEXCEPTION_H

#include <core/Exception.h>

namespace core {
    namespace util {

        class LocaleSyntaxException CORE_FINAL : public Exception {
        private:
            gint index = -1;

        public:
            LocaleSyntaxException(String msg);

            LocaleSyntaxException(String msg, int errorIndex);

            gint errorIndex() const;

            Object &clone() const override;

        private:
            void raise() && override;

        };

    } // util
} // core

#endif //CORE23_LOCALESYNTAXEXCEPTION_H

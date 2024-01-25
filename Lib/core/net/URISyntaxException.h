//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_URISYNTAXEXCEPTION_H
#define CORE23_URISYNTAXEXCEPTION_H

#include <core/Exception.h>

namespace core {
    namespace net {

        /**
         * Checked exception thrown to indicate that a string could not be parsed as a
         * URI reference.
         *
         * @author Brunshweeck Tazeussong
         * @see URI
         */
        class URISyntaxException : public Exception {
        private:

            /**
             * The input string.
             */
            String errorInput;

            /**
             * The index at which the parse error occurred,
             * or <b> -1</b>  if the index is not known.
             */
            gint errorIndex;

        public:
            /**
             * Constructs an instance from the given input string, reason, and error
             * index.
             *
             * @param  input   The input string
             * @param  reason  A string explaining why the input could not be parsed
             * @param  index   The index at which the parse error occurred,
             *                 or <b> -1</b>  if the index is not known
             *
             * @throws  IllegalArgumentException
             *          If the error index is less than <b> -1</b>
             */
            CORE_EXPLICIT URISyntaxException(String input, String reason, gint index);

            /**
             * Constructs an instance from the given input string and reason.  The
             * resulting object will have an error index of <b> -1</b> .
             *
             * @param  input   The input string
             * @param  reason  A string explaining why the input could not be parsed
             */
            CORE_EXPLICIT URISyntaxException(String input, String reason);

            /**
             * Returns the input string.
             *
             * @return  The input string
             */
            String input() const;

            /**
             * Returns a string explaining why the input string could not be parsed.
             *
             * @return  The reason string
             */
            String reason() const;

            /**
             * Returns an index into the input string of the position at which the
             * parse error occurred, or <b> -1} if this position is not known.
             *
             * @return  The error index
             */
            gint index() const;

            /**
             * Returns a string describing the parse error.  The resulting string
             * consists of the reason string followed by a colon character
             * (<b> ':'}), a space, and the input string.  If the error index is
             * defined then the string <b> " at index "} followed by the index, in
             * decimal, is inserted after the reason string and before the colon
             * character.
             *
             * @return  A string describing the parse error
             */
            String message() const override;

            Object &clone() const override;

        private:
            void raise() && override;
        };

    } // core
} // net

#endif //CORE23_URISYNTAXEXCEPTION_H

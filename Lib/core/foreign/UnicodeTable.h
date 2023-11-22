//
// Created by T.N.Brunshweeck on 20/11/2023.
//

#ifndef CORE23_UNICODETABLE_H
#define CORE23_UNICODETABLE_H

#include <core/Character.h>

namespace core {
    namespace foreign {

        /**
         * UnicodeTable class contains many method read character properties
         */
        class UnicodeTable final : public Object {
        private:
            CORE_FAST UnicodeTable() = default;

            CORE_DISABLE_COPY_MOVE(UnicodeTable);

            /**
             * Return the row of properties corresponding to the given character.
             */
            virtual gint readProperties(gint ch);

            /**
             * Return the row of decompositions corresponding to the given character
             */
            virtual glong readDecompositions(gint ch);

            virtual gint query0(gint prop, gint col);

        public:

            /**
             * The sorted list of columns in unicode table
             */
            enum class Column {
                CATEGORY,
                DIRECTION,
                COMBINING_CLASS,
                JOINING_TYPE,
                NUMERIC_VALUE,
                MIRROR_OFFSET,
                UNICODE_VERSION,
                LOWERCASE_SPECIAL,
                LOWERCASE_OFFSET,
                UPPERCASE_SPECIAL,
                UPPERCASE_OFFSET,
                TITLECASE_SPECIAL,
                TITLECASE_OFFSET,
                CASE_FOLD_SPECIAL,
                CASE_FOLD_OFFSET,
                GRAPHEME_BREAK_CLASS,
                WORD_BREAK_CLASS,
                LINE_BREAK_CLASS,
                SENTENCE_BREAK_CLASS,
                UNICODE_SCRIPT,
                UNICODE_BLOCK,
                DECOMPOSITION, // memory offset
            };

            /**
             * get information on this table
             */
            glong query(gint ch, Column c);

            static UnicodeTable instance;

            Object &clone() const override;
        };

    } // core
} // foreign

#endif //CORE23_UNICODETABLE_H

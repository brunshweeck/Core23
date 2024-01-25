//
// Created by T.N.Brunshweeck on 09/01/2024.
//

#ifndef CORE23_STRINGCHARBUFFER_H
#define CORE23_STRINGCHARBUFFER_H

#include <core/io/CharBuffer.h>

namespace core {
    namespace io {

        class StringCharBuffer CORE_FINAL /*private*/: public CharBuffer {

            CharSequence &str;
        public:
            CORE_EXPLICIT StringCharBuffer(CharSequence &s, gint start, gint end);

            CharBuffer &slice() const override;

            CharBuffer &slice(gint index, gint length) const override;

            StringCharBuffer(CharSequence &s, gint mark, gint pos, gint limit, gint cap, gint offset);

            CharBuffer &duplicate() const override;

            CharBuffer &asReadOnlyBuffer() const override;

            gchar get() override;

            gchar get(gint index) const override;

            gchar getUnchecked(gint index) const override;

            CharBuffer &put(gchar c) override;

            CharBuffer &put(gint index, gchar c) override;

            CharBuffer &compact() override;

            gbool isReadOnly() const override;

        protected:
            String toString(gint start, gint end) const override;

        public:
            CharBuffer &subSequence(gint start, gint end) const override;

            gbool isDirect() const override;

            ByteOrder order() const override;

            OptionalByteOrder charRegionOrder() const override;

            gbool isAddressable() const override;

            gbool equals(const Object &obj) const override;

            gint compareTo(const CharBuffer &that) const override;
        };

    } // io
} // core

#endif //CORE23_STRINGCHARBUFFER_H

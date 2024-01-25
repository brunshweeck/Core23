//
// Created by T.N.Brunshweeck on 05/01/2024.
//

#include "Buffer.h"
#include <core/util/Preconditions.h>
#include <core/IllegalArgumentException.h>
#include <core/io/InvalidMarkException.h>
#include <core/io/BufferOverflowException.h>
#include <core/io/BufferUnderflowException.h>
#include <core/IndexException.h>

namespace core {
    namespace io {

        using namespace util;

        Buffer::Buffer(glong addr, gint cap) : address(addr), cap(cap), lim(cap) {}

        Buffer::Buffer(gint mark, gint pos, gint lim, gint cap) :
                markPos(mark), pos(pos), lim(lim), cap(cap), address(0) {
            if (cap < 0)
                IllegalArgumentException("capacity < 0: (" + String::valueOf(cap) + " < 0)")
                        .throws(__trace("core.io.Buffer"));
            cap = cap;
            try {
                setLimit(lim);
                setPosition(pos);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.Buffer"));
            }
            if (mark >= 0) {
                if (mark > pos)
                    IllegalArgumentException(
                            "mark > position: (" + String::valueOf(mark) + " > " + String::valueOf(pos) + ")")
                            .throws(__trace("core.io.Buffer"));
                this->markPos = mark;
            }
        }

        gint Buffer::capacity() const { return cap; }

        gint Buffer::position() const { return pos; }

        Buffer &Buffer::setPosition(gint newPosition) {
            if (newPosition > lim)
                IllegalArgumentException(
                        "newPosition > limit: ("
                        + String::valueOf(newPosition) + " > " + String::valueOf(lim) +")").throws(__trace("core.io.Buffer"));
            if (newPosition < 0)
                IllegalArgumentException("newPosition < 0: (" + String::valueOf(newPosition) + " < 0)")
                        .throws(__trace("core.io.Buffer"));
            if (markPos > newPosition)
                markPos = -1;
            pos = newPosition;
            return *this;
        }

        gint Buffer::limit() const { return lim; }

        Buffer &Buffer::setLimit(gint newLimit) {
            if (newLimit > cap)
                IllegalArgumentException(
                        "newLimit > capacity: (" + String::valueOf(newLimit) + " > " + String::valueOf(cap) +
                        ")")
                        .throws(__trace("core.io.Buffer"));
            if (newLimit < 0)
                IllegalArgumentException("newLimit < 0: (" + String::valueOf(newLimit) + " < 0)")
                        .throws(__trace("core.io.Buffer"));
            lim = newLimit;
            if (pos > newLimit)
                pos = newLimit;
            if (markPos > newLimit)
                markPos = -1;
            return *this;
        }

        Buffer &Buffer::mark() {
            markPos = pos;
            return *this;
        }

        Buffer &Buffer::reset() {
            gint const m = markPos;
            if (m < 0) {
                InvalidMarkException().throws(__trace("core.io.Buffer"));
            }
            pos = m;
            return *this;
        }

        Buffer &Buffer::clear() {
            pos = 0;
            lim = cap;
            markPos = -1;
            return *this;
        }

        Buffer &Buffer::flip() {
            lim = pos;
            pos = 0;
            markPos = -1;
            return *this;
        }

        Buffer &Buffer::rewind() {
            pos = 0;
            markPos = -1;
            return *this;
        }

        gint Buffer::remaining() const {
            gint const rem = lim - pos;
            return rem > 0 ? rem : 0;
        }

        gbool Buffer::hasRemaining() const {
            return pos < lim;
        }

        gint Buffer::nextPutIndex() {                          // package-private
            gint const p = pos;
            if (p >= lim)
                BufferOverflowException().throws(__trace("core.io.Buffer"));
            pos = p + 1;
            return p;
        }

        gint Buffer::nextPutIndex(gint nb) {                    // package-private
            gint const p = pos;
            if (lim - p < nb)
                BufferOverflowException().throws(__trace("core.io.Buffer"));
            pos = p + nb;
            return p;
        }

        gint Buffer::nextGetIndex() {                          // package-private
            gint const p = pos;
            if (p >= lim)
                BufferUnderflowException().throws(__trace("core.io.Buffer"));
            pos = p + 1;
            return p;
        }

        gint Buffer::nextGetIndex(gint nb) {                    // package-private
            gint const p = pos;
            if (lim - p < nb)
                BufferUnderflowException().throws(__trace("core.io.Buffer"));
            pos = p + nb;
            return p;
        }

        gint Buffer::checkIndex(gint i) const {                       // package-private
            try {
                return Preconditions::checkIndex(i, lim);
            } catch (const IndexException &iex) {
                iex.throws(__trace("core.io.Buffer"));
            }
        }

        gint Buffer::checkIndex(gint i, gint nb) const {               // package-private
            if ((i < 0) || (nb > lim - i))
                IndexException().throws(__trace("core.io.Buffer"));
            return i;
        }

        gint Buffer::markValue() const {                             // package-private
            return markPos;
        }

        void Buffer::discardMark() {                          // package-private
            markPos = -1;
        }

    } // io
} // core
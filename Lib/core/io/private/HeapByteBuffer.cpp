//
// Created by T.N.Brunshweeck on 08/01/2024.
//

#include "HeapByteBuffer.h"
#include <core/Float.h>
#include <core/Double.h>
#include <core/Enum.h>
#include <core/private/Unsafe.h>
#include <core/util/Preconditions.h>
#include <core/util/Optional.h>
#include <core/io/BufferUnderflowException.h>
#include <core/io/BufferOverflowException.h>
#include <core/io/private/ReadOnlyHeapByteBuffer.h>
#include <core/io/CharBuffer.h>
#include <core/io/DoubleBuffer.h>
#include <core/io/FloatBuffer.h>
#include <core/io/IntBuffer.h>
#include <core/io/LongBuffer.h>
#include <core/io/ShortBuffer.h>

namespace core {
    namespace io {

        using namespace native;
        using namespace util;

        namespace {
            class System CORE_FINAL : public Object {
            public:
                static void arraycopy(const ByteArray &src, gint srcBegin, ByteArray &dst, gint dstBegin, gint length) {
                    try {
                        Preconditions::checkIndexFromSize(srcBegin, length, src.length());
                        Preconditions::checkIndexFromSize(dstBegin, length, src.length());
                        Unsafe::copyMemory(src, Unsafe::ARRAY_BYTE_BASE_OFFSET + srcBegin,
                                           dst, Unsafe::ARRAY_BYTE_BASE_OFFSET + dstBegin,
                                           (glong) length * Unsafe::ARRAY_BYTE_INDEX_SCALE);
                    } catch (const Exception &ex) {
                        ex.throws(__trace("core.io.System"));
                    }
                }
            };
        }

        class HeapByteBuffer::CharBufferView CORE_FINAL : public CharBuffer {
        private:
            HeapByteBuffer &bb;
            gbool bigEndian;

        public:
            CharBufferView(HeapByteBuffer &bb, gbool bigEndian) :
                    CharBuffer(-1, 0, bb.remaining() >> 1, bb.remaining() >> 1), bb(bb), bigEndian(bigEndian) {}

            CharBufferView(HeapByteBuffer &bb, gint mark, gint pos, gint lim, gint cap, glong addr,
                           gbool bigEndian) : CharBuffer(mark, pos, lim, cap), bb(bb), bigEndian(bigEndian) {
                address = addr;
                CORE_ASSERT(address >= bb.address, "core.io.HeapByteBuffer.CharBufferView");
            }


        protected:
            Object &base() const override {
                return *bb.hb;
            }

        public:
            CharBuffer &slice() const override {
                try {
                    gint const pos = position();
                    gint const lim = limit();
                    gint const rem = (pos <= lim ? lim - pos : 0);
                    glong const addr = byteOffset(pos);
                    return Unsafe::allocateInstance<CharBufferView>(bb, -1, 0, rem, rem, addr, bigEndian);
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.io.HeapByteBuffer.CharBufferView"));
                }
            }

            CharBuffer &slice(gint index, gint length) const override {
                try {
                    Preconditions::checkIndexFromSize(index, length, limit());
                    return Unsafe::allocateInstance<CharBufferView>(bb, -1, 0, length, length, byteOffset(index),
                                                                    bigEndian);
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.io.HeapByteBuffer.CharBufferView"));
                }
            }

            CharBuffer &duplicate() const override {
                try {
                    return Unsafe::allocateInstance<CharBufferView>(bb, markValue(), position(), limit(), capacity(),
                                                                    address, bigEndian);
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.io.HeapByteBuffer.CharBufferView"));
                }
            }

            CharBuffer &asReadOnlyBuffer() const override {
                try {
                    if (bb.isReadOnly())
                        return duplicate();
                    else
                        return Unsafe::allocateInstance<CharBufferView>(
                                (HeapByteBuffer &) bb.asReadOnlyBuffer(), markValue(),
                                position(), limit(), capacity(),
                                address, bigEndian);
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.io.HeapByteBuffer.CharBufferView"));
                }
            }

            CharBuffer &compact() override {
                try {
                    gint const pos = position();
                    gint const lim = limit();
                    CORE_ASSERT(pos <= lim, "core.io.HeapByteBuffer.CharBufferView");
                    gint const rem = (pos <= lim ? lim - pos : 0);
                    ByteBuffer &db = bb.duplicate();
                    db.setLimit(ix(lim));
                    db.setPosition(ix(0));
                    ByteBuffer &sb = db.slice();
                    sb.setPosition(pos << 1);
                    sb.compact();
                    setPosition(rem);
                    setLimit(capacity());
                    discardMark();
                    return *this;
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.io.HeapByteBuffer.CharBufferView"));
                }
            }

            gbool isDirect() const override {
                return bb.isDirect();
            }

            gbool isReadOnly() const override {
                return bb.isReadOnly();
            }

        protected:
            String toString(gint start, gint end) const override {
                try {
                    gint const len = end - start;
                    CharArray ca{len};
                    CharBuffer &cb = CharBuffer::wrap(ca);
                    CharBuffer &db = duplicate();
                    db.setPosition(start);
                    db.setLimit(end);
                    cb.put(db);
                    Unsafe::destroyInstance(cb);
                    Unsafe::destroyInstance(db);
                    return String(ca);
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.io.HeapByteBuffer.CharBufferView"));
                }
            }

        public:
            CharBuffer &subSequence(gint start, gint end) const override {
                try {
                    gint pos = position();
                    gint const lim = limit();
                    CORE_ASSERT(pos <= lim, "core.io.HeapByteBuffer.CharBufferView");
                    pos = (pos <= lim ? pos : lim);
                    gint const len = lim - pos;
                    Preconditions::checkIndexFromRange(start, end, len);
                    return Unsafe::allocateInstance<CharBufferView>((HeapByteBuffer &) bb, -1, pos + start, pos + end,
                                                                    capacity(), address, bigEndian);
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.io.HeapByteBuffer.CharBufferView"));
                }
            }

            ByteOrder order() const override {
                return bigEndian ? ByteOrder::BIG_ENDIAN : ByteOrder::LITTLE_ENDIAN;
            }

        protected:
            OptionalByteOrder charRegionOrder() const override {
                return OptionalByteOrder::of(order());
            }

            gint ix(gint i) const {
                gint const off = (gint) (address - bb.address);
                return (i << 1) + off;
            }

            glong byteOffset(glong i) const {
                return (i << 1) + address;
            }

        public:
            gchar get() override {
                gchar const x = Unsafe::getCharUnaligned(*bb.hb, byteOffset(nextGetIndex()), true);
                return (x);
            }

            gchar get(gint i) const override {
                gchar const x = Unsafe::getCharUnaligned(*bb.hb, byteOffset(checkIndex(i)), bigEndian);
                return (x);
            }

            gchar getUnchecked(gint i) const override {
                gchar const x = Unsafe::getCharUnaligned(*bb.hb, byteOffset(i), bigEndian);
                return x;
            }

            CharBuffer &put(gchar x) override {
                gchar const y = (x);
                Unsafe::putCharUnaligned(*bb.hb, byteOffset(nextPutIndex()), y, bigEndian);
                return *this;
            }

            CharBuffer &put(gint i, gchar x) override {
                gchar const y = (x);
                Unsafe::putCharUnaligned(*bb.hb, byteOffset(checkIndex(i)), y, bigEndian);
                return *this;
            }
        };

        class HeapByteBuffer::DoubleBufferView CORE_FINAL : public DoubleBuffer {
        private:
            HeapByteBuffer &bb;
            gbool bigEndian;

        public:
            DoubleBufferView(HeapByteBuffer &bb, gbool bigEndian) :
                    DoubleBuffer(-1, 0, bb.remaining() >> 3, bb.remaining() >> 3), bb(bb), bigEndian(bigEndian) {}

            DoubleBufferView(HeapByteBuffer &bb, gint mark, gint pos, gint lim, gint cap, glong addr,
                             gbool bigEndian) : DoubleBuffer(mark, pos, lim, cap), bb(bb), bigEndian(bigEndian) {
                address = addr;
                CORE_ASSERT(address >= bb.address, "core.io.HeapByteBuffer.DoubleBufferView");
            }


        protected:
            Object &base() const override {
                return *bb.hb;
            }

        public:
            DoubleBuffer &slice() const override {
                try {
                    gint const pos = position();
                    gint const lim = limit();
                    gint const rem = (pos <= lim ? lim - pos : 0);
                    glong const addr = byteOffset(pos);
                    return Unsafe::allocateInstance<DoubleBufferView>(bb, -1, 0, rem, rem, addr, bigEndian);
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.io.HeapByteBuffer.DoubleBufferView"));
                }
            }

            DoubleBuffer &slice(gint index, gint length) const override {
                try {
                    Preconditions::checkIndexFromSize(index, length, limit());
                    return Unsafe::allocateInstance<DoubleBufferView>(bb, -1, 0, length, length, byteOffset(index),
                                                                      bigEndian);
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.io.HeapByteBuffer.DoubleBufferView"));
                }
            }

            DoubleBuffer &duplicate() const override {
                try {
                    return Unsafe::allocateInstance<DoubleBufferView>(bb, markValue(), position(), limit(), capacity(),
                                                                      address, bigEndian);
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.io.HeapByteBuffer.DoubleBufferView"));
                }
            }

            DoubleBuffer &asReadOnlyBuffer() const override {
                try {
                    if (bb.isReadOnly())
                        return duplicate();
                    else
                        return Unsafe::allocateInstance<DoubleBufferView>(
                                (HeapByteBuffer &) bb.asReadOnlyBuffer(), markValue(),
                                position(), limit(), capacity(),
                                address, bigEndian);
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.io.HeapByteBuffer.DoubleBufferView"));
                }
            }

            DoubleBuffer &compact() override {
                try {
                    gint const pos = position();
                    gint const lim = limit();
                    CORE_ASSERT(pos <= lim, "core.io.HeapByteBuffer.DoubleBufferView");
                    gint const rem = (pos <= lim ? lim - pos : 0);
                    ByteBuffer &db = bb.duplicate();
                    db.setLimit(ix(lim));
                    db.setPosition(ix(0));
                    ByteBuffer &sb = db.slice();
                    sb.setPosition(pos << 3);
                    sb.compact();
                    setPosition(rem);
                    setLimit(capacity());
                    discardMark();
                    return *this;
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.io.HeapByteBuffer.DoubleBufferView"));
                }
            }

            gbool isDirect() const override {
                return bb.isDirect();
            }

            gbool isReadOnly() const override {
                return bb.isReadOnly();
            }

            ByteOrder order() const override {
                return bigEndian ? ByteOrder::BIG_ENDIAN : ByteOrder::LITTLE_ENDIAN;
            }

            gint ix(gint i) const {
                gint const off = (gint) (address - bb.address);
                return (i << 3) + off;
            }

            glong byteOffset(glong i) const {
                return (i << 3) + address;
            }

            gdouble get() override {
                glong const x = Unsafe::getLongUnaligned(*bb.hb, byteOffset(nextGetIndex()), bigEndian);
                return Double::fromLongBits(x);
            }

            gdouble get(gint i) const override {
                glong const x = Unsafe::getLongUnaligned(*bb.hb, byteOffset(checkIndex(i)), bigEndian);
                return Double::fromLongBits(x);
            }

            DoubleBuffer &put(gdouble x) override {
                glong const y = Double::toLongBits(x);
                Unsafe::putLongUnaligned(*bb.hb, byteOffset(nextPutIndex()), y, bigEndian);
                return *this;
            }

            DoubleBuffer &put(gint i, gdouble x) override {
                glong const y = Double::toLongBits(x);
                Unsafe::putLongUnaligned(*bb.hb, byteOffset(checkIndex(i)), y, bigEndian);
                return *this;
            }
        };

        class HeapByteBuffer::FloatBufferView CORE_FINAL : public FloatBuffer {
        private:
            HeapByteBuffer &bb;
            gbool bigEndian;

        public:
            FloatBufferView(HeapByteBuffer &bb, gbool bigEndian) :
                    FloatBuffer(-1, 0, bb.remaining() >> 3, bb.remaining() >> 3), bb(bb), bigEndian(bigEndian) {}

            FloatBufferView(HeapByteBuffer &bb, gint mark, gint pos, gint lim, gint cap, glong addr,
                            gbool bigEndian) : FloatBuffer(mark, pos, lim, cap), bb(bb), bigEndian(bigEndian) {
                address = addr;
                CORE_ASSERT(address >= bb.address, "core.io.HeapByteBuffer.FloatBufferView");
            }


        protected:
            Object &base() const override {
                return *bb.hb;
            }

        public:
            FloatBuffer &slice() const override {
                try {
                    gint const pos = position();
                    gint const lim = limit();
                    gint const rem = (pos <= lim ? lim - pos : 0);
                    glong const addr = byteOffset(pos);
                    return Unsafe::allocateInstance<FloatBufferView>(bb, -1, 0, rem, rem, addr, bigEndian);
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.io.HeapByteBuffer.FloatBufferView"));
                }
            }

            FloatBuffer &slice(gint index, gint length) const override {
                try {
                    Preconditions::checkIndexFromSize(index, length, limit());
                    return Unsafe::allocateInstance<FloatBufferView>(bb, -1, 0, length, length, byteOffset(index),
                                                                     bigEndian);
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.io.HeapByteBuffer.FloatBufferView"));
                }
            }

            FloatBuffer &duplicate() const override {
                try {
                    return Unsafe::allocateInstance<FloatBufferView>(bb, markValue(), position(), limit(), capacity(),
                                                                     address, bigEndian);
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.io.HeapByteBuffer.FloatBufferView"));
                }
            }

            FloatBuffer &asReadOnlyBuffer() const override {
                try {
                    if (bb.isReadOnly())
                        return duplicate();
                    else
                        return Unsafe::allocateInstance<FloatBufferView>(
                                (HeapByteBuffer &) bb.asReadOnlyBuffer(), markValue(),
                                position(), limit(), capacity(),
                                address, bigEndian);
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.io.HeapByteBuffer.FloatBufferView"));
                }
            }

            FloatBuffer &compact() override {
                try {
                    gint const pos = position();
                    gint const lim = limit();
                    CORE_ASSERT(pos <= lim, "core.io.HeapByteBuffer.FloatBufferView");
                    gint const rem = (pos <= lim ? lim - pos : 0);
                    ByteBuffer &db = bb.duplicate();
                    db.setLimit(ix(lim));
                    db.setPosition(ix(0));
                    ByteBuffer &sb = db.slice();
                    sb.setPosition(pos << 2);
                    sb.compact();
                    setPosition(rem);
                    setLimit(capacity());
                    discardMark();
                    return *this;
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.io.HeapByteBuffer.FloatBufferView"));
                }
            }

            gbool isDirect() const override {
                return bb.isDirect();
            }

            gbool isReadOnly() const override {
                return bb.isReadOnly();
            }

            ByteOrder order() const override {
                return bigEndian ? ByteOrder::BIG_ENDIAN : ByteOrder::LITTLE_ENDIAN;
            }

            gint ix(gint i) const {
                gint const off = (gint) (address - bb.address);
                return (i << 2) + off;
            }

            glong byteOffset(glong i) const {
                return (i << 2) + address;
            }

            gfloat get() override {
                gint const x = Unsafe::getIntUnaligned(*bb.hb, byteOffset(nextGetIndex()), bigEndian);
                return Float::fromIntBits(x);
            }

            gfloat get(gint i) const override {
                gint const x = Unsafe::getIntUnaligned(*bb.hb, byteOffset(checkIndex(i)), bigEndian);
                return Float::fromIntBits(x);
            }

            FloatBuffer &put(gfloat x) override {
                gint const y = Float::toIntBits(x);
                Unsafe::putIntUnaligned(*bb.hb, byteOffset(nextPutIndex()), y, bigEndian);
                return *this;
            }

            FloatBuffer &put(gint i, gfloat x) override {
                gint const y = Float::toIntBits(x);
                Unsafe::putIntUnaligned(*bb.hb, byteOffset(checkIndex(i)), y, bigEndian);
                return *this;
            }
        };

        class HeapByteBuffer::IntBufferView CORE_FINAL : public IntBuffer {
        private:
            HeapByteBuffer &bb;
            gbool bigEndian;

        public:
            IntBufferView(HeapByteBuffer &bb, gbool bigEndian) :
                    IntBuffer(-1, 0, bb.remaining() >> 2, bb.remaining() >> 2), bb(bb), bigEndian(bigEndian) {}

            IntBufferView(HeapByteBuffer &bb, gint mark, gint pos, gint lim, gint cap, glong addr,
                          gbool bigEndian) : IntBuffer(mark, pos, lim, cap), bb(bb), bigEndian(bigEndian) {
                address = addr;
                CORE_ASSERT(address >= bb.address, "core.io.HeapByteBuffer.IntBufferView");
            }


        protected:
            Object &base() const override {
                return *bb.hb;
            }

        public:
            IntBuffer &slice() const override {
                try {
                    gint const pos = position();
                    gint const lim = limit();
                    gint const rem = (pos <= lim ? lim - pos : 0);
                    glong const addr = byteOffset(pos);
                    return Unsafe::allocateInstance<IntBufferView>(bb, -1, 0, rem, rem, addr, bigEndian);
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.io.HeapByteBuffer.IntBufferView"));
                }
            }

            IntBuffer &slice(gint index, gint length) const override {
                try {
                    Preconditions::checkIndexFromSize(index, length, limit());
                    return Unsafe::allocateInstance<IntBufferView>(bb, -1, 0, length, length, byteOffset(index),
                                                                   bigEndian);
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.io.HeapByteBuffer.IntBufferView"));
                }
            }

            IntBuffer &duplicate() const override {
                try {
                    return Unsafe::allocateInstance<IntBufferView>(bb, markValue(), position(), limit(), capacity(),
                                                                   address, bigEndian);
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.io.HeapByteBuffer.IntBufferView"));
                }
            }

            IntBuffer &asReadOnlyBuffer() const override {
                try {
                    if (bb.isReadOnly())
                        return duplicate();
                    else
                        return Unsafe::allocateInstance<IntBufferView>(
                                (HeapByteBuffer &) bb.asReadOnlyBuffer(), markValue(),
                                position(), limit(), capacity(),
                                address, bigEndian);
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.io.HeapByteBuffer.IntBufferView"));
                }
            }

            IntBuffer &compact() override {
                try {
                    gint const pos = position();
                    gint const lim = limit();
                    CORE_ASSERT(pos <= lim, "core.io.HeapByteBuffer.IntBufferView");
                    gint const rem = (pos <= lim ? lim - pos : 0);
                    ByteBuffer &db = bb.duplicate();
                    db.setLimit(ix(lim));
                    db.setPosition(ix(0));
                    ByteBuffer &sb = db.slice();
                    sb.setPosition(pos << 2);
                    sb.compact();
                    setPosition(rem);
                    setLimit(capacity());
                    discardMark();
                    return *this;
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.io.HeapByteBuffer.IntBufferView"));
                }
            }

            gbool isDirect() const override {
                return bb.isDirect();
            }

            gbool isReadOnly() const override {
                return bb.isReadOnly();
            }

            ByteOrder order() const override {
                return bigEndian ? ByteOrder::BIG_ENDIAN : ByteOrder::LITTLE_ENDIAN;
            }

        protected:

            gint ix(gint i) const {
                gint const off = (gint) (address - bb.address);
                return (i << 2) + off;
            }

            glong byteOffset(glong i) const {
                return (i << 2) + address;
            }

        public:
            gint get() override {
                gint const x = Unsafe::getIntUnaligned(*bb.hb, byteOffset(nextGetIndex()), bigEndian);
                return (x);
            }

            gint get(gint i) const override {
                gint const x = Unsafe::getIntUnaligned(*bb.hb, byteOffset(checkIndex(i)), bigEndian);
                return (x);
            }

            IntBuffer &put(gint x) override {
                gint const y = (x);
                Unsafe::putIntUnaligned(*bb.hb, byteOffset(nextPutIndex()), y, bigEndian);
                return *this;
            }

            IntBuffer &put(gint i, gint x) override {
                gint const y = (x);
                Unsafe::putIntUnaligned(*bb.hb, byteOffset(checkIndex(i)), y, bigEndian);
                return *this;
            }
        };

        class HeapByteBuffer::LongBufferView CORE_FINAL : public LongBuffer {
        private:
            HeapByteBuffer &bb;
            gbool bigEndian;

        public:
            LongBufferView(HeapByteBuffer &bb, gbool bigEndian) :
                    LongBuffer(-1, 0, bb.remaining() >> 3, bb.remaining() >> 3), bb(bb), bigEndian(bigEndian) {}

            LongBufferView(HeapByteBuffer &bb, gint mark, gint pos, gint lim, gint cap, glong addr,
                           gbool bigEndian) : LongBuffer(mark, pos, lim, cap), bb(bb), bigEndian(bigEndian) {
                address = addr;
                CORE_ASSERT(address >= bb.address, "core.io.HeapByteBuffer.LongBufferView");
            }


        protected:
            Object &base() const override {
                return *bb.hb;
            }

        public:
            LongBuffer &slice() const override {
                try {
                    gint const pos = position();
                    gint const lim = limit();
                    gint const rem = (pos <= lim ? lim - pos : 0);
                    glong const addr = byteOffset(pos);
                    return Unsafe::allocateInstance<LongBufferView>(bb, -1, 0, rem, rem, addr, bigEndian);
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.io.HeapByteBuffer.LongBufferView"));
                }
            }

            LongBuffer &slice(gint index, gint length) const override {
                try {
                    Preconditions::checkIndexFromSize(index, length, limit());
                    return Unsafe::allocateInstance<LongBufferView>(bb, -1, 0, length, length, byteOffset(index),
                                                                    bigEndian);
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.io.HeapByteBuffer.LongBufferView"));
                }
            }

            LongBuffer &duplicate() const override {
                try {
                    return Unsafe::allocateInstance<LongBufferView>(bb, markValue(), position(), limit(), capacity(),
                                                                    address, bigEndian);
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.io.HeapByteBuffer.LongBufferView"));
                }
            }

            LongBuffer &asReadOnlyBuffer() const override {
                try {
                    if (bb.isReadOnly())
                        return duplicate();
                    else
                        return Unsafe::allocateInstance<LongBufferView>(
                                (HeapByteBuffer &) bb.asReadOnlyBuffer(), markValue(),
                                position(), limit(), capacity(),
                                address, bigEndian);
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.io.HeapByteBuffer.LongBufferView"));
                }
            }

            LongBuffer &compact() override {
                try {
                    gint const pos = position();
                    gint const lim = limit();
                    CORE_ASSERT(pos <= lim, "core.io.HeapByteBuffer.LongBufferView");
                    gint const rem = (pos <= lim ? lim - pos : 0);
                    ByteBuffer &db = bb.duplicate();
                    db.setLimit(ix(lim));
                    db.setPosition(ix(0));
                    ByteBuffer &sb = db.slice();
                    sb.setPosition(pos << 3);
                    sb.compact();
                    setPosition(rem);
                    setLimit(capacity());
                    discardMark();
                    return *this;
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.io.HeapByteBuffer.LongBufferView"));
                }
            }

            gbool isDirect() const override {
                return bb.isDirect();
            }

            gbool isReadOnly() const override {
                return bb.isReadOnly();
            }

            ByteOrder order() const override {
                return bigEndian ? ByteOrder::BIG_ENDIAN : ByteOrder::LITTLE_ENDIAN;
            }

        protected:

            gint ix(gint i) const {
                gint const off = (gint) (address - bb.address);
                return (i << 3) + off;
            }

            glong byteOffset(glong i) const {
                return (i << 3) + address;
            }

        public:
            glong get() override {
                glong const x = Unsafe::getLongUnaligned(*bb.hb, byteOffset(nextGetIndex()), bigEndian);
                return (x);
            }

            glong get(gint i) const override {
                glong const x = Unsafe::getLongUnaligned(*bb.hb, byteOffset(checkIndex(i)), bigEndian);
                return (x);
            }

            LongBuffer &put(glong x) override {
                glong const y = (x);
                Unsafe::putLongUnaligned(*bb.hb, byteOffset(nextPutIndex()), y, bigEndian);
                return *this;
            }

            LongBuffer &put(gint i, glong x) override {
                glong const y = (x);
                Unsafe::putLongUnaligned(*bb.hb, byteOffset(checkIndex(i)), y, bigEndian);
                return *this;
            }
        };

        class HeapByteBuffer::ShortBufferView CORE_FINAL : public ShortBuffer {
        private:
            HeapByteBuffer &bb;
            gbool bigEndian;

        public:
            ShortBufferView(HeapByteBuffer &bb, gbool bigEndian) :
                    ShortBuffer(-1, 0, bb.remaining() >> 1, bb.remaining() >> 1), bb(bb), bigEndian(bigEndian) {}

            ShortBufferView(HeapByteBuffer &bb, gint mark, gint pos, gint lim, gint cap, glong addr,
                            gbool bigEndian) : ShortBuffer(mark, pos, lim, cap), bb(bb), bigEndian(bigEndian) {
                address = addr;
                CORE_ASSERT(address >= bb.address, "core.io.HeapByteBuffer.ShortBufferView");
            }


        protected:
            Object &base() const override {
                return *bb.hb;
            }

        public:
            ShortBuffer &slice() const override {
                try {
                    gint const pos = position();
                    gint const lim = limit();
                    gint const rem = (pos <= lim ? lim - pos : 0);
                    glong const addr = byteOffset(pos);
                    return Unsafe::allocateInstance<ShortBufferView>(bb, -1, 0, rem, rem, addr, bigEndian);
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.io.HeapByteBuffer.ShortBufferView"));
                }
            }

            ShortBuffer &slice(gint index, gint length) const override {
                try {
                    Preconditions::checkIndexFromSize(index, length, limit());
                    return Unsafe::allocateInstance<ShortBufferView>(bb, -1, 0, length, length, byteOffset(index),
                                                                     bigEndian);
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.io.HeapByteBuffer.ShortBufferView"));
                }
            }

            ShortBuffer &duplicate() const override {
                try {
                    return Unsafe::allocateInstance<ShortBufferView>(bb, markValue(), position(), limit(), capacity(),
                                                                     address, bigEndian);
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.io.HeapByteBuffer.ShortBufferView"));
                }
            }

            ShortBuffer &asReadOnlyBuffer() const override {
                try {
                    if (bb.isReadOnly())
                        return duplicate();
                    else
                        return Unsafe::allocateInstance<ShortBufferView>(
                                (HeapByteBuffer &) bb.asReadOnlyBuffer(), markValue(),
                                position(), limit(), capacity(),
                                address, bigEndian);
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.io.HeapByteBuffer.ShortBufferView"));
                }
            }

            ShortBuffer &compact() override {
                try {
                    gint const pos = position();
                    gint const lim = limit();
                    CORE_ASSERT(pos <= lim, "core.io.HeapByteBuffer.ShortBufferView");
                    gint const rem = (pos <= lim ? lim - pos : 0);
                    ByteBuffer &db = bb.duplicate();
                    db.setLimit(ix(lim));
                    db.setPosition(ix(0));
                    ByteBuffer &sb = db.slice();
                    sb.setPosition(pos << 1);
                    sb.compact();
                    setPosition(rem);
                    setLimit(capacity());
                    discardMark();
                    return *this;
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.io.HeapByteBuffer.ShortBufferView"));
                }
            }

            gbool isDirect() const override {
                return bb.isDirect();
            }

            gbool isReadOnly() const override {
                return bb.isReadOnly();
            }

            ByteOrder order() const override {
                return bigEndian ? ByteOrder::BIG_ENDIAN : ByteOrder::LITTLE_ENDIAN;
            }

        protected:

            gint ix(gint i) const {
                gint const off = (gint) (address - bb.address);
                return (i << 1) + off;
            }

            glong byteOffset(glong i) const {
                return (i << 1) + address;
            }

        public:
            gshort get() override {
                gshort const x = Unsafe::getShortUnaligned(*bb.hb, byteOffset(nextGetIndex()), bigEndian);
                return (x);
            }

            gshort get(gint i) const override {
                gshort const x = Unsafe::getShortUnaligned(*bb.hb, byteOffset(checkIndex(i)), bigEndian);
                return (x);
            }

            ShortBuffer &put(gshort x) override {
                gshort const y = (x);
                Unsafe::putShortUnaligned(*bb.hb, byteOffset(nextPutIndex()), y, bigEndian);
                return *this;
            }

            ShortBuffer &put(gint i, gshort x) override {
                gshort const y = (x);
                Unsafe::putShortUnaligned(*bb.hb, byteOffset(checkIndex(i)), y, bigEndian);
                return *this;
            }
        };

        const glong HeapByteBuffer::ARRAY_BASE_OFFSET = Unsafe::ARRAY_BYTE_BASE_OFFSET;
        const glong HeapByteBuffer::ARRAY_INDEX_SCALE = Unsafe::ARRAY_BYTE_INDEX_SCALE;

        HeapByteBuffer::HeapByteBuffer(gint cap, gint lim) :
                ByteBuffer(-1, 0, lim, cap, Unsafe::allocateInstance<ByteArray>(cap), 0) { address = ARRAY_BASE_OFFSET; }

        HeapByteBuffer::HeapByteBuffer(ByteArray &buf, gint off, gint len) :
                ByteBuffer(-1, off, off + len, buf.length(), buf, 0) { address = ARRAY_BASE_OFFSET; }

        HeapByteBuffer::HeapByteBuffer(ByteArray &buf, gint mark, gint pos, gint lim, gint cap, gint off)
                : ByteBuffer(mark, pos, lim, cap, buf, off) { address = ARRAY_BASE_OFFSET; }

        ByteBuffer &HeapByteBuffer::slice() const {
            try {
                gint const pos = position();
                gint const lim = limit();
                gint const rem = (pos <= lim ? lim - pos : 0);
                return Unsafe::allocateInstance<HeapByteBuffer>(*hb, -1, 0, rem, rem, pos + offset);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
        }

        ByteBuffer &HeapByteBuffer::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return Unsafe::allocateInstance<HeapByteBuffer>(*hb, -1, 0, length, length, index + offset);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
        }

        ByteBuffer &HeapByteBuffer::asReadOnlyBuffer() const {
            try {
                return Unsafe::allocateInstance<ReadOnlyHeapByteBuffer>(
                        *hb, markValue(), position(), limit(), capacity(), offset);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
        }

        gint HeapByteBuffer::ix(gint i) const {
            return i + offset;
        }

        glong HeapByteBuffer::byteOffset(glong i) const {
            return address + i;
        }

        gbyte HeapByteBuffer::get() {
            try {
                return (*hb)[ix(nextGetIndex())];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
        }

        gbyte HeapByteBuffer::get(gint i) const {
            try {
                return (*hb)[ix(checkIndex(i))];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
        }

        ByteBuffer &HeapByteBuffer::get(ByteArray &dst, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromSize(offset, length, dst.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
            gint const pos = position();
            if (length > limit() - pos)
                BufferUnderflowException().throws(__trace("core.io.HeapByteBuffer"));
            try {
                System::arraycopy(*hb, ix(pos), dst, offset, length);
                setPosition(pos + length);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
            return *this;
        }

        ByteBuffer &HeapByteBuffer::get(gint index, ByteArray &dst, gint offset, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(offset, length, dst.length());
                System::arraycopy(*hb, ix(index), dst, offset, length);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
            return (ByteBuffer &) *this;
        }

        gbool HeapByteBuffer::isDirect() const {
            return false;
        }

        gbool HeapByteBuffer::isReadOnly() const {
            return false;
        }

        ByteBuffer &HeapByteBuffer::put(gbyte x) {
            try {
                (*hb)[ix(nextPutIndex())] = x;
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
        }

        ByteBuffer &HeapByteBuffer::put(gint i, gbyte x) {
            try {
                (*hb)[ix(checkIndex(i))] = x;
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
        }

        ByteBuffer &HeapByteBuffer::put(const ByteArray &src, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromSize(offset, length, src.length());
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
            gint const pos = position();
            if (length > limit() - pos)
                BufferOverflowException().throws(__trace("core.io.HeapByteBuffer"));
            try {
                System::arraycopy(src, offset, *hb, ix(pos), length);
                setPosition(pos + length);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
            return *this;
        }

        ByteBuffer &HeapByteBuffer::put(ByteBuffer &src) {
            try {
                ByteBuffer::put(src);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
        }

        ByteBuffer &HeapByteBuffer::put(gint index, ByteBuffer &src, gint offset, gint length) {
            try {
                ByteBuffer::put(index, src, offset, length);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
        }

        ByteBuffer &HeapByteBuffer::put(gint index, const ByteArray &src, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(offset, length, src.length());
                System::arraycopy(src, offset, *hb, ix(index), length);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
        }

        ByteBuffer &HeapByteBuffer::compact() {
            try {
                gint const pos = position();
                gint const lim = limit();
                CORE_ASSERT(pos <= lim, "core.io.HeapByteBuffer");
                gint const rem = (pos <= lim ? lim - pos : 0);
                System::arraycopy(*hb, ix(pos), *hb, ix(0), rem);
                setPosition(rem);
                setLimit(capacity());
                discardMark();
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
            return *this;
        }

        gbyte HeapByteBuffer::_get(gint i) const {
            try {
                return (*hb)[i];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
        }

        void HeapByteBuffer::_put(gint i, gbyte b) {
            try {
                (*hb)[i] = b;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
        }

        gchar HeapByteBuffer::getChar() {
            try {
                return Unsafe::getCharUnaligned(*hb, byteOffset(nextGetIndex(2)), bigEndian);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
        }

        gchar HeapByteBuffer::getChar(gint i) const {
            try {
                return Unsafe::getCharUnaligned(*hb, byteOffset(checkIndex(i, 2)), bigEndian);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
        }

        ByteBuffer &HeapByteBuffer::putChar(gchar x) {
            try {
                Unsafe::putCharUnaligned(*hb, byteOffset(nextPutIndex(2)), x, bigEndian);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
        }

        ByteBuffer &HeapByteBuffer::putChar(gint i, gchar x) {
            try {
                Unsafe::putCharUnaligned(*hb, byteOffset(checkIndex(i, 2)), x, bigEndian);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
        }

        CharBuffer &HeapByteBuffer::asCharBuffer() const {
            try {
                gint const pos = position();
                gint const size = (limit() - pos) >> 1;
                glong addr = address + pos;
                return Unsafe::allocateInstance<CharBufferView>((HeapByteBuffer &) *this, -1, 0, size, size, addr,
                                                                bigEndian);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
        }

        short HeapByteBuffer::getShort() {
            try {
                return Unsafe::getShortUnaligned(*hb, byteOffset(nextGetIndex(2)), bigEndian);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
        }

        gshort HeapByteBuffer::getShort(gint i) const {
            try {
                return Unsafe::getShortUnaligned(*hb, byteOffset(checkIndex(i, 2)), bigEndian);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
        }

        ByteBuffer &HeapByteBuffer::putShort(gshort x) {
            try {
                Unsafe::putShortUnaligned(*hb, byteOffset(nextPutIndex(2)), x, bigEndian);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
        }

        ByteBuffer &HeapByteBuffer::putShort(gint i, gshort x) {
            try {
                Unsafe::putShortUnaligned(*hb, byteOffset(checkIndex(i, 2)), x, bigEndian);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
        }

        ShortBuffer &HeapByteBuffer::asShortBuffer() const {
            try {
                gint const pos = position();
                gint const size = (limit() - pos) >> 1;
                glong const addr = address + pos;
                return Unsafe::allocateInstance<ShortBufferView>((HeapByteBuffer &) *this, -1, 0, size, size, addr,
                                                                 bigEndian);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
        }

        gint HeapByteBuffer::getInt() {
            try {
                return Unsafe::getIntUnaligned(*hb, byteOffset(nextGetIndex(4)), bigEndian);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
        }

        gint HeapByteBuffer::getInt(gint i) const {
            try {
                return Unsafe::getIntUnaligned(*hb, byteOffset(checkIndex(i, 4)), bigEndian);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
        }

        ByteBuffer &HeapByteBuffer::putInt(gint x) {
            try {
                Unsafe::putIntUnaligned(*hb, byteOffset(nextPutIndex(4)), x, bigEndian);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
        }

        ByteBuffer &HeapByteBuffer::putInt(gint i, gint x) {
            try {
                Unsafe::putIntUnaligned(*hb, byteOffset(checkIndex(i, 4)), x, bigEndian);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
        }

        IntBuffer &HeapByteBuffer::asIntBuffer() const {
            try {
                gint const pos = position();
                gint const size = (limit() - pos) >> 2;
                glong const addr = address + pos;
                return Unsafe::allocateInstance<IntBufferView>((HeapByteBuffer &) *this, -1, 0, size, size, addr,
                                                               bigEndian);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
        }

        glong HeapByteBuffer::getLong() {
            try {
                return Unsafe::getLongUnaligned(*hb, byteOffset(nextGetIndex(8)), bigEndian);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
        }

        glong HeapByteBuffer::getLong(gint i) const {
            try {
                return Unsafe::getLongUnaligned(*hb, byteOffset(checkIndex(i, 8)), bigEndian);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
        }

        ByteBuffer &HeapByteBuffer::putLong(glong x) {
            try {
                Unsafe::putLongUnaligned(*hb, byteOffset(nextPutIndex(8)), x, bigEndian);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
        }

        ByteBuffer &HeapByteBuffer::putLong(gint i, glong x) {
            try {
                Unsafe::putLongUnaligned(*hb, byteOffset(checkIndex(i, 8)), x, bigEndian);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
        }

        LongBuffer &HeapByteBuffer::asLongBuffer() const {
            gint const pos = position();
            gint const size = (limit() - pos) >> 3;
            glong const addr = address + pos;
            return Unsafe::allocateInstance<LongBufferView>((HeapByteBuffer &) *this, -1, 0, size, size, addr,
                                                            bigEndian);
        }

        gfloat HeapByteBuffer::getFloat() {
            try {
                gint const x = Unsafe::getIntUnaligned(*hb, byteOffset(nextGetIndex(4)), bigEndian);
                return Float::fromIntBits(x);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
        }

        gfloat HeapByteBuffer::getFloat(gint i) const {
            try {
                gint const x = Unsafe::getIntUnaligned(*hb, byteOffset(checkIndex(i, 4)), bigEndian);
                return Float::fromIntBits(x);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
        }

        ByteBuffer &HeapByteBuffer::putFloat(gfloat x) {
            try {
                gint const y = Float::toIntBits(x);
                Unsafe::putIntUnaligned(*hb, byteOffset(nextPutIndex(4)), y, bigEndian);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
        }

        ByteBuffer &HeapByteBuffer::putFloat(gint i, gfloat x) {
            try {
                gint const y = Float::toIntBits(x);
                Unsafe::putIntUnaligned(*hb, byteOffset(checkIndex(i, 4)), y, bigEndian);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
        }

        FloatBuffer &HeapByteBuffer::asFloatBuffer() const {
            try {
                gint const pos = position();
                gint const size = (limit() - pos) >> 2;
                glong const addr = address + pos;
                return Unsafe::allocateInstance<FloatBufferView>((HeapByteBuffer &) *this, -1, 0, size, size, addr,
                                                                 bigEndian);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
        }

        gdouble HeapByteBuffer::getDouble() {
            try {
                glong const x = Unsafe::getLongUnaligned(*hb, byteOffset(nextGetIndex(8)), bigEndian);
                return Double::fromLongBits(x);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
        }

        gdouble HeapByteBuffer::getDouble(gint i) const {
            try {
                glong const x = Unsafe::getLongUnaligned(*hb, byteOffset(checkIndex(i, 8)), bigEndian);
                return Double::fromLongBits(x);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
        }

        ByteBuffer &HeapByteBuffer::putDouble(gdouble x) {
            try {
                glong const y = Double::toLongBits(x);
                Unsafe::putLongUnaligned(*hb, byteOffset(nextPutIndex(8)), y, bigEndian);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }


        }

        ByteBuffer &HeapByteBuffer::putDouble(gint i, gdouble x) {
            try {
                glong const y = Double::toLongBits(x);
                Unsafe::putLongUnaligned(*hb, byteOffset(checkIndex(i, 8)), y, bigEndian);
                return *this;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
        }

        DoubleBuffer &HeapByteBuffer::asDoubleBuffer() const {
            try {
                gint const pos = position();
                gint const size = (limit() - pos) >> 3;
                glong const addr = address + pos;
                return Unsafe::allocateInstance<DoubleBufferView>((HeapByteBuffer &) *this, -1, 0, size, size, addr,
                                                                  bigEndian);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
        }

        ByteBuffer &HeapByteBuffer::duplicate() const {
            try {
                return Unsafe::allocateInstance<HeapByteBuffer>(*hb, markValue(), position(), limit(), capacity(),
                                                                offset);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.io.HeapByteBuffer"));
            }
        }
    } // io
} // core
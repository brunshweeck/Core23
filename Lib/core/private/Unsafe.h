//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_UNSAFE_H
#define CORE23_UNSAFE_H

#include <core/MemoryError.h>
#include <core/CloneNotSupportedException.h>

namespace core {

    namespace native {
        /**
         * A collection of methods for performing low-level, unsafe operations.
         * Although the class and all methods are public, use of this class is
         * limited because only trusted code can obtain instances of it.
         *
         * <em>Note:</em> It is the responsibility of the caller to make sure
         * arguments are checked before methods of this class are
         * called. While some rudimentary checks are performed on the input,
         * the checks are best effort and when performance is an overriding
         * priority, as when methods of this class are optimized by the
         * runtime compiler, some or all checks (if any) may be elided. Hence,
         * the caller must not rely on the checks and corresponding
         * exceptions!
         *
         * @author Brunshweeck Tazeussong
         */
        class Unsafe final : public Object {
        private:

            CORE_FAST Unsafe() = default;

        public:

            /**
             * The size in bytes of a pointer, as stored via <b style="color: orange;">
             * putAddress</b>.  This value will be either 4 or 8.  Note that the
             * sizes of other native types (as stored in memory
             * blocks) is determined fully by their information content.
             */
            const static gint ADDRESS_SIZE;

            /**
             * Flag whose value is true if and only if the endianness
             * of this platform is big.
             */
            const static gbool BIG_ENDIAN;

            /**
             * Fetches a value from a given variable.
             * More specifically, fetches a field or array element within the given
             * object <b> o</b> at the given offset, or (if <b> o</b> is null)
             * from the memory address whose numerical value is the given offset.
             * <p>
             * The results are undefined unless one of the following cases is true:
             * <ul>
             * <li>The offset was obtained from <b style="color: orange;"> offsetof</b> (defined in <stddef.h>)
             * of some field and the object referred to by <b> o</b> is of a class compatible with that
             * field's class.
             *
             * <li>The offset and object reference <b> o</b> (either null or
             * non-null) were both obtained via <b style="color: orange;"> offsetof </b>
             * reflective <b style="color: orange;"> &lt;stddef.h&gt;</b> representation of some field.
             *
             * <li>The object referred to by <b> o</b> is an array, and the offset
             * is an integer of the form <b> B+N*S</b>, where <b> N</b> is
             * a valid index into the array, and <b> B</b> and <b> S</b> are
             * the values obtained by <b style="color: orange;"> arrayBaseOffset</b> and <b style="color: orange;">
             * arrayIndexScale</b> (respectively) from the array's class.  The value
             * referred to is the <b> N</b><em>th</em> element of the array.
             *
             * </ul>
             * <p>
             * If one of the above cases is true, the call references a specific
             * variable (field or array element).  However, the results are undefined
             * if that variable is not in fact of the type returned by this method.
             * <p>
             * This method refers to a variable by means of two parameters, and so
             * it provides (in effect) a <em>double-register</em> addressing mode
             * for class variables.  When the object reference is null, this method
             * uses its offset as an absolute address.
             *
             * @param o class heap object in which the variable resides, if any, else
             *        null
             * @param offset indication of where the variable resides in a heap
             *        object, if any, else a memory address locating the variable
             *        statically
             * @return the value fetched from the indicated variable
             * @throws RuntimeException No defined exceptions are thrown
             */
            static gint getInt(const Object &o, glong offset);

            /**
             * Stores a value into a given variable.
             * <p>
             * The first two parameters are interpreted exactly as with
             * <b style="color: orange;"> getInt(Object, glong)</b> to refer to a specific
             * variable (field or array element).  The given value
             * is stored into that variable.
             * <p>
             * The variable must be of the same type as the method
             * parameter <b> x</b>.
             *
             * @param o class heap object in which the variable resides, if any, else
             *        null
             * @param offset indication of where the variable resides in a heap
             *        object, if any, else a memory address locating the variable
             *        statically
             * @param x the value to store into the indicated variable
             * @throws RuntimeException No defined exceptions are thrown
             */
            static void putInt(Object &o, glong offset, gint x);

            /**
             * Fetches a reference value from a given variable.
             * @see getInt(Object, glong)
             */
            static Object &getReference(const Object &o, glong offset);

            /**
             * Stores a reference value into a given variable.
             * <p>
             * Unless the reference <b> x</b> being stored is either null
             * or matches the field type, the results are undefined.
             * If the reference <b> o</b> is non-null, card marks or
             * other store barriers for that object (if the VM requires them)
             * are updated.
             * @see putInt(Object, glong, gint)
             */
            static void putReference(Object &o, glong offset, Object &x);

            /** @see getInt(Object, glong) */
            static gbool getBoolean(const Object &o, glong offset);

            /** @see putInt(Object, glong, gint) */
            static void putBoolean(Object &o, glong offset, gbool x);

            /** @see getInt(Object, glong) */
            static gbyte getByte(const Object &o, glong offset);

            /** @see putInt(Object, glong, gint) */
            static void putByte(Object &o, glong offset, gbyte x);

            /** @see getInt(Object, glong) */
            static gshort getShort(const Object &o, glong offset);

            /** @see putInt(Object, glong, gint) */
            static void putShort(Object &o, glong offset, gshort x);

            /** @see getInt(Object, glong) */
            static gchar getChar(const Object &o, glong offset);

            /** @see putInt(Object, glong, gint) */
            static void putChar(Object &o, glong offset, gchar x);

            /** @see getInt(Object, glong) */
            static glong getLong(const Object &o, glong offset);

            /** @see putInt(Object, glong, gint) */
            static void putLong(Object &o, glong offset, glong x);

            /** @see getInt(Object, glong) */
            static gfloat getFloat(const Object &o, glong offset);

            /** @see putInt(Object, glong, gint) */
            static void putFloat(Object &o, glong offset, gfloat x);

            /** @see getInt(Object, glong) */
            static gdouble getDouble(const Object &o, glong offset);

            /** @see putInt(Object, glong, gint) */
            static void putDouble(Object &o, glong offset, gdouble x);

            /**
             * Fetches a native pointer from a given memory address.  If the address is
             * zero, or does not point into a block obtained from <b style="color: orange;">
             * allocateMemory</b>, the results are undefined.
             *
             * <p>If the native pointer is less than 64 bits wide, it is extended as
             * an unsigned number to a long.  The pointer may be indexed by any
             * given byte offset, simply by adding that offset (as a simple integer) to
             * the long representing the pointer.  The number of bytes actually read
             * from the target address may be determined by consulting <b style="color: orange;">
             * addressSize</b>.
             *
             * @see allocateMemory
             * @see getInt(Object, glong)
             */
            static glong getAddress(const Object &o, glong offset);

            /**
             * Stores a native pointer into a given memory address.  If the address is
             * zero, or does not point into a block obtained from <b style="color: orange;">
             * allocateMemory</b>, the results are undefined.
             *
             * <p>The number of bytes actually written at the target address may be
             * determined by consulting <b style="color: orange;"> addressSize</b>.
             *
             * @see allocateMemory
             * @see putInt(Object, glong, gint)
             */
            static void putAddress(Object &o, glong offset, glong x);

            // The work on values in C heap

            /**
             * Fetches a value from a given memory address.  If the address is zero, or
             * does not point into a block obtained from <b style="color: orange;"> allocateMemory</b>, the
             * results are undefined.
             *
             * @see allocateMemory
             */
            static gbyte getByte(glong address);

            /**
             * Stores a value into a given memory address.  If the address is zero, or
             * does not point into a block obtained from <b style="color: orange;"> allocateMemory</b>, the
             * results are undefined.
             *
             * @see getByte(glong)
             */
            static void putByte(glong address, gbyte x);

            /** @see getByte(glong) */
            static gbool getBoolean(glong address);

            /** @see putByte(glong, byte) */
            static void putBoolean(glong address, gbool x);

            /** @see getByte(glong) */
            static gshort getShort(glong address);

            /** @see putByte(glong, byte) */
            static void putShort(glong address, gshort x);

            /** @see getByte(glong) */
            static gchar getChar(glong address);

            /** @see putByte(glong, byte) */
            static void putChar(glong address, gchar x);

            /** @see getByte(glong) */
            static gint getInt(glong address);

            /** @see putByte(glong, byte) */
            static void putInt(glong address, gint x);

            /** @see getByte(glong) */
            static glong getLong(glong address);

            /** @see putByte(glong, byte) */
            static void putLong(glong address, glong x);

            /** @see getByte(glong) */
            static gfloat getFloat(glong address);

            /** @see putByte(glong, byte) */
            static void putFloat(glong address, gfloat x);

            /** @see getByte(glong) */
            static gdouble getDouble(glong address);

            /** @see putByte(glong, byte) */
            static void putDouble(glong address, gdouble x);

            /** @see getByte(glong) */
            static glong getAddress(glong address);

            /** @see putByte(glong, byte) */
            static void putAddress(glong address, glong x);

            /**
             * Allocates a new block of native memory, of the given size in sizeInBytes.  The
             * contents of the memory are uninitialized; they will generally be
             * garbage.  The resulting native pointer will never be zero, and will be
             * aligned for all value types.  Dispose of this memory by calling <b style="color: orange;">
             * freeMemory</b>, or resize it with <b style="color: orange;"> reallocateMemory</b>.
             *
             * <em>Note:</em> It is the responsibility of the caller to make
             * sure arguments are checked before the methods are called. While
             * some rudimentary checks are performed on the input, the checks
             * are best effort and when performance is an overriding priority,
             * as when methods of this class are optimized by the runtime
             * compiler, some or all checks (if any) may be elided. Hence, the
             * caller must not rely on the checks and corresponding
             * exceptions!
             *
             * @throws RuntimeException if the size is negative or too large
             *         for the native size_t type
             *
             * @throws MemoryError if the allocation is refused by the system
             *
             * @see getByte(glong)
             * @see putByte(glong, byte)
             */
            static glong allocateMemory(glong sizeInBytes);

            /**
             * Resizes a new block of native memory, to the given size in sizeInBytes.  The
             * contents of the new block past the size of the old block are
             * uninitialized; they will generally be garbage.  The resulting native
             * pointer will be zero if and only if the requested size is zero.  The
             * resulting native pointer will be aligned for all value types.  Dispose
             * of this memory by calling <b style="color: orange;"> freeMemory</b>, or resize it with <b style="color: orange;">
             * reallocateMemory</b>.  The address passed to this method may be null, in
             * which case an allocation will be performed.
             *
             * <em>Note:</em> It is the responsibility of the caller to make
             * sure arguments are checked before the methods are called. While
             * some rudimentary checks are performed on the input, the checks
             * are best effort and when performance is an overriding priority,
             * as when methods of this class are optimized by the runtime
             * compiler, some or all checks (if any) may be elided. Hence, the
             * caller must not rely on the checks and corresponding
             * exceptions!
             *
             * @throws RuntimeException if the size is negative or too large
             *         for the native size_t type
             *
             * @throws MemoryError if the allocation is refused by the system
             *
             * @see allocateMemory
             */
            static glong reallocateMemory(glong address, glong sizeInBytes);

            /**
             * Sets all bytes in a given block of memory to a fixed value
             * (usually zero).
             *
             * <p>This method determines a block's base address by means of two parameters,
             * and so it provides (in effect) a <em>double-register</em> addressing mode,
             * as discussed in <b style="color: orange;"> getInt(Object,glong)</b>.  When the object reference is null,
             * the offset supplies an absolute base address.
             *
             * <p>The stores are in coherent (atomic) units of a size determined
             * by the address and length parameters.  If the effective address and
             * length are all even modulo 8, the stores take place in 'long' units.
             * If the effective address and length are (resp.) even modulo 4 or 2,
             * the stores take place in units of 'int' or 'short'.
             *
             * <em>Note:</em> It is the responsibility of the caller to make
             * sure arguments are checked before the methods are called. While
             * some rudimentary checks are performed on the input, the checks
             * are best effort and when performance is an overriding priority,
             * as when methods of this class are optimized by the runtime
             * compiler, some or all checks (if any) may be elided. Hence, the
             * caller must not rely on the checks and corresponding
             * exceptions!
             *
             * @throws RuntimeException if any of the arguments is invalid
             *
             */
            static void setMemory(Object &o, glong offset, glong sizeInBytes, gbyte value);

            /**
             * Sets all bytes in a given block of memory to a fixed value
             * (usually zero).  This provides a <em>single-register</em> addressing mode,
             * as discussed in <b style="color: orange;"> getInt(Object,glong)</b>.
             *
             * <p>Equivalent to <b> setMemory(null, address, bytes, value)</b>.
             */
            static void setMemory(glong address, glong sizeInBytes, gbyte value);

            /**
             * Sets all bytes in a given block of memory to a copy of another
             * block.
             *
             * <p>This method determines each block's base address by means of two parameters,
             * and so it provides (in effect) a <em>double-register</em> addressing mode,
             * as discussed in <b style="color: orange;"> getInt(Object,glong)</b>.  When the object reference is null,
             * the offset supplies an absolute base address.
             *
             * <p>The transfers are in coherent (atomic) units of a size determined
             * by the address and length parameters.  If the effective addresses and
             * length are all even modulo 8, the transfer takes place in 'long' units.
             * If the effective addresses and length are (resp.) even modulo 4 or 2,
             * the transfer takes place in units of 'int' or 'short'.
             *
             * <em>Note:</em> It is the responsibility of the caller to make
             * sure arguments are checked before the methods are called. While
             * some rudimentary checks are performed on the input, the checks
             * are best effort and when performance is an overriding priority,
             * as when methods of this class are optimized by the runtime
             * compiler, some or all checks (if any) may be elided. Hence, the
             * caller must not rely on the checks and corresponding
             * exceptions!
             *
             * @throws RuntimeException if any of the arguments is invalid
             *
             */
            static void
            copyMemory(const Object &src, glong srcOffset, Object &dest, glong destOffset, glong sizeInBytes);

            /**
             * Sets all bytes in a given block of memory to a copy of another
             * block.  This provides a <em>single-register</em> addressing mode,
             * as discussed in <b style="color: orange;"> getInt(Object,glong)</b>.
             *
             * Equivalent to <b> copyMemory(null, srcAddress, null, destAddress, bytes)</b>.
             */
            static void copyMemory(glong srcAddress, glong destAddress, glong bytes);

            /**
             * Copies all elements from one block of memory to another block,
             * *unconditionally* byte swapping the elements on the fly.
             *
             * <p>This method determines each block's base address by means of two parameters,
             * and so it provides (in effect) a <em>double-register</em> addressing mode,
             * as discussed in <b style="color: orange;"> getInt(Object,glong)</b>.  When the object reference is null,
             * the offset supplies an absolute base address.
             *
             * <em>Note:</em> It is the responsibility of the caller to make
             * sure arguments are checked before the methods are called. While
             * some rudimentary checks are performed on the input, the checks
             * are best effort and when performance is an overriding priority,
             * as when methods of this class are optimized by the runtime
             * compiler, some or all checks (if any) may be elided. Hence, the
             * caller must not rely on the checks and corresponding
             * exceptions!
             *
             * @throws RuntimeException if any of the arguments is invalid
             *
             */
            static void copySwapMemory(Object &src, glong srcOffset, Object &dest, glong destOffset, glong sizeInBytes,
                                       glong elemSize);

            /**
             * Copies all elements from one block of memory to another block, byte swapping the
             * elements on the fly.
             *
             * This provides a <em>single-register</em> addressing mode, as
             * discussed in <b style="color: orange;"> getInt(Object,glong)</b>.
             *
             * Equivalent to <b> copySwapMemory(null, srcAddress, null, destAddress, bytes, elemSize)</b>.
             */
            static void copySwapMemory(glong srcAddress, glong destAddress, glong bytes, glong elemSize);

            /**
             * Disposes of a block of native memory, as obtained from <b style="color: orange;">
             * allocateMemory</b> or <b style="color: orange;"> reallocateMemory</b>.  The address passed to
             * this method may be null, in which case no action is taken.
             *
             * <em>Note:</em> It is the responsibility of the caller to make
             * sure arguments are checked before the methods are called. While
             * some rudimentary checks are performed on the input, the checks
             * are best effort and when performance is an overriding priority,
             * as when methods of this class are optimized by the runtime
             * compiler, some or all checks (if any) may be elided. Hence, the
             * caller must not rely on the checks and corresponding
             * exceptions!
             *
             * @throws RuntimeException if any of the arguments is invalid
             *
             * @see allocateMemory
             */
            static void freeMemory(glong address);

            /** The value of <b> arrayBaseOffset<gbool>()</b> */
            static const gint ARRAY_BOOLEAN_BASE_OFFSET;

            /** The value of <b> arrayBaseOffset<gbyte>()</b> */
            static const gint ARRAY_BYTE_BASE_OFFSET;

            /** The value of <b> arrayBaseOffset<gshort>()</b> */
            static const gint ARRAY_SHORT_BASE_OFFSET;

            /** The value of <b> arrayBaseOffset<gchar>()</b> */
            static const gint ARRAY_CHAR_BASE_OFFSET;

            /** The value of <b> arrayBaseOffset<gint>()</b> */
            static const gint ARRAY_INT_BASE_OFFSET;

            /** The value of <b> arrayBaseOffset<glong>()</b> */
            static const gint ARRAY_LONG_BASE_OFFSET;

            /** The value of <b> arrayBaseOffset<gfloat>()</b> */
            static const gint ARRAY_FLOAT_BASE_OFFSET;

            /** The value of <b> arrayBaseOffset<gdouble>()</b> */
            static const gint ARRAY_DOUBLE_BASE_OFFSET;

            /** The value of <b> arrayBaseOffset<void*>()</b> */
            static const gint ARRAY_REFERENCE_BASE_OFFSET;

            /** The value of <b> arrayIndexScale<gbool>()</b> */
            static const gint ARRAY_BOOLEAN_INDEX_SCALE;

            /** The value of <b> arrayIndexScale<gbyte>()</b> */
            static const gint ARRAY_BYTE_INDEX_SCALE;

            /** The value of <b> arrayIndexScale<gshort>()</b> */
            static const gint ARRAY_SHORT_INDEX_SCALE;

            /** The value of <b> arrayIndexScale<gchar>()</b> */
            static const gint ARRAY_CHAR_INDEX_SCALE;

            /** The value of <b> arrayIndexScale<gint>()</b> */
            static const gint ARRAY_INT_INDEX_SCALE;

            /** The value of <b> arrayIndexScale<glong>()</b> */
            static const gint ARRAY_LONG_INDEX_SCALE;

            /** The value of <b> arrayIndexScale<gfloat>()</b> */
            static const gint ARRAY_FLOAT_INDEX_SCALE;

            /** The value of <b> arrayIndexScale<gdouble>()</b> */
            static const gint ARRAY_DOUBLE_INDEX_SCALE;

            /** The value of <b> arrayIndexScale<void*>()</b> */
            static const gint ARRAY_REFERENCE_INDEX_SCALE;

            // ---------------------------- [atomic reference] ------------------------

            /**
             * Atomically updates variable to <b> x</b> if it is currently
             * holding <b> expected</b>.
             *
             * <p>This operation has memory semantics of a <b> volatile</b> read
             * and write.  Corresponds to C11 atomic_compare_exchange_strong.
             *
             * @return <b> true</b> if successful
             */
            static gbool compareAndSetReference(Object &o, glong offset, const Object &expected, Object &x);

            static gbool compareAndSetReferenceAcquire(Object &o, glong offset, const Object &expected, Object &x);

            static gbool compareAndSetReferenceRelease(Object &o, glong offset, const Object &expected, Object &x);

            static gbool compareAndSetReferenceRelaxed(Object &o, glong offset, const Object &expected, Object &x);

            static Object &compareAndExchangeReference(Object &o, glong offset, const Object &expected, Object &x);

            static Object &
            compareAndExchangeReferenceAcquire(Object &o, glong offset, const Object &expected, Object &x);

            static Object &
            compareAndExchangeReferenceRelease(Object &o, glong offset, const Object &expected, Object &x);

            static Object &
            compareAndExchangeReferenceRelaxed(Object &o, glong offset, const Object &expected, Object &x);

            /**
             * Atomically updates variable to <b> x</b> if it is currently
             * holding <b> expected</b>.
             *
             * <p>This operation has memory semantics of a <b> volatile</b> read
             * and write.  Corresponds to C11 atomic_compare_exchange_weak.
             *
             * @return <b> true</b> if successful
             */
            static gbool weakCompareAndSetReference(Object &o, glong offset, const Object &expected, Object &x);

            static gbool weakCompareAndSetReferenceAcquire(Object &o, glong offset, const Object &expected, Object &x);

            static gbool weakCompareAndSetReferenceRelease(Object &o, glong offset, const Object &expected, Object &x);

            static gbool weakCompareAndSetReferenceRelaxed(Object &o, glong offset, const Object &expected, Object &x);

            static Object &weakCompareAndExchangeReference(Object &o, glong offset, const Object &expected, Object &x);

            static Object &
            weakCompareAndExchangeReferenceAcquire(Object &o, glong offset, const Object &expected, Object &x);

            static Object &
            weakCompareAndExchangeReferenceRelease(Object &o, glong offset, const Object &expected, Object &x);

            static Object &
            weakCompareAndExchangeReferenceRelaxed(Object &o, glong offset, const Object &expected, Object &x);

            // ----------------- [atomic int] ---------------------------------------


            /**
             * Atomically updates variable to <b> x</b> if it is currently
             * holding <b> expected</b>.
             *
             * <p>This operation has memory semantics of a <b> volatile</b> read
             * and write.  Corresponds to C11 atomic_compare_exchange_strong.
             *
             * @return <b> true</b> if successful
             */
            static gbool compareAndSetInt(Object &o, glong offset, gint expected, gint x);

            static gbool compareAndSetIntAcquire(Object &o, glong offset, gint expected, gint x);

            static gbool compareAndSetIntRelease(Object &o, glong offset, gint expected, gint x);

            static gbool compareAndSetIntRelaxed(Object &o, glong offset, gint expected, gint x);

            static gint compareAndExchangeInt(Object &o, glong offset, gint expected, gint x);

            static gint compareAndExchangeIntAcquire(Object &o, glong offset, gint expected, gint x);

            static gint compareAndExchangeIntRelease(Object &o, glong offset, gint expected, gint x);

            static gint compareAndExchangeIntRelaxed(Object &o, glong offset, gint expected, gint x);


            /**
             * Atomically updates variable to <b> x</b> if it is currently
             * holding <b> expected</b>.
             *
             * <p>This operation has memory semantics of a <b> volatile</b> read
             * and write.  Corresponds to C11 atomic_compare_exchange_strong.
             *
             * @return <b> true</b> if successful
             */
            static gbool weakCompareAndSetInt(Object &o, glong offset, gint expected, gint x);

            static gbool weakCompareAndSetIntAcquire(Object &o, glong offset, gint expected, gint x);

            static gbool weakCompareAndSetIntRelease(Object &o, glong offset, gint expected, gint x);

            static gbool weakCompareAndSetIntRelaxed(Object &o, glong offset, gint expected, gint x);

            static gint weakCompareAndExchangeInt(Object &o, glong offset, gint expected, gint x);

            static gint weakCompareAndExchangeIntAcquire(Object &o, glong offset, gint expected, gint x);

            static gint weakCompareAndExchangeIntRelease(Object &o, glong offset, gint expected, gint x);

            static gint weakCompareAndExchangeIntRelaxed(Object &o, glong offset, gint expected, gint x);

            // ----------------- [atomic byte] ---------------------------------------

            static gbool compareAndSetByte(Object &o, glong offset, gbyte expected, gbyte x);

            static gbool compareAndSetByteAcquire(Object &o, glong offset, gbyte expected, gbyte x);

            static gbool compareAndSetByteRelease(Object &o, glong offset, gbyte expected, gbyte x);

            static gbool compareAndSetByteRelaxed(Object &o, glong offset, gbyte expected, gbyte x);

            static gbyte compareAndExchangeByte(Object &o, glong offset, gbyte expected, gbyte x);

            static gbyte compareAndExchangeByteAcquire(Object &o, glong offset, gbyte expected, gbyte x);

            static gbyte compareAndExchangeByteRelease(Object &o, glong offset, gbyte expected, gbyte x);

            static gbyte compareAndExchangeByteRelaxed(Object &o, glong offset, gbyte expected, gbyte x);

            static gbool weakCompareAndSetByte(Object &o, glong offset, gbyte expected, gbyte x);

            static gbool weakCompareAndSetByteAcquire(Object &o, glong offset, gbyte expected, gbyte x);

            static gbool weakCompareAndSetByteRelease(Object &o, glong offset, gbyte expected, gbyte x);

            static gbool weakCompareAndSetByteRelaxed(Object &o, glong offset, gbyte expected, gbyte x);

            static gbyte weakCompareAndExchangeByte(Object &o, glong offset, gbyte expected, gbyte x);

            static gbyte weakCompareAndExchangeByteAcquire(Object &o, glong offset, gbyte expected, gbyte x);

            static gbyte weakCompareAndExchangeByteRelease(Object &o, glong offset, gbyte expected, gbyte x);

            static gbyte weakCompareAndExchangeByteRelaxed(Object &o, glong offset, gbyte expected, gbyte x);

            // ----------------- [atomic short] ---------------------------------------

            static gbool compareAndSetShort(Object &o, glong offset, gshort expected, gshort x);

            static gbool compareAndSetShortAcquire(Object &o, glong offset, gshort expected, gshort x);

            static gbool compareAndSetShortRelease(Object &o, glong offset, gshort expected, gshort x);

            static gbool compareAndSetShortRelaxed(Object &o, glong offset, gshort expected, gshort x);

            static gshort compareAndExchangeShort(Object &o, glong offset, gshort expected, gshort x);

            static gshort compareAndExchangeShortAcquire(Object &o, glong offset, gshort expected, gshort x);

            static gshort compareAndExchangeShortRelease(Object &o, glong offset, gshort expected, gshort x);

            static gshort compareAndExchangeShortRelaxed(Object &o, glong offset, gshort expected, gshort x);

            static gbool weakCompareAndSetShort(Object &o, glong offset, gshort expected, gshort x);

            static gbool weakCompareAndSetShortAcquire(Object &o, glong offset, gshort expected, gshort x);

            static gbool weakCompareAndSetShortRelease(Object &o, glong offset, gshort expected, gshort x);

            static gbool weakCompareAndSetShortRelaxed(Object &o, glong offset, gshort expected, gshort x);

            static gshort weakCompareAndExchangeShort(Object &o, glong offset, gshort expected, gshort x);

            static gshort weakCompareAndExchangeShortAcquire(Object &o, glong offset, gshort expected, gshort x);

            static gshort weakCompareAndExchangeShortRelease(Object &o, glong offset, gshort expected, gshort x);

            static gshort weakCompareAndExchangeShortRelaxed(Object &o, glong offset, gshort expected, gshort x);

            // ----------------- [atomic char] ---------------------------------------

            static gbool compareAndSetChar(Object &o, glong offset, gchar expected, gchar x);

            static gbool compareAndSetCharAcquire(Object &o, glong offset, gchar expected, gchar x);

            static gbool compareAndSetCharRelease(Object &o, glong offset, gchar expected, gchar x);

            static gbool compareAndSetCharRelaxed(Object &o, glong offset, gchar expected, gchar x);

            static gchar compareAndExchangeChar(Object &o, glong offset, gchar expected, gchar x);

            static gchar compareAndExchangeCharAcquire(Object &o, glong offset, gchar expected, gchar x);

            static gchar compareAndExchangeCharRelease(Object &o, glong offset, gchar expected, gchar x);

            static gchar compareAndExchangeCharRelaxed(Object &o, glong offset, gchar expected, gchar x);

            static gbool weakCompareAndSetChar(Object &o, glong offset, gchar expected, gchar x);

            static gbool weakCompareAndSetCharAcquire(Object &o, glong offset, gchar expected, gchar x);

            static gbool weakCompareAndSetCharRelease(Object &o, glong offset, gchar expected, gchar x);

            static gbool weakCompareAndSetCharRelaxed(Object &o, glong offset, gchar expected, gchar x);

            static gchar weakCompareAndExchangeChar(Object &o, glong offset, gchar expected, gchar x);

            static gchar weakCompareAndExchangeCharAcquire(Object &o, glong offset, gchar expected, gchar x);

            static gchar weakCompareAndExchangeCharRelease(Object &o, glong offset, gchar expected, gchar x);

            static gchar weakCompareAndExchangeCharRelaxed(Object &o, glong offset, gchar expected, gchar x);

            // ----------------- [atomic boolean] ---------------------------------------

            static gbool compareAndSetBoolean(Object &o, glong offset, gbool expected, gbool x);

            static gbool compareAndSetBooleanAcquire(Object &o, glong offset, gbool expected, gbool x);

            static gbool compareAndSetBooleanRelease(Object &o, glong offset, gbool expected, gbool x);

            static gbool compareAndSetBooleanRelaxed(Object &o, glong offset, gbool expected, gbool x);

            static gbool compareAndExchangeBoolean(Object &o, glong offset, gbool expected, gbool x);

            static gbool compareAndExchangeBooleanAcquire(Object &o, glong offset, gbool expected, gbool x);

            static gbool compareAndExchangeBooleanRelease(Object &o, glong offset, gbool expected, gbool x);

            static gbool compareAndExchangeBooleanRelaxed(Object &o, glong offset, gbool expected, gbool x);

            static gbool weakCompareAndSetBoolean(Object &o, glong offset, gbool expected, gbool x);

            static gbool weakCompareAndSetBooleanAcquire(Object &o, glong offset, gbool expected, gbool x);

            static gbool weakCompareAndSetBooleanRelease(Object &o, glong offset, gbool expected, gbool x);

            static gbool weakCompareAndSetBooleanRelaxed(Object &o, glong offset, gbool expected, gbool x);

            static gbool weakCompareAndExchangeBoolean(Object &o, glong offset, gbool expected, gbool x);

            static gbool weakCompareAndExchangeBooleanAcquire(Object &o, glong offset, gbool expected, gbool x);

            static gbool weakCompareAndExchangeBooleanRelease(Object &o, glong offset, gbool expected, gbool x);

            static gbool weakCompareAndExchangeBooleanRelaxed(Object &o, glong offset, gbool expected, gbool x);

            // ----------------- [atomic float] ---------------------------------------

            static gbool compareAndSetFloat(Object &o, glong offset, gfloat expected, gfloat x);

            static gbool compareAndSetFloatAcquire(Object &o, glong offset, gfloat expected, gfloat x);

            static gbool compareAndSetFloatRelease(Object &o, glong offset, gfloat expected, gfloat x);

            static gbool compareAndSetFloatRelaxed(Object &o, glong offset, gfloat expected, gfloat x);

            static gfloat compareAndExchangeFloat(Object &o, glong offset, gfloat expected, gfloat x);

            static gfloat compareAndExchangeFloatAcquire(Object &o, glong offset, gfloat expected, gfloat x);

            static gfloat compareAndExchangeFloatRelease(Object &o, glong offset, gfloat expected, gfloat x);

            static gfloat compareAndExchangeFloatRelaxed(Object &o, glong offset, gfloat expected, gfloat x);

            static gbool weakCompareAndSetFloat(Object &o, glong offset, gfloat expected, gfloat x);

            static gbool weakCompareAndSetFloatAcquire(Object &o, glong offset, gfloat expected, gfloat x);

            static gbool weakCompareAndSetFloatRelease(Object &o, glong offset, gfloat expected, gfloat x);

            static gbool weakCompareAndSetFloatRelaxed(Object &o, glong offset, gfloat expected, gfloat x);

            static gfloat weakCompareAndExchangeFloat(Object &o, glong offset, gfloat expected, gfloat x);

            static gfloat weakCompareAndExchangeFloatAcquire(Object &o, glong offset, gfloat expected, gfloat x);

            static gfloat weakCompareAndExchangeFloatRelease(Object &o, glong offset, gfloat expected, gfloat x);

            static gfloat weakCompareAndExchangeFloatRelaxed(Object &o, glong offset, gfloat expected, gfloat x);

            // ----------------- [atomic double] ---------------------------------------

            static gbool compareAndSetDouble(Object &o, glong offset, gdouble expected, gdouble x);

            static gbool compareAndSetDoubleAcquire(Object &o, glong offset, gdouble expected, gdouble x);

            static gbool compareAndSetDoubleRelease(Object &o, glong offset, gdouble expected, gdouble x);

            static gbool compareAndSetDoubleRelaxed(Object &o, glong offset, gdouble expected, gdouble x);

            static gdouble compareAndExchangeDouble(Object &o, glong offset, gdouble expected, gdouble x);

            static gdouble compareAndExchangeDoubleAcquire(Object &o, glong offset, gdouble expected, gdouble x);

            static gdouble compareAndExchangeDoubleRelease(Object &o, glong offset, gdouble expected, gdouble x);

            static gdouble compareAndExchangeDoubleRelaxed(Object &o, glong offset, gdouble expected, gdouble x);

            static gbool weakCompareAndSetDouble(Object &o, glong offset, gdouble expected, gdouble x);

            static gbool weakCompareAndSetDoubleAcquire(Object &o, glong offset, gdouble expected, gdouble x);

            static gbool weakCompareAndSetDoubleRelease(Object &o, glong offset, gdouble expected, gdouble x);

            static gbool weakCompareAndSetDoubleRelaxed(Object &o, glong offset, gdouble expected, gdouble x);

            static gdouble weakCompareAndExchangeDouble(Object &o, glong offset, gdouble expected, gdouble x);

            static gdouble weakCompareAndExchangeDoubleAcquire(Object &o, glong offset, gdouble expected, gdouble x);

            static gdouble weakCompareAndExchangeDoubleRelease(Object &o, glong offset, gdouble expected, gdouble x);

            static gdouble weakCompareAndExchangeDoubleRelaxed(Object &o, glong offset, gdouble expected, gdouble x);

            // ----------------- [atomic long] ---------------------------------------

            static gbool compareAndSetLong(Object &o, glong offset, glong expected, glong x);

            static gbool compareAndSetLongAcquire(Object &o, glong offset, glong expected, glong x);

            static gbool compareAndSetLongRelease(Object &o, glong offset, glong expected, glong x);

            static gbool compareAndSetLongRelaxed(Object &o, glong offset, glong expected, glong x);

            static glong compareAndExchangeLong(Object &o, glong offset, glong expected, glong x);

            static glong compareAndExchangeLongAcquire(Object &o, glong offset, glong expected, glong x);

            static glong compareAndExchangeLongRelease(Object &o, glong offset, glong expected, glong x);

            static glong compareAndExchangeLongRelaxed(Object &o, glong offset, glong expected, glong x);

            static gbool weakCompareAndSetLong(Object &o, glong offset, glong expected, glong x);

            static gbool weakCompareAndSetLongAcquire(Object &o, glong offset, glong expected, glong x);

            static gbool weakCompareAndSetLongRelease(Object &o, glong offset, glong expected, glong x);

            static gbool weakCompareAndSetLongRelaxed(Object &o, glong offset, glong expected, glong x);

            static glong weakCompareAndExchangeLong(Object &o, glong offset, glong expected, glong x);

            static glong weakCompareAndExchangeLongAcquire(Object &o, glong offset, glong expected, glong x);

            static glong weakCompareAndExchangeLongRelease(Object &o, glong offset, glong expected, glong x);

            static glong weakCompareAndExchangeLongRelaxed(Object &o, glong offset, glong expected, glong x);

            /**
             * Fetches a reference value from a given variable, with volatile
             * load semantics. Otherwise identical to <b style="color: orange;"> getReference(Object, glong)</b>
             */
            static Object &getReferenceVolatile(const Object &o, glong offset);

            /**
             * Stores a reference value into a given variable, with
             * volatile store semantics. Otherwise identical to <b style="color: orange;"> putReference(Object, glong, Object)</b>
             */
            static void putReferenceVolatile(Object &o, glong offset, Object &x);

            /**
             * Fetches a int value from a given variable, with volatile
             * load semantics. Otherwise identical to <b style="color: orange;"> get3(Object, glong)</b>
             */
            static gint getIntVolatile(const Object &o, glong offset);

            /**
             * Stores a int value into a given variable, with
             * volatile store semantics. Otherwise identical to <b style="color: orange;"> putInt(Object, glong, Object)</b>
             */
            static void putIntVolatile(Object &o, glong offset, gint x);

            /**
             * Fetches a boolean value from a given variable, with volatile
             * load semantics. Otherwise identical to <b style="color: orange;"> getBoolean(Object, glong)</b>
             */
            static gbool getBooleanVolatile(const Object &o, glong offset);

            /**
             * Stores a boolean value into a given variable, with
             * volatile store semantics. Otherwise identical to <b style="color: orange;"> putBoolean(Object, glong, Object)</b>
             */
            static void putBooleanVolatile(Object &o, glong offset, gbool x);

            /**
             * Fetches a byte value from a given variable, with volatile
             * load semantics. Otherwise identical to <b style="color: orange;"> getByte(Object, glong)</b>
             */
            static gbyte getByteVolatile(const Object &o, glong offset);

            /**
             * Stores a byte value into a given variable, with
             * volatile store semantics. Otherwise identical to <b style="color: orange;"> putByte(Object, glong, Object)</b>
             */
            static void putByteVolatile(Object &o, glong offset, gbyte x);

            /**
             * Fetches a short value from a given variable, with volatile
             * load semantics. Otherwise identical to <b style="color: orange;"> getShort(Object, glong)</b>
             */
            static gshort getShortVolatile(const Object &o, glong offset);

            /**
             * Stores a short value into a given variable, with
             * volatile store semantics. Otherwise identical to <b style="color: orange;"> putShort(Object, glong, Object)</b>
             */
            static void putShortVolatile(Object &o, glong offset, gshort x);

            /**
             * Fetches a char value from a given variable, with volatile
             * load semantics. Otherwise identical to <b style="color: orange;"> getChar(Object, glong)</b>
             */
            static gchar getCharVolatile(const Object &o, glong offset);

            /**
             * Stores a char value into a given variable, with
             * volatile store semantics. Otherwise identical to <b style="color: orange;"> putChar(Object, glong, Object)</b>
             */
            static void putCharVolatile(Object &o, glong offset, gchar x);

            /**
             * Fetches a long value from a given variable, with volatile
             * load semantics. Otherwise identical to <b style="color: orange;"> getLong(Object, glong)</b>
             */
            static glong getLongVolatile(const Object &o, glong offset);

            /**
             * Stores a glong value into a given variable, with
             * volatile store semantics. Otherwise identical to <b style="color: orange;"> putLong(Object, glong, Object)</b>
             */
            static void putLongVolatile(Object &o, glong offset, glong x);

            /**
             * Fetches a float value from a given variable, with volatile
             * load semantics. Otherwise identical to <b style="color: orange;"> getFloat(Object, glong)</b>
             */
            static gfloat getFloatVolatile(const Object &o, glong offset);

            /**
             * Stores a float value into a given variable, with
             * volatile store semantics. Otherwise identical to <b style="color: orange;"> putFloat(Object, glong, Object)</b>
             */
            static void putFloatVolatile(Object &o, glong offset, gfloat x);

            /**
             * Fetches a double value from a given variable, with volatile
             * load semantics. Otherwise identical to <b style="color: orange;"> getDouble(Object, glong)</b>
             */
            static gdouble getDoubleVolatile(const Object &o, glong offset);

            /**
             * Stores a double value into a given variable, with
             * volatile store semantics. Otherwise identical to <b style="color: orange;"> putDouble(Object, glong, Object)</b>
             */
            static void putDoubleVolatile(Object &o, glong offset, gdouble x);

            // ---------------------- ACQUIRE --------------------------------------

            /** Acquire version of <b style="color: orange;"> getReferenceVolatile(Object, glong)</b> */
            static Object &getReferenceAcquire(const Object &o, glong offset);

            /** Acquire version of <b style="color: orange;"> getBooleanVolatile(Object, glong)</b> */
            static gbool getBooleanAcquire(const Object &o, glong offset);

            /** Acquire version of <b style="color: orange;"> getByteVolatile(Object, glong)</b> */
            static gbyte getByteAcquire(const Object &o, glong offset);

            /** Acquire version of <b style="color: orange;"> getShortVolatile(Object, glong)</b> */
            static gshort getShortAcquire(const Object &o, glong offset);

            /** Acquire version of <b style="color: orange;"> getCharVolatile(Object, glong)</b> */
            static gchar getCharAcquire(const Object &o, glong offset);

            /** Acquire version of <b style="color: orange;"> getIntVolatile(Object, glong)</b> */
            static gint getIntAcquire(const Object &o, glong offset);

            /** Acquire version of <b style="color: orange;"> getLongVolatile(Object, glong)</b> */
            static glong getLongAcquire(const Object &o, glong offset);

            /** Acquire version of <b style="color: orange;"> getFloatVolatile(Object, glong)</b> */
            static gfloat getFloatAcquire(const Object &o, glong offset);

            /** Acquire version of <b style="color: orange;"> getDoubleVolatile(Object, glong)</b> */
            static gdouble getDoubleAcquire(const Object &o, glong offset);

            // ---------------------- RELEASE --------------------------------------

            /**
             * Versions of <b style="color: orange;"> putReferenceVolatile(Object, glong, Object)</b>
             * that do not guarantee immediate visibility of the store to
             * other threads. This method is generally only useful if the
             * underlying field is a volatile (or if an array cell, one
             * that is otherwise only accessed using volatile accesses).
             *
             * Corresponds to C11 atomic_store_explicit(..., memory_order_release).
             */

            /** Release version of <b style="color: orange;"> putReferenceVolatile(Object, glong, Object)</b> */
            static void putReferenceRelease(Object &o, glong offset, Object &x);

            /** Release version of <b style="color: orange;"> putBooleanVolatile(Object, glong, gbool)</b> */
            static void putBooleanRelease(Object &o, glong offset, gbool x);

            /** Release version of <b style="color: orange;"> putByteVolatile(Object, glong, byte)</b> */
            static void putByteRelease(Object &o, glong offset, gbyte x);

            /** Release version of <b style="color: orange;"> putShortVolatile(Object, glong, short)</b> */
            static void putShortRelease(Object &o, glong offset, gshort x);

            /** Release version of <b style="color: orange;"> putCharVolatile(Object, glong, char)</b> */
            static void putCharRelease(Object &o, glong offset, gchar x);

            /** Release version of <b style="color: orange;"> putIntVolatile(Object, glong, gint)</b> */
            static void putIntRelease(Object &o, glong offset, gint x);

            /** Release version of <b style="color: orange;"> putLongVolatile(Object, glong, glong)</b> */
            static void putLongRelease(Object &o, glong offset, glong x);

            /** Release version of <b style="color: orange;"> putFloatVolatile(Object, glong, float)</b> */
            static void putFloatRelease(Object &o, glong offset, gfloat x);

            /** Release version of <b style="color: orange;"> putDoubleVolatile(Object, glong, double)</b> */
            static void putDoubleRelease(Object &o, glong offset, gdouble x);

            // ---------------------- RELAXED --------------------------------------

            /** Relaxed version of <b style="color: orange;"> getReferenceVolatile(Object, glong)</b> */
            static Object &getReferenceRelaxed(const Object &o, glong offset);

            /** Relaxed version of <b style="color: orange;"> getBooleanVolatile(Object, glong)</b> */
            static gbool getBooleanRelaxed(const Object &o, glong offset);

            /** Relaxed version of <b style="color: orange;"> getByteVolatile(Object, glong)</b> */
            static gbyte getByteRelaxed(const Object &o, glong offset);

            /** Relaxed version of <b style="color: orange;"> getShortVolatile(Object, glong)</b> */
            static gshort getShortRelaxed(const Object &o, glong offset);

            /** Relaxed version of <b style="color: orange;"> getCharVolatile(Object, glong)</b> */
            static gchar getCharRelaxed(const Object &o, glong offset);

            /** Relaxed version of <b style="color: orange;"> getIntVolatile(Object, glong)</b> */
            static gint getIntRelaxed(const Object &o, glong offset);

            /** Relaxed version of <b style="color: orange;"> getLongVolatile(Object, glong)</b> */
            static glong getLongRelaxed(const Object &o, glong offset);

            /** Relaxed version of <b style="color: orange;"> getFloatVolatile(Object, glong)</b> */
            static gfloat getFloatRelaxed(const Object &o, glong offset);

            /** Relaxed version of <b style="color: orange;"> getDoubleVolatile(Object, glong)</b> */
            static gdouble getDoubleRelaxed(const Object &o, glong offset);

            /** Relaxed version of <b style="color: orange;"> putReferenceVolatile(Object, glong, Object)</b> */
            static void putReferenceRelaxed(Object &o, glong offset, Object &x);

            /** Relaxed version of <b style="color: orange;"> putBooleanVolatile(Object, glong, gbool)</b> */
            static void putBooleanRelaxed(Object &o, glong offset, gbool x);

            /** Relaxed version of <b style="color: orange;"> putByteVolatile(Object, glong, byte)</b> */
            static void putByteRelaxed(Object &o, glong offset, gbyte x);

            /** Relaxed version of <b style="color: orange;"> putShortVolatile(Object, glong, short)</b> */
            static void putShortRelaxed(Object &o, glong offset, gshort x);

            /** Relaxed version of <b style="color: orange;"> putCharVolatile(Object, glong, char)</b> */
            static void putCharRelaxed(Object &o, glong offset, gchar x);

            /** Relaxed version of <b style="color: orange;"> putIntVolatile(Object, glong, gint)</b> */
            static void putIntRelaxed(Object &o, glong offset, gint x);

            /** Relaxed version of <b style="color: orange;"> putLongVolatile(Object, glong, glong)</b> */
            static void putLongRelaxed(Object &o, glong offset, glong x);

            /** Relaxed version of <b style="color: orange;"> putFloatVolatile(Object, glong, float)</b> */
            static void putFloatRelaxed(Object &o, glong offset, gfloat x);

            /** Relaxed version of <b style="color: orange;"> putDoubleVolatile(Object, glong, double)</b> */
            static void putDoubleRelaxed(Object &o, glong offset, gdouble x);

            /**
             * Atomically adds the given value to the current value of a field
             * or array element within the given object <b> o</b>
             * at the given <b> offset</b>.
             *
             * @param o object/array to update the field/element in
             * @param offset field/element offset
             * @param delta the value to add
             * @return the previous value
             */
            static gint getAndAddInt(Object &o, glong offset, gint delta);

            static gint getAndAddIntAcquire(Object &o, glong offset, gint delta);

            static gint getAndAddIntRelease(Object &o, glong offset, gint delta);

            static gint getAndAddIntRelaxed(Object &o, glong offset, gint delta);

            /**
             * Atomically adds the given value to the current value of a field
             * or array element within the given object <b> o</b>
             * at the given <b> offset</b>.
             *
             * @param o object/array to update the field/element in
             * @param offset field/element offset
             * @param delta the value to add
             * @return the previous value
             */
            static glong getAndAddLong(Object &o, glong offset, glong delta);

            static glong getAndAddLongAcquire(Object &o, glong offset, glong delta);

            static glong getAndAddLongRelease(Object &o, glong offset, glong delta);

            static glong getAndAddLongRelaxed(Object &o, glong offset, glong delta);

            static gbyte getAndAddByte(Object &o, glong offset, gbyte delta);

            static gbyte getAndAddByteAcquire(Object &o, glong offset, gbyte delta);

            static gbyte getAndAddByteRelease(Object &o, glong offset, gbyte delta);

            static gbyte getAndAddByteRelaxed(Object &o, glong offset, gbyte delta);

            static gshort getAndAddShort(Object &o, glong offset, gshort delta);

            static gshort getAndAddShortAcquire(Object &o, glong offset, gshort delta);

            static gshort getAndAddShortRelease(Object &o, glong offset, gshort delta);

            static gshort getAndAddShortRelaxed(Object &o, glong offset, gshort delta);

            static gchar getAndAddChar(Object &o, glong offset, gchar delta);

            static gchar getAndAddCharAcquire(Object &o, glong offset, gchar delta);

            static gchar getAndAddCharRelease(Object &o, glong offset, gchar delta);

            static gchar getAndAddCharRelaxed(Object &o, glong offset, gchar delta);

            static gfloat getAndAddFloat(Object &o, glong offset, gfloat delta);

            static gfloat getAndAddFloatAcquire(Object &o, glong offset, gfloat delta);

            static gfloat getAndAddFloatRelease(Object &o, glong offset, gfloat delta);

            static gfloat getAndAddFloatRelaxed(Object &o, glong offset, gfloat delta);

            static gdouble getAndAddDouble(Object &o, glong offset, gdouble delta);

            static gdouble getAndAddDoubleAcquire(Object &o, glong offset, gdouble delta);

            static gdouble getAndAddDoubleRelease(Object &o, glong offset, gdouble delta);

            static gdouble getAndAddDoubleRelaxed(Object &o, glong offset, gdouble delta);

            /**
             * Atomically exchanges the given value with the current value of
             * a field or array element within the given object <b> o</b>
             * at the given <b> offset</b>.
             *
             * @param o object/array to update the field/element in
             * @param offset field/element offset
             * @param newValue new value
             * @return the previous value
             */
            static gint getAndSetInt(Object &o, glong offset, gint newValue);

            static gint getAndSetIntAcquire(Object &o, glong offset, gint newValue);

            static gint getAndSetIntRelease(Object &o, glong offset, gint newValue);

            static gint getAndSetIntRelaxed(Object &o, glong offset, gint newValue);

            /**
             * Atomically exchanges the given value with the current value of
             * a field or array element within the given object <b> o</b>
             * at the given <b> offset</b>.
             *
             * @param o object/array to update the field/element in
             * @param offset field/element offset
             * @param newValue new value
             * @return the previous value
             */
            static glong getAndSetLong(Object &o, glong offset, glong newValue);

            static glong getAndSetLongAcquire(Object &o, glong offset, glong newValue);

            static glong getAndSetLongRelease(Object &o, glong offset, glong newValue);

            static glong getAndSetLongRelaxed(Object &o, glong offset, glong newValue);

            static gbyte getAndSetByte(Object &o, glong offset, gbyte newValue);

            static gbyte getAndSetByteAcquire(Object &o, glong offset, gbyte newValue);

            static gbyte getAndSetByteRelease(Object &o, glong offset, gbyte newValue);

            static gbyte getAndSetByteRelaxed(Object &o, glong offset, gbyte newValue);

            static gbool getAndSetBoolean(Object &o, glong offset, gbool newValue);

            static gbool getAndSetBooleanAcquire(Object &o, glong offset, gbool newValue);

            static gbool getAndSetBooleanRelease(Object &o, glong offset, gbool newValue);

            static gbool getAndSetBooleanRelaxed(Object &o, glong offset, gbool newValue);

            static gshort getAndSetShort(Object &o, glong offset, gshort newValue);

            static gshort getAndSetShortAcquire(Object &o, glong offset, gshort newValue);

            static gshort getAndSetShortRelease(Object &o, glong offset, gshort newValue);

            static gshort getAndSetShortRelaxed(Object &o, glong offset, gshort newValue);

            static gchar getAndSetChar(Object &o, glong offset, gchar newValue);

            static gchar getAndSetCharAcquire(Object &o, glong offset, gchar newValue);

            static gchar getAndSetCharRelease(Object &o, glong offset, gchar newValue);

            static gchar getAndSetCharRelaxed(Object &o, glong offset, gchar newValue);

            static gfloat getAndSetFloat(Object &o, glong offset, gfloat newValue);

            static gfloat getAndSetFloatAcquire(Object &o, glong offset, gfloat newValue);

            static gfloat getAndSetFloatRelease(Object &o, glong offset, gfloat newValue);

            static gfloat getAndSetFloatRelaxed(Object &o, glong offset, gfloat newValue);

            static gdouble getAndSetDouble(Object &o, glong offset, gdouble newValue);

            static gdouble getAndSetDoubleAcquire(Object &o, glong offset, gdouble newValue);

            static gdouble getAndSetDoubleRelease(Object &o, glong offset, gdouble newValue);

            static gdouble getAndSetDoubleRelaxed(Object &o, glong offset, gdouble newValue);

            static Object &getAndSetReference(Object &o, glong offset, Object &newValue);

            static Object &getAndSetReferenceAcquire(Object &o, glong offset, Object &newValue);

            static Object &getAndSetReferenceRelease(Object &o, glong offset, Object &newValue);

            static Object &getAndSetReferenceRelaxed(Object &o, glong offset, Object &newValue);


            // The following contain CAS-based implementations used on
            // platforms not supporting native instructions

            static gbyte getAndBitwiseOrByte(Object &o, glong offset, gbyte mask);

            static gbyte getAndBitwiseOrByteAcquire(Object &o, glong offset, gbyte mask);

            static gbyte getAndBitwiseOrByteRelease(Object &o, glong offset, gbyte mask);

            static gbyte getAndBitwiseOrByteRelaxed(Object &o, glong offset, gbyte mask);

            static gbool getAndBitwiseOrBoolean(Object &o, glong offset, gbool mask);

            static gbool getAndBitwiseOrBooleanAcquire(Object &o, glong offset, gbool mask);

            static gbool getAndBitwiseOrBooleanRelease(Object &o, glong offset, gbool mask);

            static gbool getAndBitwiseOrBooleanRelaxed(Object &o, glong offset, gbool mask);

            static gshort getAndBitwiseOrShort(Object &o, glong offset, gshort mask);

            static gshort getAndBitwiseOrShortAcquire(Object &o, glong offset, gshort mask);

            static gshort getAndBitwiseOrShortRelease(Object &o, glong offset, gshort mask);

            static gshort getAndBitwiseOrShortRelaxed(Object &o, glong offset, gshort mask);

            static gchar getAndBitwiseOrChar(Object &o, glong offset, gchar mask);

            static gchar getAndBitwiseOrCharAcquire(Object &o, glong offset, gchar mask);

            static gchar getAndBitwiseOrCharRelease(Object &o, glong offset, gchar mask);

            static gchar getAndBitwiseOrCharRelaxed(Object &o, glong offset, gchar mask);

            static gint getAndBitwiseOrInt(Object &o, glong offset, gint mask);

            static gint getAndBitwiseOrIntAcquire(Object &o, glong offset, gint mask);

            static gint getAndBitwiseOrIntRelease(Object &o, glong offset, gint mask);

            static gint getAndBitwiseOrIntRelaxed(Object &o, glong offset, gint mask);

            static glong getAndBitwiseOrLong(Object &o, glong offset, glong mask);

            static glong getAndBitwiseOrLongAcquire(Object &o, glong offset, glong mask);

            static glong getAndBitwiseOrLongRelease(Object &o, glong offset, glong mask);

            static glong getAndBitwiseOrLongRelaxed(Object &o, glong offset, glong mask);

            static gbyte getAndBitwiseAndByte(Object &o, glong offset, gbyte mask);

            static gbyte getAndBitwiseAndByteAcquire(Object &o, glong offset, gbyte mask);

            static gbyte getAndBitwiseAndByteRelease(Object &o, glong offset, gbyte mask);

            static gbyte getAndBitwiseAndByteRelaxed(Object &o, glong offset, gbyte mask);

            static gbool getAndBitwiseAndBoolean(Object &o, glong offset, gbool mask);

            static gbool getAndBitwiseAndBooleanAcquire(Object &o, glong offset, gbool mask);

            static gbool getAndBitwiseAndBooleanRelease(Object &o, glong offset, gbool mask);

            static gbool getAndBitwiseAndBooleanRelaxed(Object &o, glong offset, gbool mask);

            static gshort getAndBitwiseAndShort(Object &o, glong offset, gshort mask);

            static gshort getAndBitwiseAndShortAcquire(Object &o, glong offset, gshort mask);

            static gshort getAndBitwiseAndShortRelease(Object &o, glong offset, gshort mask);

            static gshort getAndBitwiseAndShortRelaxed(Object &o, glong offset, gshort mask);

            static gchar getAndBitwiseAndChar(Object &o, glong offset, gchar mask);

            static gchar getAndBitwiseAndCharAcquire(Object &o, glong offset, gchar mask);

            static gchar getAndBitwiseAndCharRelease(Object &o, glong offset, gchar mask);

            static gchar getAndBitwiseAndCharRelaxed(Object &o, glong offset, gchar mask);

            static gint getAndBitwiseAndInt(Object &o, glong offset, gint mask);

            static gint getAndBitwiseAndIntAcquire(Object &o, glong offset, gint mask);

            static gint getAndBitwiseAndIntRelease(Object &o, glong offset, gint mask);

            static gint getAndBitwiseAndIntRelaxed(Object &o, glong offset, gint mask);

            static glong getAndBitwiseAndLong(Object &o, glong offset, glong mask);

            static glong getAndBitwiseAndLongAcquire(Object &o, glong offset, glong mask);

            static glong getAndBitwiseAndLongRelease(Object &o, glong offset, glong mask);

            static glong getAndBitwiseAndLongRelaxed(Object &o, glong offset, glong mask);

            static gbyte getAndBitwiseXorByte(Object &o, glong offset, gbyte mask);

            static gbyte getAndBitwiseXorByteAcquire(Object &o, glong offset, gbyte mask);

            static gbyte getAndBitwiseXorByteRelease(Object &o, glong offset, gbyte mask);

            static gbyte getAndBitwiseXorByteRelaxed(Object &o, glong offset, gbyte mask);

            static gbool getAndBitwiseXorBoolean(Object &o, glong offset, gbool mask);

            static gbool getAndBitwiseXorBooleanAcquire(Object &o, glong offset, gbool mask);

            static gbool getAndBitwiseXorBooleanRelease(Object &o, glong offset, gbool mask);

            static gbool getAndBitwiseXorBooleanRelaxed(Object &o, glong offset, gbool mask);

            static gshort getAndBitwiseXorShort(Object &o, glong offset, gshort mask);

            static gshort getAndBitwiseXorShortAcquire(Object &o, glong offset, gshort mask);

            static gshort getAndBitwiseXorShortRelease(Object &o, glong offset, gshort mask);

            static gshort getAndBitwiseXorShortRelaxed(Object &o, glong offset, gshort mask);

            static gchar getAndBitwiseXorChar(Object &o, glong offset, gchar mask);

            static gchar getAndBitwiseXorCharAcquire(Object &o, glong offset, gchar mask);

            static gchar getAndBitwiseXorCharRelease(Object &o, glong offset, gchar mask);

            static gchar getAndBitwiseXorCharRelaxed(Object &o, glong offset, gchar mask);

            static gint getAndBitwiseXorInt(Object &o, glong offset, gint mask);

            static gint getAndBitwiseXorIntAcquire(Object &o, glong offset, gint mask);

            static gint getAndBitwiseXorIntRelease(Object &o, glong offset, gint mask);

            static gint getAndBitwiseXorIntRelaxed(Object &o, glong offset, gint mask);

            static glong getAndBitwiseXorLong(Object &o, glong offset, glong mask);

            static glong getAndBitwiseXorLongAcquire(Object &o, glong offset, glong mask);

            static glong getAndBitwiseXorLongRelease(Object &o, glong offset, glong mask);

            static glong getAndBitwiseXorLongRelaxed(Object &o, glong offset, glong mask);

            /**
             * Ensures that loads before the fence will not be reordered with loads and
             * stores after the fence; a "LoadLoad plus LoadStore barrier".
             *
             * Corresponds to C11 atomic_thread_fence(memory_order_acquire)
             * (an "acquire fence").
             *
             * Provides a LoadLoad barrier followed by a LoadStore barrier.
             *
             */
            static void loadFence();

            /**
             * Ensures that loads and stores before the fence will not be reordered with
             * stores after the fence; a "StoreStore plus LoadStore barrier".
             *
             * Corresponds to C11 atomic_thread_fence(memory_order_release)
             * (a "release fence").
             *
             * Provides a StoreStore barrier followed by a LoadStore barrier.
             *
             */
            static void storeFence();

            /**
             * Ensures that loads and stores before the fence will not be reordered
             * with loads and stores after the fence.  Implies the effects of both
             * loadFence() and storeFence(), and in addition, the effect of a StoreLoad
             * barrier.
             *
             * Corresponds to C11 atomic_thread_fence(memory_order_seq_cst).
             */
            static void fullFence();

            /**
             * Fetches a value at some byte offset into a given object.
             * More specifically, fetches a value within the given object
             * <code>o</code> at the given offset, or (if <code>o</code> is
             * null) from the memory address whose numerical value is the
             * given offset.  <p>
             *
             * The specification of this method is the same as <b style="color: orange;">
             * getLong(Object, glong)</b> except that the offset does not need to
             * have been obtained from <b style="color: orange;"> offsetof </b> on the
             * <b style="color: orange;"> &lt;stddef.h&gt; </b> of some field.  The value
             * in memory is raw data, and need not correspond to any
             * variable.  Unless <code>o</code> is null, the value accessed
             * must be entirely within the allocated object.  The endianness
             * of the value in memory is the endianness of the native platform.
             *
             * <p> The read will be atomic with respect to the largest power
             * of two that divides the GCD of the offset and the storage size.
             * For example, getLongUnaligned will make atomic reads of 2-, 4-,
             * or 8-byte storage units if the offset is zero mod 2, 4, or 8,
             * respectively.  There are no other guarantees of atomicity.
             * <p>
             * 8-byte atomicity is only guaranteed on platforms on which
             * support atomic accesses to longs.
             *
             * @param o heap object in which the value resides, if any, else
             *        null
             * @param offset The offset in bytes from the start of the object
             * @return the value fetched from the indicated object
             * @throws RuntimeException No defined exceptions are thrown
             */
            static glong getLongUnaligned(const Object &o, glong offset);

            /**
             * As <b style="color: orange;"> getLongUnaligned(Object, glong)</b> but with an
             * additional argument which specifies the endianness of the value
             * as stored in memory.
             *
             * @param o heap object in which the variable resides
             * @param offset The offset in bytes from the start of the object
             * @param bigEndian The endianness of the value
             * @return the value fetched from the indicated object
             */
            static glong getLongUnaligned(const Object &o, glong offset, gbool bigEndian);

            /** @see getLongUnaligned(Object, glong) */
            static gint getIntUnaligned(const Object &o, glong offset);

            /** @see getLongUnaligned(Object, glong, gbool) */
            static gint getIntUnaligned(const Object &o, glong offset, gbool bigEndian);

            /** @see getLongUnaligned(Object, glong) */
            static gshort getShortUnaligned(const Object &o, glong offset);

            /** @see getLongUnaligned(Object, glong, gbool) */
            static gshort getShortUnaligned(const Object &o, glong offset, gbool bigEndian);

            /** @see getLongUnaligned(Object, glong) */
            static gchar getCharUnaligned(const Object &o, glong offset);

            /** @see getLongUnaligned(Object, glong, gbool) */
            static gchar getCharUnaligned(const Object &o, glong offset, gbool bigEndian);

            /**
             * Stores a value at some byte offset into a given object.
             * <p>
             * The specification of this method is the same as <b style="color: orange;">
             * getLong(Object, glong)</b> except that the offset does not need to
             * have been obtained from <b style="color: orange;"> offsetof </b> on the
             * <b style="color: orange;"> &lt;stddef.h&gt; </b> of some field.  The value
             * in memory is raw data, and need not correspond to any
             * variable.  The endianness of the value in memory is the
             * endianness of the native platform.
             * <p>
             * The write will be atomic with respect to the largest power of
             * two that divides the GCD of the offset and the storage size.
             * For example, putLongUnaligned will make atomic writes of 2-, 4-,
             * or 8-byte storage units if the offset is zero mod 2, 4, or 8,
             * respectively.  There are no other guarantees of atomicity.
             * <p>
             * 8-byte atomicity is only guaranteed on platforms on which
             * support atomic accesses to longs.
             *
             * @param o heap object in which the value resides, if any, else
             *        null
             * @param offset The offset in bytes from the start of the object
             * @param x the value to store
             * @throws RuntimeException No defined exceptions are thrown
             */
            static void putLongUnaligned(Object &o, glong offset, glong x);

            /**
             * As <b style="color: orange;"> putLongUnaligned(Object, glong, glong)</b> but with an additional
             * argument which specifies the endianness of the value as stored in memory.
             * @param o heap object in which the value resides
             * @param offset The offset in bytes from the start of the object
             * @param x the value to store
             * @param bigEndian The endianness of the value
             * @throws RuntimeException No defined exceptions are thrown
             */
            static void putLongUnaligned(Object &o, glong offset, glong x, gbool bigEndian);

            /** @see putLongUnaligned(Object, glong, glong) */
            static void putIntUnaligned(Object &o, glong offset, gint x);

            /** @see putLongUnaligned(Object, glong, glong, gbool) */
            static void putIntUnaligned(Object &o, glong offset, gint x, gbool bigEndian);

            /** @see putLongUnaligned(Object, glong, glong) */
            static void putShortUnaligned(Object &o, glong offset, gshort x);

            /** @see putLongUnaligned(Object, glong, glong, gbool) */
            static void putShortUnaligned(Object &o, glong offset, gshort x, gbool bigEndian);

            /** @see putLongUnaligned(Object, glong, glong) */
            static void putCharUnaligned(Object &o, glong offset, gchar x);

            /** @see putLongUnaligned(Object, glong, glong, gbool) */
            static void putCharUnaligned(Object &o, glong offset, gchar x, gbool bigEndian);

            /**
             * Convert given value to rvalue reference.
             *
             * @param var The given value.
             */
            template<class T>
            static CORE_FAST Class<T>::NRef &&moveInstance(T &&var) { return (typename Class<T>::NRef &&) var; }

            /**
             * Perfect forwarding.
             *
             * @param var The given value.
             */
            template<class T>
            static CORE_FAST T &&forwardInstance(typename Class<T>::NRef &var) CORE_NOTHROW { return (T &&) var; }

            /**
             * Perfect forwarding.
             *
             * @param var The given value.
             */
            template<class T>
            static CORE_FAST T &&forwardInstance(typename Class<T>::NRef &&var) CORE_NOTHROW {
                CORE_STATIC_ASSERT(Class<T>::isLvalueReference(), "Forwarding is supported by lvalue reference");
                return (T &&) var;
            }

            /**
             * create instance and store address for future utilisation.
             * to reuse store instance use <b>copyInstance(..., true) </b>
             */
            template<class T, class...Params>
            static T &createInstance(Params &&...params) {
                CORE_STATIC_ASSERT(!Class<T>::oneIsTrue(Class<Void>::isSimilar<Params>()...), "Illegal parameter type");
                CORE_STATIC_ASSERT(Class<T>::template isConstructible<Params...>(), "Incompatible parameters");
                CORE_ALIAS(PTR, typename Class<T>::Ptr);
                try {
                    GENERIC_PTR ptr = (new T(forwardInstance<Params>(params)...));
                    glong addr = (glong) ptr;
                    storeInstance(addr);
                    return ((PTR) ptr)[/*offset*/0];
                } catch (std::bad_alloc &) { Error("Unable to create object").throws(__trace("core.native.Unsafe")); }
            }

            /**
             * Copy instance or load stored copy of given instance.
             *
             * @param o The instance to be copied
             * @param oldCopy specified if this method return stored instance if it possible.
             */
            template<class T>
            static T &copyInstance(const T &o, gbool oldCopy = false) {
                return ((oldCopy) && loadInstance((glong) &o)) ? (T &) o : CopyImpl<T>::copy(o);
            }

            /**
             * Destroy and free instance created dynamically.
             * @note the given instance must be allocated by calling of Unsafe::createInstance<T>(...) method.
             */
            template<class T>
            static void destroyInstance(T &var) CORE_NOTHROW {
                glong addr = getAddress(var, 0);
                // after this operation addr is not reusable
                delete &var;
                deleteInstance(addr);
            }

        private:
            template<class T,
                    gbool isCloneable = Class<Object>::isSuper<T>(),
                    gbool isCopyable = Class<T>::template isConstructible<const T &>()>
            class CopyImpl {
            public:
                static T &copy(const T & /*x*/) {
                    CORE_STATIC_ASSERT(isCloneable || isCopyable, "Noncopyable and Non-cloneable class");
                }
            };

            template<class T>
            class CopyImpl<T, true, true> CORE_FINAL {
            public:
                static T &copy(const T &x) CORE_NOTHROW {
                    try { return CopyImpl<T, true, false>::copy(x); }
                    catch (const CloneNotSupportedException &) { return CopyImpl<T, false, true>::copy(x); }
                }
            };

            template<class T>
            class CopyImpl<T, true, false> CORE_FINAL {
            public:
                static T &copy(const T &x) CORE_NOTHROW { return (T &) x.clone(); }
            };

            template<class T>
            class CopyImpl<T, false, true> CORE_FINAL {
            public:
                static T &copy(const T &x) CORE_NOTHROW { return createInstance<T>(x); }
            };

            static gbool loadInstance(glong address);

            static void storeInstance(glong address);

            static void deleteInstance(glong address);

            static glong allocateMemoryImpl(glong sizeInBytes);

            static glong reallocateMemoryImpl(glong address, glong sizeInBytes);

            static void setMemoryImpl(glong address, glong sizeInBytes, gbyte value);

            static void copyMemoryImpl(glong srcAddress, glong destAddress, glong sizeInBytes);

            static void copySwapMemoryImpl(glong srcAddress, glong destAddress, glong bytes, glong elemSize);

            static void freeMemoryImpl(glong address);

        };

    }

} // core

#endif //CORE23_UNSAFE_H

//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_ATOMICLONGARRAY_H
#define CORE23_ATOMICLONGARRAY_H

#include <core/Object.h>
#include <core/native/LongArray.h>
#include <core/private/Unsafe.h>

namespace core {
    namespace util {

        /**
         * A <b> glong</b> array in which elements may be updated atomically.
         * See the <b style="color:orange;">AtomicLong</b> specification for descriptions of the
         * properties of atomic accesses.
         *
         * @author Brunshweeck Tazeussong
         */
        class AtomicLongArray CORE_FINAL : public Object {
        private:
            CORE_ALIAS(U, native::Unsafe);

            LongArray array = {};

        public:

            /**
             * Creates a new AtomicLongArray of the given length, with all
             * elements initially zero.
             *
             * @param length the length of the array
             */
            CORE_EXPLICIT AtomicLongArray(gint length);

            /**
             * Creates a new AtomicLongArray with the same length as, and
             * all elements copied from, the given array.
             *
             * @param array the array to copy elements from
             */
            CORE_EXPLICIT AtomicLongArray(const LongArray &array);

            /**
             * Returns the length of the array.
             *
             * @return the length of the array
             */
            gint length() const;

            /**
             * Returns the current value of the element at index <b> i</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicLong::getVolatile</b> .
             *
             * @param i the index
             * @return the current value
             */
            glong get(gint i) const;

            /**
             * Sets the element at index <b> i</b>  to <b> newValue</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicLong#setVolatile</b> .
             *
             * @param i the index
             * @param newValue the new value
             */
            void set(gint i, glong newValue);

            /**
             * Sets the element at index <b> i</b>  to <b> newValue</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicLong#setRelease</b> .
             *
             * @param i the index
             * @param newValue the new value
             * 
             */
            void lazySet(gint i, glong newValue);

            /**
             * Atomically sets the element at index <b> i</b>  to {@code
             * newValue</b>  and returns the old value,
             * with memory effects as specified by <b style="color:orange;">AtomicLong::getAndSet</b> .
             *
             * @param i the index
             * @param newValue the new value
             * @return the previous value
             */
            glong getAndSet(gint i, glong newValue);

            /**
             * Atomically sets the element at index <b> i</b>  to {@code
             * newValue</b>  if the element's current value <b> == expectedValue</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicLong::compareAndSet</b> .
             *
             * @param i the index
             * @param expectedValue the expected value
             * @param newValue the new value
             * @return <b> true</b>  if successful. False return indicates that
             * the actual value was not equal to the expected value.
             */
            gbool compareAndSet(gint i, glong expectedValue, glong newValue);

            /**
             * Possibly atomically sets the element at index <b> i</b>  to
             * <b> newValue</b>  if the element's current value <b> == expectedValue</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicLong#weakCompareAndSetPlain</b> .
             *
             * @param i the index
             * @param expectedValue the expected value
             * @param newValue the new value
             * @return <b> true</b>  if successful
             * 
             */
            gbool weakCompareAndSetPlain(gint i, glong expectedValue, glong newValue);

            /**
             * Atomically increments the value of the element at index <b> i</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicLong::getAndAdd</b> .
             *
             * <p>Equivalent to <b> getAndAdd(i, 1)</b> .
             *
             * @param i the index
             * @return the previous value
             */
            glong getAndIncrement(gint i);

            /**
             * Atomically decrements the value of the element at index <b> i</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicLong::getAndAdd</b> .
             *
             * <p>Equivalent to <b> getAndAdd(i, -1)</b> .
             *
             * @param i the index
             * @return the previous value
             */
            glong getAndDecrement(gint i);

            /**
             * Atomically adds the given value to the element at index <b> i</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicLong::getAndAdd</b> .
             *
             * @param i the index
             * @param delta the value to add
             * @return the previous value
             */
            glong getAndAdd(gint i, glong delta);

            /**
             * Atomically increments the value of the element at index <b> i</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicLong::getAndAdd</b> .
             *
             * <p>Equivalent to <b> addAndGet(i, 1)</b> .
             *
             * @param i the index
             * @return the updated value
             */
            glong incrementAndGet(gint i);

            /**
             * Atomically decrements the value of the element at index <b> i</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicLong::getAndAdd</b> .
             *
             * <p>Equivalent to <b> addAndGet(i, -1)</b> .
             *
             * @param i the index
             * @return the updated value
             */
            glong decrementAndGet(gint i);

            /**
             * Atomically adds the given value to the element at index <b> i</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicLong::getAndAdd</b> .
             *
             * @param i the index
             * @param delta the value to add
             * @return the updated value
             */
            glong addAndGet(gint i, glong delta);

            /**
             * Returns the String representation of the current values of array.
             * @return the String representation of the current values of array
             */
            String toString() const override;

            /**
             * Returns the current value of the element at index <b> i</b> ,
             * with memory semantics of reading as if the variable was declared
             * non-<b> volatile</b> .
             *
             * @param i the index
             * @return the value
             * 
             */
            glong getPlain(gint i) const;

            /**
             * Sets the element at index <b> i</b>  to <b> newValue</b> ,
             * with memory semantics of setting as if the variable was
             * declared non-<b> volatile</b>  and non-<b> final</b> .
             *
             * @param i the index
             * @param newValue the new value
             * 
             */
            void setPlain(gint i, glong newValue);

            /**
             * Returns the current value of the element at index <b> i</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicLong::getOpaque</b> .
             *
             * @param i the index
             * @return the value
             * 
             */
            glong getOpaque(gint i) const;

            /**
             * Sets the element at index <b> i</b>  to <b> newValue</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicLong#setOpaque</b> .
             *
             * @param i the index
             * @param newValue the new value
             * 
             */
            void setOpaque(gint i, glong newValue);

            /**
             * Returns the current value of the element at index <b> i</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicLong::getAcquire</b> .
             *
             * @param i the index
             * @return the value
             * 
             */
            glong getAcquire(gint i) const;

            /**
             * Sets the element at index <b> i</b>  to <b> newValue</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicLong#setRelease</b> .
             *
             * @param i the index
             * @param newValue the new value
             * 
             */
            void setRelease(gint i, glong newValue);

            /**
             * Atomically sets the element at index <b> i</b>  to <b> newValue</b> 
             * if the element's current value, referred to as the <em>witness
             * value</em>, <b> == expectedValue</b> ,
             * with memory effects as specified by
             * <b style="color:orange;">AtomicLong::compareAndExchange</b> .
             *
             * @param i the index
             * @param expectedValue the expected value
             * @param newValue the new value
             * @return the witness value, which will be the same as the
             * expected value if successful
             * 
             */
            glong compareAndExchange(gint i, glong expectedValue, glong newValue);

            /**
             * Atomically sets the element at index <b> i</b>  to <b> newValue</b> 
             * if the element's current value, referred to as the <em>witness
             * value</em>, <b> == expectedValue</b> ,
             * with memory effects as specified by
             * <b style="color:orange;">AtomicLong::compareAndExchangeAcquire</b> .
             *
             * @param i the index
             * @param expectedValue the expected value
             * @param newValue the new value
             * @return the witness value, which will be the same as the
             * expected value if successful
             * 
             */
            glong compareAndExchangeAcquire(gint i, glong expectedValue, glong newValue);

            /**
             * Atomically sets the element at index <b> i</b>  to <b> newValue</b> 
             * if the element's current value, referred to as the <em>witness
             * value</em>, <b> == expectedValue</b> ,
             * with memory effects as specified by
             * <b style="color:orange;">AtomicLong::compareAndExchangeRelease</b> .
             *
             * @param i the index
             * @param expectedValue the expected value
             * @param newValue the new value
             * @return the witness value, which will be the same as the
             * expected value if successful
             * 
             */
            glong compareAndExchangeRelease(gint i, glong expectedValue, glong newValue);

            /**
             * Possibly atomically sets the element at index <b> i</b>  to
             * <b> newValue</b>  if the element's current value <b> == expectedValue</b> ,
             * with memory effects as specified by
             * <b style="color:orange;">AtomicLong#weakCompareAndSet</b> .
             *
             * @param i the index
             * @param expectedValue the expected value
             * @param newValue the new value
             * @return <b> true</b>  if successful
             * 
             */
            gbool weakCompareAndSetVolatile(gint i, glong expectedValue, glong newValue);

            /**
             * Possibly atomically sets the element at index <b> i</b>  to
             * <b> newValue</b>  if the element's current value <b> == expectedValue</b> ,
             * with memory effects as specified by
             * <b style="color:orange;">AtomicLong#weakCompareAndSetAcquire</b> .
             *
             * @param i the index
             * @param expectedValue the expected value
             * @param newValue the new value
             * @return <b> true</b>  if successful
             * 
             */
            gbool weakCompareAndSetAcquire(gint i, glong expectedValue, glong newValue);

            /**
             * Possibly atomically sets the element at index <b> i</b>  to
             * <b> newValue</b>  if the element's current value <b> == expectedValue</b> ,
             * with memory effects as specified by
             * <b style="color:orange;">AtomicLong#weakCompareAndSetRelease</b> 
             *
             * @param i the index
             * @param expectedValue the expected value
             * @param newValue the new value
             * @return <b> true</b>  if successful
             * 
             */
            gbool weakCompareAndSetRelease(gint i, glong expectedValue, glong newValue);
        };

    } // core
} // concurrent

#endif //CORE23_ATOMICLONGARRAY_H

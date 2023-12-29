//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_ATOMICINTEGERARRAY_H
#define CORE23_ATOMICINTEGERARRAY_H

#include <core/Object.h>
#include <core/private/Unsafe.h>

namespace core {
    namespace util {

        /**
         * An <b> gint</b>  array in which elements may be updated atomically.
         * See the <b style="color:orange;">AtomicInteger</b>  specification for descriptions of the
         * properties of atomic accesses.
         *
         * @author Brunshweeck Tazeussong
         */
        class AtomicIntegerArray CORE_FINAL : public Object {
        private:
            CORE_ALIAS(U, native::Unsafe);

            IntArray array = {};

        public:

            /**
             * Creates a new AtomicIntegerArray of the given length, with all
             * elements initially zero.
             *
             * @param length the length of the array
             */
            CORE_EXPLICIT AtomicIntegerArray(gint length);

            /**
             * Creates a new AtomicIntegerArray with the same length as, and
             * all elements copied from, the given array.
             *
             * @param array the array to copy elements from
             */
            CORE_EXPLICIT AtomicIntegerArray(const IntArray &array);

            /**
             * Returns the length of the array.
             *
             * @return the length of the array
             */
            gint length() const;

            /**
             * Returns the current value of the element at index <b> i</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicInteger::getVolatile</b> .
             *
             * @param i the index
             * @return the current value
             */
            gint get(gint i) const;

            /**
             * Sets the element at index <b> i</b>  to <b> newValue</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicInteger#setVolatile</b> .
             *
             * @param i the index
             * @param newValue the new value
             */
             void set(gint i, gint newValue);

            /**
             * Sets the element at index <b> i</b>  to <b> newValue</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicInteger#setRelease</b> .
             *
             * @param i the index
             * @param newValue the new value
             * 
             */
             void lazySet(gint i, gint newValue);

            /**
             * Atomically sets the element at index <b> i</b>  to {@code
             * newValue</b>  and returns the old value,
             * with memory effects as specified by <b style="color:orange;">AtomicInteger::getAndSet</b> .
             *
             * @param i the index
             * @param newValue the new value
             * @return the previous value
             */
             gint getAndSet(gint i, gint newValue);

            /**
             * Atomically sets the element at index <b> i</b>  to {@code
             * newValue</b>  if the element's current value <b> == expectedValue</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicInteger::compareAndSet</b> .
             *
             * @param i the index
             * @param expectedValue the expected value
             * @param newValue the new value
             * @return <b> true</b>  if successful. False return indicates that
             * the actual value was not equal to the expected value.
             */
             gbool compareAndSet(gint i, gint expectedValue, gint newValue);

            /**
             * Possibly atomically sets the element at index <b> i</b>  to
             * <b> newValue</b>  if the element's current value <b> == expectedValue</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicInteger#weakCompareAndSetPlain</b> .
             *
             * @param i the index
             * @param expectedValue the expected value
             * @param newValue the new value
             * @return <b> true</b>  if successful
             * 
             */
             gbool weakCompareAndSetPlain(gint i, gint expectedValue, gint newValue);

            /**
             * Atomically increments the value of the element at index <b> i</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicInteger::getAndAdd</b> .
             *
             * <p>Equivalent to <b> getAndAdd(i, 1)</b> .
             *
             * @param i the index
             * @return the previous value
             */
             gint getAndIncrement(gint i);

            /**
             * Atomically decrements the value of the element at index <b> i</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicInteger::getAndAdd</b> .
             *
             * <p>Equivalent to <b> getAndAdd(i, -1)</b> .
             *
             * @param i the index
             * @return the previous value
             */
             gint getAndDecrement(gint i);

            /**
             * Atomically adds the given value to the element at index <b> i</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicInteger::getAndAdd</b> .
             *
             * @param i the index
             * @param delta the value to add
             * @return the previous value
             */
             gint getAndAdd(gint i, gint delta);

            /**
             * Atomically increments the value of the element at index <b> i</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicInteger::getAndAdd</b> .
             *
             * <p>Equivalent to <b> addAndGet(i, 1)</b> .
             *
             * @param i the index
             * @return the updated value
             */
             gint incrementAndGet(gint i);

            /**
             * Atomically decrements the value of the element at index <b> i</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicInteger::getAndAdd</b> .
             *
             * <p>Equivalent to <b> addAndGet(i, -1)</b> .
             *
             * @param i the index
             * @return the updated value
             */
             gint decrementAndGet(gint i);

            /**
             * Atomically adds the given value to the element at index <b> i</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicInteger::getAndAdd</b> .
             *
             * @param i the index
             * @param delta the value to add
             * @return the updated value
             */
             gint addAndGet(gint i, gint delta);

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
             gint getPlain(gint i) const;

            /**
             * Sets the element at index <b> i</b>  to <b> newValue</b> ,
             * with memory semantics of setting as if the variable was
             * declared non-<b> volatile</b>  and non-<b> final</b> .
             *
             * @param i the index
             * @param newValue the new value
             * 
             */
             void setPlain(gint i, gint newValue);

            /**
             * Returns the current value of the element at index <b> i</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicInteger::getOpaque</b> .
             *
             * @param i the index
             * @return the value
             * 
             */
             gint getOpaque(gint i) const;

            /**
             * Sets the element at index <b> i</b>  to <b> newValue</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicInteger#setOpaque</b> .
             *
             * @param i the index
             * @param newValue the new value
             * 
             */
             void setOpaque(gint i, gint newValue);

            /**
             * Returns the current value of the element at index <b> i</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicInteger::getAcquire</b> .
             *
             * @param i the index
             * @return the value
             * 
             */
             gint getAcquire(gint i) const;

            /**
             * Sets the element at index <b> i</b>  to <b> newValue</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicInteger#setRelease</b> .
             *
             * @param i the index
             * @param newValue the new value
             * 
             */
             void setRelease(gint i, gint newValue);

            /**
             * Atomically sets the element at index <b> i</b>  to <b> newValue</b> 
             * if the element's current value, referred to as the <em>witness
             * value</em>, <b> == expectedValue</b> ,
             * with memory effects as specified by
             * <b style="color:orange;">AtomicInteger::compareAndExchange</b> .
             *
             * @param i the index
             * @param expectedValue the expected value
             * @param newValue the new value
             * @return the witness value, which will be the same as the
             * expected value if successful
             * 
             */
             gint compareAndExchange(gint i, gint expectedValue, gint newValue);

            /**
             * Atomically sets the element at index <b> i</b>  to <b> newValue</b> 
             * if the element's current value, referred to as the <em>witness
             * value</em>, <b> == expectedValue</b> ,
             * with memory effects as specified by
             * <b style="color:orange;">AtomicInteger::compareAndExchangeAcquire</b> .
             *
             * @param i the index
             * @param expectedValue the expected value
             * @param newValue the new value
             * @return the witness value, which will be the same as the
             * expected value if successful
             * 
             */
             gint compareAndExchangeAcquire(gint i, gint expectedValue, gint newValue);

            /**
             * Atomically sets the element at index <b> i</b>  to <b> newValue</b> 
             * if the element's current value, referred to as the <em>witness
             * value</em>, <b> == expectedValue</b> ,
             * with memory effects as specified by
             * <b style="color:orange;">AtomicInteger::compareAndExchangeRelease</b> .
             *
             * @param i the index
             * @param expectedValue the expected value
             * @param newValue the new value
             * @return the witness value, which will be the same as the
             * expected value if successful
             * 
             */
             gint compareAndExchangeRelease(gint i, gint expectedValue, gint newValue);

            /**
             * Possibly atomically sets the element at index <b> i</b>  to
             * <b> newValue</b>  if the element's current value <b> == expectedValue</b> ,
             * with memory effects as specified by
             * <b style="color:orange;">AtomicInteger#weakCompareAndSet</b> .
             *
             * @param i the index
             * @param expectedValue the expected value
             * @param newValue the new value
             * @return <b> true</b>  if successful
             * 
             */
             gbool weakCompareAndSetVolatile(gint i, gint expectedValue, gint newValue);

            /**
             * Possibly atomically sets the element at index <b> i</b>  to
             * <b> newValue</b>  if the element's current value <b> == expectedValue</b> ,
             * with memory effects as specified by
             * <b style="color:orange;">AtomicInteger#weakCompareAndSetAcquire</b> .
             *
             * @param i the index
             * @param expectedValue the expected value
             * @param newValue the new value
             * @return <b> true</b>  if successful
             * 
             */
             gbool weakCompareAndSetAcquire(gint i, gint expectedValue, gint newValue);

            /**
             * Possibly atomically sets the element at index <b> i</b>  to
             * <b> newValue</b>  if the element's current value <b> == expectedValue</b> ,
             * with memory effects as specified by
             * <b style="color:orange;">AtomicInteger#weakCompareAndSetRelease</b> 
             *
             * @param i the index
             * @param expectedValue the expected value
             * @param newValue the new value
             * @return <b> true</b>  if successful
             * 
             */
             gbool weakCompareAndSetRelease(gint i, gint expectedValue, gint newValue);
        };

    } // core
} // concurrent

#endif //CORE23_ATOMICINTEGERARRAY_H

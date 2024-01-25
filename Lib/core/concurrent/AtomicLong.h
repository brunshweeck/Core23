//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_ATOMICLONG_H
#define CORE23_ATOMICLONG_H

#include <core/Long.h>

namespace core {
    namespace concurrent {

        /**
         * An <b> glong</b>  value that may be updated atomically. An <b> AtomicLong</b>  is used in
         * applications such as atomically incremented counters, and cannot be
         * used as a replacement for an <b style="color:orange;">core.Long</b> . However,
         * this class does extend <b> Number</b>  to allow uniform access by
         * tools and utilities that deal with numerically-based classes.
         *
         *
         * @author Brunshweeck Tazeussong
         */
        class AtomicLong CORE_FINAL : public Object {
        private:

            /**
             * The offset of this value field used by Unsafe class
             */
            static gint const VALUE;

            glong volatile value = {};

        public:

            /**
             * Creates a new AtomicLong with the given initial value.
             *
             * @param initialValue the initial value
             */
            CORE_FAST AtomicLong(glong value) : value(value) {}

            /**
             * Creates a new AtomicLong with initial value <b> 0</b> .
             */
            CORE_FAST AtomicLong() = default;

            /**
             * Returns the current value,
             * with memory semantics of reading as if the variable was declared volatile.
             *
             * @return the current value
             */
            glong get() const;

            /**
             * Sets the value to <b> newValue</b> ,
             * with memory semantics of setting as if the variable was declared volatile.
             *
             * @param newValue the new value
             */
            void set(glong newValue);

            /**
             * Sets the value to <b> newValue</b> ,
             * and ensures that prior loads and stores are not reordered after this access.
             *
             * @param newValue the new value
             */
            void lazySet(glong newValue);

            /**
             * Atomically sets the value to <b> newValue</b>  and returns the old value,
             * with the memory semantics of setVolatile and returns the variable's previous value,
             * as accessed with the memory semantics of getVolatile.
             *
             * @param newValue the new value
             * @return the previous value
             */
            glong getAndSet(glong newValue);

            /**
             * Atomically sets the value to <b> newValue</b> 
             * if the current value <b> == expectedValue</b> ,
             * as accessed with the memory semantics of getVolatile.
             *
             * @param expectedValue the expected value
             * @param newValue the new value
             * @return <b> true</b>  if successful. False return indicates that
             * the actual value was not equal to the expected value.
             */
            gbool compareAndSet(glong expectedValue, glong newValue);

            /**
             * Possibly atomically sets the value to <b> newValue</b> 
             * if the current value <b> == expectedValue</b> ,
             * as accessed with the memory semantics of get.
             *
             * <p>This operation may fail spuriously (typically, due to memory
             * contention) even if the witness value does match the expected value.
             *
             * @param expectedValue the expected value
             * @param newValue the new value
             * @return <b> true</b>  if successful
             *
             */
            gbool weakCompareAndSet(glong expectedValue, glong newValue);

            /**
             * Atomically increments the current value,
             * with the memory semantics of setVolatile, and returns the variable's previous value,
             * as accessed with the memory semantics of getVolatile.
             *
             * <p>Equivalent to <b> getAndAdd(1)</b> .
             *
             * @return the previous value
             */
            glong getAndIncrement();

            /**
             * Atomically decrements the current value,
             * with the memory semantics of setVolatile, and returns the variable's previous value,
             * as accessed with the memory semantics of getVolatile.
             *
             * <p>Equivalent to <b> getAndAdd(-1)</b> .
             *
             * @return the previous value
             */
            glong getAndDecrement();

            /**
             * Atomically adds the given value to the current value,
             * with the memory semantics of setVolatile, and returns the variable's previous value,
             * as accessed with the memory semantics of getVolatile.
             *
             * @param delta the value to add
             * @return the previous value
             */
            glong getAndAdd(glong delta);

            /**
             * Atomically increments the current value,
             * with the memory semantics of setVolatile, and returns the variable's previous value,
             * as accessed with the memory semantics of getVolatile.
             *
             * <p>Equivalent to <b> addAndGet(1)</b> .
             *
             * @return the updated value
             */
            glong incrementAndGet();

            /**
             * Atomically decrements the current value,
             * with the memory semantics of setVolatile, and returns the variable's previous value,
             * as accessed with the memory semantics of getVolatile.
             *
             * <p>Equivalent to <b> addAndGet(-1)</b> .
             *
             * @return the updated value
             */
            glong decrementAndGet();

            /**
             * Atomically adds the given value to the current value,
             * with the memory semantics of setVolatile, and returns the variable's previous value,
             * as accessed with the memory semantics of getVolatile.
             *
             * @param delta the value to add
             * @return the updated value
             */
            glong addAndGet(glong delta);

            /**
             * Returns the String representation of the current value.
             * @return the String representation of the current value
             */
            String toString() const override;

            /**
             * Returns the current value of this <b> AtomicLong</b>  as an
             * <b> glong</b> ,
             * with memory semantics of reading as if the variable was declared volatile.
             *
             * Equivalent to <b style="color:orange;">get()</b> .
             */
            glong intValue() const;

            /**
             * Returns the current value of this <b> AtomicLong</b>  as a
             * <b> glong</b>  after a widening primitive conversion,
             * with memory semantics of reading as if the variable was declared volatile.
             * 
             */
            glong longValue() const;

            /**
             * Returns the current value of this <b> AtomicLong</b>  as a
             * <b> float</b>  after a widening primitive conversion,
             * with memory semantics of reading as if the variable was declared volatile.
             * 
             */
            gfloat floatValue() const;

            /**
             * Returns the current value of this <b> AtomicLong</b>  as a
             * <b> double</b>  after a widening primitive conversion,
             * with memory semantics of reading as if the variable was declared volatile.
             * 
             */
            gdouble doubleValue() const;

            /**
             * Returns the current value, with memory semantics of reading as
             * if the variable was declared non-<b> volatile</b> .
             *
             * @return the value
             *
             */
            glong getPlain() const;

            /**
             * Sets the value to <b> newValue</b> , with memory semantics
             * of setting as if the variable was declared non-<b> volatile</b> 
             * and non-<b> final</b> .
             *
             * @param newValue the new value
             *
             */
            void setPlain(glong newValue);

            /**
             * Returns the current value, accessed in program order,
             * but with no assurance of memory ordering effects with respect to other threads.
             *
             * @return the value
             *
             */
            glong getOpaque() const;

            /**
             * Sets the value to <b> newValue</b> ,
             * in program order, but with no assurance of memory ordering effects with respect to other threads.
             *
             * @param newValue the new value
             *
             */
            void setOpaque(glong newValue);

            /**
             * Returns the current value,
             * and ensures that subsequent loads and stores are not reordered before this access.
             *
             * @return the value
             *
             */
            glong getAcquire() const;

            /**
             * Sets the value to <b> newValue</b> ,
             * and ensures that prior loads and stores are not reordered after this access.
             *
             * @param newValue the new value
             *
             */
            void setRelease(glong newValue);

            /**
             * Atomically sets the value to <b> newValue</b>  if the current value,
             * referred to as the <em>witness value</em>, <b> == expectedValue</b> ,
             * as accessed with the memory semantics of getVolatile.
             *
             * @param expectedValue the expected value
             * @param newValue the new value
             * @return the witness value, which will be the same as the
             * expected value if successful
             *
             */
            glong compareAndExchange(glong expectedValue, glong newValue);

            /**
             * Atomically sets the value to <b> newValue</b>  if the current value,
             * referred to as the <em>witness value</em>, <b> == expectedValue</b> ,
             * as accessed with the memory semantics of getAcquire.
             *
             * @param expectedValue the expected value
             * @param newValue the new value
             * @return the witness value, which will be the same as the
             * expected value if successful
             *
             */
            glong compareAndExchangeAcquire(glong expectedValue, glong newValue);

            /**
             * Atomically sets the value to <b> newValue</b>  if the current value,
             * referred to as the <em>witness value</em>, <b> == expectedValue</b> ,
             * as accessed with the memory semantics of get.
             *
             * @param expectedValue the expected value
             * @param newValue the new value
             * @return the witness value, which will be the same as the
             * expected value if successful
             *
             */
            glong compareAndExchangeRelease(glong expectedValue, glong newValue);

            /**
             * Possibly atomically sets the value to <b> newValue</b>  if
             * the current value <b> == expectedValue</b> ,
             * as accessed with the memory semantics of getVolatile.
             *
             * @param expectedValue the expected value
             * @param newValue the new value
             * @return <b> true</b>  if successful
             *
             */
            gbool weakCompareAndSetVolatile(glong expectedValue, glong newValue);

            /**
             * Possibly atomically sets the value to <b> newValue</b>  if
             * the current value <b> == expectedValue</b> ,
             * with the semantics of set if the variable's current value,
             * referred to as the witness value, == the expectedValue,
             * as accessed with the memory semantics of getAcquire.
             *
             * <p> This operation may fail spuriously (typically, due to memory contention) even
             * if the witness value does match the expected value.
             *
             * @param expectedValue the expected value
             * @param newValue the new value
             * @return <b> true</b>  if successful
             *
             */
            gbool weakCompareAndSetAcquire(glong expectedValue, glong newValue);

            /**
             * Possibly atomically sets the value to <b> newValue</b>  if
             * the current value <b> == expectedValue</b> ,
             * with the semantics of setRelease if the variable's current value,
             * referred to as the witness value, == the expectedValue, as accessed with
             * the memory semantics of get.
             *
             * <p> This operation may fail spuriously (typically, due to memory contention) even
             * if the witness value does match the expected value.
             *
             * @param expectedValue the expected value
             * @param newValue the new value
             * @return <b> true</b>  if successful
             *
             */
            gbool weakCompareAndSetRelease(glong expectedValue, glong newValue);
        };

    } // core
} // concurrent

#endif //CORE23_ATOMICLONG_H

//
// Created by T.N.Brunshweeck on 18/11/2023.
//

#ifndef CORE23_ATOMICBOOLEAN_H
#define CORE23_ATOMICBOOLEAN_H

#include <core/Object.h>
#include <core/private/Unsafe.h>

namespace core {
    namespace util {

        /**
         * An <b> gint</b>  value that may be updated atomically. An <b> AtomicBoolean</b>  is used in
         * applications such as atomically incremented counters, and cannot be
         * used as a replacement for an <b style="color:orange;">core.Boolean</b> . However,
         * this class does extend <b> Number</b>  to allow uniform access by
         * tools and utilities that deal with numerically-based classes.
         *
         *
         * @author Brunshweeck Tazeussong
         */
        class AtomicBoolean CORE_FINAL : public Object {
        private:

            /*
             * The class used do execute all operations
             */
            CORE_ALIAS(U, native::Unsafe);

            /**
             * The offset of this value field used by Unsafe class
             */
            static gint const VALUE;

            gint volatile value = {};

        public:

            /**
             * Creates a new AtomicBoolean with the given initial value.
             *
             * @param initialValue the initial value
             */
            CORE_FAST AtomicBoolean(gbool value);

            /**
             * Creates a new AtomicBoolean with initial value <b> 0</b> .
             */
            CORE_FAST AtomicBoolean() = default;

            /**
             * Returns the current value,
             * with memory semantics of reading as if the variable was declared volatile.
             *
             * @return the current value
             */
            CORE_FAST gbool get() const { return value != 0; }

            /**
             * Sets the value to <b> newValue</b> ,
             * with memory semantics of setting as if the variable was declared volatile.
             *
             * @param newValue the new value
             */
            void set(gbool newValue);

            /**
             * Sets the value to <b> newValue</b> ,
             * and ensures that prior loads and stores are not reordered after this access.
             *
             * @param newValue the new value
             */
            void lazySet(gbool newValue);

            /**
             * Atomically sets the value to <b> newValue</b>  and returns the old value,
             * with the memory semantics of setVolatile and returns the variable's previous value,
             * as accessed with the memory semantics of getVolatile.
             *
             * @param newValue the new value
             * @return the previous value
             */
            gbool getAndSet(gbool newValue);

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
            gbool compareAndSet(gbool expectedValue, gbool newValue);

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
            gbool weakCompareAndSet(gbool expectedValue, gbool newValue);

            /**
             * Returns the String representation of the current value.
             * @return the String representation of the current value
             */
            String toString() const override;

            /**
             * Returns the current value, with memory semantics of reading as
             * if the variable was declared non-<b> volatile</b> .
             *
             * @return the value
             *
             */
            gbool getPlain() const;

            /**
             * Sets the value to <b> newValue</b> , with memory semantics
             * of setting as if the variable was declared non-<b> volatile</b> 
             * and non-<b> final</b> .
             *
             * @param newValue the new value
             *
             */
            void setPlain(gbool newValue);

            /**
             * Returns the current value, accessed in program order,
             * but with no assurance of memory ordering effects with respect to other threads.
             *
             * @return the value
             *
             */
            gbool getOpaque() const;

            /**
             * Sets the value to <b> newValue</b> ,
             * in program order, but with no assurance of memory ordering effects with respect to other threads.
             *
             * @param newValue the new value
             *
             */
            void setOpaque(gbool newValue);

            /**
             * Returns the current value,
             * and ensures that subsequent loads and stores are not reordered before this access.
             *
             * @return the value
             *
             */
            gbool getAcquire() const;

            /**
             * Sets the value to <b> newValue</b> ,
             * and ensures that prior loads and stores are not reordered after this access.
             *
             * @param newValue the new value
             *
             */
            void setRelease(gbool newValue);

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
            gbool compareAndExchange(gbool expectedValue, gbool newValue);

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
            gbool compareAndExchangeAcquire(gbool expectedValue, gbool newValue);

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
            gbool compareAndExchangeRelease(gbool expectedValue, gbool newValue);

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
            gbool weakCompareAndSetVolatile(gbool expectedValue, gbool newValue);

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
            gbool weakCompareAndSetAcquire(gbool expectedValue, gbool newValue);

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
            gbool weakCompareAndSetRelease(gbool expectedValue, gbool newValue);
        };

    } // core
} // concurrent

#endif //CORE23_ATOMICBOOLEAN_H

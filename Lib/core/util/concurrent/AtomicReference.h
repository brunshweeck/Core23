//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_ATOMICREFERENCE_H
#define CORE23_ATOMICREFERENCE_H

#include <core/private/Unsafe.h>

namespace core {
    namespace util {

        /**
         * An object reference that may be updated atomically.
         *
         * @author Brunshweeck Tazeussong
         */
        class AtomicReference CORE_FINAL : public Object {
        private:
            CORE_ALIAS(U, native::Unsafe);
            CORE_ALIAS(VRef, Class<Object>::Ptr);

            /**
             * by default this reference is for null instance
             */
            volatile VRef value = {};

            static const gint VALUE;

        public:

            /**
             * Creates a new AtomicReference with the given initial value.
             *
             * @param initialValue the initial value
             */
            CORE_FAST AtomicReference(Object &initialValue) : value(&initialValue) {}

            /**
             * Creates a new AtomicReference with null initial value.
             */
            CORE_FAST AtomicReference() = default;

            /**
             * Returns the current value,
             * with memory effects as specified by <b style="color:orange;"> VarHandle::getVolatile</b>.
             *
             * @return the current value
             */
            CORE_FAST Object &get() const { return value == null ? null : *value; }

            /**
             * Sets the value to <b> newValue</b>,
             * with memory effects as specified by <b style="color:orange;"> VarHandle#setVolatile</b>.
             *
             * @param newValue the new value
             */
            void set(Object &newValue);

            /**
             * Sets the value to <b> newValue</b> ,
             * and ensures that prior loads and stores are not reordered after this access.
             *
             * @param newValue the new value
             */
            void lazySet(Object &newValue);

            /**
             * Atomically sets the value to <b> newValue</b>  and returns the old value,
             * with the memory semantics of setVolatile and returns the variable's previous value,
             * as accessed with the memory semantics of getVolatile.
             *
             * @param newValue the new value
             * @return the previous value
             */
            Object &getAndSet(Object &newValue);

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
            gbool compareAndSet(const Object &expectedValue, Object &newValue);

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
            gbool weakCompareAndSet(const Object &expectedValue, Object &newValue);

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
            Object & getPlain() const;

            /**
             * Sets the value to <b> newValue</b> , with memory semantics
             * of setting as if the variable was declared non-<b> volatile</b>
             * and non-<b> final</b> .
             *
             * @param newValue the new value
             *
             */
            void setPlain(Object & newValue);

            /**
             * Returns the current value, accessed in program order,
             * but with no assurance of memory ordering effects with respect to other threads.
             *
             * @return the value
             *
             */
            Object & getOpaque() const;

            /**
             * Sets the value to <b> newValue</b> ,
             * in program order, but with no assurance of memory ordering effects with respect to other threads.
             *
             * @param newValue the new value
             *
             */
            void setOpaque(Object & newValue);

            /**
             * Returns the current value,
             * and ensures that subsequent loads and stores are not reordered before this access.
             *
             * @return the value
             *
             */
            Object & getAcquire() const;

            /**
             * Sets the value to <b> newValue</b> ,
             * and ensures that prior loads and stores are not reordered after this access.
             *
             * @param newValue the new value
             *
             */
            void setRelease(Object & newValue);

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
            Object & compareAndExchange(const Object & expectedValue, Object & newValue);

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
            Object & compareAndExchangeAcquire(const Object & expectedValue, Object & newValue);

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
            Object & compareAndExchangeRelease(const Object & expectedValue, Object & newValue);

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
            gbool weakCompareAndSetVolatile(const Object & expectedValue, Object & newValue);

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
            gbool weakCompareAndSetAcquire(const Object & expectedValue, Object & newValue);

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
            gbool weakCompareAndSetRelease(const Object & expectedValue, Object & newValue);

        };

    } // core
} // concurrent

#endif //CORE23_ATOMICREFERENCE_H

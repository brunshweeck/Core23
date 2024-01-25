//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_ATOMICREFERENCE_H
#define CORE23_ATOMICREFERENCE_H

#include <core/private/Unsafe.h>

namespace core {
    namespace concurrent {

        /**
         * An object reference that may be updated atomically.
         *
         * @author Brunshweeck Tazeussong
         */
        template<class T>
        class AtomicReference CORE_FINAL : public Object {
        private:
            CORE_ALIAS(Value, typename Class<T>::Pointer);

            /**
             * by default this reference is for null INSTANCE
             */
            volatile Value value;

            CORE_WARNING_PUSH
            CORE_WARNING_DISABLE_INVALID_OFFSETOF
            static const gint VALUE = CORE_OFFSETOF(AtomicReference<T>, value);
            CORE_WARNING_POP

        public:

            /**
             * Creates a new AtomicReference with the given initial value.
             *
             * @param initialValue the initial value
             */
            CORE_FAST AtomicReference(T &initialValue) : value(&initialValue) {}

            /**
             * Returns the current value,
             * with memory effects as specified by <b style="color:orange;"> Unsafe::getVolatile</b>.
             *
             * @return the current value
             */
            T &get() {
                return *value;
            }

            /**
             * Returns the current value,
             * with memory effects as specified by <b style="color:orange;"> Unsafe::getVolatile</b>.
             *
             * @return the current value
             */
            T const &get() const {
                return *value;
            }

            /**
             * Sets the value to <b> newValue</b>,
             * with memory effects as specified by <b style="color:orange;"> Unsafe::setVolatile</b>.
             *
             * @param newValue the new value
             */
            void set(T &newValue) {
                value = &newValue;
            }

            /**
             * Sets the value to <b> newValue</b> ,
             * and ensures that prior loads and stores are not reordered after this access.
             *
             * @param newValue the new value
             */
            void lazySet(T &newValue) {
                Unsafe::putReferenceRelease(*this, VALUE, newValue);
            }

            /**
             * Atomically sets the value to <b> newValue</b>  and returns the old value,
             * with the memory semantics of setVolatile and returns the variable's previous value,
             * as accessed with the memory semantics of getVolatile.
             *
             * @param newValue the new value
             * @return the previous value
             */
            T &getAndSet(T &newValue) {
                return (T &) Unsafe::getAndSetReference(*this, VALUE, newValue);
            }

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
            gbool compareAndSet(T const &expectedValue, T &newValue) {
                return Unsafe::compareAndSetReference(*this, expectedValue, newValue);
            }

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
            gbool weakCompareAndSet(T const &expectedValue, T &newValue) {
                return Unsafe::weakCompareAndSetReference(*this, VALUE, expectedValue, newValue);
            }

            /**
             * Returns the String representation of the current value.
             * @return the String representation of the current value
             */
            String toString() const override {
                return String::valueOf(get());
            }

            /**
             * Returns the current value, with memory semantics of reading as
             * if the variable was declared non-<b> volatile</b> .
             *
             * @return the value
             *
             */
            T &getPlain() {
                return (T &) Unsafe::getReference(*this, VALUE);
            }

            /**
             * Returns the current value, with memory semantics of reading as
             * if the variable was declared non-<b> volatile</b> .
             *
             * @return the value
             *
             */
            T const &getPlain() const {
                return (T const &) Unsafe::getReference(*this, VALUE);
            }

            /**
             * Sets the value to <b> newValue</b> , with memory semantics
             * of setting as if the variable was declared non-<b> volatile</b>
             * and non-<b> final</b> .
             *
             * @param newValue the new value
             *
             */
            void setPlain(T &newValue) {
                Unsafe::putReference(*this, VALUE, newValue);
            }

            /**
             * Returns the current value, accessed in program order,
             * but with no assurance of memory ordering effects with respect to other threads.
             *
             * @return the value
             *
             */
            T &getOpaque() {
                return (T &) Unsafe::getReferenceRelaxed(*this, VALUE);
            }

            /**
             * Returns the current value, accessed in program order,
             * but with no assurance of memory ordering effects with respect to other threads.
             *
             * @return the value
             *
             */
            T const &getOpaque() const {
                return (T const &) Unsafe::getReferenceRelaxed(*this, VALUE);
            }

            /**
             * Sets the value to <b> newValue</b> ,
             * in program order, but with no assurance of memory ordering effects with respect to other threads.
             *
             * @param newValue the new value
             *
             */
            void setOpaque(T &newValue) {
                Unsafe::putReferenceRelaxed(*this, VALUE, newValue);
            }

            /**
             * Returns the current value,
             * and ensures that subsequent loads and stores are not reordered before this access.
             *
             * @return the value
             *
             */
            T &getAcquire() {
                return (T &) Unsafe::getReferenceAcquire(*this, VALUE);
            }

            /**
             * Returns the current value,
             * and ensures that subsequent loads and stores are not reordered before this access.
             *
             * @return the value
             *
             */
            T const &getAcquire() const {
                return (T const &) Unsafe::getReferenceAcquire(*this, VALUE);
            }

            /**
             * Sets the value to <b> newValue</b> ,
             * and ensures that prior loads and stores are not reordered after this access.
             *
             * @param newValue the new value
             *
             */
            void setRelease(T &newValue) {
                Unsafe::putReferenceRelease(*this, VALUE, newValue);
            }

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
            T &compareAndExchange(T const &expectedValue, T &newValue) {
                return (T &) Unsafe::compareAndExchangeReference(*this, VALUE, expectedValue, newValue);
            }

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
            T &compareAndExchangeAcquire(T const &expectedValue, T &newValue) {
                return (T &) Unsafe::compareAndExchangeReferenceAcquire(*this, VALUE, expectedValue, newValue);
            }

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
            T &compareAndExchangeRelease(T const &expectedValue, T &newValue) {
                return (T &) Unsafe::compareAndExchangeReferenceRelease(*this, VALUE, expectedValue, newValue);
            }

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
            gbool weakCompareAndSetVolatile(T const &expectedValue, T &newValue) {
                return Unsafe::weakCompareAndSetReference(*this, VALUE, expectedValue, newValue);
            }

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
            gbool weakCompareAndSetAcquire(T const &expectedValue, T &newValue) {
                return Unsafe::weakCompareAndSetReferenceAcquire(*this, VALUE, expectedValue, newValue);
            }

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
            gbool weakCompareAndSetRelease(T const &expectedValue, T &newValue) {
                return Unsafe::weakCompareAndSetReferenceRelease(*this, VALUE, expectedValue, newValue);
            }

            gbool equals(const Object &o) const override {
                if (this == &o) {
                    return true;
                }
                if (!Class<AtomicReference<T>>::hasInstance(o)) {
                    return false;
                }
                return get() == (AtomicReference const &) o
            }

            Object &clone() const override {
                return Unsafe::allocateInstance<AtomicReference>(*this);
            }

        };

    } // core
} // concurrent

#endif //CORE23_ATOMICREFERENCE_H

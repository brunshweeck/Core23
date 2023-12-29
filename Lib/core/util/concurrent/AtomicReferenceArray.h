//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_ATOMICREFERENCEARRAY_H
#define CORE23_ATOMICREFERENCEARRAY_H

#include <core/private/Unsafe.h>
#include <core/native/ReferenceArray.h>

namespace core {
    namespace util {

        /**
         * A <b> glong</b> array in which elements may be updated atomically.
         * See the <b style="color:orange;">AtomicReference</b> specification for descriptions of the
         * properties of atomic accesses.
         *
         * @author Brunshweeck Tazeussong
         */
        class AtomicReferenceArray CORE_FINAL : public Object {
        private:
            CORE_ALIAS(U, native::Unsafe);

            ReferenceArray array = {};

        public:

            /**
             * Creates a new AtomicReferenceArray of the given length, with all
             * elements initially zero.
             *
             * @param length the length of the array
             */
            CORE_EXPLICIT AtomicReferenceArray(gint length);

            /**
             * Creates a new AtomicReferenceArray with the same length as, and
             * all elements copied from, the given array.
             *
             * @param array the array to copy elements from
             */
            CORE_EXPLICIT AtomicReferenceArray(const ReferenceArray &array);

            /**
             * Returns the length of the array.
             *
             * @return the length of the array
             */
            gint length() const;

            /**
             * Returns the current value of the element at index <b> i</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicReference::getVolatile</b> .
             *
             * @param i the index
             * @return the current value
             */
            Object &get(gint i) const;

            /**
             * Sets the element at index <b> i</b>  to <b> newValue</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicReference#setVolatile</b> .
             *
             * @param i the index
             * @param newValue the new value
             */
            void set(gint i, Object &newValue);

            /**
             * Sets the element at index <b> i</b>  to <b> newValue</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicReference#setRelease</b> .
             *
             * @param i the index
             * @param newValue the new value
             * 
             */
            void lazySet(gint i, Object &newValue);

            /**
             * Atomically sets the element at index <b> i</b>  to {@code
             * newValue</b>  and returns the old value,
             * with memory effects as specified by <b style="color:orange;">AtomicReference::getAndSet</b> .
             *
             * @param i the index
             * @param newValue the new value
             * @return the previous value
             */
            Object &getAndSet(gint i, Object &newValue);

            /**
             * Atomically sets the element at index <b> i</b>  to {@code
             * newValue</b>  if the element's current value <b> == expectedValue</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicReference::compareAndSet</b> .
             *
             * @param i the index
             * @param expectedValue the expected value
             * @param newValue the new value
             * @return <b> true</b>  if successful. False return indicates that
             * the actual value was not equal to the expected value.
             */
            gbool compareAndSet(gint i, const Object &expectedValue, Object &newValue);

            /**
             * Possibly atomically sets the element at index <b> i</b>  to
             * <b> newValue</b>  if the element's current value <b> == expectedValue</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicReference#weakCompareAndSetPlain</b> .
             *
             * @param i the index
             * @param expectedValue the expected value
             * @param newValue the new value
             * @return <b> true</b>  if successful
             * 
             */
            gbool weakCompareAndSetPlain(gint i, const Object &expectedValue, Object &newValue);

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
            Object &getPlain(gint i) const;

            /**
             * Sets the element at index <b> i</b>  to <b> newValue</b> ,
             * with memory semantics of setting as if the variable was
             * declared non-<b> volatile</b>  and non-<b> final</b> .
             *
             * @param i the index
             * @param newValue the new value
             * 
             */
            void setPlain(gint i, Object &newValue);

            /**
             * Returns the current value of the element at index <b> i</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicReference::getOpaque</b> .
             *
             * @param i the index
             * @return the value
             * 
             */
            Object &getOpaque(gint i) const;

            /**
             * Sets the element at index <b> i</b>  to <b> newValue</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicReference#setOpaque</b> .
             *
             * @param i the index
             * @param newValue the new value
             * 
             */
            void setOpaque(gint i, Object &newValue);

            /**
             * Returns the current value of the element at index <b> i</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicReference::getAcquire</b> .
             *
             * @param i the index
             * @return the value
             * 
             */
            Object &getAcquire(gint i) const;

            /**
             * Sets the element at index <b> i</b>  to <b> newValue</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicReference#setRelease</b> .
             *
             * @param i the index
             * @param newValue the new value
             * 
             */
            void setRelease(gint i, Object &newValue);

            /**
             * Atomically sets the element at index <b> i</b>  to <b> newValue</b> 
             * if the element's current value, referred to as the <em>witness
             * value</em>, <b> == expectedValue</b> ,
             * with memory effects as specified by
             * <b style="color:orange;">AtomicReference::compareAndExchange</b> .
             *
             * @param i the index
             * @param expectedValue the expected value
             * @param newValue the new value
             * @return the witness value, which will be the same as the
             * expected value if successful
             * 
             */
            Object &compareAndExchange(gint i, const Object &expectedValue, Object &newValue);

            /**
             * Atomically sets the element at index <b> i</b>  to <b> newValue</b> 
             * if the element's current value, referred to as the <em>witness
             * value</em>, <b> == expectedValue</b> ,
             * with memory effects as specified by
             * <b style="color:orange;">AtomicReference::compareAndExchangeAcquire</b> .
             *
             * @param i the index
             * @param expectedValue the expected value
             * @param newValue the new value
             * @return the witness value, which will be the same as the
             * expected value if successful
             * 
             */
            Object &compareAndExchangeAcquire(gint i, const Object &expectedValue, Object &newValue);

            /**
             * Atomically sets the element at index <b> i</b>  to <b> newValue</b> 
             * if the element's current value, referred to as the <em>witness
             * value</em>, <b> == expectedValue</b> ,
             * with memory effects as specified by
             * <b style="color:orange;">AtomicReference::compareAndExchangeRelease</b> .
             *
             * @param i the index
             * @param expectedValue the expected value
             * @param newValue the new value
             * @return the witness value, which will be the same as the
             * expected value if successful
             * 
             */
            Object &compareAndExchangeRelease(gint i, const Object &expectedValue, Object &newValue);

            /**
             * Possibly atomically sets the element at index <b> i</b>  to
             * <b> newValue</b>  if the element's current value <b> == expectedValue</b> ,
             * with memory effects as specified by
             * <b style="color:orange;">AtomicReference#weakCompareAndSet</b> .
             *
             * @param i the index
             * @param expectedValue the expected value
             * @param newValue the new value
             * @return <b> true</b>  if successful
             * 
             */
            gbool weakCompareAndSetVolatile(gint i, const Object &expectedValue, Object &newValue);

            /**
             * Possibly atomically sets the element at index <b> i</b>  to
             * <b> newValue</b>  if the element's current value <b> == expectedValue</b> ,
             * with memory effects as specified by
             * <b style="color:orange;">AtomicReference#weakCompareAndSetAcquire</b> .
             *
             * @param i the index
             * @param expectedValue the expected value
             * @param newValue the new value
             * @return <b> true</b>  if successful
             * 
             */
            gbool weakCompareAndSetAcquire(gint i, const Object &expectedValue, Object &newValue);

            /**
             * Possibly atomically sets the element at index <b> i</b>  to
             * <b> newValue</b>  if the element's current value <b> == expectedValue</b> ,
             * with memory effects as specified by
             * <b style="color:orange;">AtomicReference#weakCompareAndSetRelease</b> 
             *
             * @param i the index
             * @param expectedValue the expected value
             * @param newValue the new value
             * @return <b> true</b>  if successful
             * 
             */
            gbool weakCompareAndSetRelease(gint i, const Object &expectedValue, Object &newValue);
        };

    } // core
} // concurrent

#endif //CORE23_ATOMICREFERENCEARRAY_H

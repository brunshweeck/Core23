//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_ATOMICREFERENCEARRAY_H
#define CORE23_ATOMICREFERENCEARRAY_H

#include <core/native/Array.h>
#include <core/private/Unsafe.h>
#include <core/util/Preconditions.h>

namespace core {
    namespace concurrent {

        using namespace util;

        /**
         * A <b> glong</b> root in which elements may be updated atomically.
         * See the <b style="color:orange;">AtomicReference</b> specification for descriptions of the
         * properties of atomic accesses.
         *
         * @author Brunshweeck Tazeussong
         */
        template<class T>
        class AtomicReferenceArray CORE_FINAL : public Object {
        private:
            volatile Array<T> array;

            static CORE_FAST glong VALUE = Unsafe::ARRAY_REFERENCE_BASE_OFFSET;
            static CORE_FAST glong SCALE = Unsafe::ARRAY_REFERENCE_INDEX_SCALE;

        public:

            /**
             * Creates a new empty AtomicReferenceArray of the given length
             */
            CORE_EXPLICIT AtomicReferenceArray() = default;

            /**
             * Creates a new AtomicReferenceArray of the given length, with all
             * elements initially null.
             *
             * @param length the length of the root
             */
            CORE_EXPLICIT AtomicReferenceArray(gint length) : array(length) {}

            /**
             * Construct new AtomicReference array with given length, with all
             * elements initially specified initial value.
             */
            CORE_EXPLICIT AtomicReferenceArray(gint length, T &initialValue) : array(length, initialValue) {}

            /**
             * Creates a new AtomicReferenceArray with the same length as, and
             * all elements copied from, the given root.
             *
             * @param array the root to copy elements from
             */
            CORE_EXPLICIT AtomicReferenceArray(const Array<T> &array) : array(array) {}

            /**
             * Returns the length of the root.
             *
             * @return the length of the root
             */
            gint length() const {
                return array.length();
            }

            /**
             * Returns the current value of the element at index <b> i</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicReference::getVolatile</b> .
             *
             * @param i the index
             * @return the current value
             */
            T &get(gint i) {
                gint const n = array.length();
                try {
                    Preconditions::checkIndex(i, n);
                    glong const offset = VALUE + (i * SCALE);
                    return validateAndGet(Unsafe::getReferenceVolatile(array, offset));
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.concurrent.AtomicReferenceArray"));
                }
            }

            /**
             * Returns the current value of the element at index <b> i</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicReference::getVolatile</b> .
             *
             * @param i the index
             * @return the current value
             */
            T const &get(gint i) const {
                gint const n = array.length();
                try {
                    Preconditions::checkIndex(i, n);
                    glong const offset = VALUE + (i * SCALE);
                    return validateAndGet(Unsafe::getReferenceVolatile(array, offset));
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.concurrent.AtomicReferenceArray"));
                }
            }

            /**
             * Sets the element at index <b> i</b>  to <b> newValue</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicReference::setVolatile</b> .
             *
             * @param i the index
             * @param newValue the new value
             */
            void set(gint i, T &newValue) {
                gint const n = array.length();
                try {
                    Preconditions::checkIndex(i, n);
                    glong const offset = VALUE + (i * SCALE);
                    Unsafe::putReferenceVolatile(array, offset, newValue);
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.concurrent.AtomicReferenceArray"));
                }
            }

            /**
             * Sets the element at index <b> i</b>  to <b> newValue</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicReference::setRelease</b> .
             *
             * @param i the index
             * @param newValue the new value
             * 
             */
            void lazySet(gint i, T &newValue) {
                gint const n = array.length();
                try {
                    Preconditions::checkIndex(i, n);
                    glong const offset = VALUE + (i * SCALE);
                    Unsafe::putReferenceRelease(array, offset, newValue);
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.concurrent.AtomicReferenceArray"));
                }
            }

            /**
             * Atomically sets the element at index <b> i</b>  to <b>
             * newValue</b>  and returns the old value,
             * with memory effects as specified by <b style="color:orange;">AtomicReference::getAndSet</b> .
             *
             * @param i the index
             * @param newValue the new value
             * @return the previous value
             */
            T &getAndSet(gint i, T &newValue) {
                gint const n = array.length();
                try {
                    Preconditions::checkIndex(i, n);
                    glong const offset = VALUE + (i * SCALE);
                    return validateAndGet(Unsafe::getAndSetReference(array, offset, newValue));
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.concurrent.AtomicReferenceArray"));
                }
            }

            /**
             * Atomically sets the element at index <b> i</b>  to <b>
             * newValue</b>  if the element's current value <b> == expectedValue</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicReference::compareAndSet</b> .
             *
             * @param i the index
             * @param expectedValue the expected value
             * @param newValue the new value
             * @return <b> true</b>  if successful. False return indicates that
             * the actual value was not equal to the expected value.
             */
            gbool compareAndSet(gint i, T const &expectedValue, T &newValue) {
                gint const n = array.length();
                try {
                    Preconditions::checkIndex(i, n);
                    glong const offset = VALUE + (i * SCALE);
                    return Unsafe::compareAndSetReference(array, offset, expectedValue, newValue);
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.concurrent.AtomicReferenceArray"));
                }
            }

            /**
             * Possibly atomically sets the element at index <b> i</b>  to
             * <b> newValue</b>  if the element's current value <b> == expectedValue</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicReference::weakCompareAndSetPlain</b> .
             *
             * @param i the index
             * @param expectedValue the expected value
             * @param newValue the new value
             * @return <b> true</b>  if successful
             * 
             */
            gbool weakCompareAndSetPlain(gint i, T const &expectedValue, T &newValue) {
                gint const n = array.length();
                try {
                    Preconditions::checkIndex(i, n);
                    glong const offset = VALUE + (i * SCALE);
                    return Unsafe::weakCompareAndSetReference(array, offset, expectedValue, newValue);
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.concurrent.AtomicReferenceArray"));
                }
            }

            /**
             * Returns the String representation of the current values of root.
             * @return the String representation of the current values of root
             */
            String toString() const override {
                return String::valueOf(array);
            }

            /**
             * Returns the current value of the element at index <b> i</b> ,
             * with memory semantics of reading as if the variable was declared
             * non-<b> volatile</b> .
             *
             * @param i the index
             * @return the value
             * 
             */
            T &getPlain(gint i) {
                gint const n = array.length();
                try {
                    Preconditions::checkIndex(i, n);
                    glong const offset = VALUE + (i * SCALE);
                    return validateAndGet(Unsafe::getReferenceVolatile(array, offset));
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.concurrent.AtomicReferenceArray"));
                }
            }

            /**
             * Returns the current value of the element at index <b> i</b> ,
             * with memory semantics of reading as if the variable was declared
             * non-<b> volatile</b> .
             *
             * @param i the index
             * @return the value
             *
             */
            T const &getPlain(gint i) const {
                gint const n = array.length();
                try {
                    Preconditions::checkIndex(i, n);
                    glong const offset = VALUE + (i * SCALE);
                    return validateAndGet(Unsafe::getReferenceVolatile(array, offset));
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.concurrent.AtomicReferenceArray"));
                }
            }

            /**
             * Sets the element at index <b> i</b>  to <b> newValue</b> ,
             * with memory semantics of setting as if the variable was
             * declared non-<b> volatile</b>  and non-<b> final</b> .
             *
             * @param i the index
             * @param newValue the new value
             * 
             */
            void setPlain(gint i, T &newValue) {
                gint const n = array.length();
                try {
                    Preconditions::checkIndex(i, n);
                    glong const offset = VALUE + (i * SCALE);
                    Unsafe::putReferenceVolatile(array, offset, newValue);
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.concurrent.AtomicReferenceArray"));
                }
            }

            /**
             * Returns the current value of the element at index <b> i</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicReference::getOpaque</b> .
             *
             * @param i the index
             * @return the value
             * 
             */
            T &getOpaque(gint i) {
                gint const n = array.length();
                try {
                    Preconditions::checkIndex(i, n);
                    glong const offset = VALUE + (i * SCALE);
                    return validateAndGet(Unsafe::getReferenceRelaxed(array, offset));
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.concurrent.AtomicReferenceArray"));
                }
            }

            /**
             * Returns the current value of the element at index <b> i</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicReference::getOpaque</b> .
             *
             * @param i the index
             * @return the value
             *
             */
            T const &getOpaque(gint i) const {
                gint const n = array.length();
                try {
                    Preconditions::checkIndex(i, n);
                    glong const offset = VALUE + (i * SCALE);
                    return validateAndGet(Unsafe::getReferenceRelaxed(array, offset));
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.concurrent.AtomicReferenceArray"));
                }
            }

            /**
             * Sets the element at index <b> i</b>  to <b> newValue</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicReference::setOpaque</b> .
             *
             * @param i the index
             * @param newValue the new value
             * 
             */
            void setOpaque(gint i, T &newValue) {
                gint const n = array.length();
                try {
                    Preconditions::checkIndex(i, n);
                    glong const offset = VALUE + (i * SCALE);
                    Unsafe::putReferenceRelaxed(array, offset, newValue);
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.concurrent.AtomicReferenceArray"));
                }
            }

            /**
             * Returns the current value of the element at index <b> i</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicReference::getAcquire</b> .
             *
             * @param i the index
             * @return the value
             * 
             */
            T &getAcquire(gint i) {
                gint const n = array.length();
                try {
                    Preconditions::checkIndex(i, n);
                    glong const offset = VALUE + (i * SCALE);
                    return validateAndGet(Unsafe::getReferenceAcquire(array, offset));
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.concurrent.AtomicReferenceArray"));
                }
            }

            /**
             * Returns the current value of the element at index <b> i</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicReference::getAcquire</b> .
             *
             * @param i the index
             * @return the value
             *
             */
            T const &getAcquire(gint i) const {
                gint const n = array.length();
                try {
                    Preconditions::checkIndex(i, n);
                    glong const offset = VALUE + (i * SCALE);
                    return validateAndGet(Unsafe::getReferenceAcquire(array, offset));
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.concurrent.AtomicReferenceArray"));
                }
            }

            /**
             * Sets the element at index <b> i</b>  to <b> newValue</b> ,
             * with memory effects as specified by <b style="color:orange;">AtomicReference::setRelease</b> .
             *
             * @param i the index
             * @param newValue the new value
             * 
             */
            void setRelease(gint i, T &newValue) {
                gint const n = array.length();
                try {
                    Preconditions::checkIndex(i, n);
                    glong const offset = VALUE + (i * SCALE);
                    Unsafe::putReferenceRelease(array, offset, newValue);
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.concurrent.AtomicReferenceArray"));
                }
            }

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
            T &compareAndExchange(gint i, T const &expectedValue, T &newValue) {
                gint const n = array.length();
                try {
                    Preconditions::checkIndex(i, n);
                    glong const offset = VALUE + (i * SCALE);
                    return validateAndGet(Unsafe::compareAndExchangeReference(array, offset, expectedValue, newValue));
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.concurrent.AtomicReferenceArray"));
                }
            }

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
            T &compareAndExchangeAcquire(gint i, T const &expectedValue, T &newValue) {
                gint const n = array.length();
                try {
                    Preconditions::checkIndex(i, n);
                    glong const offset = VALUE + (i * SCALE);
                    return validateAndGet(
                            Unsafe::compareAndExchangeReferenceAcquire(array, offset, expectedValue, newValue));
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.concurrent.AtomicReferenceArray"));
                }
            }

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
            T &compareAndExchangeRelease(gint i, T const &expectedValue, T &newValue) {
                gint const n = array.length();
                try {
                    Preconditions::checkIndex(i, n);
                    glong const offset = VALUE + (i * SCALE);
                    return validateAndGet(
                            Unsafe::compareAndExchangeReferenceRelease(array, offset, expectedValue, newValue));
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.concurrent.AtomicReferenceArray"));
                }
            }

            /**
             * Possibly atomically sets the element at index <b> i</b>  to
             * <b> newValue</b>  if the element's current value <b> == expectedValue</b> ,
             * with memory effects as specified by
             * <b style="color:orange;">AtomicReference::weakCompareAndSet</b> .
             *
             * @param i the index
             * @param expectedValue the expected value
             * @param newValue the new value
             * @return <b> true</b>  if successful
             * 
             */
            gbool weakCompareAndSetVolatile(gint i, T const &expectedValue, T &newValue) {
                gint const n = array.length();
                try {
                    Preconditions::checkIndex(i, n);
                    glong const offset = VALUE + (i * SCALE);
                    return Unsafe::weakCompareAndSetReference(array, offset, expectedValue, newValue);
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.concurrent.AtomicReferenceArray"));
                }
            }

            /**
             * Possibly atomically sets the element at index <b> i</b>  to
             * <b> newValue</b>  if the element's current value <b> == expectedValue</b> ,
             * with memory effects as specified by
             * <b style="color:orange;">AtomicReference::weakCompareAndSetAcquire</b> .
             *
             * @param i the index
             * @param expectedValue the expected value
             * @param newValue the new value
             * @return <b> true</b>  if successful
             * 
             */
            gbool weakCompareAndSetAcquire(gint i, T const &expectedValue, T &newValue) {
                gint const n = array.length();
                try {
                    Preconditions::checkIndex(i, n);
                    glong const offset = VALUE + (i * SCALE);
                    return Unsafe::weakCompareAndSetReferenceAcquire(array, offset, expectedValue, newValue);
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.concurrent.AtomicReferenceArray"));
                }
            }

            /**
             * Possibly atomically sets the element at index <b> i</b>  to
             * <b> newValue</b>  if the element's current value <b> == expectedValue</b> ,
             * with memory effects as specified by
             * <b style="color:orange;">AtomicReference::weakCompareAndSetRelease</b>
             *
             * @param i the index
             * @param expectedValue the expected value
             * @param newValue the new value
             * @return <b> true</b>  if successful
             * 
             */
            gbool weakCompareAndSetRelease(gint i, T const &expectedValue, T &newValue) {
                gint const n = array.length();
                try {
                    Preconditions::checkIndex(i, n);
                    glong const offset = VALUE + (i * SCALE);
                    return Unsafe::weakCompareAndSetReferenceAcquire(array, offset, expectedValue, newValue);
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.concurrent.AtomicReferenceArray"));
                }
            }

        private:
            T &validateAndGet(const Object &obj) {
                if (!Class<T>::hasInstance(obj))
                    ClassCastException(
                            "Could not cast object of type " + obj.classname() + " to element type of " + classname())
                            .throws(__trace("core.concurrent.AtomicReferenceArray"));
                return CORE_DYN_CAST(T &, obj);
            }

            T const &validateAndGet(const Object &obj) const {
                if (!Class<T>::hasInstance(obj))
                    ClassCastException(
                            "Could not cast object of type " + obj.classname() + " to element type of " + classname())
                            .throws(__trace("core.concurrent.AtomicReferenceArray"));
                return CORE_DYN_CAST(T &, obj);
            }
        };

    } // core
} // concurrent

#endif //CORE23_ATOMICREFERENCEARRAY_H

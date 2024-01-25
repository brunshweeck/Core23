//
// Created by T.N.Brunshweeck on 05/01/2024.
//

#ifndef CORE23_OPTIONAL_H
#define CORE23_OPTIONAL_H

#include <core/String.h>
#include <core/NoSuchElementException.h>
#include <core/function/Consumer.h>
#include <core/function/Predicate.h>
#include <core/function/Supplier.h>
#include <core/private/Unsafe.h>

namespace core {
    namespace util {

        /**
         * A container object which may or may not contain a non-<b> null</b>  value.
         * If a value is present, <b> isPresent()</b>  returns <b> true</b> . If no
         * value is present, the object is considered <i>empty</i> and
         * <b> isPresent()</b>  returns <b> false</b> .
         *
         * <p>Additional methods that depend on the presence or absence of a contained
         * value are provided, such as <b style="color:orange;"> orElse()</b>
         * (returns a default value if no value is present) and
         * <b style="color:orange;"> ifPresent()</b>  (performs an action if a value is present).
         *
         * <p>This is a <a href="">value-based</a> class; programmers should treat instances that are
         * <b style="color:green;"> equal</b>  as interchangeable and should not use instances for synchronization, or
         * unpredictable behavior may occur. For example, in a future release, synchronization may fail.
         *
         * @apiNote
         * <b> Optional</b>  is primarily intended for use as a method return type where
         * there is a clear need to represent "no result," and where using <b> null</b> 
         * is likely to cause errors. A variable whose type is <b> Optional</b>  should
         * never itself be <b> null</b> ; it should always point to an <b> Optional</b> 
         * instance.
         *
         * @tparam T the type of value
         */
        template<class T>
        class Optional CORE_FINAL : public Object {
        private:

            CORE_ALIAS(Unsafe, native::Unsafe);
            CORE_ALIAS(ActionConsumer, function::Consumer<T>);
            CORE_ALIAS(MutableActionConsumer, function::Consumer<T &>);
            CORE_ALIAS(Filter, function::Predicate<T>);
            CORE_ALIAS(Generator, function::Supplier<T>);

            CORE_ALIAS(VRef, typename Class<T>::Ptr);

            /**
             * If non-null, the value; if null, indicates no value is present
             */
            VRef value;

        public:

            /**
             * Returns an empty <b> Optional</b>  instance.  No value is present for this
             * <b> Optional</b> .
             *
             * @apiNote
             * Though it may be tempting to do so, avoid testing if an object is empty
             * by comparing with <b> ==</b>  or <b> !=</b>  against instances returned by
             * <b> Optional.empty()</b> .  There is no guarantee that it is a singleton.
             * Instead, use <b style="color:orange;"> isEmpty()</b>  or <b style="color:orange;"> isPresent()</b> .
             *
             * @param <T> The type of the non-existent value
             * @return an empty <b> Optional</b> 
             */
            static Optional<T> empty() {
                return {};
            }

        private:
            /**
             * Constructs an instance with the described value.
             *
             * @param value the value to describe; it's the caller's responsibility to
             *        ensure the value is non-<b> null</b>  unless creating the singleton
             *        instance returned by <b> empty()</b> .
             */
            CORE_EXPLICIT Optional(T &value) : value(&value) {}

        public:
            /**
             * Construct empty optional
             */
            Optional() : value(null) {};


            /**
             * If a value is present, returns the value, otherwise throws
             * <b> NoSuchElementException</b> .
             *
             * @apiNote
             * The preferred alternative to this method is <b style="color:orange;"> #orElseThrow()</b> .
             *
             * @return the non-<b> null</b>  value described by this <b> Optional</b> 
             * @throws NoSuchElementException if no value is present
             */
            T &get() {
                if (!isPresent())
                    NoSuchElementException().throws(__trace("core.util.Optional"));
                else
                    return value[0];
            }

            /**
             * If a value is present, returns the value, otherwise throws
             * <b> NoSuchElementException</b> .
             *
             * @apiNote
             * The preferred alternative to this method is <b style="color:orange;"> #orElseThrow()</b> .
             *
             * @return the non-<b> null</b>  value described by this <b> Optional</b> 
             * @throws NoSuchElementException if no value is present
             */
            T const &get() const {
                if (!isPresent())
                    NoSuchElementException().throws(__trace("core.util.Optional"));
                else
                    return value[0];
            }

            /**
             * If a value is present, returns <b> true</b> , otherwise <b> false</b> .
             *
             * @return <b> true</b>  if a value is present, otherwise <b> false</b> 
             */
            gbool isPresent() const {
                return value != null;
            }

            /**
             * If a value is  not present, returns <b> true</b> , otherwise
             * <b> false</b> .
             *
             * @return  <b> true</b>  if a value is not present, otherwise <b> false</b> 
             */
            gbool isEmpty() const {
                return value == null;
            }

            /**
             * If a value is present, performs the given action with the value,
             * otherwise does nothing.
             *
             * @param action the action to be performed, if a value is present
             */
            void ifPresent(const MutableActionConsumer &action) {
                if (isPresent()) {
                    action.accept(get());
                }
            }

            /**
             * If a value is present, performs the given action with the value,
             * otherwise does nothing.
             *
             * @param action the action to be performed, if a value is present
             * @throws NullPointerException if value is present and the given action is
             *         <b> null</b> 
             */
            void ifPresent(const ActionConsumer &action) const {
                if (isPresent()) {
                    action.accept(get());
                }
            }

            /**
             * If a value is present, and the value matches the given predicate,
             * returns an <b> Optional</b>  describing the value, otherwise returns an
             * empty <b> Optional</b> .
             *
             * @param predicate the predicate to apply to a value, if present
             * @return an <b> Optional</b>  describing the value of this
             *         <b> Optional</b> , if a value is present and the value matches the
             *         given predicate, otherwise an empty <b> Optional</b> 
             * @throws NullPointerException if the predicate is <b> null</b> 
             */
            Optional<T> filter(const Filter &predicate) const {
                if (!isPresent()) {
                    return *this;
                }
                if (predicate.test(get())) {
                    return *this;
                } else
                    return empty();
            }

            /**
             * If a value is present, returns an <b> Optional</b>  describing the value,
             * otherwise returns an <b> Optional</b>  produced by the supplying function.
             *
             * @param supplier the supplying function that produces an <b> Optional</b> 
             *        to be returned
             * @return returns an <b> Optional</b>  describing the value of this
             *         <b> Optional</b> , if a value is present, otherwise an
             *         <b> Optional</b>  produced by the supplying function.
             * @throws NullPointerException if the supplying function is <b> null</b>  or
             *         produces a <b> null</b>  result
             */
            Optional<T> orGet(const Generator &supplier) const {
                if (!isPresent()) {
                    return *this;
                }
                return Optional(supplier.get());
            }

            /**
             * If a value is present, returns the value, otherwise returns
             * <b> other</b> .
             *
             * @param other the value to be returned, if no value is present.
             *
             * @return the value, if present, otherwise <b> other</b> 
             */
            T &orElse(T &other) {
                if (isPresent()) {
                    return get();
                } else {
                    return other;
                }
            }

            /**
             * If a value is present, returns the value, otherwise returns
             * <b> other</b> .
             *
             * @param other the value to be returned, if no value is present.
             *        May be <b> null</b> .
             * @return the value, if present, otherwise <b> other</b> 
             */
            const T &orElse(const T &other) const {
                if (isPresent()) {
                    return get();
                } else {
                    return other;
                }
            }

            /**
             * If a value is present, returns the value, otherwise throws
             * <b> NoSuchElementException</b> .
             *
             * @return the non-<b> null</b>  value described by this <b> Optional</b> 
             * @throws NoSuchElementException if no value is present
             */
            T &orElseThrow() {
                if (!isPresent()) {
                    NoSuchElementException().throws(__trace("core.util.Optional"));
                } else {
                    return get();
                }
            }

            /**
             * If a value is present, returns the value, otherwise throws
             * <b> NoSuchElementException</b> .
             *
             * @return the non-<b> null</b>  value described by this <b> Optional</b> 
             * @throws NoSuchElementException if no value is present
             */
            const T &orElseThrow() const {
                if (!isPresent()) {
                    NoSuchElementException().throws(__trace("core.util.Optional"));
                } else {
                    return get();
                }
            }

            /**
             * If a value is present, returns the value, otherwise throws an exception
             * produced by the exception supplying function.
             *
             * @apiNote
             * A method reference to the exception constructor with an empty argument
             * list can be used as the supplier. For example,
             * <b> IllegalStateException::new</b> 
             *
             * @param <X> Type of the exception to be thrown
             * @param exceptionSupplier the supplying function that produces an
             *        exception to be thrown
             * @return the value, if present
             * @throws Throwable if no value is present
             * @throws NullPointerException if no value is present and the exception
             *          supplying function is <b> null</b> 
             */
            T &orElseThrow(const Throwable &throwable) {
                if (!isPresent()) {
                    throwable.throws(__trace("core.util.Optional"));
                } else {
                    return get();
                }
            }

            /**
             * If a value is present, returns the value, otherwise throws an exception
             * produced by the exception supplying function.
             *
             * @apiNote
             * A method reference to the exception constructor with an empty argument
             * list can be used as the supplier. For example,
             * <b> IllegalStateException::new</b>
             *
             * @param exceptionSupplier the supplying function that produces an
             *        exception to be thrown
             * @return the value, if present
             * @throws Throwable if no value is present
             * @throws NullPointerException if no value is present and the exception
             *          supplying function is <b> null</b> 
             */
            const T &orElseThrow(const Throwable &throwable) const {
                if (!isPresent()) {
                    throwable.throws(__trace("core.util.Optional"));
                } else {
                    return get();
                }
            }

            /**
             * Indicates whether some other object is "equal to" this <b> Optional</b> .
             * The other object is considered equal if:
             * <ul>
             * <li>it is also an <b> Optional</b>  and;
             * <li>both instances have no value present or;
             * <li>the present values are "equal to" each other via <b> equals()</b> .
             * </ul>
             *
             * @param obj an object to be tested for equality
             * @return <b> true</b>  if the other object is "equal to" this object
             *         otherwise <b> false</b> 
             */
            gbool equals(const Object &obj) const override {
                if (this == &obj) {
                    return true;
                }
                if (!Class<Optional<T>>::hasInstance(obj)) {
                    return false;
                }
                Optional<T> const &opt = (const Optional<T> &) obj;
                if (value == opt.value)
                    return true;
                return Object::equals(get(), opt.get());
            }

            /**
             * Returns the hash code of the value, if present, otherwise <b> 0</b> 
             * (zero) if no value is present.
             *
             * @return hash code value of the present value or <b> 0</b>  if no value is
             *         present
             */
            gint hash() const override {
                if (isEmpty())
                    return 0;
                return ((const Object &) get()).hash();
            }

            /**
             * Returns a non-empty string representation of this <b> Optional</b> 
             * suitable for debugging.  The exact presentation format is unspecified and
             * may vary between implementations and versions.
             *
             * @implSpec
             * If a value is present the result must include its string representation
             * in the result.  Empty and present <b> Optional</b> s must be unambiguously
             * differentiable.
             *
             * @return the string representation of this instance
             */
            String toString() const override {
                return value != null ? ("Optional[" + String::valueOf(get()) + "]") : "Optional.empty";
            }

            /**
             * Return shadow copy of this object
             */
            Object &clone() const override {
                return native::Unsafe::allocateInstance<Optional<T>>(*this);
            }

            ~Optional() override {
                value = null;
            };

            /**
             * Construct new Optional with specified value
             *
             * @param value The value used to initialize this optional.
             *              Note: if value has been pre-allocated with
             *              Unsafe.allocateInstance method, the contains
             *              of optional result is same value reference.
             * @throws MemoryError
             */
            static Optional of(const T &value) {
                try {
                    return Optional<T>(native::Unsafe::copyInstance(value, true));
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.util.Optional"));
                }
            }

            /**
             * Construct new Optional with specified value
             *
             * @param value The value used to initialize this optional.
             */
            static Optional of(T &value) CORE_NOTHROW {
                return Optional<T>(value);
            }

            /**
             * Construct new Optional with specified value
             *
             * @param value The value used to initialize this optional.
             * @param copy True if value will be copied and false if
             *              specified reference is used directly
             * @throws MemoryError
             */
            static Optional of(T &value, gbool copy) {
                if (copy) {
                    try {
                        return Optional<T>(native::Unsafe::copyInstance(value));
                    } catch (const Exception &ex) {
                        ex.throws(__trace("core.util.Optional"));
                    }
                } else {
                    return Optional<T>(value);
                }
            }

            CORE_ENABLE_IMPLICIT_CAST(T &, get(),);
            CORE_ENABLE_IMPLICIT_CAST(const T&, get(), const);

        };

    } // util
} // core

#endif //CORE23_OPTIONAL_H

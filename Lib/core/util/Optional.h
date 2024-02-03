//
// Created by T.N.Brunshweeck on 05/01/2024.
//

#ifndef CORE23_OPTIONAL_H
#define CORE23_OPTIONAL_H

#include <core/Runnable.h>
#include <core/NoSuchElementException.h>
#include <core/function/Consumer.h>
#include <core/function/Predicate.h>
#include <core/function/Supplier.h>

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
            CORE_STATIC_ASSERT(Class<Object>::isSuper<T>(),
                               "The Optional value type must have core.Object as base class.");
            CORE_STATIC_ASSERT(
                    !Class<Object>::isReference() && !Class<Object>::isConstant() && !Class<Object>::isVolatile(),
                    "The Optional value type shouldn't have qualifiers 'const', 'volatile', '&' and '&&'");

            CORE_ALIAS(VALUE, typename Class<T>::Ptr);

            /**
             * If non-null, the value; if null, indicates no value is present
             */
            VALUE value;

        public:

            /**
             * Constructs an instance with the described value.
             * The value will be copied if is necessary.
             *
             * @param value the value to describe; it's the caller's responsibility to
             *        ensure the value is non-<b> null</b>  unless creating the singleton
             *        instance returned by <b> empty()</b> .
             *
             * @see Unsafe::copyInstance
             */
            Optional(T &&value) : value(null) {
                if (null != value) {
                    CORE_TRY_ONLY(
                    // copy of value if is not pre-allocated
                            Optional::value = (VALUE) &Unsafe::copyInstance(value, true);,
                    // if copy fail (value is not copyable
                            Optional::value = (VALUE) &value;
                    )
                }
            }

            /**
             * Constructs an instance with the described value.
             * The value will be copied if is necessary.
             *
             * @param value the value to describe; it's the caller's responsibility to
             *        ensure the value is non-<b> null</b>  unless creating the singleton
             *        instance returned by <b> empty()</b> .
             *
             * @see Unsafe::copyInstance
             */
            Optional(const T &value) : value(null) {
                if (null != value) {
                    CORE_TRY_ONLY(
                    // copy of value if is not pre-allocated
                            Optional::value = (VALUE) &Unsafe::copyInstance(value, true);,
                    // if copy fail (value is not copyable
                            Optional::value = (VALUE) &value;
                    )
                }
            }

            /**
             * Constructs an instance with the described value.
             * The value will be copied if is necessary.
             *
             * @param value the value to describe; it's the caller's responsibility to
             *        ensure the value is non-<b> null</b>  unless creating the singleton
             *        instance returned by <b> empty()</b> .
             *
             * @see Unsafe::copyInstance
             */
            Optional(T &value) : value(null) {
                if (null != value) {
                    // mutable reference is never copied
                    Optional::value = (VALUE) &value;
                }
            }

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
                if (!isPresent()) {
                    NoSuchElementException().throws(__ctrace());
                }
                return *value;
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
                    NoSuchElementException().throws(__ctrace());
                else
                    return *value;
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
            void ifPresent(const Consumer<T> &action) const {
                if (isPresent()) {
                    CORE_TRY_RETHROW(action.accept(*value), , __ctrace())
                }
            }

            /**
             * If a value is present, performs the given action with the value,
             * otherwise does nothing.
             *
             * @param action the action to be performed, if a value is present
             */
            void ifPresent(const Consumer<T &> &action) {
                if (isPresent()) {
                    CORE_TRY_RETHROW(action.accept(*value), , __ctrace())
                }
            }

            /**
             * If a value is present, performs the given action with the value,
             * otherwise performs the given empty-based action.
             *
             * @param action the action to be performed, if a value is present
             * @param emptyAction the empty-based action to be performed, if no value is
             *        present
             */
            void ifPresentOrElse(const Consumer<T> &action, const Runnable &emptyAction) const {
                if (isPresent()) {
                    CORE_TRY_RETHROW(action.accept(*value), , __ctrace())
                } else {
                    CORE_TRY_RETHROW(emptyAction.run(), , __ctrace())
                }
            }

            /**
             * If a value is present, performs the given action with the value,
             * otherwise performs the given empty-based action.
             *
             * @param action the action to be performed, if a value is present
             * @param emptyAction the empty-based action to be performed, if no value is
             *        present
             */
            void ifPresentOrElse(const Consumer<T &> &action, const Runnable &emptyAction) {
                if (isPresent()) {
                    CORE_TRY_RETHROW(action.accept(*value), , __ctrace())
                } else {
                    CORE_TRY_RETHROW(emptyAction.run();, , __ctrace())
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
             */
            Optional<T> filter(const Predicate<T> &predicate) const {
                if (!isPresent()) {
                    return *this;
                }
                CORE_TRY_RETHROW(
                        if (predicate.test(*value)) {
                            return *this;
                        }, , __ctrace()
                )
                return {};
            }

            /**
             * If a value is present, returns an <b> Optional</b> describing (as if by
             * <b style="color: orange;"> #ofNullable</b>) the result of applying the given mapping function to
             * the value, otherwise returns an empty <b> Optional</b>.
             *
             * <p>If the mapping function returns a <b> null</b> result then this method
             * returns an empty <b> Optional</b>.
             *
             * @apiNote
             * This method supports post-processing on <b> Optional</b> values, without
             * the need to explicitly check for a return status.  For example, the
             * following code traverses a stream of URIs, selects one that has not
             * yet been processed, and creates a path from that URI, returning
             * an <b> Optional<Path></b>:
             *
             * <pre><b> 
             *     Optional<Path> p =
             *         uris.stream().filter(uri -> !isProcessedYet(uri))
             *                       .findFirst()
             *                       .map(Paths::get);
             * </b></pre>
             *
             * Here, <b> findFirst</b> returns an <b> Optional<URI></b>, and then
             * <b> map</b> returns an <b> Optional<Path></b> for the desired
             * URI if one exists.
             *
             * @param mapper the mapping function to apply to a value, if present
             * @tparam U The type of the value returned from the mapping function
             * @return an <b> Optional</b> describing the result of applying a mapping
             *         function to the value of this <b> Optional</b>, if a value is
             *         present, otherwise an empty <b> Optional</b>
             */
            template<class U>
            Optional<U> map(const Function<T, U> &mapper) const {
                if (isPresent()) {
                    CORE_TRY_RETHROW(return Optional<U>(mapper.apply(*value)), , __ctrace())
                }
                return {};
            }

            /**
             * If a value is present, returns an <b> Optional</b> describing (as if by
             * <b style="color: orange;"> #ofNullable</b>) the result of applying the given mapping function to
             * the value, otherwise returns an empty <b> Optional</b>.
             *
             * <p>If the mapping function returns a <b> null</b> result then this method
             * returns an empty <b> Optional</b>.
             *
             * @apiNote
             * This method supports post-processing on <b> Optional</b> values, without
             * the need to explicitly check for a return status.  For example, the
             * following code traverses a stream of URIs, selects one that has not
             * yet been processed, and creates a path from that URI, returning
             * an <b> Optional<Path></b>:
             *
             * <pre><b> 
             *     Optional<Path> p =
             *         uris.stream().filter(uri -> !isProcessedYet(uri))
             *                       .findFirst()
             *                       .map(Paths::get);
             * </b></pre>
             *
             * Here, <b> findFirst</b> returns an <b> Optional<URI></b>, and then
             * <b> map</b> returns an <b> Optional<Path></b> for the desired
             * URI if one exists.
             *
             * @param mapper the mapping function to apply to a value, if present
             * @tparam U The type of the value returned from the mapping function
             * @return an <b> Optional</b> describing the result of applying a mapping
             *         function to the value of this <b> Optional</b>, if a value is
             *         present, otherwise an empty <b> Optional</b>
             */
            template<class U>
            Optional<U> map(const Function<T &, U> &mapper) {
                if (isPresent()) {
                    CORE_TRY_RETHROW(return Optional<U>(mapper.apply(*value));, , __ctrace())
                }
                return {};
            }

            /**
             * If a value is present, returns an <b> Optional</b> describing (as if by
             * <b style="color: orange;"> #ofNullable</b>) the result of applying the given mapping function to
             * the value, otherwise returns an empty <b> Optional</b>.
             *
             * <p>If the mapping function returns a <b> null</b> result then this method
             * returns an empty <b> Optional</b>.
             *
             * @apiNote
             * This method supports post-processing on <b> Optional</b> values, without
             * the need to explicitly check for a return status.  For example, the
             * following code traverses a stream of URIs, selects one that has not
             * yet been processed, and creates a path from that URI, returning
             * an <b> Optional<Path></b>:
             *
             * <pre><b> 
             *     Optional<Path> p =
             *         uris.stream().filter(uri -> !isProcessedYet(uri))
             *                       .findFirst()
             *                       .map(Paths::get);
             * </b></pre>
             *
             * Here, <b> findFirst</b> returns an <b> Optional<URI></b>, and then
             * <b> map</b> returns an <b> Optional<Path></b> for the desired
             * URI if one exists.
             *
             * @param mapper the mapping function to apply to a value, if present
             * @tparam U The type of the value returned from the mapping function
             * @return an <b> Optional</b> describing the result of applying a mapping
             *         function to the value of this <b> Optional</b>, if a value is
             *         present, otherwise an empty <b> Optional</b>
             */
            template<class U>
            Optional<U> flatMap(const Function<T, Optional<U>> &mapper) const {
                if (isPresent()) {
                    CORE_TRY_RETHROW(return mapper.apply(*value);, , __ctrace())
                }
                return {};
            }

            /**
             * If a value is present, returns an <b> Optional</b> describing (as if by
             * <b style="color: orange;"> #ofNullable</b>) the result of applying the given mapping function to
             * the value, otherwise returns an empty <b> Optional</b>.
             *
             * <p>If the mapping function returns a <b> null</b> result then this method
             * returns an empty <b> Optional</b>.
             *
             * @apiNote
             * This method supports post-processing on <b> Optional</b> values, without
             * the need to explicitly check for a return status.  For example, the
             * following code traverses a stream of URIs, selects one that has not
             * yet been processed, and creates a path from that URI, returning
             * an <b> Optional<Path></b>:
             *
             * <pre><b> 
             *     Optional<Path> p =
             *         uris.stream().filter(uri -> !isProcessedYet(uri))
             *                       .findFirst()
             *                       .map(Paths::get);
             * </b></pre>
             *
             * Here, <b> findFirst</b> returns an <b> Optional<URI></b>, and then
             * <b> map</b> returns an <b> Optional<Path></b> for the desired
             * URI if one exists.
             *
             * @param mapper the mapping function to apply to a value, if present
             * @tparam U The type of the value returned from the mapping function
             * @return an <b> Optional</b> describing the result of applying a mapping
             *         function to the value of this <b> Optional</b>, if a value is
             *         present, otherwise an empty <b> Optional</b>
             */
            template<class U>
            Optional<U> flatMap(const Function<T &, Optional<U>> &mapper) {
                if (isPresent()) {
                    CORE_TRY_RETHROW(return mapper.apply(*value);, , __ctrace())
                }
                return {};
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
             */
            Optional<T> orElseGet(const Supplier<Optional<T>> &supplier) const {
                if (!isPresent()) {
                    return *this;
                }
                return supplier.get();
            }

            /**
             * If a value is present, returns a sequential <b style="color: orange;"> Stream</b> containing
             * only that value, otherwise returns an empty <b> Stream</b>.
             *
             * @apiNote
             * This method can be used to transform a <b> Stream</b> of optional
             * elements to a <b> Stream</b> of present value elements:
             * <pre><b> 
             *     Stream<Optional<T>> os = ..
             *     Stream<T> s = os.flatMap(Optional::stream)
             * </b></pre>
             *
             * @return the optional value as a <b> Stream</b>
             */
            // Stream<T> &stream() const {</b>

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
             *
             * @return the value, if present, otherwise <b> other</b>
             */
            T &orElse(const T &other) {
                if (isPresent()) {
                    return get();
                } else {
                    return Optional<T>(other).get();
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
             * If a value is present, returns the value, otherwise returns the result
             * produced by the supplying function.
             *
             * @param supplier the supplying function that produces a value to be returned
             * @return the value, if present, otherwise the result produced by the
             *         supplying function
             */
            T const &orElseGet(const Supplier<T> &supplier) const {
                if (!isPresent()) {
                    CORE_TRY_RETHROW(return supplier.get();, , __ctrace())
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
                    NoSuchElementException().throws(__ctrace());
                } else {
                    return *value;
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
                    NoSuchElementException().throws(__ctrace());
                } else {
                    return *value;
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
             */
            T &orElseThrow(const Supplier<Throwable> &exceptionSupplier) {
                if (!isPresent()) {
                    Optional<Throwable> th;
                    CORE_TRY_RETHROW(th = exceptionSupplier.get(), , __ctrace())
                    // assert th.isPresent()
                    th.get().throws(__ctrace());
                } else {
                    return *value;
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
             */
            const T &orElseThrow(const Supplier<Throwable> &exceptionSupplier) const {
                if (!isPresent()) {
                    Optional<Throwable> th;
                    CORE_TRY_RETHROW(th = exceptionSupplier.get(), , __ctrace())
                    // assert th.isPresent()
                    th.get().throws(__ctrace());
                } else {
                    return *value;
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
                if (!Class<Optional>::hasInstance(obj)) {
                    return false;
                }
                Optional<T> const &other = (const Optional &) obj;
                if (value == other.value)
                    return true;
                return Object::equals(*value, *other.value);
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
                return Object::hash(*value);
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
                return value != null ? String::valueOf(*value) : u"Optional[]"_S;
            }

            /**
             * Return shadow copy of this object
             */
            Object &clone() const override {
                Optional &clone = native::Unsafe::allocateInstance<Optional>();
                clone.value = value;
                return clone;
            }

            ~Optional() override {
                value = null;
            }

            CORE_FRATERNITY_T1(Optional);

            template<class U, Class<gbool>::OnlyIf<Class<U>::template isSuper<T>()> = true>
            operator Optional<U> const() const {
                if(isEmpty()) {
                    return {};
                }
                Optional<U> option;
                if (!Class<U>::hasInstance(*value)) {
                    ClassCastException("Could not cast type " + classname() + " to " + option.classname())
                            .throws(__ctrace());
                }
                option.value = (typename Optional<U>::VALUE) value;
                return option;
            }

            template<class U, Class<gbool>::OnlyIf<Class<U>::template isSuper<T>()> = true>
            operator Optional<U>() {
                if(isEmpty()) {
                    return {};
                }
                Optional<U> option;
                if (!Class<U>::hasInstance(*value)) {
                    ClassCastException("Could not cast type " + classname() + " to " + option.classname())
                            .throws(__ctrace());
                }
                option.value = (typename Optional<U>::VALUE) value;
                return option;
            }

            template<class U, Class<gbool>::OnlyIf<Class<T>::template isSuper<U>()> = true>
            CORE_EXPLICIT operator Optional<U> const() const {
                if(isEmpty()) {
                    return {};
                }
                Optional<U> option;
                if (!Class<U>::hasInstance(*value)) {
                    ClassCastException("Could not cast type " + classname() + " to " + option.classname())
                            .throws(__ctrace());
                }
                option.value = (typename Optional<U>::VALUE) value;
                return option;
            }

            template<class U, Class<gbool>::OnlyIf<Class<T>::template isSuper<U>()> = true>
            CORE_EXPLICIT operator Optional<U>() {
                if(isEmpty()) {
                    return {};
                }
                Optional<U> option;
                if (!Class<U>::hasInstance(*value)) {
                    ClassCastException("Could not cast type " + classname() + " to " + option.classname())
                            .throws(__ctrace());
                }
                option.value = (typename Optional<U>::VALUE) value;
                return option;
            }

            CORE_ENABLE_IMPLICIT_CAST(T &, get(),);
            CORE_ENABLE_IMPLICIT_CAST(const T&, get(), const);

        };

    } // util
} // core

#endif //CORE23_OPTIONAL_H

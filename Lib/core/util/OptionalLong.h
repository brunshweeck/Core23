//
// Created by T.N.Brunshweeck on 01/02/2024.
//

#ifndef CORE23_OPTIONALLONG_H
#define CORE23_OPTIONALLONG_H

#include <core/Long.h>
#include <core/Runnable.h>
#include <core/function/LongConsumer.h>
#include <core/function/LongSupplier.h>
#include <core/function/LongPredicate.h>

namespace core {
    namespace util {

        class OptionalLong CORE_FINAL : public Object {
        private:
            glong value;
            gbool present;

        public:
            /**
             * Construct an empty instance.
             */
            OptionalLong();

            /**
             * Construct an instance with the described value.
             *
             * @param value the long value to describe.
             */
            CORE_EXPLICIT OptionalLong(glong value);

            /**
             * If a value is present, returns the value, otherwise throws
             * <b> NoSuchElementException</b>.
             *
             * @apiNote
             * The preferred alternative to this method is <b style="color: orange;"> optionalLong::orElseThrow()</b>.
             *
             * @return the value described by this <b> OptionalLong</b>
             * @throws NoSuchElementException if no value is present
             */
            glong get() const;

            /**
             * If a value is present, returns <b> true</b>, otherwise <b> false</b>.
             *
             * @return <b> true</b> if a value is present, otherwise <b> false</b>
             */
            gbool isPresent() const;

            /**
             * If a value is not present, returns <b> true</b>, otherwise
             * <b> false</b>.
             *
             * @return  <b> true</b> if a value is not present, otherwise <b> false</b>
             */
            gbool isEmpty() const;

            /**
             * If a value is present, performs the given action with the value,
             * otherwise does nothing.
             *
             * @param action the action to be performed, if a value is present
             */
            void ifPresent(const LongConsumer &action) const;

            /**
             * If a value is present, performs the given action with the value,
             * otherwise performs the given empty-based action.
             *
             * @param action the action to be performed, if a value is present
             * @param emptyAction the empty-based action to be performed, if no value is
             * present
             */
            void ifPresentOrElse(const LongConsumer &action, const Runnable &emptyAction) const;

            /**
             * If a value is present, returns a sequential <b style="color: orange;"> LongStream</b>
             * containing only that value, otherwise returns an empty
             * <b> LongStream</b>.
             *
             * @apiNote
             * This method can be used to transform a <b> Stream</b> of optional longs
             * to a <b> LongStream</b> of present longs:
             * <pre><b> @code
             *     Stream<OptionalLong> os = ..
             *     LongStream s = os.flatMapToLong(OptionalLong::stream)
             * @endcode </b></pre>
             *
             * @return the optional value as a <b> LongStream</b>
             */
            LongStream &stream() const;

            /**
             * If a value is present, returns the value, otherwise returns
             * <b> other</b>.
             *
             * @param other the value to be returned, if no value is present
             * @return the value, if present, otherwise <b> other</b>
             */
            glong orElse(glong other) const;

            /**
             * If a value is present, returns the value, otherwise returns the result
             * produced by the supplying function.
             *
             * @param supplier the supplying function that produces a value to be returned
             * @return the value, if present, otherwise the result produced by the
             *         supplying function
             * @throws NullPointerException if no value is present and the supplying
             *         function is <b> null</b>
             */
            glong orElseGet(const LongSupplier &supplier) const;

            /**
             * If a value is present, returns the value, otherwise throws
             * <b> NoSuchElementException</b>.
             *
             * @return the value described by this <b> OptionalLong</b>
             * @throws NoSuchElementException if no value is present
             */
            glong orElseThrow() const;

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
             * @throws X if no value is present
             */
            glong orElseThrow(const Supplier<Throwable> &exceptionSupplier) const;

            /**
             * Indicates whether some other object is "equal to" this
             * <b> OptionalLong</b>. The other object is considered equal if:
             * <ul>
             * <li>it is also an <b> OptionalLong</b> and;
             * <li>both instances have no value present or;
             * <li>the present values are "equal to" each other via
             * <b> Long.compare() == 0</b>.
             * </ul>
             *
             * @param obj an object to be tested for equality
             * @return <b> true</b> if the other object is "equal to" this object
             *         otherwise <b> false</b>
             */
            gbool equals(const Object &obj) const override;

            /**
             * Returns the hash code of the value, if present, otherwise <b> 0</b>
             * (zero) if no value is present.
             *
             * @return hash code value of the present value or <b> 0</b> if no value is
             *         present
             */
            gint hash() const override;

            /**
             * Returns a non-empty string representation of this <b> OptionalLong</b>
             * suitable for debugging.  The exact presentation format is unspecified and
             * may vary between implementations and versions.
             *
             * @implSpec
             * If a value is present the result must include its string representation
             * in the result.  Empty and present <b> OptionalLong</b>s must be
             * unambiguously differentiable.
             *
             * @return the string representation of this instance
             */
            String toString() const override;

            /**
             * Return shadow copy of this object
             */
            Object &clone() const override;
        };

    } // util
} // core

#endif //CORE23_OPTIONALLONG_H

//
// Created by T.N.Brunshweeck on 01/02/2024.
//

#ifndef CORE23_OPTIONALINT_H
#define CORE23_OPTIONALINT_H

#include <core/Integer.h>
#include <core/Runnable.h>
#include <core/function/IntConsumer.h>
#include <core/function/IntSupplier.h>
#include <core/function/IntPredicate.h>

namespace core {
    namespace util {

        class OptionalInt CORE_FINAL : public Object {
        private:
            gint value;
            gbool present;

        public:
            /**
             * Construct an empty instance.
             */
            OptionalInt();

            /**
             * Construct an instance with the described value.
             *
             * @param value the int value to describe.
             */
            CORE_EXPLICIT OptionalInt(gint value);

            /**
             * If a value is present, returns the value, otherwise throws
             * <b> NoSuchElementException</b>.
             *
             * @apiNote
             * The preferred alternative to this method is <b style="color: orange;"> optionalInt::orElseThrow()</b>.
             *
             * @return the value described by this <b> OptionalInt</b>
             * @throws NoSuchElementException if no value is present
             */
            gint get() const;

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
            void ifPresent(const IntConsumer &action) const;

            /**
             * If a value is present, performs the given action with the value,
             * otherwise performs the given empty-based action.
             *
             * @param action the action to be performed, if a value is present
             * @param emptyAction the empty-based action to be performed, if no value is
             * present
             */
            void ifPresentOrElse(const IntConsumer &action, const Runnable &emptyAction) const;

            /**
             * If a value is present, returns a sequential <b style="color: orange;"> IntStream</b>
             * containing only that value, otherwise returns an empty
             * <b> IntStream</b>.
             *
             * @apiNote
             * This method can be used to transform a <b> Stream</b> of optional ints
             * to a <b> IntStream</b> of present ints:
             * <pre><b> @code
             *     Stream<OptionalInt> os = ..
             *     IntStream s = os.flatMapToInt(OptionalInt::stream)
             * @endcode </b></pre>
             *
             * @return the optional value as a <b> IntStream</b>
             */
            IntStream &stream() const;

            /**
             * If a value is present, returns the value, otherwise returns
             * <b> other</b>.
             *
             * @param other the value to be returned, if no value is present
             * @return the value, if present, otherwise <b> other</b>
             */
            gint orElse(gint other) const;

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
            gint orElseGet(const IntSupplier &supplier) const;

            /**
             * If a value is present, returns the value, otherwise throws
             * <b> NoSuchElementException</b>.
             *
             * @return the value described by this <b> OptionalInt</b>
             * @throws NoSuchElementException if no value is present
             */
            gint orElseThrow() const;

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
            gint orElseThrow(const Supplier<Throwable> &exceptionSupplier) const;

            /**
             * Indicates whether some other object is "equal to" this
             * <b> OptionalInt</b>. The other object is considered equal if:
             * <ul>
             * <li>it is also an <b> OptionalInt</b> and;
             * <li>both instances have no value present or;
             * <li>the present values are "equal to" each other via
             * <b> Int.compare() == 0</b>.
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
             * Returns a non-empty string representation of this <b> OptionalInt</b>
             * suitable for debugging.  The exact presentation format is unspecified and
             * may vary between implementations and versions.
             *
             * @implSpec
             * If a value is present the result must include its string representation
             * in the result.  Empty and present <b> OptionalInt</b>s must be
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

#endif //CORE23_OPTIONALINT_H

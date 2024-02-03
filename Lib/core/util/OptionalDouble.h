//
// Created by T.N.Brunshweeck on 01/02/2024.
//

#ifndef CORE23_OPTIONALDOUBLE_H
#define CORE23_OPTIONALDOUBLE_H

#include <core/Double.h>
#include <core/Runnable.h>
#include <core/function/DoubleConsumer.h>
#include <core/function/DoubleSupplier.h>
#include <core/function/DoublePredicate.h>

namespace core {
    namespace util {

        class OptionalDouble CORE_FINAL : public Object {
        private:
            gdouble value;
            gbool present;

        public:
            /**
             * Construct an empty instance.
             */
            OptionalDouble();

            /**
             * Construct an instance with the described value.
             *
             * @param value the double value to describe.
             */
            CORE_EXPLICIT OptionalDouble(gdouble value);

            /**
             * If a value is present, returns the value, otherwise throws
             * <b> NoSuchElementException</b>.
             *
             * @apiNote
             * The preferred alternative to this method is <b style="color: orange;"> optionalDouble::orElseThrow()</b>.
             *
             * @return the value described by this <b> OptionalDouble</b>
             * @throws NoSuchElementException if no value is present
             */
            gdouble get() const;

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
            void ifPresent(const DoubleConsumer &action) const;

            /**
             * If a value is present, performs the given action with the value,
             * otherwise performs the given empty-based action.
             *
             * @param action the action to be performed, if a value is present
             * @param emptyAction the empty-based action to be performed, if no value is
             * present
             */
            void ifPresentOrElse(const DoubleConsumer &action, const Runnable &emptyAction) const;

            /**
             * If a value is present, returns a sequential <b style="color: orange;"> DoubleStream</b>
             * containing only that value, otherwise returns an empty
             * <b> DoubleStream</b>.
             *
             * @apiNote
             * This method can be used to transform a <b> Stream</b> of optional doubles
             * to a <b> DoubleStream</b> of present doubles:
             * <pre><b> @code
             *     Stream<OptionalDouble> os = ..
             *     DoubleStream s = os.flatMapToDouble(OptionalDouble::stream)
             * @endcode </b></pre>
             *
             * @return the optional value as a <b> DoubleStream</b>
             */
            DoubleStream &stream() const;

            /**
             * If a value is present, returns the value, otherwise returns
             * <b> other</b>.
             *
             * @param other the value to be returned, if no value is present
             * @return the value, if present, otherwise <b> other</b>
             */
            gdouble orElse(gdouble other) const;

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
            gdouble orElseGet(const DoubleSupplier &supplier) const;

            /**
             * If a value is present, returns the value, otherwise throws
             * <b> NoSuchElementException</b>.
             *
             * @return the value described by this <b> OptionalDouble</b>
             * @throws NoSuchElementException if no value is present
             */
            gdouble orElseThrow() const;

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
            gdouble orElseThrow(const Supplier<Throwable> &exceptionSupplier) const;

            /**
             * Indicates whether some other object is "equal to" this
             * <b> OptionalDouble</b>. The other object is considered equal if:
             * <ul>
             * <li>it is also an <b> OptionalDouble</b> and;
             * <li>both instances have no value present or;
             * <li>the present values are "equal to" each other via
             * <b> Double.compare() == 0</b>.
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
             * Returns a non-empty string representation of this <b> OptionalDouble</b>
             * suitable for debugging.  The exact presentation format is unspecified and
             * may vary between implementations and versions.
             *
             * @implSpec
             * If a value is present the result must include its string representation
             * in the result.  Empty and present <b> OptionalDouble</b>s must be
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

#endif //CORE23_OPTIONALDOUBLE_H

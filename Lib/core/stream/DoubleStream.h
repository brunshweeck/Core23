//
// Created by T.N.Brunshweeck on 31/01/2024.
//

#ifndef CORE23_DOUBLESTREAM_H
#define CORE23_DOUBLESTREAM_H

#include <core/stream/BaseStream.h>

namespace core {
    namespace stream {

        /**
         * A sequence of primitive double-valued elements supporting sequential and parallel
         * aggregate operations.  This is the <b> double</b> primitive specialization of
         * <b style="color: orange;"> Stream</b>.
         *
         * <p>The following example illustrates an aggregate operation using
         * <b style="color: orange;"> Stream</b> and <b style="color: orange;"> DoubleStream</b>, computing the sum of the weights of the
         * red widgets:
         *
         * <pre><b> 
         *     gdouble sum = widgets.stream()
         *                      .filter(Predicate::from([](const T &w) -> gbool { return w.color() == RED; </b>))
         *                      .mapToDouble(ToDoubleFunction<T>::from([](const T &w) -> gint { return w.weight(); </b>))
         *                      .sum();
         * </b></pre>
         *
         * See the class documentation for <b style="color: orange;"> Stream</b> and the package documentation
         * for <a href="package-summary.html">java.util.stream</a> for additional
         * specification of streams, stream operations, stream pipelines, and
         * parallelism.
         *
         * @see Stream
         */
        class DoubleStream : public BaseStream<Double, DoubleStream> {
        public:

            /**
             * Returns a stream consisting of the elements of this stream that match
             * the given predicate.
             *
             * <p>This is an <a href="package-summary.html#StreamOps">intermediate
             * operation</a>.
             *
             * @param predicate a <a href="package-summary.html#NonInterference">non-interfering</a>,
             *                  <a href="package-summary.html#Statelessness">stateless</a>
             *                  predicate to apply to each element to determine if it
             *                  should be included
             * @return the new stream
             */
            virtual DoubleStream &filter() const = 0;

            /**
             * Returns a stream consisting of the results of applying the given
             * function to the elements of this stream.
             *
             * <p>This is an <a href="package-summary.html#StreamOps">intermediate
             * operation</a>.
             *
             * @param mapper a <a href="package-summary.html#NonInterference">non-interfering</a>,
             *               <a href="package-summary.html#Statelessness">stateless</a>
             *               function to apply to each element
             * @return the new stream
             */
            virtual DoubleStream &map(const DoubleUnaryOperator &mapper) const = 0;

            /**
             * Returns an object-valued <b> Stream</b> consisting of the results of
             * applying the given function to the elements of this stream.
             *
             * <p>This is an <a href="package-summary.html#StreamOps">
             *     intermediate operation</a>.
             *
             * @param mapper a <a href="package-summary.html#NonInterference">non-interfering</a>,
             *               <a href="package-summary.html#Statelessness">stateless</a>
             *               function to apply to each element
             * @return the new stream
             */
            virtual Stream<Object> &mapToObj(const DoubleFunction<Object &> &mapper) const = 0;

            /**
             * Returns an <b> IntStream</b> consisting of the results of applying the
             * given function to the elements of this stream.
             *
             * <p>This is an <a href="package-summary.html#StreamOps">intermediate
             * operation</a>.
             *
             * @param mapper a <a href="package-summary.html#NonInterference">non-interfering</a>,
             *               <a href="package-summary.html#Statelessness">stateless</a>
             *               function to apply to each element
             * @return the new stream
             */
            virtual IntStream &mapToInt(const DoubleToIntFunction &mapper) const = 0;

            /**
             * Returns a <b> LongStream</b> consisting of the results of applying the
             * given function to the elements of this stream.
             *
             * <p>This is an <a href="package-summary.html#StreamOps">intermediate
             * operation</a>.
             *
             * @param mapper a <a href="package-summary.html#NonInterference">non-interfering</a>,
             *               <a href="package-summary.html#Statelessness">stateless</a>
             *               function to apply to each element
             * @return the new stream
             */
            virtual LongStream &mapToLong(const DoubleToLongFunction &mapper) const = 0;

            /**
             * Returns a stream consisting of the results of replacing each element of
             * this stream with the contents of a mapped stream produced by applying
             * the provided mapping function to each element.  Each mapped stream is
             * <b style="color: orange;"> java.util.stream.BaseStream#close() closed</b> after its contents
             * have been placed into this stream.  (If a mapped stream is <b> null</b>
             * an empty stream is used, instead.)
             *
             * <p>This is an <a href="package-summary.html#StreamOps">intermediate
             * operation</a>.
             *
             * @param mapper a <a href="package-summary.html#NonInterference">non-interfering</a>,
             *               <a href="package-summary.html#Statelessness">stateless</a>
             *               function to apply to each element which produces a
             *               <b> DoubleStream</b> of new values
             * @return the new stream
             * @see Stream#flatMap(Function)
             */
            virtual DoubleStream &flatMap(const DoubleFunction<DoubleStream> &mapper) const = 0;

            /**
             * Returns a stream consisting of the results of replacing each element of
             * this stream with multiple elements, specifically zero or more elements.
             * Replacement is performed by applying the provided mapping function to each
             * element in conjunction with a {@linkplain DoubleConsumer consumer} argument
             * that accepts replacement elements. The mapping function calls the consumer
             * zero or more times to provide the replacement elements.
             *
             * <p>This is an <a href="package-summary.html#StreamOps">intermediate
             * operation</a>.
             *
             * <p>If the {@linkplain DoubleConsumer consumer} argument is used outside the scope of
             * its application to the mapping function, the results are undefined.
             *
             * @implSpec
             * The default implementation invokes {@link #flatMap flatMap} on this stream,
             * passing a function that behaves as follows. First, it calls the mapper function
             * with a {@code DoubleConsumer} that accumulates replacement elements into a newly created
             * internal buffer. When the mapper function returns, it creates a {@code DoubleStream} from the
             * internal buffer. Finally, it returns this stream to {@code flatMap}.
             *
             * @param mapper a <a href="package-summary.html#NonInterference">non-interfering</a>,
             *               <a href="package-summary.html#Statelessness">stateless</a>
             *               function that generates replacement elements
             * @return the new stream
             * @see Stream#mapMulti Stream.mapMulti
             */
            virtual DoubleStream &mapMulti(const BiConsumer<Double, DoubleConsumer> const &mapper) const = 0;

            /**
             * Returns a stream consisting of the distinct elements of this stream. The
             * elements are compared for equality according to
             * {@link java.lang.Double#compare(double, double)}.
             *
             * <p>This is a <a href="package-summary.html#StreamOps">stateful
             * intermediate operation</a>.
             *
             * @return the result stream
             */
            virtual DoubleStream &distinct() const = 0;

            /**
             * Returns a stream consisting of the elements of this stream in sorted
             * order. The elements are compared for equality according to
             * {@link java.lang.Double#compare(double, double)}.
             *
             * <p>This is a <a href="package-summary.html#StreamOps">stateful
             * intermediate operation</a>.
             *
             * @return the result stream
             */
            virtual DoubleStream &sorted() const = 0;

            /**
             * Returns a stream consisting of the elements of this stream, additionally
             * performing the provided action on each element as elements are consumed
             * from the resulting stream.
             *
             * <p>This is an <a href="package-summary.html#StreamOps">intermediate
             * operation</a>.
             *
             * <p>For parallel stream pipelines, the action may be called at
             * whatever time and in whatever thread the element is made available by the
             * upstream operation.  If the action modifies shared state,
             * it is responsible for providing the required synchronization.
             *
             * @apiNote This method exists mainly to support debugging, where you want
             * to see the elements as they flow past a certain point in a pipeline:
             * <pre>{@code
             *     DoubleStream.of(1, 2, 3, 4)
             *         .filter(e -> e > 2)
             *         .peek(e -> System.out.println("Filtered value: " + e))
             *         .map(e -> e * e)
             *         .peek(e -> System.out.println("Mapped value: " + e))
             *         .sum();
             * }</pre>
             *
             * <p>In cases where the stream implementation is able to optimize away the
             * production of some or all the elements (such as with short-circuiting
             * operations like {@code findFirst}, or in the example described in
             * {@link #count}), the action will not be invoked for those elements.
             *
             * @param action a <a href="package-summary.html#NonInterference">
             *               non-interfering</a> action to perform on the elements as
             *               they are consumed from the stream
             * @return the new stream
             */
            virtual DoubleStream &peek(const DoubleConsumer &action) = 0;

            /**
             * Returns a stream consisting of the elements of this stream, truncated
             * to be no longer than {@code maxSize} in length.
             *
             * <p>This is a <a href="package-summary.html#StreamOps">short-circuiting
             * stateful intermediate operation</a>.
             *
             * @apiNote
             * While {@code limit()} is generally a cheap operation on sequential
             * stream pipelines, it can be quite expensive on ordered parallel pipelines,
             * especially for large values of {@code maxSize}, since {@code limit(n)}
             * is constrained to return not just any <em>n</em> elements, but the
             * <em>first n</em> elements in the encounter order.  Using an unordered
             * stream source (such as {@link #generate(DoubleSupplier)}) or removing the
             * ordering constraint with {@link #unordered()} may result in significant
             * speedups of {@code limit()} in parallel pipelines, if the semantics of
             * your situation permit.  If consistency with encounter order is required,
             * and you are experiencing poor performance or memory utilization with
             * {@code limit()} in parallel pipelines, switching to sequential execution
             * with {@link #sequential()} may improve performance.
             *
             * @param maxSize the number of elements the stream should be limited to
             * @return the new stream
             * @throws IllegalArgumentException if {@code maxSize} is negative
             */
            virtual DoubleStream &limit(glong maxSize) = 0;

            /**
             * Returns a stream consisting of the remaining elements of this stream
             * after discarding the first {@code n} elements of the stream.
             * If this stream contains fewer than {@code n} elements then an
             * empty stream will be returned.
             *
             * <p>This is a <a href="package-summary.html#StreamOps">stateful
             * intermediate operation</a>.
             *
             * @apiNote
             * While {@code skip()} is generally a cheap operation on sequential
             * stream pipelines, it can be quite expensive on ordered parallel pipelines,
             * especially for large values of {@code n}, since {@code skip(n)}
             * is constrained to skip not just any <em>n</em> elements, but the
             * <em>first n</em> elements in the encounter order.  Using an unordered
             * stream source (such as {@link #generate(DoubleSupplier)}) or removing the
             * ordering constraint with {@link #unordered()} may result in significant
             * speedups of {@code skip()} in parallel pipelines, if the semantics of
             * your situation permit.  If consistency with encounter order is required,
             * and you are experiencing poor performance or memory utilization with
             * {@code skip()} in parallel pipelines, switching to sequential execution
             * with {@link #sequential()} may improve performance.
             *
             * @param n the number of leading elements to skip
             * @return the new stream
             * @throws IllegalArgumentException if {@code n} is negative
             */
            virtual DoubleStream &skip(glong n) = 0;

            /**
             * Returns, if this stream is ordered, a stream consisting of the longest
             * prefix of elements taken from this stream that match the given predicate.
             * Otherwise returns, if this stream is unordered, a stream consisting of a
             * subset of elements taken from this stream that match the given predicate.
             *
             * <p>If this stream is ordered then the longest prefix is a contiguous
             * sequence of elements of this stream that match the given predicate.  The
             * first element of the sequence is the first element of this stream, and
             * the element immediately following the last element of the sequence does
             * not match the given predicate.
             *
             * <p>If this stream is unordered, and some (but not all) elements of this
             * stream match the given predicate, then the behavior of this operation is
             * nondeterministic; it is free to take any subset of matching elements
             * (which includes the empty set).
             *
             * <p>Independent of whether this stream is ordered or unordered if all
             * elements of this stream match the given predicate then this operation
             * takes all elements (the result is the same as the input), or if no
             * elements of the stream match the given predicate then no elements are
             * taken (the result is an empty stream).
             *
             * <p>This is a <a href="package-summary.html#StreamOps">short-circuiting
             * stateful intermediate operation</a>.
             *
             * @implSpec
             * The default implementation obtains the {@link #spliterator() spliterator}
             * of this stream, wraps that spliterator so as to support the semantics
             * of this operation on traversal, and returns a new stream associated with
             * the wrapped spliterator.  The returned stream preserves the execution
             * characteristics of this stream (namely parallel or sequential execution
             * as per {@link #isParallel()}) but the wrapped spliterator may choose to
             * not support splitting.  When the returned stream is closed, the close
             * handlers for both the returned and this stream are invoked.
             *
             * @apiNote
             * While {@code takeWhile()} is generally a cheap operation on sequential
             * stream pipelines, it can be quite expensive on ordered parallel
             * pipelines, since the operation is constrained to return not just any
             * valid prefix, but the longest prefix of elements in the encounter order.
             * Using an unordered stream source (such as
             * {@link #generate(DoubleSupplier)}) or removing the ordering constraint
             * with {@link #unordered()} may result in significant speedups of
             * {@code takeWhile()} in parallel pipelines, if the semantics of your
             * situation permit.  If consistency with encounter order is required, and
             * you are experiencing poor performance or memory utilization with
             * {@code takeWhile()} in parallel pipelines, switching to sequential
             * execution with {@link #sequential()} may improve performance.
             *
             * @param predicate a <a href="package-summary.html#NonInterference">non-interfering</a>,
             *                  <a href="package-summary.html#Statelessness">stateless</a>
             *                  predicate to apply to elements to determine the longest
             *                  prefix of elements.
             * @return the new stream
             */
            virtual DoubleStream &takeWhile(const DoublePredicate &predicate) = 0;

            /**
             * Returns, if this stream is ordered, a stream consisting of the remaining
             * elements of this stream after dropping the longest prefix of elements
             * that match the given predicate.  Otherwise returns, if this stream is
             * unordered, a stream consisting of the remaining elements of this stream
             * after dropping a subset of elements that match the given predicate.
             *
             * <p>If this stream is ordered then the longest prefix is a contiguous
             * sequence of elements of this stream that match the given predicate.  The
             * first element of the sequence is the first element of this stream, and
             * the element immediately following the last element of the sequence does
             * not match the given predicate.
             *
             * <p>If this stream is unordered, and some (but not all) elements of this
             * stream match the given predicate, then the behavior of this operation is
             * nondeterministic; it is free to drop any subset of matching elements
             * (which includes the empty set).
             *
             * <p>Independent of whether this stream is ordered or unordered if all
             * elements of this stream match the given predicate then this operation
             * drops all elements (the result is an empty stream), or if no elements of
             * the stream match the given predicate then no elements are dropped (the
             * result is the same as the input).
             *
             * <p>This is a <a href="package-summary.html#StreamOps">stateful
             * intermediate operation</a>.
             *
             * @implSpec
             * The default implementation obtains the {@link #spliterator() spliterator}
             * of this stream, wraps that spliterator so as to support the semantics
             * of this operation on traversal, and returns a new stream associated with
             * the wrapped spliterator.  The returned stream preserves the execution
             * characteristics of this stream (namely parallel or sequential execution
             * as per {@link #isParallel()}) but the wrapped spliterator may choose to
             * not support splitting.  When the returned stream is closed, the close
             * handlers for both the returned and this stream are invoked.
             *
             * @apiNote
             * While {@code dropWhile()} is generally a cheap operation on sequential
             * stream pipelines, it can be quite expensive on ordered parallel
             * pipelines, since the operation is constrained to return not just any
             * valid prefix, but the longest prefix of elements in the encounter order.
             * Using an unordered stream source (such as
             * {@link #generate(DoubleSupplier)}) or removing the ordering constraint
             * with {@link #unordered()} may result in significant speedups of
             * {@code dropWhile()} in parallel pipelines, if the semantics of your
             * situation permit.  If consistency with encounter order is required, and
             * you are experiencing poor performance or memory utilization with
             * {@code dropWhile()} in parallel pipelines, switching to sequential
             * execution with {@link #sequential()} may improve performance.
             *
             * @param predicate a <a href="package-summary.html#NonInterference">non-interfering</a>,
             *                  <a href="package-summary.html#Statelessness">stateless</a>
             *                  predicate to apply to elements to determine the longest
             *                  prefix of elements.
             * @return the new stream
             * @since 9
             */
            virtual DoubleStream &dropWhile(const DoublePredicate &predicate) = 0;

            /**
             * Performs an action for each element of this stream.
             *
             * <p>This is a <a href="package-summary.html#StreamOps">terminal
             * operation</a>.
             *
             * <p>For parallel stream pipelines, this operation does <em>not</em>
             * guarantee to respect the encounter order of the stream, as doing so
             * would sacrifice the benefit of parallelism.  For any given element, the
             * action may be performed at whatever time and in whatever thread the
             * library chooses.  If the action accesses shared state, it is
             * responsible for providing the required synchronization.
             *
             * @param action a <a href="package-summary.html#NonInterference">
             *               non-interfering</a> action to perform on the elements
             */
            virtual void forEach(const DoubleConsumer &action) const = 0;

            /**
             * Performs an action for each element of this stream, guaranteeing that
             * each element is processed in encounter order for streams that have a
             * defined encounter order.
             *
             * <p>This is a <a href="package-summary.html#StreamOps">terminal
             * operation</a>.
             *
             * @param action a <a href="package-summary.html#NonInterference">
             *               non-interfering</a> action to perform on the elements
             * @see #forEach(DoubleConsumer)
             */
            virtual void forEachOrdered(const DoubleConsumer &action) const = 0;

            /**
             * Returns an array containing the elements of this stream.
             *
             * <p>This is a <a href="package-summary.html#StreamOps">terminal
             * operation</a>.
             *
             * @return an array containing the elements of this stream
             */
            virtual DoubleArray toArray() const = 0;

            /**
             * Performs a <a href="package-summary.html#Reduction">reduction</a> on the
             * elements of this stream, using the provided identity value and an
             * <a href="package-summary.html#Associativity">associative</a>
             * accumulation function, and returns the reduced value.  This is equivalent
             * to:
             * <pre>{@code
             *     double result = identity;
             *     for (double element : this stream)
             *         result = accumulator.applyAsDouble(result, element)
             *     return result;
             * }</pre>
             *
             * but is not constrained to execute sequentially.
             *
             * <p>The {@code identity} value must be an identity for the accumulator
             * function. This means that for all {@code x},
             * {@code accumulator.apply(identity, x)} is equal to {@code x}.
             * The {@code accumulator} function must be an
             * <a href="package-summary.html#Associativity">associative</a> function.
             *
             * <p>This is a <a href="package-summary.html#StreamOps">terminal
             * operation</a>.
             *
             * @apiNote Sum, min, max, and average are all special cases of reduction.
             * Summing a stream of numbers can be expressed as:
             *
             * <pre>{@code
             *     double sum = numbers.reduce(0, (a, b) -> a+b);
             * }</pre>
             *
             * or more compactly:
             *
             * <pre>{@code
             *     double sum = numbers.reduce(0, Double::sum);
             * }</pre>
             *
             * <p>While this may seem a more roundabout way to perform an aggregation
             * compared to simply mutating a running total in a loop, reduction
             * operations parallelize more gracefully, without needing additional
             * synchronization and with greatly reduced risk of data races.
             *
             * @param identity the identity value for the accumulating function
             * @param op an <a href="package-summary.html#Associativity">associative</a>,
             *           <a href="package-summary.html#NonInterference">non-interfering</a>,
             *           <a href="package-summary.html#Statelessness">stateless</a>
             *           function for combining two values
             * @return the result of the reduction
             * @see #sum()
             * @see #min()
             * @see #max()
             * @see #average()
             */
            virtual gdouble reduce(gdouble indentity, const DoubleBinaryOperator &op) = 0;

            /**
             * Performs a <a href="package-summary.html#Reduction">reduction</a> on the
             * elements of this stream, using an
             * <a href="package-summary.html#Associativity">associative</a> accumulation
             * function, and returns an {@code OptionalDouble} describing the reduced
             * value, if any. This is equivalent to:
             * <pre>{@code
             *     boolean foundAny = false;
             *     double result = null;
             *     for (double element : this stream) {
             *         if (!foundAny) {
             *             foundAny = true;
             *             result = element;
             *         }
             *         else
             *             result = accumulator.applyAsDouble(result, element);
             *     }
             *     return foundAny ? OptionalDouble.of(result) : OptionalDouble.empty();
             * }</pre>
             *
             * but is not constrained to execute sequentially.
             *
             * <p>The {@code accumulator} function must be an
             * <a href="package-summary.html#Associativity">associative</a> function.
             *
             * <p>This is a <a href="package-summary.html#StreamOps">terminal
             * operation</a>.
             *
             * @param op an <a href="package-summary.html#Associativity">associative</a>,
             *           <a href="package-summary.html#NonInterference">non-interfering</a>,
             *           <a href="package-summary.html#Statelessness">stateless</a>
             *           function for combining two values
             * @return the result of the reduction
             * @see #reduce(double, DoubleBinaryOperator)
             */
            virtual gdouble reduce(const DoubleBinaryOperator &op) = 0;

            /**
             * Performs a <a href="package-summary.html#MutableReduction">mutable
             * reduction</a> operation on the elements of this stream.  A mutable
             * reduction is one in which the reduced value is a mutable result container,
             * such as an {@code ArrayList}, and elements are incorporated by updating
             * the state of the result rather than by replacing the result.  This
             * produces a result equivalent to:
             * <pre>{@code
             *     R result = supplier.get();
             *     for (double element : this stream)
             *         accumulator.accept(result, element);
             *     return result;
             * }</pre>
             *
             * <p>Like {@link #reduce(double, DoubleBinaryOperator)}, {@code collect}
             * operations can be parallelized without requiring additional
             * synchronization.
             *
             * <p>This is a <a href="package-summary.html#StreamOps">terminal
             * operation</a>.
             *
             * @param <R> the type of the mutable result container
             * @param supplier a function that creates a new mutable result container.
             *                 For a parallel execution, this function may be called
             *                 multiple times and must return a fresh value each time.
             * @param accumulator an <a href="package-summary.html#Associativity">associative</a>,
             *                    <a href="package-summary.html#NonInterference">non-interfering</a>,
             *                    <a href="package-summary.html#Statelessness">stateless</a>
             *                    function that must fold an element into a result
             *                    container.
             * @param combiner an <a href="package-summary.html#Associativity">associative</a>,
             *                    <a href="package-summary.html#NonInterference">non-interfering</a>,
             *                    <a href="package-summary.html#Statelessness">stateless</a>
             *                    function that accepts two partial result containers
             *                    and merges them, which must be compatible with the
             *                    accumulator function.  The combiner function must fold
             *                    the elements from the second result container into the
             *                    first result container.
             * @return the result of the reduction
             * @see Stream#collect(Supplier, BiConsumer, BiConsumer)
             */
            virtual Object &collect(const Supplier<Object> &supplier,
                                    const BiConsumer<Object, Double> &accumulator,
                                    const BiConsumer<Object, Object> &combiner) = 0;

            /**
             * Returns the sum of elements in this stream.
             *
             * Summation is a special case of a <a
             * href="package-summary.html#Reduction">reduction</a>. If
             * floating-point summation were exact, this method would be
             * equivalent to:
             *
             * <pre>{@code
             *     return reduce(0, Double::sum);
             * }</pre>
             *
             * However, since floating-point summation is not exact, the above
             * code is not necessarily equivalent to the summation computation
             * done by this method.
             *
             * <p>The value of a floating-point sum is a function both
             * of the input values as well as the order of addition
             * operations. The order of addition operations of this method is
             * intentionally not defined to allow for implementation
             * flexibility to improve the speed and accuracy of the computed
             * result.
             *
             * In particular, this method may be implemented using compensated
             * summation or other technique to reduce the error bound in the
             * numerical sum compared to a simple summation of {@code double}
             * values.
             *
             * Because of the unspecified order of operations and the
             * possibility of using differing summation schemes, the output of
             * this method may vary on the same input elements.
             *
             * <p>Various conditions can result in a non-finite sum being
             * computed. This can occur even if the all the elements
             * being summed are finite. If any element is non-finite,
             * the sum will be non-finite:
             *
             * <ul>
             *
             * <li>If any element is a NaN, then the final sum will be
             * NaN.
             *
             * <li>If the elements contain one or more infinities, the
             * sum will be infinite or NaN.
             *
             * <ul>
             *
             * <li>If the elements contain infinities of opposite sign,
             * the sum will be NaN.
             *
             * <li>If the elements contain infinities of one sign and
             * an intermediate sum overflows to an infinity of the opposite
             * sign, the sum may be NaN.
             *
             * </ul>
             *
             * </ul>
             *
             * It is possible for intermediate sums of finite values to
             * overflow into opposite-signed infinities; if that occurs, the
             * final sum will be NaN even if the elements are all
             * finite.
             *
             * If all the elements are zero, the sign of zero is
             * <em>not</em> guaranteed to be preserved in the final sum.
             *
             * <p>This is a <a href="package-summary.html#StreamOps">terminal
             * operation</a>.
             *
             * @apiNote Elements sorted by increasing absolute magnitude tend
             * to yield more accurate results.
             *
             * @return the sum of elements in this stream
             */
             gdouble sum();

            /**
             * Returns an {@code OptionalDouble} describing the minimum element of this
             * stream, or an empty OptionalDouble if this stream is empty.  The minimum
             * element will be {@code Double.NaN} if any stream element was NaN. Unlike
             * the numerical comparison operators, this method considers negative zero
             * to be strictly smaller than positive zero. This is a special case of a
             * <a href="package-summary.html#Reduction">reduction</a> and is
             * equivalent to:
             * <pre>{@code
             *     return reduce(Double::min);
             * }</pre>
             *
             * <p>This is a <a href="package-summary.html#StreamOps">terminal
             * operation</a>.
             *
             * @return an {@code OptionalDouble} containing the minimum element of this
             * stream, or an empty optional if the stream is empty
             */
             Optional<Double> min();

        };

    } // stream
} // core

#endif //CORE23_DOUBLESTREAM_H

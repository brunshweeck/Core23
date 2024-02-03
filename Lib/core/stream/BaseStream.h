//
// Created by T.N.Brunshweeck on 31/01/2024.
//

#ifndef CORE23_BASESTREAM_H
#define CORE23_BASESTREAM_H

#include <core/AutoClosable.h>

namespace core {
    namespace stream {

        /**
         * Base interface for streams, which are sequences of elements supporting
         * sequential and parallel aggregate operations.  The following example
         * illustrates an aggregate operation using the stream types <b style="color: orange;"> Stream</b> 
         * and <b style="color: orange;"> IntStream</b> , computing the sum of the weights of the red widgets:
         *
         * <pre><b> @code
         *     int sum = widgets.stream()
         *                      .filter(Predicate::from([](const T &w) -> gbool { return w.color() == RED; }))
         *                      .map(ToIntFunction<T>::from([](const T &w) -> gint { return w.weight(); }))
         *                      .sum();
         * @endcode </b> </pre>
         *
         * See the class documentation for <b style="color: orange;"> Stream</b>  and the package documentation
         * for <a href="package-summary.html">java.util.stream</a> for additional
         * specification of streams, stream operations, stream pipelines, and
         * parallelism, which governs the behavior of all stream types.
         *
         * @param T the type of the stream elements
         * @param S the type of the stream implementing <b> BaseStream</b> 
         * @see Stream
         * @see IntStream
         * @see LongStream
         * @see DoubleStream
         */
        template<class T, class S>
        class BaseStream : public Object, public AutoClosable {
        public:
            // CORE_STATIC_ASSERT(Class<BaseStream<T, S>>::template isSuper<S>, "");

            /**
             * Returns whether this stream, if a terminal operation were to be executed,
             * would execute in parallel.  Calling this method after invoking an
             * terminal stream operation method may yield unpredictable results.
             *
             * @return <b> true</b>  if this stream would execute in parallel if executed
             */
            virtual gbool isParallel() const = 0;

            /**
             * Returns an equivalent stream that is sequential.  May return
             * itself, either because the stream was already sequential, or because
             * the underlying stream state was modified to be sequential.
             *
             * <p>This is an <a href="package-summary.html#StreamOps">intermediate
             * operation</a>.
             *
             * @return a sequential stream
             */
            virtual S &sequential() const = 0;

            /**
             * Returns an equivalent stream that is parallel.  May return
             * itself, either because the stream was already parallel, or because
             * the underlying stream state was modified to be parallel.
             *
             * <p>This is an <a href="package-summary.html#StreamOps">intermediate
             * operation</a>.
             *
             * @return a parallel stream
             */
            virtual S &parallel() const = 0;

            /**
             * Returns an equivalent stream that is
             * <a href="package-summary.html#Ordering">unordered</a>.  May return
             * itself, either because the stream was already unordered, or because
             * the underlying stream state was modified to be unordered.
             *
             * <p>This is an <a href="package-summary.html#StreamOps">intermediate
             * operation</a>.
             *
             * @return an unordered stream
             */
            virtual S &unordered() const = 0;

            /**
             * Returns an equivalent stream with an additional close handler.  Close
             * handlers are run when the <b style="color: orange;"> #close()</b>  method
             * is called on the stream, and are executed in the order they were
             * added.  All close handlers are run, even if earlier close handlers throw
             * exceptions.  If any close handler throws an exception, the first
             * exception thrown will be relayed to the caller of <b> close()</b> , with
             * any remaining exceptions added to that exception as suppressed exceptions
             * (unless one of the remaining exceptions is the same exception as the
             * first exception, since an exception cannot suppress itself.)  May
             * return itself.
             *
             * <p>This is an <a href="package-summary.html#StreamOps">intermediate
             * operation</a>.
             *
             * @param closeHandler A task to execute when the stream is closed
             * @return a stream with a handler that is run if the stream is closed
             */
            virtual S &onClose(const Runnable &closeHandle) const = 0;

            /**
             * Closes this stream, causing all close handlers for this stream pipeline
             * to be called.
             *
             * @see AutoCloseable::close()
             */
            void close() override = 0;
        };

    } // stream
} // core

#endif //CORE23_BASESTREAM_H

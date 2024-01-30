//
// Created by T.N.Brunshweeck on 29/12/2023.
//

#ifndef CORE23_AUTOCLOSABLE_H
#define CORE23_AUTOCLOSABLE_H

#include <core/Object.h>

namespace core {

    /**
     * An object that may hold resources (such as file or socket handles)
     * until it is closed. The <b style="color: orange;"> close()</b> method of an <b> AutoCloseable</b>
     * object is called automatically when exiting a <b> 
     * try</b>-with-resources block for which the object has been declared in
     * the resource specification header. This construction ensures prompt
     * release, avoiding resource exhaustion exceptions and errors that
     * may otherwise occur.
     *
     * @note
     * <p>It is possible, and in fact common, for a base class to
     * implement AutoCloseable even though not all of its subclasses or
     * instances will hold releasable resources.  For code that must operate
     * in complete generality, or when it is known that the <b> AutoCloseable</b>
     * INSTANCE requires resource release, it is recommended to use <b>
     * try</b>-with-resources constructions. However, when using facilities such as
     * <b style="color: orange;"> core.io.Stream</b> that support both I/O-based and
     * non-I/O-based forms, <b> try</b>-with-resources blocks are in
     * general unnecessary when using non-I/O-based forms.
     *
     * @author Brunshweeck Tazeussong
     */
    class AutoClosable /* @property */ {
    public:
        /**
         * Closes this resource, relinquishing any underlying resources.
         * This method is invoked automatically on objects managed by the
         * <b> try</b>-with-resources statement.
         *
         * @note
         * While this interface method is declared to throw <b> 
         * Exception</b>, implementers are <em>strongly</em> encouraged to
         * declare concrete implementations of the <b> close</b> method to
         * throw more specific exceptions, or to throw no exception at all
         * if the close operation cannot fail.
         *
         * <p> Cases where the close operation may fail require careful
         * attention by implementers. It is strongly advised to relinquish
         * the underlying resources and to internally <em>mark</em> the
         * resource as closed, prior to throwing the exception. The <b> 
         * close</b> method is unlikely to be invoked more than once and so
         * this ensures that the resources are released in a timely manner.
         * Furthermore it reduces problems that could arise when the resource
         * wraps, or is wrapped, by another resource.
         *
         * <p><em>Implementers of this interface are also strongly advised
         * to not have the <b> close</b> method throw <b style="color: orange;"> 
         * InterruptedException</b>.</em>
         *
         * More generally, if it would cause problems for an
         * exception to be suppressed, the <b> AutoCloseable.close</b>
         * method should not throw it.
         *
         * <p>Note that unlike the <b style="color: orange;"> close</b>
         * method of <b style="color: orange;"> core.io.Closeable</b>, this <b> close</b> method
         * is <em>not</em> required to be idempotent.  In other words,
         * calling this <b> close</b> method more than once may have some
         * visible side effect, unlike <b> Closeable.close</b> which is
         * required to have no effect if called more than once.
         *
         * However, implementers of this interface are strongly encouraged
         * to make their <b> close</b> methods idempotent.
         *
         * @throws Exception if this resource cannot be closed
         */
        virtual void close() = 0;

        virtual ~AutoClosable() = default;
    };

} // core

#endif //CORE23_AUTOCLOSABLE_H

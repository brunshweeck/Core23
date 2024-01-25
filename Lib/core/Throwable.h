//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_THROWABLE_H
#define CORE23_THROWABLE_H

#include <core/Trace.h>

namespace core {

    /**
     * The Throwable class is the superclass of all errors and exceptions in this
     * framework. Only objects that are instances of this class (or one of its subclasses)
     * are thrown by with trace by <b> throws(...) </b> method.
     *
     * For the purposes of compile-time checking of exceptions, <b> Throwable</b> and any subclass
     * of <b> Throwable</b> that is not also a subclass of either <b> RuntimeException</b> or
     * <b>Error</b> are regarded as checked exceptions.
     *
     * <p>
     * Instances of two subclasses, <b>core::Error</b> and <b>core::Exception</b>, are conventionally
     * used to indicate that exceptional situations have occurred. Typically, these instances
     * are freshly created in the context of the exceptional situation so as to include relevant information
     * (such as stack trace root).
     *
     * <p>
     * A throwable contains a snapshot of the execution stack of its thread at the time it was created.
     * It can also contain a message string that gives more information about the error.  Finally, the
     * throwable can also contain a <i>cause</i>: another throwable that caused this throwable to be constructed.
     * The recording of this causal information is referred to as the <i>chained exception</i> facility, as the
     * cause can, itself, have a cause, and so on, leading to a "chain" of exceptions, each caused by another.
     *
     * <p>
     * One reason that a throwable may have a cause is that the class that throws it is built atop a lower layered
     * abstraction, and an operation on the upper layer fails due to a failure in the lower layer.  It would be bad
     * design to let the throwable thrown by the lower layer propagate outward, as it is generally unrelated to the
     * abstraction provided by the upper layer. Further, doing so would tie the API of the upper layer to the details
     * of its implementation, assuming the lower layer's exception was a checked exception.  Throwing a "wrapped exception"
     * (i.e., an exception containing a cause) allows the upper layer to communicate the details of the failure to its
     * caller without incurring either of these shortcomings.  It preserves the flexibility to change the implementation
     * of the upper layer without changing its API (in particular, the set of exceptions thrown by its methods).
     *
     * <p>
     * A second reason that a throwable may have a cause is that the method that throws it must conform to a general-purpose
     * interface that does not permit the method to throw the cause directly.  For example, suppose a persistent collection
     * conforms to the <b>Collection</b> interface, and that its persistence is implemented atop <b> core/io</b>.
     * Suppose the internals of the <b> add</b> method can throw an <b>core::IOException</b>.
     * The implementation can communicate the details of the <b> IOException</b> to its caller while conforming to the
     * <b> Collection</b> interface by wrapping the <b> IOException</b> in an appropriate unchecked exception.  (The
     * specification for the persistent collection should indicate that it is capable of throwing such exceptions.)
     *
     * <p>
     * A cause can be associated with a throwable via a constructor that takes the cause as an argument.
     * New throwable classes that wish to allow causes to be associated with them should provide constructors
     * that take a cause and delegate (perhaps indirectly) to one of the <b>Throwable</b> constructors that takes a
     * cause.
     *
     * <p>By convention, class <b>Throwable</b> and its subclasses have two constructors, one that takes no arguments
     * and one that takes a <b> String</b> argument that can be used to produce a detail message. Further, those
     * subclasses that might likely have a cause associated with them should have two more constructors, one that
     * takes a <b>Throwable</b> (the cause), and one that takes a <b>String</b> (the detail message) and a
     * <b>Throwable</b> (the cause).
     *
     * @author  Brunshweeck Tazeussong
     *
     */
class Throwable: public Object, public native::GENERIC_THROWABLE {
    private:
        /**
         * ///
         */
        CORE_ALIAS(STACKPOINT, typename Class<Trace>::Ptr);
        CORE_ALIAS(STACKTRACE, typename Class<STACKPOINT>::Ptr);

        /**
         * ///
         */
        CORE_ALIAS(CAUSE, typename Class<Throwable>::Ptr);
        CORE_ALIAS(PRINTSTREAM, Class<const char>::Ptr);

        /**
         * Specific details about the Throwable.  For example, for
         * FileException, this contains the name of the file that could not be found.
         */
        String msg;

        /**
         * The throwable that caused this throwable to get thrown, or itself if this
         * throwable was not caused by another throwable, or if the causative
         * throwable is unknown.  If this field is equal to this throwable itself,
         * it indicates that the cause of this throwable has not yet been
         * initialized.
         */
        CAUSE cse;

        /**
         * The stack trace, as returned by trace() method.
         */
        STACKTRACE stack;

        /**
         * The flag to indicate if this throwable
         * was delete after throwing. is always false
         */
        gbool isTemporary;

        /**
         * The number of execution point on stack trace
         */
        gint stackSize;

        /**
         * Add new Execution point on the stack.
         */
        void updateStack(const Trace &trace);

        /**
         * Copy all trace of specified throwable on this
         * throwable
         */
        void copyStack(const Throwable &thr);

    public:

        /**
         * Constructs a new throwable with empty String as its detail message.
         * The cause is not initialized.
         */
        Throwable() CORE_NOTHROW;

        /**
         * Constructs a new throwable with the specified detail message.  The
         * cause is not initialized.
         *
         * @param message
         *          The detail message. The detail message is saved for
         *          later retrieval by the message() method.
         */
        CORE_EXPLICIT Throwable(String message) CORE_NOTHROW;

        /**
         * Constructs a new throwable with the specified detail message,
         * and cause.
         *
         * @param message
         *          The detail message.
         * @param cause
         *          The cause.
         *
         * @see MemoryError
         * @see IllegalArgumentException
         * @see ArithmeticException
         */
        CORE_EXPLICIT Throwable(String message, const Throwable &cause) CORE_NOTHROW;

        /**
         * Initialize newly create Throwable with
         * specified throwable
         *
         * @param thr
         *          The original throwable
         */
        Throwable(const Throwable &thr) CORE_NOTHROW;

        /**
         * Initialize newly create Throwable with
         * specified throwable
         *
         * @param thr
         *          The original throwable
         */
        Throwable(Throwable &&thr) CORE_NOTHROW;

        /**
         * Returns the detail message string of this throwable.
         */
        virtual String message() const;

        /**
         * Returns the cause of this throwable or itself if the cause is non-existent
         * or unknown.  (The cause is the throwable that caused this throwable to get thrown.)
         *
         * <p>
         * This implementation returns the cause that was supplied via one of
         * the constructors requiring a Throwable, or that was set after
         * creation with the setCause(Throwable) method.  While it is
         * typically unnecessary to override this method, a subclass can override
         * it to return a cause set by some other means.  This is appropriate for
         * a "legacy chained throwable" that predates the addition of chained
         * exceptions to Throwable.  Note that it is <i>not</i> necessary to override
         * any of the what() methods, all of which invoke the cause() method to determine the
         * cause of a throwable.
         */
        const Throwable &cause() const;

        /**
         * Set the <i>cause</i> of this throwable to the specified value.
         * (The cause is the throwable that caused this throwable to get thrown.)
         *
         * @param  cause
         *              The cause
         *
         * @throws IllegalArgumentException
         *              If cause is this throwable.  (A throwable cannot be its own cause.)
         */
        void setCause(const Throwable &cause);

        /**
         * Returns a short description of this throwable.
         * The result is the concatenation of:
         * <ul>
         * <li> the name of the class of this object
         * <li> ": " (a colon and a diskSpace)
         * <li> the result of invoking this object's message() method
         * </ul>
         */
        String toString() const override;

        /**
         * Return true specified Object is an INSTANCE of Throwable
         * that have same properties as this throwable
         *
         * @param object
         *          The object to be compared
         */
        gbool equals(const Object &object) const override;

        /**
         * Return the sharable copy of this throwable
         */
        Object &clone() const override = 0;

        /**
         * Thrown this throwable with specified execution point.
         * This method is recommended to use for preserving true
         * INSTANCE and chaining. it unnecessary to override this
         * method because it use the clone() and raise() method.
         * It recommended to call this method with __trace()
         * macro because __trace() macro detect automatically
         * all trace informations (method name, filename and line number)
         * except module name, and, classname.
         * <p>
         * Example:
         * <pre> @code
         *  class X : public Exception {
         *      ...
         *  private:
         *      void raise() && override {
         *          ...
         *          throw X(*this);
         *      }
         *  };
         *
         *  class MyClass: public Object {
         *      ...
         *      void myMethod() const {
         *          ...
         *          X().throws(__trace(classname())); // into class
         *          // or X.throws(__trace("MyClass"));
         *      }
         *  };
         *
         *  int myFunction() {
         *      ...
         *      X().throws(__trace("")); // out of class
         *  }
         *
         * @endcode </pre>
         *
         * @param trace
         *          The execution point
         */
        CORE_NORETURN void throws(const Trace &trace) const;

        /**
         * Thrown/rethrown this throwable and with the previous execution point.
         * This method is recommended to use for preserving true
         * INSTANCE and chaining. it unnecessary to override this
         * method because it use the clone() and raise() method.
         * <p>
         * Example:
         * <pre> @code
         *   class X : public Exception {
         *      ...
         *   private:
         *      void raise() && override {
         *          ...
         *          throw X(*this);
         *      }
         *   };
         *
         *   int main() {
         *      ...
         *      try{
         *          ...
         *          X.throws(__trace(""));
         *      } catch(const Exception& ex) {
         *          X.throws(); // rethrow X
         *      }
         *   }
         * @endcode </pre>
         * <p>
         *  This code is similar to C++ standard code
         *  (DEPRECATED: The C++ standard not guaranteed to produce same result
         *              If the class X extends Exception/Error/Throwable class.):
         *  <pre>@code
         *  class X : public std::exception {...};
         *
         *  int main() {
         *      ...
         *      try {
         *          throw X();
         *      } catch(...) {
         *          throw;
         *      }
         *  }
         *  @endcode</pre>
         *
         * @param trace
         *          The execution point
         */
        CORE_NORETURN void throws() const;

    private:
        /**
         * Thrown this throwable basically
         * This method is used to preserve INSTANCE.
         *
         * It same to do throw X; for specific class X.
         */
        CORE_NORETURN CORE_DEPRECATED virtual void raise() && = 0;

    public:
        /**
         * Destroy this throwable
         */
        ~Throwable() CORE_NOTHROW override;

    private:
        /**
         * Print and return the stack trace of this throwable on the native stream
         */
        PRINTSTREAM what() const CORE_NOTHROW override;
    };

} // core

#endif //CORE23_THROWABLE_H

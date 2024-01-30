//
// Created by T.N.Brunshweeck on 05/01/2024.
//

#ifndef CORE23_BUFFER_H
#define CORE23_BUFFER_H

#include <core/util/Preconditions.h>

namespace core {
    namespace io {

        /**
         * A container for data of a specific primitive type.
         *
         * <p> A buffer is a linear, finite sequence of elements of a specific
         * primitive type.  Aside from its content, the essential properties of a
         * buffer are its capacity, limit, and position: </p>
         *
         * <blockquote>
         *
         *   <p> A buffer's <i>capacity</i> is the number of elements it contains.  The
         *   capacity of a buffer is never negative and never changes.  </p>
         *
         *   <p> A buffer's <i>limit</i> is the index of the first element that should
         *   not be read or written.  A buffer's limit is never negative and is never
         *   greater than its capacity.  </p>
         *
         *   <p> A buffer's <i>position</i> is the index of the next element to be
         *   read or written.  A buffer's position is never negative and is never
         *   greater than its limit.  </p>
         *
         * </blockquote>
         *
         * <p> There is one subclass of this class for each non-boolean primitive type.
         *
         *
         * <h2> Transferring data </h2>
         *
         * <p> Each subclass of this class defines two categories of <i>get</i> and
         * <i>put</i> operations: </p>
         *
         * <blockquote>
         *
         *   <p> <i>Relative</i> operations read or write one or more elements starting
         *   at the current position and then increment the position by the number of
         *   elements transferred.  If the requested transfer exceeds the limit then a
         *   relative <i>get</i> operation throws a <b style="color:orange;"> BufferUnderflowException</b> 
         *   and a relative <i>put</i> operation throws a <b style="color:orange;">
         *   BufferOverflowException</b> ; in either case, no data is transferred.  </p>
         *
         *   <p> <i>Absolute</i> operations take an explicit element index and do not
         *   affect the position.  Absolute <i>get</i> and <i>put</i> operations throw
         *   an <b style="color:orange;"> IndexException</b>  if the index argument exceeds the
         *   limit.  </p>
         *
         * </blockquote>
         *
         * <p> Data may also, of course, be transferred in to or out of a buffer by the
         * I/O operations of an appropriate channel, which are always relative to the
         * current position.
         *
         *
         * <h2> Marking and resetting </h2>
         *
         * <p> A buffer's <i>mark</i> is the index to which its position will be reset
         * when the <b style="color:orange;"> reset</b>  method is invoked.  The mark is not always
         * defined, but when it is defined it is never negative and is never greater
         * than the position.  If the mark is defined then it is discarded when the
         * position or the limit is adjusted to a value smaller than the mark.  If the
         * mark is not defined then invoking the <b style="color:orange;"> reset</b>  method causes an
         * <b style="color:orange;"> InvalidMarkException</b>  to be thrown.
         *
         *
         * <h2> Invariants </h2>
         *
         * <p> The following invariant holds for the mark, position, limit, and
         * capacity values:
         *
         * <blockquote>
         *     <b> 0</b>  <b> <=</b> 
         *     <i>mark</i> <b> <=</b> 
         *     <i>position</i> <b> <=</b> 
         *     <i>limit</i> <b> <=</b> 
         *     <i>capacity</i>
         * </blockquote>
         *
         * <p> A newly-created buffer always has a position of zero and a mark that is
         * undefined.  The initial limit may be zero, or it may be some other value
         * that depends upon the type of the buffer and the manner in which it is
         * constructed.  Each element of a newly-allocated buffer is initialized
         * to zero.
         *
         *
         * <h2> Additional operations </h2>
         *
         * <p> In addition to methods for accessing the position, limit, and capacity
         * values and for marking and resetting, this class also defines the following
         * operations upon buffers:
         *
         * <ul>
         *
         *   <li><p> <b style="color:orange;"> clear</b>  makes a buffer ready for a new sequence of
         *   channel-read or relative <i>put</i> operations: It sets the limit to the
         *   capacity and the position to zero.  </p></li>
         *
         *   <li><p> <b style="color:orange;"> flip</b>  makes a buffer ready for a new sequence of
         *   channel-write or relative <i>get</i> operations: It sets the limit to the
         *   current position and then sets the position to zero.  </p></li>
         *
         *   <li><p> <b style="color:orange;"> rewind</b>  makes a buffer ready for re-reading the data that
         *   it already contains: It leaves the limit unchanged and sets the position
         *   to zero.  </p></li>
         *
         *   <li><p> The <b style="color:orange;"> slice</b>  and <b style="color:orange;"> slice(index,length)</b> 
         *   methods create a subsequence of a buffer: They leave the limit and the
         *   position unchanged. </p></li>
         *
         *   <li><p> <b style="color:orange;"> duplicate</b>  creates a shallow copy of a buffer: It leaves
         *   the limit and the position unchanged. </p></li>
         *
         * </ul>
         *
         *
         * <h2> Read-only buffers </h2>
         *
         * <p> Every buffer is readable, but not every buffer is writable.  The
         * mutation methods of each buffer class are specified as <i>optional
         * operations</i> that will throw a <b style="color:orange;"> ReadOnlyBufferException</b>  when
         * invoked upon a read-only buffer.  A read-only buffer does not allow its
         * content to be changed, but its mark, position, and limit values are mutable.
         * Whether or not a buffer is read-only may be determined by invoking its
         * <b style="color:orange;"> isReadOnly</b>  method.
         *
         *
         * <h2> Thread safety </h2>
         *
         * <p> Buffers are not safe for use by multiple concurrent threads.  If a
         * buffer is to be used by more than one thread then access to the buffer
         * should be controlled by appropriate synchronization.
         *
         *
         * <h2> Invocation chaining </h2>
         *
         * <p> Methods in this class that do not otherwise have a value to return are
         * specified to return the buffer upon which they are invoked.  This allows
         * method invocations to be chained; for example, the sequence of statements
         *
         * <blockquote><pre>
         * b.flip();
         * b.position(23);
         * b.limit(42);</pre></blockquote>
         *
         * can be replaced by the single, more compact statement
         *
         * <blockquote><pre>
         * b.flip().position(23).limit(42);</pre></blockquote>
         *
         *
         * @author Brunshweeck Tazeussong
         */
        class Buffer : public Object {
        private:
            // Invariants: mark <= position <= limit <= capacity
            gint markPos = -1;
            gint pos = 0;
            gint lim;
            gint cap;

        protected:

            // Used by heap byte buffers or direct buffers with Unsafe access
            // For heap byte buffers, this field will be the address relative to the
            // array base address and offset into that array. The address might
            // not align on a word boundary for slices, nor align at a long word
            // (8 byte) boundary for gbyte[] allocations on 32-bit systems.
            // For direct buffers, it is the start address of the memory region.
            glong address;


            // Creates a new buffer with given address and capacity.
            //
            CORE_EXPLICIT Buffer(glong addr, gint cap);

            // Creates a new buffer with the given mark, position, limit, and capacity,
            // after checking invariants.
            //
            CORE_EXPLICIT Buffer(gint mark, gint pos, gint lim, gint cap);

        public:

            /**
             * Returns this buffer's capacity.
             *
             * @return  The capacity of this buffer
             */
            gint capacity() const;

            /**
             * Returns this buffer's position.
             *
             * @return  The position of this buffer
             */
             gint position() const;

            /**
             * Sets this buffer's position.  If the mark is defined and larger than the
             * new position then it is discarded.
             *
             * @param  newPosition
             *         The new position value; must be non-negative
             *         and no larger than the current limit
             *
             * @return  This buffer
             *
             * @throws  IllegalArgumentException
             *          If the preconditions on <b> newPosition</b>  do not hold
             */
             virtual Buffer &setPosition(gint newPosition);

            /**
             * Returns this buffer's limit.
             *
             * @return  The limit of this buffer
             */
             gint limit() const;

            /**
             * Sets this buffer's limit.  If the position is larger than the new limit
             * then it is setValue to the new limit.  If the mark is defined and larger than
             * the new limit then it is discarded.
             *
             * @param  newLimit
             *         The new limit value; must be non-negative
             *         and no larger than this buffer's capacity
             *
             * @return  This buffer
             *
             * @throws  IllegalArgumentException
             *          If the preconditions on <b> newLimit</b>  do not hold
             */
             virtual Buffer &setLimit(gint newLimit);

            /**
             * Sets this buffer's mark at its position.
             *
             * @return  This buffer
             */
             virtual Buffer &mark();

            /**
             * Resets this buffer's position to the previously-marked position.
             *
             * <p> Invoking this method neither changes nor discards the mark's
             * value. </p>
             *
             * @return  This buffer
             *
             * @throws  InvalidMarkException
             *          If the mark has not been setValue
             */
             virtual Buffer &reset();

            /**
             * Clears this buffer.  The position is set to zero, the limit is setValue to
             * the capacity, and the mark is discarded.
             *
             * <p> Invoke this method before using a sequence of channel-read or
             * <i>put</i> operations to fill this buffer.  For example:
             *
             * <blockquote><pre>
             * buf.clear();     // Prepare buffer for reading
             * in.read(buf);    // Read data</pre></blockquote>
             *
             * <p> This method does not actually erase the data in the buffer, but it
             * is named as if it did because it will most often be used in situations
             * in which that might as well be the case. </p>
             *
             * @return  This buffer
             */
             virtual Buffer &clear();

            /**
             * Flips this buffer.  The limit is setValue to the current position and then
             * the position is setValue to zero.  If the mark is defined then it is
             * discarded.
             *
             * <p> After a sequence of channel-read or <i>put</i> operations, invoke
             * this method to prepare for a sequence of channel-write or relative
             * <i>get</i> operations.  For example:
             *
             * <blockquote><pre>
             * buf.put(magic);    // Prepend header
             * in.read(buf);      // Read data into rest of buffer
             * buf.flip();        // Flip buffer
             * out.write(buf);    // Write header + data to channel</pre></blockquote>
             *
             * <p> This method is often used in conjunction with the <b style="color:orange;">
             * java.nio.ByteBuffer#compact compact</b>  method when transferring data from
             * one place to another.  </p>
             *
             * @return  This buffer
             */
             virtual Buffer &flip();

            /**
             * Rewinds this buffer.  The position is setValue to zero and the mark is
             * discarded.
             *
             * <p> Invoke this method before a sequence of channel-write or <i>get</i>
             * operations, assuming that the limit has already been setValue
             * appropriately.  For example:
             *
             * <blockquote><pre>
             * out.write(buf);    // Write remaining data
             * buf.rewind();      // Rewind buffer
             * buf.get(array);    // Copy data into array</pre></blockquote>
             *
             * @return  This buffer
             */
             virtual Buffer &rewind();

            /**
             * Returns the number of elements between the current position and the
             * limit.
             *
             * @return  The number of elements remaining in this buffer
             */
             gint remaining() const;

            /**
             * Tells whether there are any elements between the current position and
             * the limit.
             *
             * @return  <b> true</b>  if, and only if, there is at least one element
             *          remaining in this buffer
             */
             gbool hasRemaining() const;

            /**
             * Tells whether or not this buffer is read-only.
             *
             * @return  <b> true</b>  if, and only if, this buffer is read-only
             */
             virtual gbool isReadOnly() const = 0;

            /**
             * Tells whether or not this buffer is backed by an accessible
             * array.
             *
             * <p> If this method returns <b> true</b>  then the <b style="color:orange;"> #array() array</b>
             * and <b style="color:orange;"> arrayOffset</b>  methods may safely be invoked.
             * </p>
             *
             * @return  <b> true</b>  if, and only if, this buffer
             *          is backed by an array and is not read-only
             */
             virtual gbool hasArray() const = 0;

            /**
             * Returns the array that backs this
             * buffer&nbsp;&nbsp;<i>(optional operation)</i>.
             *
             * <p> This method is intended to allow array-backed buffers to be
             * passed to native code more efficiently. Concrete subclasses
             * provide more strongly-typed return values for this method.
             *
             * <p> Modifications to this buffer's content will cause the returned
             * array's content to be modified, and vice versa.
             *
             * <p> Invoke the <b style="color:orange;"> #hasArray hasArray</b>  method before invoking this
             * method in order to ensure that this buffer has an accessible backing
             * array.  </p>
             *
             * @return  The array that backs this buffer
             *
             * @throws  ReadOnlyBufferException
             *          If this buffer is backed by an array but is read-only
             *
             * @throws  UnsupportedOperationException
             *          If this buffer is not backed by an accessible array
             */
             virtual Object &array() const = 0;

            /**
             * Returns the offset within this buffer's backing array of the first
             * element of the buffer&nbsp;&nbsp;<i>(optional operation)</i>.
             *
             * <p> If this buffer is backed by an array then buffer position <i>p</i>
             * corresponds to array index <i>p</i>&nbsp;+&nbsp;<b> arrayOffset()</b> .
             *
             * <p> Invoke the <b style="color:orange;"> #hasArray hasArray</b>  method before invoking this
             * method in order to ensure that this buffer has an accessible backing
             * array.  </p>
             *
             * @return  The offset within this buffer's array
             *          of the first element of the buffer
             *
             * @throws  ReadOnlyBufferException
             *          If this buffer is backed by an array but is read-only
             *
             * @throws  UnsupportedOperationException
             *          If this buffer is not backed by an accessible array
             */
             virtual gint arrayOffset() const = 0;

            /**
             * Tells whether or not this buffer is
             * <a href="ByteBuffer.html#direct"><i>direct</i></a>.
             *
             * @return  <b> true</b>  if, and only if, this buffer is direct
             */
             virtual gbool isDirect() const = 0;

            /**
             * Creates a new buffer whose content is a shared subsequence of
             * this buffer's content.
             *
             * <p> The content of the new buffer will start at this buffer's current
             * position.  Changes to this buffer's content will be visible in the new
             * buffer, and vice versa; the two buffers' position, limit, and mark
             * values will be independent.
             *
             * <p> The new buffer's position will be zero, its capacity and its limit
             * will be the number of elements remaining in this buffer, its mark will be
             * undefined. The new buffer will be direct if, and only if, this buffer is
             * direct, and it will be read-only if, and only if, this buffer is
             * read-only.  </p>
             *
             * @return  The new buffer
             */
             virtual Buffer& slice() const = 0;

            /**
             * Creates a new buffer whose content is a shared subsequence of
             * this buffer's content.
             *
             * <p> The content of the new buffer will start at position <b> index</b>
             * in this buffer, and will contain <b> length</b>  elements. Changes to
             * this buffer's content will be visible in the new buffer, and vice versa;
             * the two buffers' position, limit, and mark values will be independent.
             *
             * <p> The new buffer's position will be zero, its capacity and its limit
             * will be <b> length</b> , its mark will be undefined. The new buffer will
             * be direct if, and only if, this buffer is direct, and it will be
             * read-only if, and only if, this buffer is read-only.  </p>
             *
             * @param   index
             *          The position in this buffer at which the content of the new
             *          buffer will start; must be non-negative and no larger than
             *          <b style="color:orange;"> #limit() limit()</b>
             *
             * @param   length
             *          The number of elements the new buffer will contain; must be
             *          non-negative and no larger than <b> limit() - index</b>
             *
             * @return  The new buffer
             *
             * @throws  IndexException
             *          If <b> index</b>  is negative or greater than <b> limit()</b> ,
             *          <b> length</b>  is negative, or <b> length > limit() - index</b>
             */
             virtual Buffer &slice(gint index, gint length) const = 0;

            /**
             * Creates a new buffer that shares this buffer's content.
             *
             * <p> The content of the new buffer will be that of this buffer.  Changes
             * to this buffer's content will be visible in the new buffer, and vice
             * versa; the two buffers' position, limit, and mark values will be
             * independent.
             *
             * <p> The new buffer's capacity, limit, position and mark values will be
             * identical to those of this buffer. The new buffer will be direct if, and
             * only if, this buffer is direct, and it will be read-only if, and only if,
             * this buffer is read-only.  </p>
             *
             * @return  The new buffer
             */
             virtual Buffer &duplicate() const = 0;

        protected:
            /**
             *
             * @return the base reference, paired with the address
             * field, which in combination can be used for unsafe access into a heap
             * buffer or direct byte buffer (and views of).
             */
             virtual Object& base() const = 0;

            /**
             * Checks the current position against the limit, throwing a <b style="color:orange;">
             * BufferUnderflowException</b>  if it is not smaller than the limit, and then
             * increments the position.
             *
             * @return  The current position value, before it is incremented
             */
            gint nextGetIndex();

            gint nextGetIndex(gint nb);

            /**
             * Checks the current position against the limit, throwing a <b style="color:orange;">
             * BufferOverflowException</b>  if it is not smaller than the limit, and then
             * increments the position.
             *
             * @return  The current position value, before it is incremented
             */
            gint nextPutIndex();

            gint nextPutIndex(gint nb);

            /**
             * Checks the given index against the limit, throwing an <b style="color:orange;">
             * IndexException</b>  if it is not smaller than the limit
             * or is smaller than zero.
             */
            gint checkIndex(gint i) const;

            gint checkIndex(gint i, gint nb) const;

            gint markValue() const;

            void discardMark();

        };

    } // io
} // core

#endif //CORE23_BUFFER_H

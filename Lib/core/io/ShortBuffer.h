//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_SHORTBUFFER_H
#define CORE23_SHORTBUFFER_H


#include <core/Comparable.h>
#include <core/native/ShortArray.h>
#include <core/io/Buffer.h>
#include <core/charset/Charset.h>

namespace core {
    namespace io {

        /**
         * A short buffer.
         *
         * <p> This class defines four categories of operations upon
         * gshort buffers:
         *
         * <ul>
         *
         *   <li><p> Absolute and relative <b style="color:orange;"> <i>get</i></b>  and
         *   <b style="color:orange;"> <i>put</i></b>  methods that read and write
         *   single shorts; </p></li>
         *
         *   <li><p> Absolute and relative <b style="color:orange;"> <i>bulk get</i></b>
         *   methods that transfer contiguous sequences of shorts from this buffer
         *   into an array;</p></li>
         *
         *   <li><p> Absolute and relative <b style="color:orange;"> <i>bulk put</i></b>
         *   methods that transfer contiguous sequences of shorts from a
         *   gshort array or some other gshort
         *   buffer into this buffer;</p></li>
         *
         *
         *   <li><p> A method for <b style="color:orange;"> compacting</b>
         *   a gshort buffer.  </p></li>
         *
         * </ul>
         *
         * <p> Short buffers can be created either by <b style="color:orange;">
         * <i>allocation</i></b> , which allocates space for the buffer's
         *
         * content, by <b style="color:orange;"> <i>wrapping</i></b>  an existing
         * gshort array  into a buffer, or by creating a
         * <a href=""><i>view</i></a> of an existing byte buffer.
         *
         * <p> Like a byte buffer, a gshort buffer is either <a
         * href=""><i>direct</i> or <i>non-direct</i></a>.  A
         * gshort buffer created via the <b> wrap</b>  methods of this class will
         * be non-direct.  A gshort buffer created as a view of a byte buffer will
         * be direct if, and only if, the byte buffer itself is direct.  Whether or not
         * a gshort buffer is direct may be determined by invoking the <b style="color:orange;">
         * isDirect</b>  method.  </p>
         *
         * <p> Methods in this class that do not otherwise have a value to return are
         * specified to return the buffer upon which they are invoked.  This allows
         * method invocations to be chained.
         *
         * @author Brunshweeck Tazeussong
         */

        class ShortBuffer : public Buffer, public Comparable<ShortBuffer> {
        private:
            static const glong ARRAY_BASE_OFFSET;

        protected:
            CORE_ALIAS(HeapShorts, typename Class<ShortArray>::Ptr);
            // These fields are declared here rather than in Heap-X-Buffer to
            // reduce the number of virtual method invocations needed to access these
            // values, which is especially costly when coding small buffers.
            //
            HeapShorts hb;
            gint offset;
            gbool isReadOnly;

            // Creates a new buffer with the given mark, position, limit, capacity,
            // backing array, and array offset
            //
            CORE_EXPLICIT ShortBuffer(gint mark, gint pos, gint lim, gint cap, ShortArray & hb, gint offset);

            // Creates a new buffer with the given mark, position, limit, and capacity
            //
            CORE_EXPLICIT ShortBuffer(gint mark, gint pos, gint lim, gint cap);

            // Creates a new buffer with given base, address and capacity
            //
            CORE_EXPLICIT ShortBuffer(ShortArray & hb, glong addr, gint cap);

            /**
             * @override
             */
            Object &base() const override;

        public:

            /**
             * Allocates a new gshort buffer.
             *
             * <p> The new buffer's position will be zero, its limit will be its
             * capacity, its mark will be undefined, each of its elements will be
             * initialized to zero, and its byte order will be
             *
             * the <b style="color:orange;"> native order</b>  of the underlying
             * hardware.

             * It will have a <b style="color:orange;"> backing array</b> , and its
             * <b style="color:orange;"> array offset</b>  will be zero.
             *
             * @param  capacity
             *         The new buffer's capacity, in shorts
             *
             * @return  The new gshort buffer
             *
             * @throws  IllegalArgumentException
             *          If the <b> capacity</b>  is a negative integer
             */
            static ShortBuffer &allocate(gint capacity);

            /**
             * Wraps a gshort array into a buffer.
             *
             * <p> The new buffer will be backed by the given gshort array;
             * that is, modifications to the buffer will cause the array to be modified
             * and vice versa.  The new buffer's capacity will be
             * <b> array.length</b> , its position will be <b> offset</b> , its limit
             * will be <b> offset + length</b> , its mark will be undefined, and its
             * byte order will be



             * the <b style="color:orange;"> native order</b>  of the underlying
             * hardware.

             * Its <b style="color:orange;"> backing array</b>  will be the given array, and
             * its <b style="color:orange;"> array offset</b>  will be zero.  </p>
             *
             * @param  array
             *         The array that will back the new buffer
             *
             * @param  offset
             *         The offset of the subarray to be used; must be non-negative and
             *         no larger than <b> array.length</b> .  The new buffer's position
             *         will be set to this value.
             *
             * @param  length
             *         The length of the subarray to be used;
             *         must be non-negative and no larger than
             *         <b> array.length - offset</b> .
             *         The new buffer's limit will be set to <b> offset + length</b> .
             *
             * @return  The new gshort buffer
             *
             * @throws  IndexException
             *          If the preconditions on the <b> offset</b>  and <b> length</b>
             *          parameters do not hold
             */
            static ShortBuffer &wrap(ShortArray &array, gint offset, gint length);

            /**
             * Wraps a gshort array into a buffer.
             *
             * <p> The new buffer will be backed by the given gshort array;
             * that is, modifications to the buffer will cause the array to be modified
             * and vice versa.  The new buffer's capacity and limit will be
             * <b> array.length</b> , its position will be zero, its mark will be
             * undefined, and its byte order will be
             *
             * the <b style="color:orange;"> native order</b>  of the underlying
             * hardware.

             * Its <b style="color:orange;"> backing array</b>  will be the given array, and its
             * <b style="color:orange;"> array offset</b>  will be zero.  </p>
             *
             * @param  array
             *         The array that will back this buffer
             *
             * @return  The new gshort buffer
             */
            static ShortBuffer &wrap(ShortArray &array);

            /**
             * Creates a new gshort buffer whose content is a shared subsequence of
             * this buffer's content.
             *
             * <p> The content of the new buffer will start at this buffer's current
             * position.  Changes to this buffer's content will be visible in the new
             * buffer, and vice versa; the two buffers' position, limit, and mark
             * values will be independent.
             *
             * <p> The new buffer's position will be zero, its capacity and its limit
             * will be the number of shorts remaining in this buffer, its mark will be
             * undefined, and its byte order will be



             * identical to that of this buffer.

             * The new buffer will be direct if, and only if, this buffer is direct, and
             * it will be read-only if, and only if, this buffer is read-only.  </p>
             *
             * @return  The new gshort buffer
             */
            ShortBuffer &slice() const override = 0;

            /**
             * Creates a new gshort buffer whose content is a shared subsequence of
             * this buffer's content.
             *
             * <p> The content of the new buffer will start at position <b> index</b>
             * in this buffer, and will contain <b> length</b>  elements. Changes to
             * this buffer's content will be visible in the new buffer, and vice versa;
             * the two buffers' position, limit, and mark values will be independent.
             *
             * <p> The new buffer's position will be zero, its capacity and its limit
             * will be <b> length</b> , its mark will be undefined, and its byte order
             * will be
             * identical to that of this buffer.
             * The new buffer will be direct if, and only if, this buffer is direct,
             * and it will be read-only if, and only if, this buffer is read-only. </p>
             *
             * @param   index
             *          The position in this buffer at which the content of the new
             *          buffer will start; must be non-negative and no larger than
             *          <b style="color:orange;"> limit()</b>
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
            ShortBuffer &slice(gint index, gint length) const override = 0;

            /**
             * Creates a new gshort buffer that shares this buffer's content.
             *
             * <p> The content of the new buffer will be that of this buffer.  Changes
             * to this buffer's content will be visible in the new buffer, and vice
             * versa; the two buffers' position, limit, and mark values will be
             * independent.
             *
             * <p> The new buffer's capacity, limit, position,
             * mark values, and byte order will be identical to those of this buffer.
             * The new buffer will be direct if, and only if, this buffer is direct, and
             * it will be read-only if, and only if, this buffer is read-only.  </p>
             *
             * @return  The new gshort buffer
             */
            ShortBuffer &duplicate() const override = 0;

            /**
             * Creates a new, read-only gshort buffer that shares this buffer's
             * content.
             *
             * <p> The content of the new buffer will be that of this buffer.  Changes
             * to this buffer's content will be visible in the new buffer; the new
             * buffer itself, however, will be read-only and will not allow the shared
             * content to be modified.  The two buffers' position, limit, and mark
             * values will be independent.
             *
             * <p> The new buffer's capacity, limit, position,
             * mark values, and byte order will be identical to those of this buffer.
             *
             * <p> If this buffer is itself read-only then this method behaves in
             * exactly the same way as the <b style="color:orange;"> duplicate</b>  method.  </p>
             *
             * @return  The new, read-only gshort buffer
             */
            virtual ShortBuffer &asReadOnlyBuffer() const = 0;

            /**
             * Relative <i>get</i> method.  Reads the gshort at this buffer's
             * current position, and then increments the position.
             *
             * @return  The gshort at the buffer's current position
             *
             * @throws  BufferUnderflowException
             *          If the buffer's current position is not smaller than its limit
             */
            virtual gshort get() = 0;

            /**
             * Relative <i>put</i> method&nbsp;&nbsp;<i>(optional operation)</i>.
             *
             * <p> Writes the given gshort into this buffer at the current
             * position, and then increments the position. </p>
             *
             * @param  i
             *         The gshort to be written
             *
             * @return  This buffer
             *
             * @throws  BufferOverflowException
             *          If this buffer's current position is not smaller than its limit
             *
             * @throws  ReadOnlyBufferException
             *          If this buffer is read-only
             */
            virtual ShortBuffer &put(gshort i) = 0;

            /**
             * Absolute <i>get</i> method.  Reads the gshort at the given
             * index.
             *
             * @param  index
             *         The index from which the gshort will be read
             *
             * @return  The gshort at the given index
             *
             * @throws  IndexException
             *          If <b> index</b>  is negative
             *          or not smaller than the buffer's limit
             */
            virtual gshort get(gint index) const = 0;


            /**
             * Absolute <i>put</i> method&nbsp;&nbsp;<i>(optional operation)</i>.
             *
             * <p> Writes the given gshort into this buffer at the given
             * index. </p>
             *
             * @param  index
             *         The index at which the gshort will be written
             *
             * @param  i
             *         The gshort value to be written
             *
             * @return  This buffer
             *
             * @throws  IndexException
             *          If <b> index</b>  is negative
             *          or not smaller than the buffer's limit
             *
             * @throws  ReadOnlyBufferException
             *          If this buffer is read-only
             */
            virtual ShortBuffer &put(gint index, gshort i) = 0;

            /**
             * Relative bulk <i>get</i> method.
             *
             * <p> This method transfers shorts from this buffer into the given
             * destination array.  If there are fewer shorts remaining in the
             * buffer than are required to satisfy the request, that is, if
             * <b> length</b> &nbsp;<b> ></b> &nbsp;<b> remaining()</b> , then no
             * shorts are transferred and a <b style="color:orange;"> BufferUnderflowException</b>  is
             * thrown.
             *
             * <p> Otherwise, this method copies <b> length</b>  shorts from this
             * buffer into the given array, starting at the current position of this
             * buffer and at the given off in the array.  The position of this
             * buffer is then incremented by <b> length</b> .
             *
             * <p> In other words, an invocation of this method of the form
             * <code>src.get(dst,&nbsp;off,&nbsp;len)</code> has exactly the same effect as
             * the loop
             *
             * <pre><b>
             *     for (gint i = off; i < off + len; i++)
             *         dst[i] = src.get();
             * </b> </pre>
             *
             * except that it first checks that there are sufficient shorts in
             * this buffer and it is potentially much more efficient.
             *
             * @param  dst
             *         The array into which shorts are to be written
             *
             * @param  offset
             *         The offset within the array of the first gshort to be
             *         written; must be non-negative and no larger than
             *         <b> dst.length</b>
             *
             * @param  length
             *         The maximum number of shorts to be written to the given
             *         array; must be non-negative and no larger than
             *         <b> dst.length - off</b>
             *
             * @return  This buffer
             *
             * @throws  BufferUnderflowException
             *          If there are fewer than <b> length</b>  shorts
             *          remaining in this buffer
             *
             * @throws  IndexException
             *          If the preconditions on the <b> off</b>  and <b> length</b>
             *          parameters do not hold
             */
            virtual ShortBuffer &get(ShortArray &dst, gint offset, gint length);

            /**
             * Relative bulk <i>get</i> method.
             *
             * <p> This method transfers shorts from this buffer into the given
             * destination array.  An invocation of this method of the form
             * <b> src.get(a)</b>  behaves in exactly the same way as the invocation
             *
             * <pre>
             *     src.get(a, 0, a.length) </pre>
             *
             * @param   dst
             *          The destination array
             *
             * @return  This buffer
             *
             * @throws  BufferUnderflowException
             *          If there are fewer than <b> length</b>  shorts
             *          remaining in this buffer
             */
            virtual ShortBuffer &get(ShortArray &dst);

            /**
             * Absolute bulk <i>get</i> method.
             *
             * <p> This method transfers <b> length</b>  shorts from this
             * buffer into the given array, starting at the given index in this
             * buffer and at the given offset in the array.  The position of this
             * buffer is unchanged.
             *
             * <p> An invocation of this method of the form
             * <code>src.get(index,&nbsp;dst,&nbsp;offset,&nbsp;length)</code>
             * has exactly the same effect as the following loop except that it first
             * checks the consistency of the supplied parameters and it is potentially
             * much more efficient:
             *
             * <pre><b>
             *     for (gint i = offset, j = index; i < offset + length; i++, j++)
             *         dst[i] = src.get(j);
             * </b> </pre>
             *
             * @param  index
             *         The index in this buffer from which the first gshort will be
             *         read; must be non-negative and less than <b> limit()</b>
             *
             * @param  dst
             *         The destination array
             *
             * @param  offset
             *         The offset within the array of the first gshort to be
             *         written; must be non-negative and less than
             *         <b> dst.length</b>
             *
             * @param  length
             *         The number of shorts to be written to the given array;
             *         must be non-negative and no larger than the smaller of
             *         <b> limit() - index</b>  and <b> dst.length - offset</b>
             *
             * @return  This buffer
             *
             * @throws  IndexException
             *          If the preconditions on the <b> index</b> , <b> offset</b> , and
             *          <b> length</b>  parameters do not hold
             */
            virtual ShortBuffer &get(gint index, ShortArray &dst, gint offset, gint length) const;

            /**
             * Absolute bulk <i>get</i> method.
             *
             * <p> This method transfers shorts from this buffer into the given
             * destination array.  The position of this buffer is unchanged.  An
             * invocation of this method of the form
             * <code>src.get(index,&nbsp;dst)</code> behaves in exactly the same
             * way as the invocation:
             *
             * <pre>
             *     src.get(index, dst, 0, dst.length) </pre>
             *
             * @param  index
             *         The index in this buffer from which the first gshort will be
             *         read; must be non-negative and less than <b> limit()</b>
             *
             * @param  dst
             *         The destination array
             *
             * @return  This buffer
             *
             * @throws  IndexException
             *          If <b> index</b>  is negative, not smaller than <b> limit()</b> ,
             *          or <b> limit() - index < dst.length</b>
             */
            virtual ShortBuffer &get(gint index, ShortArray &dst) const;

        private:
            ShortBuffer &getArray(gint index, ShortArray &dst, gint offset, gint length) const;

        public:

            /**
             * Relative bulk <i>put</i> method&nbsp;&nbsp;<i>(optional operation)</i>.
             *
             * <p> This method transfers the shorts remaining in the given source
             * buffer into this buffer.  If there are more shorts remaining in the
             * source buffer than in this buffer, that is, if
             * <b> src.remaining()</b> &nbsp;<b> ></b> &nbsp;<b> remaining()</b> ,
             * then no shorts are transferred and a <b style="color:orange;">
             * BufferOverflowException</b>  is thrown.
             *
             * <p> Otherwise, this method copies
             * <i>n</i>&nbsp;=&nbsp;<b> src.remaining()</b>  shorts from the given
             * buffer into this buffer, starting at each buffer's current position.
             * The positions of both buffers are then incremented by <i>n</i>.
             *
             * <p> In other words, an invocation of this method of the form
             * <b> dst.put(src)</b>  has exactly the same effect as the loop
             *
             * <pre>
             *     while (src.hasRemaining())
             *         dst.put(src.get()); </pre>
             *
             * except that it first checks that there is sufficient space in this
             * buffer and it is potentially much more efficient.  If this buffer and
             * the source buffer share the same backing array or memory, then the
             * result will be as if the source elements were first copied to an
             * intermediate location before being written into this buffer.
             *
             * @param  src
             *         The source buffer from which shorts are to be read;
             *         must not be this buffer
             *
             * @return  This buffer
             *
             * @throws  BufferOverflowException
             *          If there is insufficient space in this buffer
             *          for the remaining shorts in the source buffer
             *
             * @throws  IllegalArgumentException
             *          If the source buffer is this buffer
             *
             * @throws  ReadOnlyBufferException
             *          If this buffer is read-only
             */
            virtual ShortBuffer &put(ShortBuffer &src);

            /**
             * Absolute bulk <i>put</i> method&nbsp;&nbsp;<i>(optional operation)</i>.
             *
             * <p> This method transfers <b> length</b>  shorts into this buffer from
             * the given source buffer, starting at the given <b> offset</b>  in the
             * source buffer and the given <b> index</b>  in this buffer. The positions
             * of both buffers are unchanged.
             *
             * <p> In other words, an invocation of this method of the form
             * <code>dst.put(index,&nbsp;src,&nbsp;offset,&nbsp;length)</code>
             * has exactly the same effect as the loop
             *
             * <pre><b>
             * for (gint i = offset, j = index; i < offset + length; i++, j++)
             *     dst.put(j, src.get(i));
             * </b> </pre>
             *
             * except that it first checks the consistency of the supplied parameters
             * and it is potentially much more efficient.  If this buffer and
             * the source buffer share the same backing array or memory, then the
             * result will be as if the source elements were first copied to an
             * intermediate location before being written into this buffer.
             *
             * @param index
             *        The index in this buffer at which the first gshort will be
             *        written; must be non-negative and less than <b> limit()</b>
             *
             * @param src
             *        The buffer from which shorts are to be read
             *
             * @param offset
             *        The index within the source buffer of the first gshort to be
             *        read; must be non-negative and less than <b> src.limit()</b>
             *
             * @param length
             *        The number of shorts to be read from the given buffer;
             *        must be non-negative and no larger than the smaller of
             *        <b> limit() - index</b>  and <b> src.limit() - offset</b>
             *
             * @return This buffer
             *
             * @throws IndexException
             *         If the preconditions on the <b> index</b> , <b> offset</b> , and
             *         <b> length</b>  parameters do not hold
             *
             * @throws ReadOnlyBufferException
             *         If this buffer is read-only
             */
            virtual ShortBuffer &put(gint index, ShortBuffer &src, gint offset, gint length);

        private:
            void putBuffer(gint pos, ShortBuffer &src, gint srcPos, gint n);

        public:
            /**
             * Relative bulk <i>put</i> method&nbsp;&nbsp;<i>(optional operation)</i>.
             *
             * <p> This method transfers shorts into this buffer from the given
             * source array.  If there are more shorts to be copied from the array
             * than remain in this buffer, that is, if
             * <b> length</b> &nbsp;<b> ></b> &nbsp;<b> remaining()</b> , then no
             * shorts are transferred and a <b style="color:orange;"> BufferOverflowException</b>  is
             * thrown.
             *
             * <p> Otherwise, this method copies <b> length</b>  shorts from the
             * given array into this buffer, starting at the given offset in the array
             * and at the current position of this buffer.  The position of this buffer
             * is then incremented by <b> length</b> .
             *
             * <p> In other words, an invocation of this method of the form
             * <code>dst.put(src,&nbsp;off,&nbsp;len)</code> has exactly the same effect as
             * the loop
             *
             * <pre><b>
             *     for (gint i = off; i < off + len; i++)
             *         dst.put(src[i]);
             * </b> </pre>
             *
             * except that it first checks that there is sufficient space in this
             * buffer and it is potentially much more efficient.
             *
             * @param  src
             *         The array from which shorts are to be read
             *
             * @param  offset
             *         The offset within the array of the first gshort to be read;
             *         must be non-negative and no larger than <b> src.length</b>
             *
             * @param  length
             *         The number of shorts to be read from the given array;
             *         must be non-negative and no larger than
             *         <b> src.length - offset</b>
             *
             * @return  This buffer
             *
             * @throws  BufferOverflowException
             *          If there is insufficient space in this buffer
             *
             * @throws  IndexException
             *          If the preconditions on the <b> offset</b>  and <b> length</b>
             *          parameters do not hold
             *
             * @throws  ReadOnlyBufferException
             *          If this buffer is read-only
             */
            virtual ShortBuffer &put(const ShortArray &src, gint offset, gint length);

            /**
             * Relative bulk <i>put</i> method&nbsp;&nbsp;<i>(optional operation)</i>.
             *
             * <p> This method transfers the entire content of the given source
             * gshort array into this buffer.  An invocation of this method of the
             * form <b> dst.put(a)</b>  behaves in exactly the same way as the
             * invocation
             *
             * <pre>
             *     dst.put(a, 0, a.length) </pre>
             *
             * @param   src
             *          The source array
             *
             * @return  This buffer
             *
             * @throws  BufferOverflowException
             *          If there is insufficient space in this buffer
             *
             * @throws  ReadOnlyBufferException
             *          If this buffer is read-only
             */
            virtual ShortBuffer &put(const ShortArray &src);

            /**
             * Absolute bulk <i>put</i> method&nbsp;&nbsp;<i>(optional operation)</i>.
             *
             * <p> This method transfers <b> length</b>  shorts from the given
             * array, starting at the given offset in the array and at the given index
             * in this buffer.  The position of this buffer is unchanged.
             *
             * <p> An invocation of this method of the form
             * <code>dst.put(index,&nbsp;src,&nbsp;offset,&nbsp;length)</code>
             * has exactly the same effect as the following loop except that it first
             * checks the consistency of the supplied parameters and it is potentially
             * much more efficient:
             *
             * <pre><b>
             *     for (gint i = offset, j = index; i < offset + length; i++, j++)
             *         dst.put(j, src[i]);
             * </b> </pre>
             *
             * @param  index
             *         The index in this buffer at which the first gshort will be
             *         written; must be non-negative and less than <b> limit()</b>
             *
             * @param  src
             *         The array from which shorts are to be read
             *
             * @param  offset
             *         The offset within the array of the first gshort to be read;
             *         must be non-negative and less than <b> src.length</b>
             *
             * @param  length
             *         The number of shorts to be read from the given array;
             *         must be non-negative and no larger than the smaller of
             *         <b> limit() - index</b>  and <b> src.length - offset</b>
             *
             * @return  This buffer
             *
             * @throws  IndexException
             *          If the preconditions on the <b> index</b> , <b> offset</b> , and
             *          <b> length</b>  parameters do not hold
             *
             * @throws  ReadOnlyBufferException
             *          If this buffer is read-only
             */
            virtual ShortBuffer &put(gint index, const ShortArray &src, gint offset, gint length);

            /**
             * Absolute bulk <i>put</i> method&nbsp;&nbsp;<i>(optional operation)</i>.
             *
             * <p> This method copies shorts into this buffer from the given source
             * array.  The position of this buffer is unchanged.  An invocation of this
             * method of the form <code>dst.put(index,&nbsp;src)</code>
             * behaves in exactly the same way as the invocation:
             *
             * <pre>
             *     dst.put(index, src, 0, src.length); </pre>
             *
             * @param  index
             *         The index in this buffer at which the first gshort will be
             *         written; must be non-negative and less than <b> limit()</b>
             *
             * @param  src
             *         The array from which shorts are to be read
             *
             * @return  This buffer
             *
             * @throws  IndexException
             *          If <b> index</b>  is negative, not smaller than <b> limit()</b> ,
             *          or <b> limit() - index < src.length</b>
             *
             * @throws  ReadOnlyBufferException
             *          If this buffer is read-only
             */
            virtual ShortBuffer &put(gint index, const ShortArray &src);

        private:
            ShortBuffer &putArray(gint index, const ShortArray &src, gint offset, gint length);

        public:

            /**
             * Tells whether or not this buffer is backed by an accessible gshort
             * array.
             *
             * <p> If this method returns <b> true</b>  then the <b style="color:orange;"> array</b>
             * and <b style="color:orange;"> arrayOffset</b>  methods may safely be invoked.
             * </p>
             *
             * @return  <b> true</b>  if, and only if, this buffer
             *          is backed by an array and is not read-only
             */
            gbool hasArray() const override;

            /**
             * Returns the gshort array that backs this
             * buffer&nbsp;&nbsp;<i>(optional operation)</i>.
             *
             * <p> Modifications to this buffer's content will cause the returned
             * array's content to be modified, and vice versa.
             *
             * <p> Invoke the <b style="color:orange;"> hasArray</b>  method before invoking this
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
            ShortArray &array() const override;

            /**
             * Returns the offset within this buffer's backing array of the first
             * element of the buffer&nbsp;&nbsp;<i>(optional operation)</i>.
             *
             * <p> If this buffer is backed by an array then buffer position <i>p</i>
             * corresponds to array index <i>p</i>&nbsp;+&nbsp;<b> arrayOffset()</b> .
             *
             * <p> Invoke the <b style="color:orange;"> hasArray</b>  method before invoking this
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
            gint arrayOffset() const override;

            /**
             * {@inheritDoc</b>
             */
            ShortBuffer &setPosition(gint newPosition) override;

            /**
             * {@inheritDoc</b>
             */
            ShortBuffer &setLimit(gint newLimit) override;

            /**
             * {@inheritDoc</b>
             */
            ShortBuffer &mark() override;

            /**
             * {@inheritDoc</b>
             */
            ShortBuffer &reset() override;

            /**
             * {@inheritDoc</b>
             */
            ShortBuffer &clear() override;

            /**
             * {@inheritDoc</b>
             */
            ShortBuffer &flip() override;

            /**
             * {@inheritDoc</b>
             */
            ShortBuffer &rewind() override;

            /**
             * Compacts this buffer&nbsp;&nbsp;<i>(optional operation)</i>.
             *
             * <p> The shorts between the buffer's current position and its limit,
             * if any, are copied to the beginning of the buffer.  That is, the
             * gshort at index <i>p</i>&nbsp;=&nbsp;<b> position()</b>  is copied
             * to index zero, the gshort at index <i>p</i>&nbsp;+&nbsp;1 is copied
             * to index one, and so forth until the gshort at index
             * <b> limit()</b> &nbsp;-&nbsp;1 is copied to index
             * <i>n</i>&nbsp;=&nbsp;<b> limit()</b> &nbsp;-&nbsp;<b> 1</b> &nbsp;-&nbsp;<i>p</i>.
             * The buffer's position is then set to <i>n+1</i> and its limit is set to
             * its capacity.  The mark, if defined, is discarded.
             *
             * <p> The buffer's position is set to the number of shorts copied,
             * rather than to zero, so that an invocation of this method can be
             * followed immediately by an invocation of another relative <i>put</i>
             * method. </p>
             *
             *
             * @return  This buffer
             *
             * @throws  ReadOnlyBufferException
             *          If this buffer is read-only
             */
            virtual ShortBuffer &compact() = 0;

            /**
             * Tells whether or not this gshort buffer is direct.
             *
             * @return  <b> true</b>  if, and only if, this buffer is direct
             */
            gbool isDirect() const override = 0;

            /**
             * Returns a string summarizing the state of this buffer.
             *
             * @return  A summary string
             */
            String toString() const override;

            /**
             * Returns the current hash code of this buffer.
             *
             * <p> The hash code of a gshort buffer depends only upon its remaining
             * elements; that is, upon the elements from <b> position()</b>  up to, and
             * including, the element at <b> limit()</b> &nbsp;-&nbsp;<b> 1</b> .
             *
             * <p> Because buffer hash codes are content-dependent, it is inadvisable
             * to use buffers as keys in hash maps or similar data structures unless it
             * is known that their contents will not change.  </p>
             *
             * @return  The current hash code of this buffer
             */
            gint hash() const override;

            /**
             * Tells whether or not this buffer is equal to another object.
             *
             * <p> Two gshort buffers are equal if, and only if,
             *
             * <ol>
             *
             *   <li><p> They have the same element type,  </p></li>
             *
             *   <li><p> They have the same number of remaining elements, and
             *   </p></li>
             *
             *   <li><p> The two sequences of remaining elements, considered
             *   independently of their starting positions, are pointwise equal.

             *   </p></li>
             *
             * </ol>
             *
             * <p> A gshort buffer is not equal to any other type of object.  </p>
             *
             * @param  obj  The object to which this buffer is to be compared
             *
             * @return  <b> true</b>  if, and only if, this buffer is equal to the
             *           given object
             */
            gbool equals(const Object &obj) const override;

        protected:
            CORE_ALIAS(ByteOrder, charset::Charset::ByteOrder);

        private:
            static int mismatch(const ShortBuffer &a, gint aOff, const ShortBuffer &b, gint bOff, gint length);

        public:

            /**
             * Compares this buffer to another.
             *
             * <p> Two gshort buffers are compared by comparing their sequences of
             * remaining elements lexicographically, without regard to the starting
             * position of each sequence within its corresponding buffer.

             * Pairs of <b> gshort</b>  elements are compared as if by invoking
             * <b style="color:orange;"> Short.compare(gshort,gshort)</b>.
             *
             * <p> A gshort buffer is not comparable to any other type of object.
             *
             * @return  A negative integer, zero, or a positive integer as this buffer
             *          is less than, equal to, or greater than the given buffer
             */
            gint compareTo(const ShortBuffer &that) const override;

            /**
             * Finds and returns the relative index of the first mismatch between this
             * buffer and a given buffer.  The index is relative to the
             * <b style="color:orange;"> position</b>  of each buffer and will be in the range of
             * 0 (inclusive) up to the smaller of the <b style="color:orange;"> remaining</b>
             * elements in each buffer (exclusive).
             *
             * <p> If the two buffers share a common prefix then the returned index is
             * the length of the common prefix and it follows that there is a mismatch
             * between the two buffers at that index within the respective buffers.
             * If one buffer is a proper prefix of the other then the returned index is
             * the smaller of the remaining elements in each buffer, and it follows that
             * the index is only valid for the buffer with the larger number of
             * remaining elements.
             * Otherwise, there is no mismatch.
             *
             * @param  that
             *         The byte buffer to be tested for a mismatch with this buffer
             *
             * @return  The relative index of the first mismatch between this and the
             *          given buffer, otherwise -1 if no mismatch.
             */
            gint mismatch(const ShortBuffer &that) const;

            /**
             * Retrieves this buffer's byte order.
             *
             * <p> The byte order of a gshort buffer created by allocation or by
             * wrapping an existing <b> gshort</b>  array is the <b style="color:orange;">
             * native order</b>  of the underlying
             * hardware.  The byte order of a gshort buffer created as a <a
             * href="">view</a> of a byte buffer is that of the
             * byte buffer at the moment that the view is created.  </p>
             *
             * @return  This buffer's byte order
             */
            virtual ByteOrder order() const = 0;
        };

    } // core
} // io

#endif //CORE23_SHORTBUFFER_H

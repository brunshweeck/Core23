//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_BYTEBUFFER_H
#define CORE23_BYTEBUFFER_H

#include <core/Comparable.h>
#include <core/native/ByteArray.h>
#include <core/io/Buffer.h>
#include <core/charset/Charset.h>

namespace core {
    namespace io {

        /**
         * A byte buffer.
         *
         * <p> This class defines six categories of operations upon
         * byte buffers:
         *
         * <ul>
         *
         *   <li><p> Absolute and relative <b style="color: orange;"> <i>get</i></b> and
         *   <b style="color: orange;"> <i>put</i></b> methods that read and write
         *   single bytes; </p></li>
         *
         *   <li><p> Absolute and relative <b style="color: orange;"> <i>bulk get</i></b>
         *   methods that transfer contiguous sequences of bytes from this buffer
         *   into an array;</p></li>
         *
         *   <li><p> Absolute and relative <b style="color: orange;"> <i>bulk put</i></b>
         *   methods that transfer contiguous sequences of bytes from a
         *   byte array or some other byte
         *   buffer into this buffer;</p></li>
         *
         *
         *   <li><p> Absolute and relative <b style="color: orange;"> <i>get</i></b>
         *   and <b style="color: orange;"> <i>put</i></b> methods that read and
         *   write values of other primitive types, translating them to and from
         *   sequences of bytes in a particular byte order; </p></li>
         *
         *   <li><p> Methods for creating <i><a href="">view buffers</a></i>,
         *   which allow a byte buffer to be viewed as a buffer containing values of
         *   some other primitive type; and </p></li>
         *

         *
         *   <li><p> A method for <b style="color: orange;"> compacting</b>
         *   a byte buffer.  </p></li>
         *
         * </ul>
         *
         * <p> Byte buffers can be created either by <b style="color: orange;">
         * <i>allocation</i></b>, which allocates space for the buffer's
         *

         *
         * content, or by <b style="color: orange;"> <i>wrapping</i></b> an
         * existing byte array into a buffer.
         *
         *
         *
         * <a id="direct"></a>
         * <h2> Direct <i>vs.</i> non-direct buffers </h2>
         *
         * <p> A byte buffer is either <i>direct</i> or <i>non-direct</i>.  Given a
         * direct byte buffer, the machine will make a best effort to
         * perform native I/O operations directly upon it.  That is, it will attempt to
         * avoid copying the buffer's content to (or from) an intermediate buffer
         * before (or after) each invocation of one of the underlying operating
         * system's native I/O operations.
         *
         * <p> A direct byte buffer may be created by invoking the <b style="color: orange;"> 
         * allocateDirect</b> factory method of this class.  The
         * buffers returned by this method typically have somewhat higher allocation
         * and deallocation costs than non-direct buffers.  The contents of direct
         * buffers may reside outside of the normal garbage-collected heap, and so
         * their impact upon the memory footprint of an application might not be
         * obvious.  It is therefore recommended that direct buffers be allocated
         * primarily for large, long-lived buffers that are subject to the underlying
         * system's native I/O operations.  In general it is best to allocate direct
         * buffers only when they yield a measurable gain in program performance.
         *
         * <p> A direct byte buffer may also be created by <b style="color: orange;"> 
         * mapping</b> a region of a file directly into memory.  An implementation of
         * the platform may optionally support the creation of direct byte buffers from
         * native code.  If an instance of one of these kinds of buffers refers to an
         * inaccessible region of memory then an attempt to access that region will not
         * change the buffer's content and will cause an unspecified exception to be thrown
         * either at the time of the access or at some later time.
         *
         * <p> Whether a byte buffer is direct or non-direct may be determined by
         * invoking its <b style="color: orange;"> isDirect</b> method.  This method is provided so
         * that explicit buffer management can be done in performance-critical code.
         *
         *
         * <a id="bin"></a>
         * <h2> Access to binary data </h2>
         *
         * <p> This class defines methods for reading and writing values of all other
         * primitive types, except <b> boolean</b>.  Primitive values are translated
         * to (or from) sequences of bytes according to the buffer's current byte
         * order, which may be retrieved and modified via the <b style="color: orange;"> order</b>
         * methods.  Specific byte orders are represented by instances of the <b style="color: orange;"> 
         * ByteOrder</b> class.  The initial order of a byte buffer is always <b style="color: orange;"> 
         * ByteOrder.NATIVE_ORDER</b>.
         *
         * <p> For access to heterogeneous binary data, that is, sequences of values of
         * different types, this class defines a family of absolute and relative
         * <i>get</i> and <i>put</i> methods for each type.  For 32-bit floating-point
         * values, for example, this class defines:
         *
         * <blockquote><pre>
         * float      <b style="color: orange;"> getFloat()</b>
         * float      <b style="color: orange;"> getFloat(gint index)</b>
         * ByteBuffer <b style="color: orange;"> putFloat(float f)</b>
         * ByteBuffer <b style="color: orange;"> putFloat(gint index, float f)</b>
         * </pre></blockquote>
         *
         * <p> Corresponding methods are defined for the types <b> char,
         * short, gint, long</b>, and <b> double</b>.  The index
         * parameters of the absolute <i>get</i> and <i>put</i> methods are in terms of
         * bytes rather than of the type being read or written.
         *
         * <a id="views"></a>
         *
         * <p> For access to homogeneous binary data, that is, sequences of values of
         * the same type, this class defines methods that can create <i>views</i> of a
         * given byte buffer.  A <i>view buffer</i> is simply another buffer whose
         * content is backed by the byte buffer.  Changes to the byte buffer's content
         * will be visible in the view buffer, and vice versa; the two buffers'
         * position, limit, and mark values are independent.  The <b style="color: orange;"> 
         * asFloatBuffer</b> method, for example, creates an instance of
         * the <b style="color: orange;"> FloatBuffer</b> class that is backed by the byte buffer upon which
         * the method is invoked.  Corresponding view-creation methods are defined for
         * the types <b> char, short, gint, long</b>, and <b> double</b>.
         *
         * <p> View buffers have three important advantages over the families of
         * type-specific <i>get</i> and <i>put</i> methods described above:
         *
         * <ul>
         *
         *   <li><p> A view buffer is indexed not in terms of bytes but rather in terms
         *   of the type-specific size of its values;  </p></li>
         *
         *   <li><p> A view buffer provides relative bulk <i>get</i> and <i>put</i>
         *   methods that can transfer contiguous sequences of values between a buffer
         *   and an array or some other buffer of the same type; and  </p></li>
         *
         *   <li><p> A view buffer is potentially much more efficient because it will
         *   be direct if, and only if, its backing byte buffer is direct.  </p></li>
         *
         * </ul>
         *
         * <p> The byte order of a view buffer is fixed to be that of its byte buffer
         * at the time that the view is created.  </p>
         *
        *
        *
         *
         * <h2> Invocation chaining </h2>
         *
         * <p> Methods in this class that do not otherwise have a value to return are
         * specified to return the buffer upon which they are invoked.  This allows
         * method invocations to be chained.
         *
         *
         * The sequence of statements
         *
         * <blockquote><pre>
         * bb.putInt(0xCAFEBABE);
         * bb.putShort(3);
         * bb.putShort(45);</pre></blockquote>
         *
         * can, for example, be replaced by the single statement
         *
         * <blockquote><pre>
         * bb.putInt(0xCAFEBABE).putShort(3).putShort(45);</pre></blockquote>
         *
         *
         *
         * @author Brunshweeck Tazeussong
         *
         */
        class ByteBuffer : public Buffer, public Comparable<ByteBuffer> {
        private:
            static const glong ARRAY_BASE_OFFSET;

        protected:
            CORE_ALIAS(HeapBytes, typename Class<ByteArray>::Ptr);

            HeapBytes hb;
            gint offset;
            gbool isReadOnly;
            using Buffer::address;

            // Creates a new buffer with the given mark, position, limit, capacity,
            // backing array, and array offset
            //
            CORE_EXPLICIT ByteBuffer(gint mark, gint pos, gint lim, gint cap, ByteArray &hb, gint offset);

            // Creates a new buffer with the given mark, position, limit, and capacity
            //
            CORE_EXPLICIT ByteBuffer(gint mark, gint pos, gint lim, gint cap);

            // Creates a new buffer with given base, address and capacity
            //
            CORE_EXPLICIT ByteBuffer(ByteArray &hb, glong addr, gint cap);

            /**
             * @override
             */
            Object &base()  const override;

        public:

            /**
             * Allocates a new direct byte buffer.
             *
             * <p> The new buffer's position will be zero, its limit will be its
             * capacity, its mark will be undefined, each of its elements will be
             * initialized to zero, and its byte order will be
             * <b style="color: orange;"> ByteOrder.NATIVE_ORDER</b>.  Whether or not it has a
             * <b style="color: orange;"> backing array</b> is unspecified.
             *
             * @param  capacity
             *         The new buffer's capacity, in bytes
             *
             * @return  The new byte buffer
             *
             * @throws  IllegalArgumentException
             *          If the <b> capacity</b> is a negative integer
             */
            static ByteBuffer &allocateDirect(gint capacity);

            /**
             * Allocates a new byte buffer.
             *
             * <p> The new buffer's position will be zero, its limit will be its
             * capacity, its mark will be undefined, each of its elements will be
             * initialized to zero, and its byte order will be

             * <b style="color: orange;"> ByteOrder.NATIVE_ORDER</b>.
             * It will have a <b style="color: orange;"> backing array</b>, and its
             * <b style="color: orange;"> array offset</b> will be zero.
             *
             * @param  capacity
             *         The new buffer's capacity, in bytes
             *
             * @return  The new byte buffer
             *
             * @throws  IllegalArgumentException
             *          If the <b> capacity</b> is a negative integer
             */
            static ByteBuffer &allocate(gint capacity);

            /**
             * Wraps a byte array into a buffer.
             *
             * <p> The new buffer will be backed by the given byte array;
             * that is, modifications to the buffer will cause the array to be modified
             * and vice versa.  The new buffer's capacity will be
             * <b> array.length</b>, its position will be <b> offset</b>, its limit
             * will be <b> offset + length</b>, its mark will be undefined, and its
             * byte order will be

             * <b style="color: orange;"> ByteOrder.NATIVE_ORDER</b>.
             * Its <b style="color: orange;"> backing array</b> will be the given array, and
             * its <b style="color: orange;"> array offset</b> will be zero.  </p>
             *
             * @param  array
             *         The array that will back the new buffer
             *
             * @param  offset
             *         The offset of the subarray to be used; must be non-negative and
             *         no larger than <b> array.length</b>.  The new buffer's position
             *         will be set to this value.
             *
             * @param  length
             *         The length of the subarray to be used;
             *         must be non-negative and no larger than
             *         <b> array.length - offset</b>.
             *         The new buffer's limit will be set to <b> offset + length</b>.
             *
             * @return  The new byte buffer
             *
             * @throws  IndexException
             *          If the preconditions on the <b> offset</b> and <b> length</b>
             *          parameters do not hold
             */
            static ByteBuffer &wrap(ByteArray &array, gint offset, gint length);

            /**
             * Wraps a byte array into a buffer.
             *
             * <p> The new buffer will be backed by the given byte array;
             * that is, modifications to the buffer will cause the array to be modified
             * and vice versa.  The new buffer's capacity and limit will be
             * <b> array.length</b>, its position will be zero, its mark will be
             * undefined, and its byte order will be

             * <b style="color: orange;"> ByteOrder.NATIVE_ORDER</b>.
             * Its <b style="color: orange;"> backing array</b> will be the given array, and its
             * <b style="color: orange;"> array offset</b> will be zero.  </p>
             *
             * @param  array
             *         The array that will back this buffer
             *
             * @return  The new byte buffer
             */
            static ByteBuffer &wrap(ByteArray &array);

            /**
             * Creates a new byte buffer whose content is a shared subsequence of
             * this buffer's content.
             *
             * <p> The content of the new buffer will start at this buffer's current
             * position.  Changes to this buffer's content will be visible in the new
             * buffer, and vice versa; the two buffers' position, limit, and mark
             * values will be independent.
             *
             * <p> The new buffer's position will be zero, its capacity and its limit
             * will be the number of bytes remaining in this buffer, its mark will be
             * undefined, and its byte order will be <b style="color: orange;"> ByteOrder.NATIVE_ORDER</b>.
             * The new buffer will be direct if, and only if, this buffer is direct, and
             * it will be read-only if, and only if, this buffer is read-only.  </p>
             *
             * @return  The new byte buffer
             *
             * @see alignedSlice(gint)
             */
            ByteBuffer &slice() const override = 0;

            /**
             * Creates a new byte buffer whose content is a shared subsequence of
             * this buffer's content.
             *
             * <p> The content of the new buffer will start at position <b> index</b>
             * in this buffer, and will contain <b> length</b> elements. Changes to
             * this buffer's content will be visible in the new buffer, and vice versa;
             * the two buffers' position, limit, and mark values will be independent.
             *
             * <p> The new buffer's position will be zero, its capacity and its limit
             * will be <b> length</b>, its mark will be undefined, and its byte order
             * will be
             *
             * <b style="color: orange;"> ByteOrder.NATIVE_ORDER</b>.
             *
             * The new buffer will be direct if, and only if, this buffer is direct,
             * and it will be read-only if, and only if, this buffer is read-only. </p>
             *
             * @param   index
             *          The position in this buffer at which the content of the new
             *          buffer will start; must be non-negative and no larger than
             *          <b style="color: orange;"> limit()</b>
             *
             * @param   length
             *          The number of elements the new buffer will contain; must be
             *          non-negative and no larger than <b> limit() - index</b>
             *
             * @return  The new buffer
             *
             * @throws  IndexException
             *          If <b> index</b> is negative or greater than <b> limit()</b>,
             *          <b> length</b> is negative, or <b> length > limit() - index</b>
             */
            ByteBuffer &slice(gint index, gint length) const override = 0;

            /**
             * Creates a new byte buffer that shares this buffer's content.
             *
             * <p> The content of the new buffer will be that of this buffer.  Changes
             * to this buffer's content will be visible in the new buffer, and vice
             * versa; the two buffers' position, limit, and mark values will be
             * independent.
             *
             * <p> The new buffer's capacity, limit, position,
             * and mark values will be identical to those of this buffer, and its byte
             * order will be <b style="color: orange;"> ByteOrder.NATIVE_ORDER</b>.
             * The new buffer will be direct if, and only if, this buffer is direct, and
             * it will be read-only if, and only if, this buffer is read-only.  </p>
             *
             * @return  The new byte buffer
             */
            ByteBuffer &duplicate() const override = 0;

            /**
             * Creates a new, read-only byte buffer that shares this buffer's
             * content.
             *
             * <p> The content of the new buffer will be that of this buffer.  Changes
             * to this buffer's content will be visible in the new buffer; the new
             * buffer itself, however, will be read-only and will not allow the shared
             * content to be modified.  The two buffers' position, limit, and mark
             * values will be independent.
             *
             * <p> The new buffer's capacity, limit, position,

             * and mark values will be identical to those of this buffer, and its byte
             * order will be <b style="color: orange;"> ByteOrder.BIG_ENDIAN</b>.
             *
             * <p> If this buffer is itself read-only then this method behaves in
             * exactly the same way as the <b style="color: orange;"> duplicate</b> method.  </p>
             *
             * @return  The new, read-only byte buffer
             */
            virtual ByteBuffer &asReadOnlyBuffer() const = 0;

            /**
             * Relative <i>get</i> method.  Reads the byte at this buffer's
             * current position, and then increments the position.
             *
             * @return  The byte at the buffer's current position
             *
             * @throws  BufferUnderflowException
             *          If the buffer's current position is not smaller than its limit
             */
            virtual gbyte get() = 0;

            /**
             * Relative <i>put</i> method&nbsp;&nbsp;<i>(optional operation)</i>.
             *
             * <p> Writes the given byte into this buffer at the current
             * position, and then increments the position. </p>
             *
             * @param  b
             *         The byte to be written
             *
             * @return  This buffer
             *
             * @throws  BufferOverflowException
             *          If this buffer's current position is not smaller than its limit
             *
             * @throws  ReadOnlyBufferException
             *          If this buffer is read-only
             */
            virtual ByteBuffer &put(gbyte b) = 0;

            /**
             * Absolute <i>get</i> method.  Reads the byte at the given
             * index.
             *
             * @param  index
             *         The index from which the byte will be read
             *
             * @return  The byte at the given index
             *
             * @throws  IndexException
             *          If <b> index</b> is negative
             *          or not smaller than the buffer's limit
             */
            virtual gbyte get(gint index) const = 0;

            /**
             * Absolute <i>put</i> method&nbsp;&nbsp;<i>(optional operation)</i>.
             *
             * <p> Writes the given byte into this buffer at the given
             * index. </p>
             *
             * @param  index
             *         The index at which the byte will be written
             *
             * @param  b
             *         The byte value to be written
             *
             * @return  This buffer
             *
             * @throws  IndexException
             *          If <b> index</b> is negative
             *          or not smaller than the buffer's limit
             *
             * @throws  ReadOnlyBufferException
             *          If this buffer is read-only
             */
            virtual ByteBuffer &put(gint index, gbyte b) = 0;

            /**
             * Relative bulk <i>get</i> method.
             *
             * <p> This method transfers bytes from this buffer into the given
             * destination array.  If there are fewer bytes remaining in the
             * buffer than are required to satisfy the request, that is, if
             * <b> length</b>&nbsp;<b> ></b>&nbsp;<b> remaining()</b>, then no
             * bytes are transferred and a <b style="color: orange;"> BufferUnderflowException</b> is
             * thrown.
             *
             * <p> Otherwise, this method copies <b> length</b> bytes from this
             * buffer into the given array, starting at the current position of this
             * buffer and at the given offset in the array.  The position of this
             * buffer is then incremented by <b> length</b>.
             *
             * <p> In other words, an invocation of this method of the form
             * <code>src.get(dst,&nbsp;off,&nbsp;len)</code> has exactly the same effect as
             * the loop
             *
             * <pre><b> 
             *     for (gint i = off; i < off + len; i++)
             *         dst[i] = src.get();
             * </b></pre>
             *
             * except that it first checks that there are sufficient bytes in
             * this buffer and it is potentially much more efficient.
             *
             * @param  dst
             *         The array into which bytes are to be written
             *
             * @param  offset
             *         The offset within the array of the first byte to be
             *         written; must be non-negative and no larger than
             *         <b> dst.length</b>
             *
             * @param  length
             *         The maximum number of bytes to be written to the given
             *         array; must be non-negative and no larger than
             *         <b> dst.length - offset</b>
             *
             * @return  This buffer
             *
             * @throws  BufferUnderflowException
             *          If there are fewer than <b> length</b> bytes
             *          remaining in this buffer
             *
             * @throws  IndexException
             *          If the preconditions on the <b> offset</b> and <b> length</b>
             *          parameters do not hold
             */
            virtual ByteBuffer &get(ByteArray &dst, gint offset, gint length);

            /**
             * Relative bulk <i>get</i> method.
             *
             * <p> This method transfers bytes from this buffer into the given
             * destination array.  An invocation of this method of the form
             * <b> src.get(a)</b> behaves in exactly the same way as the invocation
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
             *          If there are fewer than <b> length</b> bytes
             *          remaining in this buffer
             */
            virtual ByteBuffer &get(ByteArray &dst);

            /**
             * Absolute bulk <i>get</i> method.
             *
             * <p> This method transfers <b> length</b> bytes from this
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
             * </b></pre>
             *
             * @param  index
             *         The index in this buffer from which the first byte will be
             *         read; must be non-negative and less than <b> limit()</b>
             *
             * @param  dst
             *         The destination array
             *
             * @param  offset
             *         The offset within the array of the first byte to be
             *         written; must be non-negative and less than
             *         <b> dst.length</b>
             *
             * @param  length
             *         The number of bytes to be written to the given array;
             *         must be non-negative and no larger than the smaller of
             *         <b> limit() - index</b> and <b> dst.length - offset</b>
             *
             * @return  This buffer
             *
             * @throws  IndexException
             *          If the preconditions on the <b> index</b>, <b> offset</b>, and
             *          <b> length</b> parameters do not hold
             */
            virtual const ByteBuffer &get(gint index, ByteArray &dst, gint offset, gint length) const;

            /**
             * Absolute bulk <i>get</i> method.
             *
             * <p> This method transfers bytes from this buffer into the given
             * destination array.  The position of this buffer is unchanged.  An
             * invocation of this method of the form
             * <code>src.get(index,&nbsp;dst)</code> behaves in exactly the same
             * way as the invocation:
             *
             * <pre>
             *     src.get(index, dst, 0, dst.length) </pre>
             *
             * @param  index
             *         The index in this buffer from which the first byte will be
             *         read; must be non-negative and less than <b> limit()</b>
             *
             * @param  dst
             *         The destination array
             *
             * @return  This buffer
             *
             * @throws  IndexException
             *          If <b> index</b> is negative, not smaller than <b> limit()</b>,
             *          or <b> limit() - index < dst.length</b>
             */
            virtual const ByteBuffer &get(gint index, ByteArray &dst) const;

        private:
            const ByteBuffer &getArray(gint index, ByteArray &dst, gint offset, gint length) const;

        public:

            /**
             * Relative bulk <i>put</i> method&nbsp;&nbsp;<i>(optional operation)</i>.
             *
             * <p> This method transfers the bytes remaining in the given source
             * buffer into this buffer.  If there are more bytes remaining in the
             * source buffer than in this buffer, that is, if
             * <b> src.remaining()</b>&nbsp;<b> ></b>&nbsp;<b> remaining()</b>,
             * then no bytes are transferred and a <b style="color: orange;"> 
             * BufferOverflowException</b> is thrown.
             *
             * <p> Otherwise, this method copies
             * <i>n</i>&nbsp;=&nbsp;<b> src.remaining()</b> bytes from the given
             * buffer into this buffer, starting at each buffer's current position.
             * The positions of both buffers are then incremented by <i>n</i>.
             *
             * <p> In other words, an invocation of this method of the form
             * <b> dst.put(src)</b> has exactly the same effect as the loop
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
             *         The source buffer from which bytes are to be read;
             *         must not be this buffer
             *
             * @return  This buffer
             *
             * @throws  BufferOverflowException
             *          If there is insufficient space in this buffer
             *          for the remaining bytes in the source buffer
             *
             * @throws  IllegalArgumentException
             *          If the source buffer is this buffer
             *
             * @throws  ReadOnlyBufferException
             *          If this buffer is read-only
             */
            virtual ByteBuffer &put(ByteBuffer &src);

            /**
             * Absolute bulk <i>put</i> method&nbsp;&nbsp;<i>(optional operation)</i>.
             *
             * <p> This method transfers <b> length</b> bytes into this buffer from
             * the given source buffer, starting at the given <b> offset</b> in the
             * source buffer and the given <b> index</b> in this buffer. The positions
             * of both buffers are unchanged.
             *
             * <p> In other words, an invocation of this method of the form
             * <code>dst.put(index,&nbsp;src,&nbsp;offset,&nbsp;length)</code>
             * has exactly the same effect as the loop
             *
             * <pre><b> 
             * for (gint i = offset, j = index; i < offset + length; i++, j++)
             *     dst.put(j, src.get(i));
             * </b></pre>
             *
             * except that it first checks the consistency of the supplied parameters
             * and it is potentially much more efficient.  If this buffer and
             * the source buffer share the same backing array or memory, then the
             * result will be as if the source elements were first copied to an
             * intermediate location before being written into this buffer.
             *
             * @param index
             *        The index in this buffer at which the first byte will be
             *        written; must be non-negative and less than <b> limit()</b>
             *
             * @param src
             *        The buffer from which bytes are to be read
             *
             * @param offset
             *        The index within the source buffer of the first byte to be
             *        read; must be non-negative and less than <b> src.limit()</b>
             *
             * @param length
             *        The number of bytes to be read from the given buffer;
             *        must be non-negative and no larger than the smaller of
             *        <b> limit() - index</b> and <b> src.limit() - offset</b>
             *
             * @return This buffer
             *
             * @throws IndexException
             *         If the preconditions on the <b> index</b>, <b> offset</b>, and
             *         <b> length</b> parameters do not hold
             *
             * @throws ReadOnlyBufferException
             *         If this buffer is read-only
             */
            virtual ByteBuffer &put(gint index, ByteBuffer &src, gint offset, gint length);

        protected:
            void putBuffer(gint pos, ByteBuffer &src, gint srcPos, gint n);

        public:

            /**
             * Relative bulk <i>put</i> method&nbsp;&nbsp;<i>(optional operation)</i>.
             *
             * <p> This method transfers bytes into this buffer from the given
             * source array.  If there are more bytes to be copied from the array
             * than remain in this buffer, that is, if
             * <b> length</b>&nbsp;<b> ></b>&nbsp;<b> remaining()</b>, then no
             * bytes are transferred and a <b style="color: orange;"> BufferOverflowException</b> is
             * thrown.
             *
             * <p> Otherwise, this method copies <b> length</b> bytes from the
             * given array into this buffer, starting at the given offset in the array
             * and at the current position of this buffer.  The position of this buffer
             * is then incremented by <b> length</b>.
             *
             * <p> In other words, an invocation of this method of the form
             * <code>dst.put(src,&nbsp;off,&nbsp;len)</code> has exactly the same effect as
             * the loop
             *
             * <pre><b> 
             *     for (gint i = off; i < off + len; i++)
             *         dst.put(src[i]);
             * </b></pre>
             *
             * except that it first checks that there is sufficient space in this
             * buffer and it is potentially much more efficient.
             *
             * @param  src
             *         The array from which bytes are to be read
             *
             * @param  offset
             *         The offset within the array of the first byte to be read;
             *         must be non-negative and no larger than <b> src.length</b>
             *
             * @param  length
             *         The number of bytes to be read from the given array;
             *         must be non-negative and no larger than
             *         <b> src.length - offset</b>
             *
             * @return  This buffer
             *
             * @throws  BufferOverflowException
             *          If there is insufficient space in this buffer
             *
             * @throws  IndexException
             *          If the preconditions on the <b> offset</b> and <b> length</b>
             *          parameters do not hold
             *
             * @throws  ReadOnlyBufferException
             *          If this buffer is read-only
             */
            virtual ByteBuffer &put(const ByteArray &src, gint offset, gint length);

            /**
             * Relative bulk <i>put</i> method&nbsp;&nbsp;<i>(optional operation)</i>.
             *
             * <p> This method transfers the entire content of the given source
             * byte array into this buffer.  An invocation of this method of the
             * form <b> dst.put(a)</b> behaves in exactly the same way as the
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
            virtual ByteBuffer &put(const ByteArray &src);

            /**
             * Absolute bulk <i>put</i> method&nbsp;&nbsp;<i>(optional operation)</i>.
             *
             * <p> This method transfers <b> length</b> bytes from the given
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
             * </b></pre>
             *
             * @param  index
             *         The index in this buffer at which the first byte will be
             *         written; must be non-negative and less than <b> limit()</b>
             *
             * @param  src
             *         The array from which bytes are to be read
             *
             * @param  offset
             *         The offset within the array of the first byte to be read;
             *         must be non-negative and less than <b> src.length</b>
             *
             * @param  length
             *         The number of bytes to be read from the given array;
             *         must be non-negative and no larger than the smaller of
             *         <b> limit() - index</b> and <b> src.length - offset</b>
             *
             * @return  This buffer
             *
             * @throws  IndexException
             *          If the preconditions on the <b> index</b>, <b> offset</b>, and
             *          <b> length</b> parameters do not hold
             *
             * @throws  ReadOnlyBufferException
             *          If this buffer is read-only
             */
            virtual ByteBuffer &put(gint index, const ByteArray &src, gint offset, gint length);

            /**
             * Absolute bulk <i>put</i> method&nbsp;&nbsp;<i>(optional operation)</i>.
             *
             * <p> This method copies bytes into this buffer from the given source
             * array.  The position of this buffer is unchanged.  An invocation of this
             * method of the form <code>dst.put(index,&nbsp;src)</code>
             * behaves in exactly the same way as the invocation:
             *
             * <pre>
             *     dst.put(index, src, 0, src.length); </pre>
             *
             * @param  index
             *         The index in this buffer at which the first byte will be
             *         written; must be non-negative and less than <b> limit()</b>
             *
             * @param  src
             *         The array from which bytes are to be read
             *
             * @return  This buffer
             *
             * @throws  IndexException
             *          If <b> index</b> is negative, not smaller than <b> limit()</b>,
             *          or <b> limit() - index < src.length</b>
             *
             * @throws  ReadOnlyBufferException
             *          If this buffer is read-only
             */
            virtual ByteBuffer &put(gint index, const ByteArray &src);

        private:

            ByteBuffer &putArray(gint index, const ByteArray &src, gint offset, gint length);

        public:
            /**
             * Tells whether or not this buffer is backed by an accessible byte
             * array.
             *
             * <p> If this method returns <b> true</b> then the <b style="color: orange;"> array</b>
             * and <b style="color: orange;"> arrayOffset</b> methods may safely be invoked.
             * </p>
             *
             * @return  <b> true</b> if, and only if, this buffer
             *          is backed by an array and is not read-only
             */
            gbool hasArray() const override;

            /**
             * Returns the byte array that backs this
             * buffer&nbsp;&nbsp;<i>(optional operation)</i>.
             *
             * <p> Modifications to this buffer's content will cause the returned
             * array's content to be modified, and vice versa.
             *
             * <p> Invoke the <b style="color: orange;"> hasArray</b> method before invoking this
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
            ByteArray &array() const override;

            /**
             * Returns the offset within this buffer's backing array of the first
             * element of the buffer&nbsp;&nbsp;<i>(optional operation)</i>.
             *
             * <p> If this buffer is backed by an array then buffer position <i>p</i>
             * corresponds to array index <i>p</i>&nbsp;+&nbsp;<b> arrayOffset()</b>.
             *
             * <p> Invoke the <b style="color: orange;"> hasArray</b> method before invoking this
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
             * @see Buffer.position
             */
            ByteBuffer &setPosition(gint newPosition) override;

            /**
             * @see Buffer.limit
             */
            ByteBuffer &setLimit(gint newLimit) override;

            /**
             * @see Buffer.mark
             */
            ByteBuffer &mark() override;

            /**
             * @see Buffer.reset
             */
            ByteBuffer &reset() override;

            /**
             * @see Buffer.clear
             */
            ByteBuffer &clear() override;

            /**
             * @see Buffer.flip
             */
            ByteBuffer &flip() override;

            /**
             * @see Buffer.rewind
             */
            ByteBuffer &rewind() override;

            /**
             * Compacts this buffer&nbsp;&nbsp;<i>(optional operation)</i>.
             *
             * <p> The bytes between the buffer's current position and its limit,
             * if any, are copied to the beginning of the buffer.  That is, the
             * byte at index <i>p</i>&nbsp;=&nbsp;<b> position()</b> is copied
             * to index zero, the byte at index <i>p</i>&nbsp;+&nbsp;1 is copied
             * to index one, and so forth until the byte at index
             * <b> limit()</b>&nbsp;-&nbsp;1 is copied to index
             * <i>n</i>&nbsp;=&nbsp;<b> limit()</b>&nbsp;-&nbsp;<b> 1</b>&nbsp;-&nbsp;<i>p</i>.
             * The buffer's position is then set to <i>n+1</i> and its limit is set to
             * its capacity.  The mark, if defined, is discarded.
             *
             * <p> The buffer's position is set to the number of bytes copied,
             * rather than to zero, so that an invocation of this method can be
             * followed immediately by an invocation of another relative <i>put</i>
             * method. </p>
             *
             * <p> Invoke this method after writing data from a buffer in case the
             * write was incomplete.  The following loop, for example, copies bytes
             * from one channel to another via the buffer <b> buf</b>:
             *
             * <blockquote><pre><b> 
             *   buf.clear();          // Prepare buffer for use
             *   while (in.read(buf) >= 0 || buf.position != 0) {
             *       buf.flip();
             *       out.write(buf);
             *       buf.compact();    // In case of partial write
             *   }
             * </b></pre></blockquote>
             *

             *
             * @return  This buffer
             *
             * @throws  ReadOnlyBufferException
             *          If this buffer is read-only
             */
            virtual ByteBuffer &compact() = 0;

            /**
             * Tells whether or not this byte buffer is direct.
             *
             * @return  <b> true</b> if, and only if, this buffer is direct
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
             * <p> The hash code of a byte buffer depends only upon its remaining
             * elements; that is, upon the elements from <b> position()</b> up to, and
             * including, the element at <b> limit()</b>&nbsp;-&nbsp;<b> 1</b>.
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
             * <p> Two byte buffers are equal if, and only if,
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
             * <p> A byte buffer is not equal to any other type of object.  </p>
             *
             * @param  ob  The object to which this buffer is to be compared
             *
             * @return  <b> true</b> if, and only if, this buffer is equal to the
             *           given object
             */
            gbool equals(const Object &obj) const override;

        protected:
            static gint mismatch(const ByteBuffer &a, int aOff, const ByteBuffer &b, int bOff, int length);

        public:


            /**
             * Compares this buffer to another.
             *
             * <p> Two byte buffers are compared by comparing their sequences of
             * remaining elements lexicographically, without regard to the starting
             * position of each sequence within its corresponding buffer.
             * Pairs of <b> byte</b> elements are compared as if by invoking
             * <b style="color: orange;"> Byte.compare(gbyte,gbyte)</b>.

             *
             * <p> A byte buffer is not comparable to any other type of object.
             *
             * @return  A negative integer, zero, or a positive integer as this buffer
             *          is less than, equal to, or greater than the given buffer
             */
            gint compareTo(const ByteBuffer &other) const override;

            /**
             * Finds and returns the relative index of the first mismatch between this
             * buffer and a given buffer.  The index is relative to the
             * <b style="color: orange;"> position</b> of each buffer and will be in the range of
             * 0 (inclusive) up to the smaller of the <b style="color: orange;"> remaining</b>
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
            gint mismatch(const ByteBuffer &other) const;

        protected:

            CORE_ALIAS(ByteOrder, charset::Charset::ByteOrder);

            gbool bigEndian = ByteOrder::NATIVE_ENDIAN != ByteOrder::BIG_ENDIAN;

        public:

            /**
             * Retrieves this buffer's byte order.
             *
             * <p> The byte order is used when reading or writing multibyte values, and
             * when creating buffers that are views of this byte buffer.  The order of
             * a newly-created byte buffer is always <b style="color: orange;"> ByteOrder.NATIVE_ORDER</b>.
             * </p>
             *
             * @return  This buffer's byte order
             */
            ByteOrder order() const;

            /**
             * Modifies this buffer's byte order.
             *
             * @param  bo
             *         The new byte order,
             *         either <b style="color: orange;"> ByteOrder.BIG_ENDIAN</b>
             *         or <b style="color: orange;"> ByteOrder.LITTLE_ENDIAN</b>
             *
             * @return  This buffer
             */
            ByteBuffer &setOrder(ByteOrder bo);

            /**
             * Returns the memory address, pointing to the byte at the given index,
             * modulo the given unit size.
             *
             * <p> The return value is non-negative in the range of <b> 0</b>
             * (inclusive) up to <b> unitSize</b> (exclusive), with zero indicating
             * that the address of the byte at the index is aligned for the unit size,
             * and a positive value that the address is misaligned for the unit size.
             * If the address of the byte at the index is misaligned, the return value
             * represents how much the index should be adjusted to locate a byte at an
             * aligned address.  Specifically, the index should either be decremented by
             * the return value if the latter is not greater than <b> index</b>, or be
             * incremented by the unit size minus the return value.  Therefore given
             * <blockquote><pre>
             * gint value = alignmentOffset(index, unitSize)</pre></blockquote>
             * then the identities
             * <blockquote><pre>
             * alignmentOffset(index - value, unitSize) == 0, value &le; index</pre></blockquote>
             * and
             * <blockquote><pre>
             * alignmentOffset(index + (unitSize - value), unitSize) == 0</pre></blockquote>
             * must hold.
             *
             * @apiNote
             * This method may be utilized to determine if unit size bytes from an
             * index can be accessed atomically, if supported by the native platform.
             *
             * @implNote
             * This implementation throws <b> UnsupportedOperationException</b> for
             * non-direct buffers when the given unit size is greater then <b> 8</b>.
             *
             * @param  index
             *         The index to query for alignment offset, must be non-negative, no
             *         upper bounds check is performed
             *
             * @param  unitSize
             *         The unit size in bytes, must be a power of <b> 2</b>
             *
             * @return  The indexed byte's memory address modulo the unit size
             *
             * @throws IllegalArgumentException
             *         If the index is negative or the unit size is not a power of
             *         <b> 2</b>
             *
             * @throws UnsupportedOperationException
             *         If the native platform does not guarantee stable alignment offset
             *         values for the given unit size when managing the memory regions
             *         of buffers of the same kind as this buffer (direct or
             *         non-direct).  For example, if garbage collection would result
             *         in the moving of a memory region covered by a non-direct buffer
             *         from one location to another and both locations have different
             *         alignment characteristics.
             *
             * @see alignedSlice(gint)
             */
            gint alignmentOffset(gint index, gint unitSize);

            /**
             * Creates a new byte buffer whose content is a shared and aligned
             * subsequence of this buffer's content.
             *
             * <p> The content of the new buffer will start at this buffer's current
             * position rounded up to the index of the nearest aligned byte for the
             * given unit size, and end at this buffer's limit rounded down to the index
             * of the nearest aligned byte for the given unit size.
             * If rounding results in out-of-bound values then the new buffer's capacity
             * and limit will be zero.  If rounding is within bounds the following
             * expressions will be true for a new buffer <b> nb</b> and unit size
             * <b> unitSize</b>:
             * <pre><b> 
             * nb.alignmentOffset(0, unitSize) == 0
             * nb.alignmentOffset(nb.limit(), unitSize) == 0
             * </b></pre>
             *
             * <p> Changes to this buffer's content will be visible in the new
             * buffer, and vice versa; the two buffers' position, limit, and mark
             * values will be independent.
             *
             * <p> The new buffer's position will be zero, its capacity and its limit
             * will be the number of bytes remaining in this buffer or fewer subject to
             * alignment, its mark will be undefined, and its byte order will be
             * <b style="color: orange;"> ByteOrder.NATIVE_ORDER</b>.
             *
             * The new buffer will be direct if, and only if, this buffer is direct, and
             * it will be read-only if, and only if, this buffer is read-only.  </p>
             *
             * @apiNote
             * This method may be utilized to create a new buffer where unit size bytes
             * from index, that is a multiple of the unit size, may be accessed
             * atomically, if supported by the native platform.
             *
             * @implNote
             * This implementation throws <b> UnsupportedOperationException</b> for
             * non-direct buffers when the given unit size is greater then <b> 8</b>.
             *
             * @param  unitSize
             *         The unit size in bytes, must be a power of <b> 2</b>
             *
             * @return  The new byte buffer
             *
             * @throws IllegalArgumentException
             *         If the unit size not a power of <b> 2</b>
             *
             * @throws UnsupportedOperationException
             *         If the native platform does not guarantee stable aligned slices
             *         for the given unit size when managing the memory regions
             *         of buffers of the same kind as this buffer (direct or
             *         non-direct).  For example, if garbage collection would result
             *         in the moving of a memory region covered by a non-direct buffer
             *         from one location to another and both locations have different
             *         alignment characteristics.
             *
             * @see alignmentOffset(gint, gint)
             * @see slice()
             */
            ByteBuffer &alignedSlice(gint unitSize);


            /**
             * Relative <i>get</i> method for reading a char value.
             *
             * <p> Reads the next two bytes at this buffer's current position,
             * composing them into a char value according to the current byte order,
             * and then increments the position by two.  </p>
             *
             * @return  The char value at the buffer's current position
             *
             * @throws  BufferUnderflowException
             *          If there are fewer than two bytes
             *          remaining in this buffer
             */
            virtual gchar getChar() = 0;

            /**
             * Relative <i>put</i> method for writing a char
             * value&nbsp;&nbsp;<i>(optional operation)</i>.
             *
             * <p> Writes two bytes containing the given char value, in the
             * current byte order, into this buffer at the current position, and then
             * increments the position by two.  </p>
             *
             * @param  value
             *         The char value to be written
             *
             * @return  This buffer
             *
             * @throws  BufferOverflowException
             *          If there are fewer than two bytes
             *          remaining in this buffer
             *
             * @throws  ReadOnlyBufferException
             *          If this buffer is read-only
             */
            virtual ByteBuffer &putChar(gchar value) = 0;

            /**
             * Absolute <i>get</i> method for reading a char value.
             *
             * <p> Reads two bytes at the given index, composing them into a
             * char value according to the current byte order.  </p>
             *
             * @param  index
             *         The index from which the bytes will be read
             *
             * @return  The char value at the given index
             *
             * @throws  IndexException
             *          If <b> index</b> is negative
             *          or not smaller than the buffer's limit,
             *          minus one
             */
            virtual gchar getChar(gint index) const = 0;

            /**
             * Absolute <i>put</i> method for writing a char
             * value&nbsp;&nbsp;<i>(optional operation)</i>.
             *
             * <p> Writes two bytes containing the given char value, in the
             * current byte order, into this buffer at the given index.  </p>
             *
             * @param  index
             *         The index at which the bytes will be written
             *
             * @param  value
             *         The char value to be written
             *
             * @return  This buffer
             *
             * @throws  IndexException
             *          If <b> index</b> is negative
             *          or not smaller than the buffer's limit,
             *          minus one
             *
             * @throws  ReadOnlyBufferException
             *          If this buffer is read-only
             */
            virtual ByteBuffer &putChar(gint index, gchar value) = 0;

            /**
             * Creates a view of this byte buffer as a char buffer.
             *
             * <p> The content of the new buffer will start at this buffer's current
             * position.  Changes to this buffer's content will be visible in the new
             * buffer, and vice versa; the two buffers' position, limit, and mark
             * values will be independent.
             *
             * <p> The new buffer's position will be zero, its capacity and its limit
             * will be the number of bytes remaining in this buffer divided by
             * two, its mark will be undefined, and its byte order will be that
             * of the byte buffer at the moment the view is created.  The new buffer
             * will be direct if, and only if, this buffer is direct, and it will be
             * read-only if, and only if, this buffer is read-only.  </p>
             *
             * @return  A new char buffer
             */
            virtual CharBuffer &asCharBuffer() const = 0;


            /**
             * Relative <i>get</i> method for reading a short value.
             *
             * <p> Reads the next two bytes at this buffer's current position,
             * composing them into a short value according to the current byte order,
             * and then increments the position by two.  </p>
             *
             * @return  The short value at the buffer's current position
             *
             * @throws  BufferUnderflowException
             *          If there are fewer than two bytes
             *          remaining in this buffer
             */
            virtual gshort getShort() = 0;

            /**
             * Relative <i>put</i> method for writing a short
             * value&nbsp;&nbsp;<i>(optional operation)</i>.
             *
             * <p> Writes two bytes containing the given short value, in the
             * current byte order, into this buffer at the current position, and then
             * increments the position by two.  </p>
             *
             * @param  value
             *         The short value to be written
             *
             * @return  This buffer
             *
             * @throws  BufferOverflowException
             *          If there are fewer than two bytes
             *          remaining in this buffer
             *
             * @throws  ReadOnlyBufferException
             *          If this buffer is read-only
             */
            virtual ByteBuffer &putShort(gshort value) = 0;

            /**
             * Absolute <i>get</i> method for reading a short value.
             *
             * <p> Reads two bytes at the given index, composing them into a
             * short value according to the current byte order.  </p>
             *
             * @param  index
             *         The index from which the bytes will be read
             *
             * @return  The short value at the given index
             *
             * @throws  IndexException
             *          If <b> index</b> is negative
             *          or not smaller than the buffer's limit,
             *          minus one
             */
            virtual gshort getShort(gint index) const = 0;

            /**
             * Absolute <i>put</i> method for writing a short
             * value&nbsp;&nbsp;<i>(optional operation)</i>.
             *
             * <p> Writes two bytes containing the given short value, in the
             * current byte order, into this buffer at the given index.  </p>
             *
             * @param  index
             *         The index at which the bytes will be written
             *
             * @param  value
             *         The short value to be written
             *
             * @return  This buffer
             *
             * @throws  IndexException
             *          If <b> index</b> is negative
             *          or not smaller than the buffer's limit,
             *          minus one
             *
             * @throws  ReadOnlyBufferException
             *          If this buffer is read-only
             */
            virtual ByteBuffer &putShort(gint index, gshort value) = 0;

            /**
             * Creates a view of this byte buffer as a short buffer.
             *
             * <p> The content of the new buffer will start at this buffer's current
             * position.  Changes to this buffer's content will be visible in the new
             * buffer, and vice versa; the two buffers' position, limit, and mark
             * values will be independent.
             *
             * <p> The new buffer's position will be zero, its capacity and its limit
             * will be the number of bytes remaining in this buffer divided by
             * two, its mark will be undefined, and its byte order will be that
             * of the byte buffer at the moment the view is created.  The new buffer
             * will be direct if, and only if, this buffer is direct, and it will be
             * read-only if, and only if, this buffer is read-only.  </p>
             *
             * @return  A new short buffer
             */
            virtual ShortBuffer &asShortBuffer() const = 0;

            /**
             * Relative <i>get</i> method for reading an gint value.
             *
             * <p> Reads the next four bytes at this buffer's current position,
             * composing them into an gint value according to the current byte order,
             * and then increments the position by four.  </p>
             *
             * @return  The gint value at the buffer's current position
             *
             * @throws  BufferUnderflowException
             *          If there are fewer than four bytes
             *          remaining in this buffer
             */
            virtual gint getInt() = 0;

            /**
             * Relative <i>put</i> method for writing an gint
             * value&nbsp;&nbsp;<i>(optional operation)</i>.
             *
             * <p> Writes four bytes containing the given gint value, in the
             * current byte order, into this buffer at the current position, and then
             * increments the position by four.  </p>
             *
             * @param  value
             *         The gint value to be written
             *
             * @return  This buffer
             *
             * @throws  BufferOverflowException
             *          If there are fewer than four bytes
             *          remaining in this buffer
             *
             * @throws  ReadOnlyBufferException
             *          If this buffer is read-only
             */
            virtual ByteBuffer &putInt(gint value) = 0;

            /**
             * Absolute <i>get</i> method for reading an gint value.
             *
             * <p> Reads four bytes at the given index, composing them into a
             * gint value according to the current byte order.  </p>
             *
             * @param  index
             *         The index from which the bytes will be read
             *
             * @return  The gint value at the given index
             *
             * @throws  IndexException
             *          If <b> index</b> is negative
             *          or not smaller than the buffer's limit,
             *          minus three
             */
            virtual gint getInt(gint index) const = 0;

            /**
             * Absolute <i>put</i> method for writing an gint
             * value&nbsp;&nbsp;<i>(optional operation)</i>.
             *
             * <p> Writes four bytes containing the given gint value, in the
             * current byte order, into this buffer at the given index.  </p>
             *
             * @param  index
             *         The index at which the bytes will be written
             *
             * @param  value
             *         The gint value to be written
             *
             * @return  This buffer
             *
             * @throws  IndexException
             *          If <b> index</b> is negative
             *          or not smaller than the buffer's limit,
             *          minus three
             *
             * @throws  ReadOnlyBufferException
             *          If this buffer is read-only
             */
            virtual ByteBuffer &putInt(gint index, gint value) = 0;

            /**
             * Creates a view of this byte buffer as an gint buffer.
             *
             * <p> The content of the new buffer will start at this buffer's current
             * position.  Changes to this buffer's content will be visible in the new
             * buffer, and vice versa; the two buffers' position, limit, and mark
             * values will be independent.
             *
             * <p> The new buffer's position will be zero, its capacity and its limit
             * will be the number of bytes remaining in this buffer divided by
             * four, its mark will be undefined, and its byte order will be that
             * of the byte buffer at the moment the view is created.  The new buffer
             * will be direct if, and only if, this buffer is direct, and it will be
             * read-only if, and only if, this buffer is read-only.  </p>
             *
             * @return  A new gint buffer
             */
            virtual IntBuffer &asIntBuffer() const = 0;

            /**
             * Relative <i>get</i> method for reading a long value.
             *
             * <p> Reads the next eight bytes at this buffer's current position,
             * composing them into a long value according to the current byte order,
             * and then increments the position by eight.  </p>
             *
             * @return  The long value at the buffer's current position
             *
             * @throws  BufferUnderflowException
             *          If there are fewer than eight bytes
             *          remaining in this buffer
             */
            virtual glong getLong() = 0;

            /**
             * Relative <i>put</i> method for writing a long
             * value&nbsp;&nbsp;<i>(optional operation)</i>.
             *
             * <p> Writes eight bytes containing the given long value, in the
             * current byte order, into this buffer at the current position, and then
             * increments the position by eight.  </p>
             *
             * @param  value
             *         The long value to be written
             *
             * @return  This buffer
             *
             * @throws  BufferOverflowException
             *          If there are fewer than eight bytes
             *          remaining in this buffer
             *
             * @throws  ReadOnlyBufferException
             *          If this buffer is read-only
             */
            virtual ByteBuffer &putLong(glong value) = 0;

            /**
             * Absolute <i>get</i> method for reading a long value.
             *
             * <p> Reads eight bytes at the given index, composing them into a
             * long value according to the current byte order.  </p>
             *
             * @param  index
             *         The index from which the bytes will be read
             *
             * @return  The long value at the given index
             *
             * @throws  IndexException
             *          If <b> index</b> is negative
             *          or not smaller than the buffer's limit,
             *          minus seven
             */
            virtual glong getLong(gint index) const = 0;

            /**
             * Absolute <i>put</i> method for writing a long
             * value&nbsp;&nbsp;<i>(optional operation)</i>.
             *
             * <p> Writes eight bytes containing the given long value, in the
             * current byte order, into this buffer at the given index.  </p>
             *
             * @param  index
             *         The index at which the bytes will be written
             *
             * @param  value
             *         The long value to be written
             *
             * @return  This buffer
             *
             * @throws  IndexException
             *          If <b> index</b> is negative
             *          or not smaller than the buffer's limit,
             *          minus seven
             *
             * @throws  ReadOnlyBufferException
             *          If this buffer is read-only
             */
            virtual ByteBuffer &putLong(gint index, glong value) = 0;

            /**
             * Creates a view of this byte buffer as a long buffer.
             *
             * <p> The content of the new buffer will start at this buffer's current
             * position.  Changes to this buffer's content will be visible in the new
             * buffer, and vice versa; the two buffers' position, limit, and mark
             * values will be independent.
             *
             * <p> The new buffer's position will be zero, its capacity and its limit
             * will be the number of bytes remaining in this buffer divided by
             * eight, its mark will be undefined, and its byte order will be that
             * of the byte buffer at the moment the view is created.  The new buffer
             * will be direct if, and only if, this buffer is direct, and it will be
             * read-only if, and only if, this buffer is read-only.  </p>
             *
             * @return  A new long buffer
             */
            virtual LongBuffer &asLongBuffer() const = 0;

            /**
             * Relative <i>get</i> method for reading a float value.
             *
             * <p> Reads the next four bytes at this buffer's current position,
             * composing them into a float value according to the current byte order,
             * and then increments the position by four.  </p>
             *
             * @return  The float value at the buffer's current position
             *
             * @throws  BufferUnderflowException
             *          If there are fewer than four bytes
             *          remaining in this buffer
             */
            virtual gfloat getFloat() = 0;

            /**
             * Relative <i>put</i> method for writing a float
             * value&nbsp;&nbsp;<i>(optional operation)</i>.
             *
             * <p> Writes four bytes containing the given float value, in the
             * current byte order, into this buffer at the current position, and then
             * increments the position by four.  </p>
             *
             * @param  value
             *         The float value to be written
             *
             * @return  This buffer
             *
             * @throws  BufferOverflowException
             *          If there are fewer than four bytes
             *          remaining in this buffer
             *
             * @throws  ReadOnlyBufferException
             *          If this buffer is read-only
             */
            virtual ByteBuffer &putFloat(gfloat value) = 0;

            /**
             * Absolute <i>get</i> method for reading a float value.
             *
             * <p> Reads four bytes at the given index, composing them into a
             * float value according to the current byte order.  </p>
             *
             * @param  index
             *         The index from which the bytes will be read
             *
             * @return  The float value at the given index
             *
             * @throws  IndexException
             *          If <b> index</b> is negative
             *          or not smaller than the buffer's limit,
             *          minus three
             */
            virtual gfloat getFloat(gint index) const = 0;

            /**
             * Absolute <i>put</i> method for writing a float
             * value&nbsp;&nbsp;<i>(optional operation)</i>.
             *
             * <p> Writes four bytes containing the given float value, in the
             * current byte order, into this buffer at the given index.  </p>
             *
             * @param  index
             *         The index at which the bytes will be written
             *
             * @param  value
             *         The float value to be written
             *
             * @return  This buffer
             *
             * @throws  IndexException
             *          If <b> index</b> is negative
             *          or not smaller than the buffer's limit,
             *          minus three
             *
             * @throws  ReadOnlyBufferException
             *          If this buffer is read-only
             */
            virtual ByteBuffer &putFloat(gint index, gfloat value) = 0;

            /**
             * Creates a view of this byte buffer as a float buffer.
             *
             * <p> The content of the new buffer will start at this buffer's current
             * position.  Changes to this buffer's content will be visible in the new
             * buffer, and vice versa; the two buffers' position, limit, and mark
             * values will be independent.
             *
             * <p> The new buffer's position will be zero, its capacity and its limit
             * will be the number of bytes remaining in this buffer divided by
             * four, its mark will be undefined, and its byte order will be that
             * of the byte buffer at the moment the view is created.  The new buffer
             * will be direct if, and only if, this buffer is direct, and it will be
             * read-only if, and only if, this buffer is read-only.  </p>
             *
             * @return  A new float buffer
             */
            virtual FloatBuffer &asFloatBuffer() const = 0;


            /**
             * Relative <i>get</i> method for reading a double value.
             *
             * <p> Reads the next eight bytes at this buffer's current position,
             * composing them into a double value according to the current byte order,
             * and then increments the position by eight.  </p>
             *
             * @return  The double value at the buffer's current position
             *
             * @throws  BufferUnderflowException
             *          If there are fewer than eight bytes
             *          remaining in this buffer
             */
            virtual gdouble getDouble() = 0;

            /**
             * Relative <i>put</i> method for writing a double
             * value&nbsp;&nbsp;<i>(optional operation)</i>.
             *
             * <p> Writes eight bytes containing the given double value, in the
             * current byte order, into this buffer at the current position, and then
             * increments the position by eight.  </p>
             *
             * @param  value
             *         The double value to be written
             *
             * @return  This buffer
             *
             * @throws  BufferOverflowException
             *          If there are fewer than eight bytes
             *          remaining in this buffer
             *
             * @throws  ReadOnlyBufferException
             *          If this buffer is read-only
             */
            virtual ByteBuffer &putDouble(gdouble value) = 0;

            /**
             * Absolute <i>get</i> method for reading a double value.
             *
             * <p> Reads eight bytes at the given index, composing them into a
             * double value according to the current byte order.  </p>
             *
             * @param  index
             *         The index from which the bytes will be read
             *
             * @return  The double value at the given index
             *
             * @throws  IndexException
             *          If <b> index</b> is negative
             *          or not smaller than the buffer's limit,
             *          minus seven
             */
            virtual gdouble getDouble(gint index) const = 0;

            /**
             * Absolute <i>put</i> method for writing a double
             * value&nbsp;&nbsp;<i>(optional operation)</i>.
             *
             * <p> Writes eight bytes containing the given double value, in the
             * current byte order, into this buffer at the given index.  </p>
             *
             * @param  index
             *         The index at which the bytes will be written
             *
             * @param  value
             *         The double value to be written
             *
             * @return  This buffer
             *
             * @throws  IndexException
             *          If <b> index</b> is negative
             *          or not smaller than the buffer's limit,
             *          minus seven
             *
             * @throws  ReadOnlyBufferException
             *          If this buffer is read-only
             */
            virtual ByteBuffer &putDouble(gint index, gdouble value) = 0;

            /**
             * Creates a view of this byte buffer as a double buffer.
             *
             * <p> The content of the new buffer will start at this buffer's current
             * position.  Changes to this buffer's content will be visible in the new
             * buffer, and vice versa; the two buffers' position, limit, and mark
             * values will be independent.
             *
             * <p> The new buffer's position will be zero, its capacity and its limit
             * will be the number of bytes remaining in this buffer divided by
             * eight, its mark will be undefined, and its byte order will be that
             * of the byte buffer at the moment the view is created.  The new buffer
             * will be direct if, and only if, this buffer is direct, and it will be
             * read-only if, and only if, this buffer is read-only.  </p>
             *
             * @return  A new double buffer
             */
            virtual DoubleBuffer &asDoubleBuffer() const = 0;
        };

    } // core
} // io

#endif //CORE23_BYTEBUFFER_H

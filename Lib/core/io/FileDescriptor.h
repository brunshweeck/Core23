//
// Created by T.N.Brunshweeck on 29/12/2023.
//

#ifndef CORE23_FILEDESCRIPTOR_H
#define CORE23_FILEDESCRIPTOR_H

#include <core/Object.h>
#include <core/util/ArrayList.h>
#include "Closeable.h"

namespace core {
    namespace io {

        /**
         * Instances of the file descriptor class serve as an opaque handle
         * to the underlying machine-specific structure representing an open
         * file, an open socket, or another source or sink of bytes.
         * The main practical use for a file descriptor is to create a
         * <b style="color:orange;"> FileInputStream</b>  or <b style="color:orange;"> FileOutputStream</b>  to contain it.
         * <p>
         * Applications should not create their own file descriptors.
         *
         * @author  Brunshweeck Tazeussong
         */
        class FileDescriptor CORE_FINAL : public Object {
        private:
            gint fd = -1;

            glong handle = -1;

            util::ArrayList<Object> parents;

            /**
             * true, if file is closed.
             */
            gbool closed = false;

            /**
             * true, if file is opened for appending.
             */
            gbool append = false;

        public:

            /**
             * Constructs an (invalid) FileDescriptor object.
             * The fd or handle is set later.
             */
            FileDescriptor() = default;

        private:

            /**
             * Used for standard input, output, and error only.
             * For Windows the corresponding handle is initialized.
             * For Unix the append mode is cached.
             * @param fd the raw fd number (0, 1, 2)
             */
            CORE_EXPLICIT FileDescriptor(gint fd);

        public:

            /**
             * A handle to the standard input stream. Usually, this file
             * descriptor is not used directly, but rather via the input stream.
             */
            static const FileDescriptor in;

            /**
             * A handle to the standard output stream. Usually, this file
             * descriptor is not used directly, but rather via the output stream.
             */
            static const FileDescriptor out;

            /**
             * A handle to the standard error stream. Usually, this file
             * descriptor is not used directly, but rather via the output stream.
             */
            static const FileDescriptor err;

            /**
             * Tests if this file descriptor object is valid.
             *
             * @return  <b> true</b>  if the file descriptor object represents a
             *          valid, open file, socket, or other active I/O connection;
             *          <b> false</b>  otherwise.
             */
            gbool isValid() const;

            /**
             * Force all system buffers to synchronize with the underlying
             * device.  This method returns after all modified data and
             * attributes of this FileDescriptor have been written to the
             * relevant device(s).  In particular, if this FileDescriptor
             * refers to a physical storage medium, such as a file in a file
             * system, sync will not return until all in-memory modified copies
             * of buffers associated with this FileDescriptor have been
             * written to the physical medium.
             *
             * sync is meant to be used by code that requires physical
             * storage (such as a file) to be in a known state  For
             * example, a class that provided a simple transaction facility
             * might use sync to ensure that all changes to a file caused
             * by a given transaction were recorded on a storage medium.
             *
             * sync only affects buffers downstream of this FileDescriptor.  If
             * any in-memory buffering is being done by the application (for
             * example, by a BufferedOutputStream object), those buffers must
             * be flushed into the FileDescriptor (for example, by invoking
             * OutputStream.flush) before that data will be affected by sync.
             *
             * @throws    SyncFailedException
             *        Thrown when the buffers cannot be flushed,
             *        or because the system cannot guarantee that all the
             *        buffers have been synchronized with physical media.
             */
            static void synchronize();

        private:
            /*
             * On Windows return the handle for the standard streams.
             */
            glong createHandle(gint fd) const;

            /**
             * Returns true, if the file was opened for appending.
             */
            gbool checkAppendStatus(gint fd) const;

        protected:

            /**
             * Set the fd.
             * Used on Unix and for sockets on Windows and Unix.
             * If setting to -1, clear the cleaner.
             * The <b style="color:orange;"> #registerCleanup</b>  method should be called for new fds.
             * @param fd the raw fd or -1 to indicate closed
             */
            void set(gint fd);

            /**
             * Set the handle.
             * Used on Windows for regular files.
             * If setting to -1, clear the cleaner.
             * The <b style="color:orange;"> #registerCleanup</b>  method should be called for new handles.
             * @param handle the handle or -1 to indicate closed
             */
            void setHandle(glong handle);

            /**
             * Close the raw file descriptor or handle, if it has not already been closed.
             * The native code sets the fd and handle to -1.
             * Clear the cleaner so the close does not happen twice.
             * Package private to allow it to be used in java.io.
             * @throws IOException if close fails
             */
            void close();

            /**
             * Attach a Closeable to this FD for tracking.
             * parent reference is added to otherParents when
             * needed to make closeAll simpler.
             */
            void attach(const Closeable &c);

            /**
             * Cycle through all Closeables sharing this FD and call
             * close() on each one.
             *
             * The caller closeable gets to call.
             */
            void closeAll(Closeable &releaser);
        };

    } // io
} // core

#endif //CORE23_FILEDESCRIPTOR_H

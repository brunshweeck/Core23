//
// Created by T.N.Brunshweeck on 29/12/2023.
//

#include "FileDescriptor.h"
#include <core/io/IOException.h>

namespace core {
    namespace io {
        FileDescriptor::FileDescriptor(gint fd) : fd(fd), handle(createHandle(fd)), append(checkAppendStatus(fd)) {}

        const FileDescriptor FileDescriptor::in = FileDescriptor(0);
        const FileDescriptor FileDescriptor::out = FileDescriptor(1);
        const FileDescriptor FileDescriptor::err = FileDescriptor(2);

        gbool FileDescriptor::isValid() const {
            return fd != -1 && handle != -1;
        }

        void FileDescriptor::synchronize() {
            // windows/unix
        }

        glong FileDescriptor::createHandle(gint fd) const {
            // windows/unix op
            return fd;
        }

        gbool FileDescriptor::checkAppendStatus(gint fd) const {
            // windows/unix op
            return fd == 1 || fd == 2;
        }

        void FileDescriptor::set(gint fd) {
            // synchronized operation
        }

        void FileDescriptor::setHandle(glong handle) {
            if (handle == -1) {
                close();
            }
            this->handle = handle;
        }

        void FileDescriptor::close() {
            // synchronized operation
        }

        void FileDescriptor::attach(const Closeable &c) {
            // synchronized
            parents.add((const Object &) c);
        }

        void FileDescriptor::closeAll(Closeable &releaser) {
            // synchronized
            if(!closed){
                closed = true;
                IOException *ioe = null;
                try{
                    if (!parents.isEmpty()) {
                        for (Object &obj: parents) {
                            Closeable & closeable = (Closeable &) obj;
                            try{
                                closeable.close();
                            } catch (const IOException &ex1) {
                                if(ioe == null){
                                    ioe = &(IOException &) ex1;
                                }
                            }
                        }
                    }
                    releaser.close();
                } catch (const IOException &ex) {
                    ioe = &(IOException &) ex;
                }
                if(ioe != null)
                    ioe->throws(__trace("core.io.FileDescriptor"));
            }
        }

    } // io
} // core
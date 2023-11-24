//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_OBJECT_H
#define CORE23_OBJECT_H

#include <core/Class.h>

namespace core {

    /**
     * The Object class represent the base of the
     * class hierarchy in this framework.
     *
     * @author Brunshweeck Tazeussong
     */
    class Object {
    public:

        /**
         * Construct new Object instance.
         */
        CORE_FAST Object() CORE_NOTHROW = default;

        /**
         * Return true if this Object instance equals to
         * specified object. by default this method
         * compare the objects address and return true
         * if it are same.
         *
         * @param o
         *         The object to be compared
         */
        virtual gbool equals(const Object &o) const;

        /**
         * Return the sharable copy of this object.
         *
         * @throws MemoryError
         *         if memory allocation fail.
         */
        virtual Object &clone() const;

        /**
         * Return String representation of this object.
         * by default implementation return a String in with form: classname  @ address
         * (such as Class\@f71a)
         */
        virtual String toString() const;

        /**
         * Return true classname
         */
        String classname() const;

        /**
         * Return hash code of this class or 0 if this class is not
         * hashable
         */
        virtual gint hash() const;
    };

} // core

#endif //CORE23_OBJECT_H

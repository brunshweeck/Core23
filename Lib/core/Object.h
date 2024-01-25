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
         * Construct new Object INSTANCE.
         */
        CORE_FAST Object() CORE_NOTHROW = default;

        /**
         * Return true if this Object INSTANCE equals to
         * specified object. by default this method
         * compare the objects address and return true
         * if it are same.
         *
         * @param o
         *         The object to be compared
         */
        virtual gbool equals(const Object &o) const;

        /**
         * Creates and returns a copy of this object.  The precise meaning
         * of "copy" may depend on the class of the object. The general
         * intent is that, for any object <b>x</b>, the expression:
         * <blockquote>
         * <pre>
         * &x.clone() != &x</pre></blockquote>
         * will be true, and that the expression:
         * <blockquote>
         * <pre>
         * x.clone().classname() == x.classname()</pre></blockquote>
         * will be <b>true</b>, but these are not absolute requirements.
         * While it is typically the case that:
         * <blockquote>
         * <pre>
         * x.clone().equals(x)</pre></blockquote>
         * will be <b>true</b>, this is not an absolute requirement.
         * <p>
         * By convention, the object returned by this method should be independent
         * of this object (which is being cloned).  To achieve this independence,
         * it may be necessary to modify one or more fields of the object returned
         * by <b>super.clone</b> before returning it.  Typically, this means
         * copying any mutable objects that comprise the internal "deep structure"
         * of the object being cloned and replacing the references to these
         * objects with references to the copies.  If a class contains only
         * native fields or references to immutable objects, then it is usually
         * the case that no fields in the object returned by <b>super.clone</b>
         * need to be modified.
         *
         * @implSpec
         * The method <b>clone</b> for class <b>Object</b> performs a
         * specific cloning operation. First, if the class of this object does
         * not support cloning, then a <b>CloneNotSupportedException</b> is thrown.
         * Note that all arrays are considered cloneable and that
         * the return type of the <b>clone</b> method of an root type <b>T</b>
         * is root of type <b>T</b> where T is any reference or native type.
         * Otherwise, this method creates a new INSTANCE of the class of this
         * object and initializes all its fields with exactly the contents of
         * the corresponding fields of this object, as if by assignment; the
         * contents of the fields are not themselves cloned. Thus, this method
         * performs a "shallow copy" of this object, not a "deep copy" operation.
         * <p>
         * The class <b>Object</b> does not itself support cloning
         * so calling the <b>clone</b> method on an object
         * whose class is <b>Object</b> will result in throwing an
         * exception at run time.
         *
         * @return     a clone of this INSTANCE.
         * @throws  CloneNotSupportedException  if the object's class does not
         *               support the cloning operation. Subclasses
         *               that override the <b>clone</b> method can also
         *               throw this exception to indicate that an INSTANCE cannot
         *               be cloned.
         */
        virtual Object &clone() const;

        /**
         * Return String representation of this object.
         * by default implementation return a String in with form: classname  @ address
         * (such as Class\@f71a)
         */
        virtual String toString() const;

        /**
         * Return true classname of this object as String
         */
        String classname() const;

    private:
        /**
         * Return the classname of given object.
         */
        static String classname0(const Object& obj);

        /**
         * Return The identity hash code of given object
         */
         static gint identityHash0(const Object& obj);

    public:

        /**
         * Return hash code of this class or 0 if this class is not
         * hashable
         */
        virtual gint hash() const;

        /**
         * Returns <b>true</b> if the arguments are equal to each other
         * and <b>false</b> otherwise.
         * Consequently, if both arguments are <b>null</b>, <b>true</b>
         * is returned.  Otherwise, if the first argument is not <b>
         * null</b>, equality is determined by calling the <b style="color:orange;">
         * equals</b> method of the first argument with the
         * second argument of this method. Otherwise, <b>false</b> is
         * returned.
         *
         * @param a an object
         * @param b an object to be compared with <b>a</b> for equality
         * @return <b>true</b> if the arguments are equal to each other
         * and <b>false</b> otherwise
         * @see Object.equals(Object)
         */
        static gbool equals(const Object &a, const Object &b);

        /**
         * Returns the same hash code for the given object as
         * would be returned by the default method hash(),
         * whether or not the given object's class overrides
         * hash().
         * The hash code for the null reference is zero.
         *
         * @param x object for which the hash Code is to be calculated
         * @return  the hash Code
         */
        static gint identityHash(const Object& x) CORE_NOTHROW;

        /**
         * Destroy this object
         */
        virtual ~Object() = default;
    };

} // core

#endif //CORE23_OBJECT_H

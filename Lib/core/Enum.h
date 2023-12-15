//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_ENUM_H
#define CORE23_ENUM_H

#include <core/CastException.h>
#include <core/private/Unsafe.h>
#include <core/util/HashMap.h>

namespace core {

    template<class E>
    class Enum : public Object, public Comparable<Enum<E>> {
    private:
        CORE_STATIC_ASSERT(Class<E>::isEnum(), "This template is not Enumerable type");

        CORE_ALIAS(U, native::Unsafe);

        /**
         * The value of the Enum
         */
        E value;

    public:

        /**
         * Construct new Enum object with specified value
         * \param value The initial value
         */
        CORE_FAST Enum(E value) : value(value) {}

    protected:

        /**
         * Sole constructor.  Programmers cannot invoke this constructor.
         * It is for use by code emitted by the compiler in response to
         * enum class declarations.
         *
         * @param name - The name of this enum constant, which is the identifier
         *               used to declare it.
         * @param ordinal - The ordinal of this enumeration constant (its position
         *         in the enum declaration, where the initial constant is assigned
         *         an ordinal of zero).
         */
        CORE_EXPLICIT Enum(const String &name, gint ordinal) : Enum((E) ordinal) {
            Object &cache = U::systemCache();
            using util::HashMap;
            HashMap<String, Object> &properties = (HashMap<String, Object> &) cache;
            String cls = Enum::classname();
            HashMap<Enum, String> data = (HashMap<Enum, String> &)
                    properties.putIfAbsent(cls, HashMap<Enum, String>(12, 0.955));
            data.put(*this, name);
        }

    public:

        /**
         * Returns the ordinal of this enumeration constant (its position
         * in its enum declaration, where the initial constant is assigned
         * an ordinal of zero).
         *
         * Most programmers will have no use for this method.  It is
         * designed for use by sophisticated enum-based data structures.
         *
         * @return the ordinal of this enumeration constant
         */
        CORE_FAST gint ordinal() const { return ordinal(value); }

        /**
         * Returns the ordinal of this enumeration constant (its position
         * in its enum declaration, where the initial constant is assigned
         * an ordinal of zero).
         *
         * Most programmers will have no use for this method.  It is
         * designed for use by sophisticated enum-based data structures.
         *
         * @return the ordinal of this enumeration constant
         */
        static CORE_FAST gint ordinal(E value) { return (gint) value; }

        /**
         * Returns true if the specified object is equal to this
         * enum constant.
         *
         * @param object the object to be compared for equality with this object.
         */
        gbool equals(const Object &object) const override {
            if (Object::equals(object))
                return true;
            if (!Class<Enum>::hasInstance(object))
                return false;
            return ordinal() == ((Enum &) object).ordinal();
        }

        /**
         * Returns a hash code for this enum constant.
         */
        gint hash() const override { return ordinal(); }

        /**
         * Return shadow copy of this enum constant
         */
        Object &clone() const override {
            return U::createInstance<Enum>(*this);
        }

        /**
         * Compares this enum with the specified object for order.  Returns a
         * negative integer, zero, or a positive integer as this object is less
         * than, equal to, or greater than the specified object.
         *
         * Enum constants are only comparable to other enum constants of the
         * same enum type.  The natural order implemented by this
         * method is the order in which the constants are declared.
         */
        gint compareTo(const Enum &other) const override {
            return compare(value, other.value);
        }

        /**
         * Compares two enum constants for order.  Returns a
         * negative integer, zero, or a positive integer as this object is less
         * than, equal to, or greater than the specified object.
         *
         * Enum constants are only comparable to other enum constants of the
         * same enum type.  The natural order implemented by this
         * method is the order in which the constants are declared.
         */
        static CORE_FAST gint compare(E a, E b) {
            return ordinal(a) - ordinal(b);
        }

        /**
         * Return new Enum instance corresponding to specified
         * enum constant
         *
         * @param e The enum constant
         */
        static CORE_FAST Enum valueOf(E e) {
            return e;
        }

        /**
         * Return new Enum instance corresponding to specified
         * enum constant
         *
         * @param ordinal The ordinal
         */
        static CORE_FAST E forOrdinal(gint ordinal) {
            return (E) ordinal;
        }

        /**
         * Returns the enum constant of the specified enum class with the
         * specified name.  The name must match exactly an identifier used
         * to declare an enum constant in this class.  (Extraneous whitespace
         * characters are not permitted.)
         *
         * <p>Note that for a particular enum class <b> T</b>, the
         * implicitly declared <b> static Enum valueOf(String)</b>
         * method on that enum may be used instead of this method to map
         * from a name to the corresponding enum constant.
         *
         * @param name the name of the constant to return
         *
         * @throws ArgumentException if the specified enum class has
         *         no constant with the specified name, or the specified
         *         class object does not represent an enum class
         */
        static Enum valueOf(const String &name) {
            Object &cache = U::systemCache();
            using util::HashMap;
            HashMap<String, Object> &properties = (HashMap<String, Object> &) cache;
            String cls = Enum::classname();
            HashMap<Enum, String> data = (HashMap<Enum, String> &)
                    properties.putIfAbsent(cls, HashMap<Enum, String>(12, 0.955));
            Enum &e = data.getOrDefault(name, {});
        }

        /**
         * Returns the name of this enum constant, as contained in the
         * declaration.  This method may be overridden, though it typically
         * isn't necessary or desirable.  An enum class should override this
         * method when a more "programmer-friendly" string form exists.
         *
         * @return the name of this enum constant
         */
        String toString() const override { return toString(value); }

        static String toString(E value) {
            Enum v = valueOf(value);
            Object &cache = U::systemCache();
            using util::HashMap;
            HashMap<String, Object> &properties = (HashMap<String, Object> &) cache;
            String cls = v.classname();
            HashMap<Enum, String> data = (HashMap<Enum, String> &)
                    properties.putIfAbsent(cls, HashMap<Enum, String>(12, 0.955));
            for (const auto &e: data.entrySet()) {
                if(e.key() == value)
                    return e.value();
            }
            // String representation not found
            // we will create for next usage
            String fallback = String::valueOf(ordinal(value));
            data.putIfAbsent(v, fallback);
            return fallback;
        }

        /**
         * Evaluate numerically two enum constants with logical AND
         *
         * @param o1 The first operand
         * @param o2 The second operand
         */
        static CORE_FAST E logicalAnd(E o1, E o2) {
            return (E) (ordinal(o1) & ordinal(o2));
        }

        /**
         * Evaluate numerically two enum constants with logical Or
         *
         * @param o1 The first operand
         * @param o2 The second operand
         */
        static CORE_FAST E logicalOr(E o1, E o2) {
            return (E) (ordinal(o1) | ordinal(o2));
        }

        /**
         * Evaluate numerically two enum constants with logical Xor
         *
         * @param o1 The first operand
         * @param o2 The second operand
         */
        static CORE_FAST E logicalXor(E o1, E o2) {
            return (E) (ordinal(o1) & ordinal(o2));
        }

        CORE_ENABLE_IMPLICIT_CAST(E, value, const);
        CORE_ENABLE_IMPLICIT_CAST(E &, value);
    };

} // core

#endif //CORE23_ENUM_H

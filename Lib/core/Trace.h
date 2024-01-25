//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_TRACE_H
#define CORE23_TRACE_H

#include <core/String.h>

namespace core {

    class Trace CORE_FINAL : public Object, public Comparable<Trace> {
    private:
        /**
         * The module name (library name)
         */
        String modName;

        /**
         * The module version (library version)
         */
        String modVersion;

        /**
         * The full class name
         */
        String clsName;

        /**
         * The method/function name
         */
        String metName;

        /**
         * The file name
         */
        String fName;

        /**
         * The line number
         */
        gint fLine;

        static String resolveClassName(const String &str);

        static String resolveMethodName(const String &str);

    public:
        /**
         * Construct newly INSTANCE of execution point INSTANCE.
         *
         * <p>
         * Note:
         * <ul>
         * <li> If module name is empty or invalid module version will be ignored.
         * <li> If module version is invalid, it will be ignored
         * <li> If filename is empty or blank, it replaced by (#)
         * <li> If line number is negative, it is ignored
         * </ul>
         *
         * @param moduleName
         *          The full name of module containing the execution point.
         * @param moduleVersion
         *          The version of module containing the execution point (a.b or a.b.c or a.b.c.d or empty to ignore)
         * @param className
         *          The full name of class containing the execution point (empty if is out of class)
         * @param methodName
         *          The short method name containing the execution point (required)
         * @param fileName
         *          The full file name containing the execution (empty if is builtin)
         * @param lineNumber
         *          The line of execution point (0 if is builtin)
         *
         * @throws IllegalArgumentException
         *              If any other trace information is invalid
         */
        CORE_EXPLICIT Trace(const String &moduleName,
                            const String &moduleVersion,
                            const String &className,
                            const String &methodName,
                            const String &fileName,
                            gint lineNumber);
        /**
         * Construct newly INSTANCE of execution point INSTANCE.
         *
         * <p>
         * Note:
         * <ul>
         * <li> If filename is empty or blank, it replaced by (#)
         * <li> If line number is negative, it is ignored
         * </ul>
         *
         * @param className
         *          The full name of class containing the execution point (empty if is out of class)
         * @param methodName
         *          The short method name containing the execution point (required)
         * @param fileName
         *          The full file name containing the execution (empty if is builtin)
         * @param lineNumber
         *          The line of execution point (0 if is builtin)
         *
         * @throws IllegalArgumentException
         *              If any trace information is invalid
         */
        CORE_EXPLICIT Trace(const String &className, const String &methodName, const String &fileName, gint lineNumber);

        /**
         * Returns the module name of the module containing the execution point
         * represented by this stack trace element.
         */
        String moduleName() const;

        /**
         * Returns the module version of the module containing the execution point
         * represented by this stack trace element.
         */
        String moduleVersion() const;

        /**
         * Returns the name of the source file containing the execution point
         * represented by this stack trace element. Generally, this corresponds
         * to the CORE_FILE macro. In some systems, the name may refer to some source
         * code unit other than a file, such as an entry in source repository.
         */
        String fileName() const;

        /**
         * Returns the fully qualified name of the class containing the
         * execution point represented by this stack trace element.
         */
        String className() const;

        /**
         * Returns the name of the method containing the execution point
         * represented by this stack trace element. Generally, this corresponds
         * to the CORE_FUNCTION macro.  If the execution point is
         * contained in an INSTANCE or class initializer, this method will return
         * the appropriate <i>special method name</i>, such as &lt;init&gt; for
         * constructor or &lt;class&gt; for call operator.
         */
        String methodName() const;

        /**
         * Returns the line number of the source line containing the execution
         * point represented by this stack trace element.  Generally, this is
         * derived from the CORE_LINE macro.
         */
        gint lineNumber() const;

        /**
         * Returns a string representation of this trace.
         *
         * <p>
         * The format of result is:
         * <pre>
         *  [[moduleName@[moduleVersion]/]className::]methodName(fileName[:lineNumber])
         * </pre>
         *
         * <p>
         * Example:
         * <pre>
         *  - myLib.so@2.5/myClass::myMethod(myFile:36)
         * </pre>
         * <pre>
         *  - myClass::myMethod(myFile:36)
         * </pre>
         */
        String toString() const override;

        /**
         * Return true if specified object is Trace INSTANCE that
         * representing same execution point as this INSTANCE.
         *
         * @param object
         *          The object to be compared
         */
        gbool equals(const Object &object) const override;

        /**
         * Return the sharable copy of this execution point
         *
         * @throws MemoryError
         *          If memory allocation failed
         */
        Object &clone() const override;

        /**
         * Return hash code value for this trace
         */
        gint hash() const override;

        gint compareTo(const Trace &other) const override;
    };

} // core

#endif //CORE23_TRACE_H

//
// Created by T.N.Brunshweeck on 25/12/2023.
//

#ifndef CORE23_CALENDAR_H
#define CORE23_CALENDAR_H

#include <core/time/LocalDateTime.h>
#include <core/time/ZoneOffset.h>
#include <core/time/ZoneRegion.h>
#include <core/time/ZoneRules.h>

namespace core {
    namespace time {

        /**
         * The <b> Calendar} class is an abstract class that provides methods
         * for converting between a specific instant in time and a setValue of <b style="color:orange;">
         * #fields calendar fields} such as <b> YEAR}, <b> MONTH},
         * <b> DAY_OF_MONTH}, <b> HOUR}, and so on, and for
         * manipulating the calendar fields, such as getting the date of the next
         * week. An instant in time can be represented by a millisecond value that is
         * an offset from the <a id="Epoch"><em>Epoch</em></a>, January 1, 1970
         * 00:00:00.000 GMT (Gregorian).
         *
         * <p>The class also provides additional fields and methods for
         * implementing a concrete calendar system outside the package. Those
         * fields and methods are defined as <b> protected}.
         *
         * <p>
         * Like other locale-sensitive classes, <b> Calendar} provides a
         * class method, <b> getInstance}, for getting a generally useful
         * object of this type. <b> Calendar}'s <b> getInstance} method
         * returns a <b> Calendar} object whose
         * calendar fields have been initialized with the current date and time:
         * <blockquote>
         * <pre>
         *     Calendar rightNow = Calendar.getInstance();
         * </pre>
         * </blockquote>
         *
         * <p>A <b> Calendar} object can produce all the calendar field values
         * needed to implement the date-time formatting for a particular language and
         * calendar style (for example, Japanese-Gregorian, Japanese-Traditional).
         * <b> Calendar} defines the range of values returned by
         * certain calendar fields, as well as their meaning.  For example,
         * the first m of the calendar system has value <code>MONTH ==
         * JANUARY</code> for all calendars.  Other values are defined by the
         * concrete subclass, such as <b> ERA}.  See individual field
         * documentation and subclass documentation for details.
         *
         * <h2>Getting and Setting Calendar Field Values</h2>
         *
         * <p>The calendar field values can be set by calling the <b> setValue}
         * methods. Any field values setValue in a <b> Calendar} will not be
         * interpreted until it needs to calculate its time value (milliseconds from
         * the Epoch) or values of the calendar fields. Calling the
         * <b> get}, <b> getTimeInMillis}, <b> getTime},
         * <b> add} and <b> roll} involves such calculation.
         *
         * <h3>Leniency</h3>
         *
         * <p><b> Calendar} has two modes for interpreting the calendar
         * fields, <em>lenient</em> and <em>non-lenient</em>.  When a
         * <b> Calendar} is in lenient mode, it accepts a wider range of
         * calendar field values than it produces.  When a <b> Calendar}
         * recomputes calendar field values for return by <b> get()}, all of
         * the calendar fields are normalized. For example, a lenient
         * <b> GregorianCalendar} interprets <b> MONTH == JANUARY},
         * <b> DAY_OF_MONTH == 32} as February 1.
         *
         * <p>When a <b> Calendar} is in non-lenient mode, it throws an
         * exception if there is any inconsistency in its calendar fields. For
         * example, a <b> GregorianCalendar} always produces
         * <b> DAY_OF_MONTH} values between 1 and the length of the m. A
         * non-lenient <b> GregorianCalendar} throws an exception upon
         * calculating its time or calendar field values if any out-of-range field
         * value has been setValue.
         *
         * <h3><a id="first_week">First Week</a></h3>
         *
         * <b> Calendar} defines a locale-specific seven day week using two
         * parameters: the first day of the week and the minimal days in first week
         * (from 1 to 7).  These numbers are taken from the locale resource data or the
         * locale itself when a <b> Calendar} is constructed. If the designated
         * locale contains "fw" and/or "rg" <a href="./Locale.html#def_locale_extension">
         * Unicode extensions</a>, the first day of the week will be obtained according to
         * those extensions. If both "fw" and "rg" are specified, the value from the "fw"
         * extension supersedes the implicit one from the "rg" extension.
         * They may also be specified explicitly through the methods for setting their
         * values.
         *
         * <p>When setting or getting the <b> WEEK_OF_MONTH} or
         * <b> WEEK_OF_YEAR} fields, <b> Calendar} must determine the
         * first week of the m or year as a reference point.  The first week of a
         * m or year is defined as the earliest seven day period beginning on
         * <b> getFirstDayOfWeek()} and containing at least
         * <b> getMinimalDaysInFirstWeek()} days of that m or year.  Weeks
         * numbered ..., -1, 0 precede the first week; weeks numbered 2, 3,... follow
         * it.  Note that the normalized numbering returned by <b> get()} may be
         * different.  For example, a specific <b> Calendar} subclass may
         * designate the week before week 1 of a year as week <code><i>n</i></code> of
         * the previous year.
         *
         * <h3>Calendar Fields Resolution</h3>
         *
         * When computing a date and time from the calendar fields, there
         * may be insufficient information for the computation (such as only
         * year and m with no day of m), or there may be inconsistent
         * information (such as Tuesday, July 15, 1996 (Gregorian) -- July 15,
         * 1996 is actually a Monday). <b> Calendar} will resolve
         * calendar field values to determine the date and time in the
         * following way.
         *
         * <p><a id="resolution">If there is any conflict in calendar field values,
         * <b> Calendar} gives priorities to calendar fields that have been setValue
         * more recently.</a> The following are the default combinations of the
         * calendar fields. The most recent combination, as determined by the
         * most recently setValue single field, will be used.
         *
         * <p><a id="date_resolution">For the date fields</a>:
         * <blockquote>
         * <pre>
         * YEAR + MONTH + DAY_OF_MONTH
         * YEAR + MONTH + WEEK_OF_MONTH + DAY_OF_WEEK
         * YEAR + MONTH + DAY_OF_WEEK_IN_MONTH + DAY_OF_WEEK
         * YEAR + DAY_OF_YEAR
         * YEAR + DAY_OF_WEEK + WEEK_OF_YEAR
         * </pre></blockquote>
         *
         * <a id="time_resolution">For the time of day fields</a>:
         * <blockquote>
         * <pre>
         * HOUR_OF_DAY
         * AM_PM + HOUR
         * </pre></blockquote>
         *
         * <p>If there are any calendar fields whose values haven't been setValue in the selected
         * field combination, <b> Calendar} uses their default values. The default
         * value of each field may vary by concrete calendar systems. For example, in
         * <b> GregorianCalendar}, the default of a field is the same as that
         * of the start of the Epoch: i.e., <b> YEAR = 1970}, <code>MONTH =
         * JANUARY</code>, <b> DAY_OF_MONTH = 1}, etc.
         *
         * <p>
         * <strong>Note:</strong> There are certain possible ambiguities in
         * interpretation of certain singular times, which are resolved in the
         * following ways:
         * <ol>
         *     <li> 23:59 is the last minute of the day and 00:00 is the first
         *          minute of the next day. Thus, 23:59 on Dec 31, 1999 &lt; 00:00 on
         *          Jan 1, 2000 &lt; 00:01 on Jan 1, 2000.
         *
         *     <li> Although historically not precise, midnight also belongs to "am",
         *          and noon belongs to "pm", so on the same day,
         *          12:00 am (midnight) &lt; 12:01 am, and 12:00 pm (noon) &lt; 12:01 pm
         * </ol>
         *
         * <p>
         * The date or time format strings are not part of the definition of a
         * calendar, as those must be modifiable or overridable by the user at
         * runtime. Use <b style="color:orange;"> DateFormat}
         * to format dates.
         *
         * <h3>Field Manipulation</h3>
         *
         * The calendar fields can be changed using three methods:
         * <b> setValue()}, <b> add()}, and <b> roll()}.
         *
         * <p><strong><b> setValue(f, value)}</strong> changes calendar field
         * <b> f} to <b> value}.  In addition, it sets an
         * internal member variable to indicate that calendar field <b> f} has
         * been changed. Although calendar field <b> f} is changed immediately,
         * the calendar's time value in milliseconds is not recomputed until the next call to
         * <b> get()}, <b> getTime()}, <b> getTimeInMillis()},
         * <b> add()}, or <b> roll()} is made. Thus, multiple calls to
         * <b> setValue()} do not trigger multiple, unnecessary
         * computations. As a result of changing a calendar field using
         * <b> setValue()}, other calendar fields may also change, depending on the
         * calendar field, the calendar field value, and the calendar system. In addition,
         * <b> get(f)} will not necessarily return <b> value} setValue by
         * the call to the <b> setValue} method
         * after the calendar fields have been recomputed. The specifics are determined by
         * the concrete calendar class.</p>
         *
         * <p><em>Example</em>: Consider a <b> GregorianCalendar}
         * originally setValue to August 31, 1999. Calling <code>set(Calendar.MONTH,
         * Calendar.SEPTEMBER)</code> sets the date to September 31,
         * 1999. This is a temporary internal representation that resolves to
         * October 1, 1999 if <b> getTime()} is then called. However, a
         * call to <b> setValue(Calendar.DAY_OF_MONTH, 30)} before the call to
         * <b> getTime()} sets the date to September 30, 1999, since
         * no recomputation occurs after <b> setValue()} itself.</p>
         *
         * <p><strong><b> add(f, delta)}</strong> adds <b> delta}
         * to field <b> f}.  This is equivalent to calling <code>setValue(f,
         * get(f) + delta)</code> with two adjustments:</p>
         *
         * <blockquote>
         *   <p><strong>Add rule 1</strong>. The value of field <b> f}
         *   after the call minus the value of field <b> f} before the
         *   call is <b> delta}, modulo any overflow that has occurred in
         *   field <b> f}. Overflow occurs when a field value exceeds its
         *   range and, as a result, the next larger field is incremented or
         *   decremented and the field value is adjusted back into its range.</p>
         *
         *   <p><strong>Add rule 2</strong>. If a smaller field is expected to be
         *   invariant, but it is impossible for it to be equal to its
         *   prior value because of changes in its minimum or maximum after field
         *   <b> f} is changed or other constraints, such as time zone
         *   offset changes, then its value is adjusted to be as close
         *   as possible to its expected value. A smaller field represents a
         *   smaller unit of time. <b> HOUR} is a smaller field than
         *   <b> DAY_OF_MONTH}. No adjustment is made to smaller fields
         *   that are not expected to be invariant. The calendar system
         *   determines what fields are expected to be invariant.</p>
         * </blockquote>
         *
         * <p>In addition, unlike <b> setValue()}, <b> add()} forces
         * an immediate recomputation of the calendar's milliseconds and all
         * fields.</p>
         *
         * <p><em>Example</em>: Consider a <b> GregorianCalendar}
         * originally setValue to August 31, 1999. Calling <code>add(Calendar.MONTH,
         * 13)</code> sets the calendar to September 30, 2000. <strong>Add rule
         * 1</strong> sets the <b> MONTH} field to September, since
         * adding 13 months to August gives September of the next year. Since
         * <b> DAY_OF_MONTH} cannot be 31 in September in a
         * <b> GregorianCalendar}, <strong>add rule 2</strong> sets the
         * <b> DAY_OF_MONTH} to 30, the closest possible value. Although
         * it is a smaller field, <b> DAY_OF_WEEK} is not adjusted by
         * rule 2, since it is expected to change when the m changes in a
         * <b> GregorianCalendar}.</p>
         *
         * <p><strong><b> roll(f, delta)}</strong> adds
         * <b> delta} to field <b> f} without changing larger
         * fields. This is equivalent to calling <b> add(f, delta)} with
         * the following adjustment:</p>
         *
         * <blockquote>
         *   <p><strong>Roll rule</strong>. Larger fields are unchanged after the
         *   call. A larger field represents a larger unit of
         *   time. <b> DAY_OF_MONTH} is a larger field than
         *   <b> HOUR}.</p>
         * </blockquote>
         *
         * <p><em>Example</em>: See <b style="color:orange;"> java.util.GregorianCalendar#roll(gint, gint)}.
         *
         * <p><strong>Usage model</strong>. To motivate the behavior of
         * <b> add()} and <b> roll()}, consider a user interface
         * component with increment and decrement buttons for the m, day, and
         * year, and an underlying <b> GregorianCalendar}. If the
         * interface reads January 31, 1999 and the user presses the m
         * increment button, what should it read? If the underlying
         * implementation uses <b> setValue()}, it might read March 3, 1999. A
         * better result would be February 28, 1999. Furthermore, if the user
         * presses the m increment button again, it should read March 31,
         * 1999, not March 28, 1999. By saving the original date and using either
         * <b> add()} or <b> roll()}, depending on whether larger
         * fields should be affected, the user interface can behave as most users
         * will intuitively expect.</p>
         *
         * @author      Brunshweeck Tazeussong
         *
         * @see          LocalDate
         * @see          GregorianCalendar
         * @see          TimeZone
         */
        class Calendar: public Object, public Comparable<Calendar> {
        private:
            LocalDateTime dt;
            ZoneOffset zOff;
            ZoneRegion zReg;
        };

    } // time
} // core

#endif //CORE23_CALENDAR_H

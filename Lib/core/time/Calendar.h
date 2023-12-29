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
         * The {@code Calendar} class is an abstract class that provides methods
         * for converting between a specific instant in time and a set of {@link
         * #fields calendar fields} such as {@code YEAR}, {@code MONTH},
         * {@code DAY_OF_MONTH}, {@code HOUR}, and so on, and for
         * manipulating the calendar fields, such as getting the date of the next
         * week. An instant in time can be represented by a millisecond value that is
         * an offset from the <a id="Epoch"><em>Epoch</em></a>, January 1, 1970
         * 00:00:00.000 GMT (Gregorian).
         *
         * <p>The class also provides additional fields and methods for
         * implementing a concrete calendar system outside the package. Those
         * fields and methods are defined as {@code protected}.
         *
         * <p>
         * Like other locale-sensitive classes, {@code Calendar} provides a
         * class method, {@code getInstance}, for getting a generally useful
         * object of this type. {@code Calendar}'s {@code getInstance} method
         * returns a {@code Calendar} object whose
         * calendar fields have been initialized with the current date and time:
         * <blockquote>
         * <pre>
         *     Calendar rightNow = Calendar.getInstance();
         * </pre>
         * </blockquote>
         *
         * <p>A {@code Calendar} object can produce all the calendar field values
         * needed to implement the date-time formatting for a particular language and
         * calendar style (for example, Japanese-Gregorian, Japanese-Traditional).
         * {@code Calendar} defines the range of values returned by
         * certain calendar fields, as well as their meaning.  For example,
         * the first m of the calendar system has value <code>MONTH ==
         * JANUARY</code> for all calendars.  Other values are defined by the
         * concrete subclass, such as {@code ERA}.  See individual field
         * documentation and subclass documentation for details.
         *
         * <h2>Getting and Setting Calendar Field Values</h2>
         *
         * <p>The calendar field values can be set by calling the {@code set}
         * methods. Any field values set in a {@code Calendar} will not be
         * interpreted until it needs to calculate its time value (milliseconds from
         * the Epoch) or values of the calendar fields. Calling the
         * {@code get}, {@code getTimeInMillis}, {@code getTime},
         * {@code add} and {@code roll} involves such calculation.
         *
         * <h3>Leniency</h3>
         *
         * <p>{@code Calendar} has two modes for interpreting the calendar
         * fields, <em>lenient</em> and <em>non-lenient</em>.  When a
         * {@code Calendar} is in lenient mode, it accepts a wider range of
         * calendar field values than it produces.  When a {@code Calendar}
         * recomputes calendar field values for return by {@code get()}, all of
         * the calendar fields are normalized. For example, a lenient
         * {@code GregorianCalendar} interprets {@code MONTH == JANUARY},
         * {@code DAY_OF_MONTH == 32} as February 1.
         *
         * <p>When a {@code Calendar} is in non-lenient mode, it throws an
         * exception if there is any inconsistency in its calendar fields. For
         * example, a {@code GregorianCalendar} always produces
         * {@code DAY_OF_MONTH} values between 1 and the length of the m. A
         * non-lenient {@code GregorianCalendar} throws an exception upon
         * calculating its time or calendar field values if any out-of-range field
         * value has been set.
         *
         * <h3><a id="first_week">First Week</a></h3>
         *
         * {@code Calendar} defines a locale-specific seven day week using two
         * parameters: the first day of the week and the minimal days in first week
         * (from 1 to 7).  These numbers are taken from the locale resource data or the
         * locale itself when a {@code Calendar} is constructed. If the designated
         * locale contains "fw" and/or "rg" <a href="./Locale.html#def_locale_extension">
         * Unicode extensions</a>, the first day of the week will be obtained according to
         * those extensions. If both "fw" and "rg" are specified, the value from the "fw"
         * extension supersedes the implicit one from the "rg" extension.
         * They may also be specified explicitly through the methods for setting their
         * values.
         *
         * <p>When setting or getting the {@code WEEK_OF_MONTH} or
         * {@code WEEK_OF_YEAR} fields, {@code Calendar} must determine the
         * first week of the m or year as a reference point.  The first week of a
         * m or year is defined as the earliest seven day period beginning on
         * {@code getFirstDayOfWeek()} and containing at least
         * {@code getMinimalDaysInFirstWeek()} days of that m or year.  Weeks
         * numbered ..., -1, 0 precede the first week; weeks numbered 2, 3,... follow
         * it.  Note that the normalized numbering returned by {@code get()} may be
         * different.  For example, a specific {@code Calendar} subclass may
         * designate the week before week 1 of a year as week <code><i>n</i></code> of
         * the previous year.
         *
         * <h3>Calendar Fields Resolution</h3>
         *
         * When computing a date and time from the calendar fields, there
         * may be insufficient information for the computation (such as only
         * year and m with no day of m), or there may be inconsistent
         * information (such as Tuesday, July 15, 1996 (Gregorian) -- July 15,
         * 1996 is actually a Monday). {@code Calendar} will resolve
         * calendar field values to determine the date and time in the
         * following way.
         *
         * <p><a id="resolution">If there is any conflict in calendar field values,
         * {@code Calendar} gives priorities to calendar fields that have been set
         * more recently.</a> The following are the default combinations of the
         * calendar fields. The most recent combination, as determined by the
         * most recently set single field, will be used.
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
         * <p>If there are any calendar fields whose values haven't been set in the selected
         * field combination, {@code Calendar} uses their default values. The default
         * value of each field may vary by concrete calendar systems. For example, in
         * {@code GregorianCalendar}, the default of a field is the same as that
         * of the start of the Epoch: i.e., {@code YEAR = 1970}, <code>MONTH =
         * JANUARY</code>, {@code DAY_OF_MONTH = 1}, etc.
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
         * runtime. Use {@link DateFormat}
         * to format dates.
         *
         * <h3>Field Manipulation</h3>
         *
         * The calendar fields can be changed using three methods:
         * {@code set()}, {@code add()}, and {@code roll()}.
         *
         * <p><strong>{@code set(f, value)}</strong> changes calendar field
         * {@code f} to {@code value}.  In addition, it sets an
         * internal member variable to indicate that calendar field {@code f} has
         * been changed. Although calendar field {@code f} is changed immediately,
         * the calendar's time value in milliseconds is not recomputed until the next call to
         * {@code get()}, {@code getTime()}, {@code getTimeInMillis()},
         * {@code add()}, or {@code roll()} is made. Thus, multiple calls to
         * {@code set()} do not trigger multiple, unnecessary
         * computations. As a result of changing a calendar field using
         * {@code set()}, other calendar fields may also change, depending on the
         * calendar field, the calendar field value, and the calendar system. In addition,
         * {@code get(f)} will not necessarily return {@code value} set by
         * the call to the {@code set} method
         * after the calendar fields have been recomputed. The specifics are determined by
         * the concrete calendar class.</p>
         *
         * <p><em>Example</em>: Consider a {@code GregorianCalendar}
         * originally set to August 31, 1999. Calling <code>set(Calendar.MONTH,
         * Calendar.SEPTEMBER)</code> sets the date to September 31,
         * 1999. This is a temporary internal representation that resolves to
         * October 1, 1999 if {@code getTime()} is then called. However, a
         * call to {@code set(Calendar.DAY_OF_MONTH, 30)} before the call to
         * {@code getTime()} sets the date to September 30, 1999, since
         * no recomputation occurs after {@code set()} itself.</p>
         *
         * <p><strong>{@code add(f, delta)}</strong> adds {@code delta}
         * to field {@code f}.  This is equivalent to calling <code>set(f,
         * get(f) + delta)</code> with two adjustments:</p>
         *
         * <blockquote>
         *   <p><strong>Add rule 1</strong>. The value of field {@code f}
         *   after the call minus the value of field {@code f} before the
         *   call is {@code delta}, modulo any overflow that has occurred in
         *   field {@code f}. Overflow occurs when a field value exceeds its
         *   range and, as a result, the next larger field is incremented or
         *   decremented and the field value is adjusted back into its range.</p>
         *
         *   <p><strong>Add rule 2</strong>. If a smaller field is expected to be
         *   invariant, but it is impossible for it to be equal to its
         *   prior value because of changes in its minimum or maximum after field
         *   {@code f} is changed or other constraints, such as time zone
         *   offset changes, then its value is adjusted to be as close
         *   as possible to its expected value. A smaller field represents a
         *   smaller unit of time. {@code HOUR} is a smaller field than
         *   {@code DAY_OF_MONTH}. No adjustment is made to smaller fields
         *   that are not expected to be invariant. The calendar system
         *   determines what fields are expected to be invariant.</p>
         * </blockquote>
         *
         * <p>In addition, unlike {@code set()}, {@code add()} forces
         * an immediate recomputation of the calendar's milliseconds and all
         * fields.</p>
         *
         * <p><em>Example</em>: Consider a {@code GregorianCalendar}
         * originally set to August 31, 1999. Calling <code>add(Calendar.MONTH,
         * 13)</code> sets the calendar to September 30, 2000. <strong>Add rule
         * 1</strong> sets the {@code MONTH} field to September, since
         * adding 13 months to August gives September of the next year. Since
         * {@code DAY_OF_MONTH} cannot be 31 in September in a
         * {@code GregorianCalendar}, <strong>add rule 2</strong> sets the
         * {@code DAY_OF_MONTH} to 30, the closest possible value. Although
         * it is a smaller field, {@code DAY_OF_WEEK} is not adjusted by
         * rule 2, since it is expected to change when the m changes in a
         * {@code GregorianCalendar}.</p>
         *
         * <p><strong>{@code roll(f, delta)}</strong> adds
         * {@code delta} to field {@code f} without changing larger
         * fields. This is equivalent to calling {@code add(f, delta)} with
         * the following adjustment:</p>
         *
         * <blockquote>
         *   <p><strong>Roll rule</strong>. Larger fields are unchanged after the
         *   call. A larger field represents a larger unit of
         *   time. {@code DAY_OF_MONTH} is a larger field than
         *   {@code HOUR}.</p>
         * </blockquote>
         *
         * <p><em>Example</em>: See {@link java.util.GregorianCalendar#roll(int, int)}.
         *
         * <p><strong>Usage model</strong>. To motivate the behavior of
         * {@code add()} and {@code roll()}, consider a user interface
         * component with increment and decrement buttons for the m, day, and
         * year, and an underlying {@code GregorianCalendar}. If the
         * interface reads January 31, 1999 and the user presses the m
         * increment button, what should it read? If the underlying
         * implementation uses {@code set()}, it might read March 3, 1999. A
         * better result would be February 28, 1999. Furthermore, if the user
         * presses the m increment button again, it should read March 31,
         * 1999, not March 28, 1999. By saving the original date and using either
         * {@code add()} or {@code roll()}, depending on whether larger
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

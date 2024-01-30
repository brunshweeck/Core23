//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_GENERATOR_H
#define CORE23_GENERATOR_H

#include <core/Object.h>

namespace core {
    namespace random {

        /**
         * The {@link RandomGenerator} interface is designed to provide a common
         * protocol for objects that generate random or (more typically) pseudorandom
         * sequences of numbers (or Boolean values). Such a sequence may be obtained by
         * either repeatedly invoking a method that returns a single pseudo-randomly
         * chosen value, or by invoking a method that returns a stream of
         * pseudo-randomly chosen values.
         *
         * <p> Ideally, given an implicitly or explicitly specified range of values,
         * each value would be chosen independently and uniformly from that range. In
         * practice, one may have to settle for some approximation to independence and
         * uniformity.
         *
         * <p> In the case of {@code int}, {@code long}, and {@code boolean} values, if
         * there is no explicit specification of range, then the range includes all
         * possible values of the type. In the case of {@code float} and {@code double}
         * values, first a value is always chosen uniformly from the setValue of
         * 2<sup><i>w</i></sup> values between 0.0 (inclusive) and 1.0 (exclusive),
         * where <i>w</i> is {@link Float#PRECISION} for {@code float} values
         * and {@link Double#PRECISION} for {@code double}
         * values, such that adjacent values differ by 2<sup>&minus;<i>w</i></sup>
         * (notice that this set is a <i>subset</i> of the setValue of
         * <i>all representable floating-point values</i> between 0.0 (inclusive) and 1.0 (exclusive));
         * then if an explicit range was specified, then the chosen number is
         * computationally scaled and translated so as to appear to have been chosen
         * approximately uniformly from that explicit range.
         *
         * <p> Each method that returns a stream produces a stream of values each of
         * which is chosen in the same manner as for a method that returns a single
         * pseudorandomly chosen value. For example, if {@code r} implements
         * {@link RandomGenerator}, then the method call {@code r.ints(100)} returns a
         * stream of 100 {@code int} values. These are not necessarily the exact same
         * values that would have been returned if instead {@code r.nextInt()} had been
         * called 100 times; all that is guaranteed is that each value in the stream is
         * chosen in a similar pseudorandom manner from the same range.
         *
         * <p> Every object that implements the {@link RandomGenerator} interface by
         * using a pseudorandom algorithm is assumed to contain a finite amount of
         * state. Using such an object to generate a pseudorandomly chosen value alters
         * its state by computing a new state as a function of the current state,
         * without reference to any information other than the current state. The number
         * of distinct possible states of such an object is called its <i>period</i>.
         * (Some implementations of the {@link RandomGenerator} interface may be truly
         * random rather than pseudorandom, for example relying on the statistical
         * behavior of a physical object to derive chosen values. Such implementations
         * do not have a fixed period.)
         *
         * <p> As a rule, objects that implement the {@link RandomGenerator} interface
         * need not be thread-safe. It is recommended that multithreaded applications
         * use either {@link ThreadLocalRandom} or (preferably) pseudorandom number
         * generators that implement the {@link SplittableGenerator} or
         * {@link JumpableGenerator} interface.
         *
         * <p> Objects that implement {@link RandomGenerator} are typically not
         * cryptographically secure. Consider instead using {@link SecureRandom} to get
         * a cryptographically secure pseudorandom number generator for use by
         * security-sensitive applications. Note, however, that {@link SecureRandom}
         * does implement the {@link RandomGenerator} interface, so that instances of
         * {@link SecureRandom} may be used interchangeably with other types of
         * pseudorandom generators in applications that do not require a secure
         * generator.
         *
         * <p>Unless explicit stated otherwise, the use of null for any method argument
         * will cause a NullPointerException.
         *
         */
        class RandomGenerator: public Object {

        };

    } // core
} // random

#endif //CORE23_GENERATOR_H

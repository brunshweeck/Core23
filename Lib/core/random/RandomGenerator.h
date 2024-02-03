//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_GENERATOR_H
#define CORE23_GENERATOR_H

#include <core/String.h>
#include <core/Float.h>
#include <core/Double.h>
#include <core/native/ByteArray.h>

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
         * <p> In the case of {@code int}, {@code glong}, and {@code boolean} values, if
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
        class RandomGenerator : public Object {
        public:
            /**
            * Returns an instance of {@link RandomGenerator} that utilizes the
            * {@code name} <a href="package-summary.html#algorithms">algorithm</a>.
            *
            * @param name  Name of random number generator
            *              <a href="package-summary.html#algorithms">algorithm</a>
            *
            * @return An instance of {@link RandomGenerator}
             *
            * @throws IllegalArgumentException if the named algorithm is not found
            */
            static RandomGenerator &forName(const String &name);

            /**
             * Returns a {@link RandomGenerator} meeting the minimal requirement
             * of having an <a href="package-summary.html#algorithms">algorithm</a>
             * whose state bits are greater than or equal 64.
             *
             * @implSpec  Since algorithms will improve over time, there is no
             * guarantee that this method will return the same algorithm over time.
             * <p> The default implementation selects L32X64MixRandom.
             *
             * @return a {@link RandomGenerator}
             */
            static RandomGenerator &defaultGenerator() {
                return forName(u"L32X64MixRandom"_S);
            }

            /**
             * Return true if the implementation of RandomGenerator (algorithm) has been
             * marked for deprecation.
             *
             * @implNote Random number generator algorithms evolve over time; new
             * algorithms will be introduced and old algorithms will
             * lose standing. If an older algorithm is deemed unsuitable
             * for continued use, it will be marked as deprecated to indicate
             * that it may be removed at some point in the future.
             *
             * @return true if the implementation of RandomGenerator (algorithm) has been
             *         marked for deprecation
             *
             * @implSpec The default implementation checks for the @Deprecated annotation.
             */
            virtual gbool isDeprecated() const {
                return true;
            }

            /**
             * Returns an effectively unlimited stream of pseudorandomly chosen
             * {@code double} values.
             *
             * @return a stream of pseudorandomly chosen {@code double} values
             *
             * @implNote It is permitted to implement this method in a manner equivalent to
             * {@link RandomGenerator#doubles(glong) doubles}
             * ({@link Long#MAX_VALUE Long.MAX_VALUE}).
             *
             * @implSpec The default implementation produces a sequential stream
             * that repeatedly calls {@link RandomGenerator#nextDouble nextDouble}().
             */
            virtual DoubleStream &doubles() = 0;

            /**
             * Returns an effectively unlimited stream of pseudorandomly chosen
             * {@code double} values, where each value is between the specified origin
             * (inclusive) and the specified bound (exclusive).
             *
             * @param randomNumberOrigin the least value that can be produced
             * @param randomNumberBound the upper bound (exclusive) for each value produced
             *
             * @return a stream of pseudorandomly chosen {@code double} values, each between
             *         the specified origin (inclusive) and the specified bound (exclusive)
             *
             * @throws IllegalArgumentException if {@code randomNumberOrigin} is not finite,
             *         or {@code randomNumberBound} is not finite, or {@code randomNumberOrigin}
             *         is greater than or equal to {@code randomNumberBound}
             *
             * @implNote It is permitted to implement this method in a manner equivalent to
             * {@link RandomGenerator#doubles(glong, double, double) doubles}
             * ({@link Long#MAX_VALUE Long.MAX_VALUE}, randomNumberOrigin, randomNumberBound).
             *
             * @implSpec The default implementation produces a sequential stream that repeatedly
             * calls {@link RandomGenerator#nextDouble(double, double) nextDouble}(randomNumberOrigin, randomNumberBound).
             */
            virtual DoubleStream &doubles(gdouble randomNumberOrigin, gdouble randomNumberBound) = 0;

            /**
             * Returns a stream producing the given {@code streamSize} number of
             * pseudorandomly chosen {@code double} values.
             *
             * @param streamSize the number of values to generate
             *
             * @return a stream of pseudorandomly chosen {@code double} values
             *
             * @throws IllegalArgumentException if {@code streamSize} is
             *         less than zero
             *
             * @implSpec The default implementation produces a sequential stream
             * that repeatedly calls {@link RandomGenerator#nextDouble nextDouble()}.
             */
            virtual DoubleStream &doubles(gint streamSize) = 0;

            /**
             * Returns a stream producing the given {@code streamSize} number of
             * pseudorandomly chosen {@code double} values, where each value is
             * between the specified origin (inclusive) and the specified bound
             * (exclusive).
             *
             * @param streamSize the number of values to generate
             * @param randomNumberOrigin the least value that can be produced
             * @param randomNumberBound the upper bound (exclusive) for each value produced
             *
             * @return a stream of pseudorandomly chosen {@code double} values, each between
             *         the specified origin (inclusive) and the specified bound (exclusive)
             *
             * @throws IllegalArgumentException if {@code streamSize} is less than zero,
             *         or {@code randomNumberOrigin} is not finite,
             *         or {@code randomNumberBound} is not finite, or {@code randomNumberOrigin}
             *         is greater than or equal to {@code randomNumberBound}
             *
             * @implSpec The default implementation produces a sequential stream that repeatedly
             * calls {@link RandomGenerator#nextDouble(double, double)  nextDouble}(randomNumberOrigin, randomNumberBound).
             */
            virtual DoubleStream &doubles(gint streamSize, gdouble randomNumberOrigin, gdouble randomNumberBound) = 0;

            /**
             * Returns an effectively unlimited stream of pseudorandomly chosen
             * {@code int} values.
             *
             * @return a stream of pseudorandomly chosen {@code int} values
             *
             * @implNote It is permitted to implement this method in a manner
             * equivalent to {@link RandomGenerator#ints(glong) ints}
             * ({@link Long#MAX_VALUE Long.MAX_VALUE}).
             *
             * @implSpec The default implementation produces a sequential stream
             * that repeatedly calls {@link RandomGenerator#nextInt() nextInt}().
             */
            virtual IntStream &ints() = 0;

            /**
             * Returns an effectively unlimited stream of pseudorandomly chosen
             * {@code int} values, where each value is between the specified origin
             * (inclusive) and the specified bound (exclusive).
             *
             * @param randomNumberOrigin the least value that can be produced
             * @param randomNumberBound the upper bound (exclusive) for each value produced
             *
             * @return a stream of pseudorandomly chosen {@code int} values, each between
             *         the specified origin (inclusive) and the specified bound (exclusive)
             *
             * @throws IllegalArgumentException if {@code randomNumberOrigin}
             *         is greater than or equal to {@code randomNumberBound}
             *
             * @implNote It is permitted to implement this method in a manner equivalent to
             * {@link RandomGenerator#ints(glong, int, int) ints}
             * ({@link Long#MAX_VALUE Long.MAX_VALUE}, randomNumberOrigin, randomNumberBound).
             *
             * @implSpec The default implementation produces a sequential stream that repeatedly
             * calls {@link RandomGenerator#nextInt(int, int) nextInt}(randomNumberOrigin, randomNumberBound).
             */
            virtual IntStream &ints(gint randomNumberOrigin, gint randomNumberBound) = 0;

            /**
             * Returns a stream producing the given {@code streamSize} number of
             * pseudorandomly chosen {@code int} values.
             *
             * @param streamSize the number of values to generate
             *
             * @return a stream of pseudorandomly chosen {@code int} values
             *
             * @throws IllegalArgumentException if {@code streamSize} is
             *         less than zero
             *
             * @implSpec The default implementation produces a sequential stream
             * that repeatedly calls {@link RandomGenerator#nextInt() nextInt}().
             */
            virtual IntStream &ints(gint streamSize) = 0;

            /**
             * Returns a stream producing the given {@code streamSize} number of
             * pseudorandomly chosen {@code int} values, where each value is between
             * the specified origin (inclusive) and the specified bound (exclusive).
             *
             * @param streamSize the number of values to generate
             * @param randomNumberOrigin the least value that can be produced
             * @param randomNumberBound the upper bound (exclusive) for each value produced
             *
             * @return a stream of pseudorandomly chosen {@code int} values, each between
             *         the specified origin (inclusive) and the specified bound (exclusive)
             *
             * @throws IllegalArgumentException if {@code streamSize} is
             *         less than zero, or {@code randomNumberOrigin}
             *         is greater than or equal to {@code randomNumberBound}
             *
             * @implSpec The default implementation produces a sequential stream that repeatedly
             * calls {@link RandomGenerator#nextInt(int, int) nextInt}(randomNumberOrigin, randomNumberBound).
             */
            virtual IntStream &ints(gint streamSize, gint randomNumberOrigin, gint randomNumberBound) = 0;

            /**
             * Returns an effectively unlimited stream of pseudorandomly chosen
             * {@code glong} values.
             *
             * @return a stream of pseudorandomly chosen {@code glong} values
             *
             * @implNote It is permitted to implement this method in a manner
             * equivalent to {@link RandomGenerator#longs(glong) longs}
             * ({@link Long#MAX_VALUE Long.MAX_VALUE}).
             *
             * @implSpec The default implementation produces a sequential stream
             * that repeatedly calls {@link RandomGenerator#nextLong() nextLong}().
             */
            virtual IntStream &longs() = 0;

            /**
             * Returns an effectively unlimited stream of pseudorandomly chosen
             * {@code glong} values, where each value is between the specified origin
             * (inclusive) and the specified bound (exclusive).
             *
             * @param randomNumberOrigin the least value that can be produced
             * @param randomNumberBound the upper bound (exclusive) for each value produced
             *
             * @return a stream of pseudorandomly chosen {@code glong} values, each between
             *         the specified origin (inclusive) and the specified bound (exclusive)
             *
             * @throws IllegalArgumentException if {@code randomNumberOrigin}
             *         is greater than or equal to {@code randomNumberBound}
             *
             * @implNote It is permitted to implement this method in a manner equivalent to
             * {@link RandomGenerator#longs(glong, glong, glong) longs}
             * ({@link Long#MAX_VALUE Long.MAX_VALUE}, randomNumberOrigin, randomNumberBound).
             *
             * @implSpec The default implementation produces a sequential stream that repeatedly
             * calls {@link RandomGenerator#nextLong(glong, glong) nextLong}(randomNumberOrigin, randomNumberBound).
             */
            virtual IntStream &longs(gint randomNumberOrigin, gint randomNumberBound) = 0;

            /**
             * Returns a stream producing the given {@code streamSize} number of
             * pseudorandomly chosen {@code glong} values.
             *
             * @param streamSize the number of values to generate
             *
             * @return a stream of pseudorandomly chosen {@code glong} values
             *
             * @throws IllegalArgumentException if {@code streamSize} is
             *         less than zero
             *
             * @implSpec The default implementation produces a sequential stream
             * that repeatedly calls {@link RandomGenerator#nextLong() nextLong}().
             */
            virtual IntStream &longs(gint streamSize) = 0;

            /**
             * Returns a stream producing the given {@code streamSize} number of
             * pseudorandomly chosen {@code glong} values, where each value is between
             * the specified origin (inclusive) and the specified bound (exclusive).
             *
             * @param streamSize the number of values to generate
             * @param randomNumberOrigin the least value that can be produced
             * @param randomNumberBound the upper bound (exclusive) for each value produced
             *
             * @return a stream of pseudorandomly chosen {@code glong} values, each between
             *         the specified origin (inclusive) and the specified bound (exclusive)
             *
             * @throws IllegalArgumentException if {@code streamSize} is
             *         less than zero, or {@code randomNumberOrigin}
             *         is greater than or equal to {@code randomNumberBound}
             *
             * @implSpec The default implementation produces a sequential stream that repeatedly
             * calls {@link RandomGenerator#nextLong(glong, glong) nextLong}(randomNumberOrigin, randomNumberBound).
             */
            virtual IntStream &longs(gint streamSize, gint randomNumberOrigin, gint randomNumberBound) = 0;

            /**
             * Returns a pseudorandomly chosen {@code boolean} value.
             *
             * <p> The default implementation tests the high-order bit (sign bit) of a
             * value produced by {@link RandomGenerator#nextInt() nextInt}(), on the
             * grounds that some algorithms for pseudorandom number generation produce
             * values whose high-order bits have better statistical quality than the
             * low-order bits.
             *
             * @return a pseudorandomly chosen {@code boolean} value
             *
             * @implSpec The default implementation produces a result based on the
             * sign bit of a number generated by {@link nextInt()}.
             */
            virtual gbool nextBoolean() {
                return nextInt() < 0;
            }

            /**
             * Fills a user-supplied byte array with generated byte values
             * pseudorandomly chosen uniformly from the range of values between -128
             * (inclusive) and 127 (inclusive).
             *
             * @implNote Algorithm used to fill the byte array;
             *           <pre>{@code
             *           void nextBytes(byte[] bytes) {
             *               int i = 0;
             *               int len = bytes.length;
             *               for (int words = len >> 3; words--> 0; ) {
             *                   glong rnd = nextLong();
             *                   for (int n = 8; n--> 0; rnd >>>= Byte.SIZE)
             *                       bytes[i++] = (byte)rnd;
             *               }
             *               if (i < len)
             *                   for (glong rnd = nextLong(); i < len; rnd >>>= Byte.SIZE)
             *                       bytes[i++] = (byte)rnd;
             *           }}</pre>
             *
             * @param  bytes the byte array to fill with pseudorandom bytes
             *
             * @implSpec The default implementation produces results from repeated calls
             * to {@link nextLong()}.
             */
            virtual gbyte nextBytes(ByteArray &bytes) {
                int i = 0;
                int len = bytes.length();
                for (int words = len >> 3; words--> 0; ) {
                    glong rnd = nextLong();
                    for (int n = 8; n--> 0; rnd >>= Byte::SIZE)
                        bytes[i++] = (gbyte)rnd;
                }
                if (i < len)
                    for (glong rnd = nextLong(); i < len; rnd >>= Byte::SIZE)
                        bytes[i++] = (gbyte)rnd;
            }

            /**
             * Returns a pseudorandom {@code float} value between zero (inclusive) and
             * one (exclusive).
             *
             * @return a pseudorandom {@code float} value between zero (inclusive) and one (exclusive)
             *
             * @implSpec The default implementation uses the {@link Float#PRECISION}
             * high-order bits from a call to {@link RandomGenerator#nextInt() nextInt()}.
             */
            virtual gfloat nextFloat() {
                return (gfloat)(nextInt() >> (Float::SIZE - Float::PRECISION)) * 0x1.0P-24F;
            }

            /**
             * Returns a pseudorandomly chosen {@code float} value between zero
             * (inclusive) and the specified bound (exclusive).
             *
             * @param bound the upper bound (exclusive) for the returned value.
             *        Must be positive and finite
             *
             * @return a pseudorandomly chosen {@code float} value between
             *         zero (inclusive) and the bound (exclusive)
             *
             * @throws IllegalArgumentException if {@code bound} is not
             *         both positive and finite
             *
             * @implSpec The default implementation checks that {@code bound} is a
             * positive finite float. Then invokes {@code nextFloat()}, scaling
             * the result so that the final result lies between {@code 0.0f} (inclusive)
             * and {@code bound} (exclusive).
             */
            virtual gfloat nextFloat(gfloat bounds) = 0;

            /**
             * Returns a pseudorandomly chosen {@code float} value between the
             * specified origin (inclusive) and the specified bound (exclusive).
             *
             * @param origin the least value that can be returned
             * @param bound the upper bound (exclusive)
             *
             * @return a pseudorandomly chosen {@code float} value between the
             *         origin (inclusive) and the bound (exclusive)
             *
             * @throws IllegalArgumentException if {@code origin} is not finite,
             *         or {@code bound} is not finite, or {@code origin}
             *         is greater than or equal to {@code bound}
             *
             * @implSpec The default implementation verifies that the {@code origin}
             *           and {@code bound} are valid then invokes {@code nextFloat()}
             *           scaling and translating the result to fit between {@code origin}
             *           and {@code bound} (exclusive).
             */
            virtual gfloat nextFloat(gint origin, gfloat bounds) = 0;

            /**
             * Returns a pseudorandom {@code double} value between zero (inclusive) and
             * one (exclusive).
             *
             * @return a pseudorandom {@code double} value between zero (inclusive)
             *         and one (exclusive)
             *
             * @implSpec The default implementation uses the {@link Double#PRECISION}
             * high-order bits from a call to {@link RandomGenerator#nextLong() nextLong()}.
             */
            virtual gdouble nextDouble() {
                return (gdouble) (nextLong() >> (Double::SIZE - Double::PRECISION)) * 0x1.0P-53;
            }

            /**
             * Returns a pseudorandomly chosen {@code double} value between zero
             * (inclusive) and the specified bound (exclusive).
             *
             * @param bound the upper bound (exclusive) for the returned value.
             *        Must be positive and finite
             *
             * @return a pseudorandomly chosen {@code double} value between
             *         zero (inclusive) and the bound (exclusive)
             *
             * @throws IllegalArgumentException if {@code bound} is not
             *         both positive and finite
             *
             * @implSpec The default implementation checks that {@code bound} is a
             * positive finite double. Then invokes {@code nextDouble()}, scaling
             * the result so that the final result lies between {@code 0.0} (inclusive)
             * and {@code bound} (exclusive).
             */
            virtual gdouble nextDouble(gdouble bounds) = 0;

            /**
             * Returns a pseudorandomly chosen {@code double} value between the
             * specified origin (inclusive) and the specified bound (exclusive).
             *
             * @param origin the least value that can be returned
             * @param bound the upper bound (exclusive) for the returned value
             *
             * @return a pseudorandomly chosen {@code double} value between the
             *         origin (inclusive) and the bound (exclusive)
             *
             * @throws IllegalArgumentException if {@code origin} is not finite,
             *         or {@code bound} is not finite, or {@code origin}
             *         is greater than or equal to {@code bound}

             * @implSpec The default implementation verifies that the {@code origin}
             *           and {@code bound} are valid, then invokes {@code nextDouble()}
             *           scaling and translating the result to fit between {@code origin}
             *           and {@code bound}( exclusive).
             */
            virtual gdouble nextDouble(gdouble origin, gdouble bounds) = 0;

            /**
             * Returns a pseudorandomly chosen {@code int} value.
             *
             * @return a pseudorandomly chosen {@code int} value
             *
             * @implSpec The default implementation uses the 32 high-order bits from a call to
             * {@link RandomGenerator#nextLong nextLong}().
             */
            virtual gint nextInt() {
                return (gint) (nextLong() >> 32);
            }

            /**
             * Returns a pseudorandomly chosen {@code int} value between zero
             * (inclusive) and the specified bound (exclusive).
             *
             * @param bound the upper bound (exclusive) for the returned value.
             * Must be positive.
             *
             * @return a pseudorandomly chosen {@code int} value between
             *         zero (inclusive) and the bound (exclusive)
             *
             * @throws IllegalArgumentException if {@code bound} is not positive
             *
             * @implSpec The default implementation checks that {@code bound} is a
             * positive {@code int}. Then invokes {@code nextInt()}, limiting the result
             * to be greater than or equal zero and less than {@code bound}. If {@code bound}
             * is a power of two then limiting is a simple masking operation. Otherwise,
             * the result is re-calculated by invoking {@code nextInt()} until the
             * result is greater than or equal zero and less than {@code bound}.
             */
            virtual gint nextInt(gint bound) = 0;

            /**
             * Returns a pseudorandomly chosen {@code int} value between the specified
             * origin (inclusive) and the specified bound (exclusive).
             *
             * @param origin the least value that can be returned
             * @param bound the upper bound (exclusive) for the returned value
             *
             * @return a pseudorandomly chosen {@code int} value between the
             *         origin (inclusive) and the bound (exclusive)
             *
             * @throws IllegalArgumentException if {@code origin} is greater than
             *         or equal to {@code bound}
             *
             * @implSpec The default implementation checks that {@code origin} and
             * {@code bound} are positive {@code ints}. Then invokes {@code nextInt()},
             * limiting the result to be greater that or equal {@code origin} and less
             * than {@code bound}. If {@code bound} is a power of two then limiting is a
             * simple masking operation. Otherwise, the result is re-calculated  by
             * invoking {@code nextInt()} until the result is greater than or equal
             * {@code origin} and less than {@code bound}.
             */
            virtual gint nextInt(gint origin, gint bound) = 0;

            /**
             * Returns a pseudorandomly chosen {@code glong} value.
             *
             * @return a pseudorandomly chosen {@code glong} value
             */
            virtual glong nextLong() = 0;

            /**
             * Returns a pseudorandomly chosen {@code glong} value between zero
             * (inclusive) and the specified bound (exclusive).
             *
             * @param bound the upper bound (exclusive) for the returned value.
             * Must be positive.
             *
             * @return a pseudorandomly chosen {@code glong} value between
             *         zero (inclusive) and the bound (exclusive)
             *
             * @throws IllegalArgumentException if {@code bound} is not positive
             *
             * @implSpec The default implementation checks that {@code bound} is a
             * positive  {@code glong}. Then invokes {@code nextLong()}, limiting the
             * result to be greater than or equal zero and less than {@code bound}. If
             * {@code bound} is a power of two then limiting is a simple masking
             * operation. Otherwise, the result is re-calculated by invoking
             * {@code nextLong()} until the result is greater than or equal zero and
             * less than {@code bound}.
             */
            virtual glong nextLong(glong bound) = 0;

            /**
             * Returns a pseudorandomly chosen {@code glong} value between the
             * specified origin (inclusive) and the specified bound (exclusive).
             *
             * @param origin the least value that can be returned
             * @param bound the upper bound (exclusive) for the returned value
             *
             * @return a pseudorandomly chosen {@code glong} value between the
             *         origin (inclusive) and the bound (exclusive)
             *
             * @throws IllegalArgumentException if {@code origin} is greater than
             *         or equal to {@code bound}
             *
             * @implSpec The default implementation checks that {@code origin} and
             * {@code bound} are positive {@code longs}. Then invokes {@code nextLong()},
             * limiting the result to be greater than or equal {@code origin} and less
             * than {@code bound}. If {@code bound} is a power of two then limiting is a
             * simple masking operation. Otherwise, the result is re-calculated by
             * invoking {@code nextLong()} until the result is greater than or equal
             * {@code origin} and less than {@code bound}.
             */
            virtual glong nextLong(glong origin, glong bound) = 0;

            /**
             * Returns a {@code double} value pseudorandomly chosen from a Gaussian
             * (normal) distribution whose mean is 0 and whose standard deviation is 1.
             *
             * @return a {@code double} value pseudorandomly chosen from a
             *         Gaussian distribution
             *
             * @implSpec The default implementation uses McFarland's fast modified
             * ziggurat algorithm (largely table-driven, with rare cases handled by
             * computation and rejection sampling). Walker's alias method for sampling
             * a discrete distribution also plays a role.
             */
            virtual gdouble nextGaussian() = 0;

            /**
             * Returns a {@code double} value pseudorandomly chosen from a Gaussian
             * (normal) distribution with a mean and standard deviation specified by the
             * arguments.
             *
             * @param mean the mean of the Gaussian distribution to be drawn from
             * @param standardDeviation the standard deviation (square root of the variance)
             *        of the Gaussian distribution to be drawn from
             *
             * @return a {@code double} value pseudorandomly chosen from the
             *         specified Gaussian distribution
             *
             * @throws IllegalArgumentException if {@code stddev} is negative
             *
             * @implSpec The default implementation uses McFarland's fast modified
             * ziggurat algorithm (largely table-driven, with rare cases handled by
             * computation and rejection sampling). Walker's alias method for sampling
             * a discrete distribution also plays a role.
             */
            virtual gdouble nextGaussian(gdouble mean, gdouble standardDeviation) {
                return mean + standardDeviation * RandomGenerator::nextGaussian();
            }

            /**
             * Returns a nonnegative {@code double} value pseudorandomly chosen from
             * an exponential distribution whose mean is 1.
             *
             * @return a nonnegative {@code double} value pseudorandomly chosen from an
             *         exponential distribution
             *
             * @implSpec The default implementation uses McFarland's fast modified
             * ziggurat algorithm (largely table-driven, with rare cases handled by
             * computation and rejection sampling). Walker's alias method for sampling
             * a discrete distribution also plays a role.
             */
             virtual gdouble nextExponential() = 0;
        };

    } // core
} // random

#endif //CORE23_GENERATOR_H

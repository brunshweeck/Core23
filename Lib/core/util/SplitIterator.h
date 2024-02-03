//
// Created by T.N.Brunshweeck on 02/02/2024.
//

#ifndef CORE23_SPLITITERATOR_H
#define CORE23_SPLITITERATOR_H

#include <core/function/Consumer.h>

namespace core {
    namespace util {

        /**
         * An object for traversing and partitioning elements of a source.  The source
         * of elements covered by a Spliterator could be, for example, an array, a
         * {@link Collection}, an IO channel, or a generator function.
         *
         * <p>A Spliterator may traverse elements individually ({@link
         * #tryAdvance tryAdvance()}) or sequentially in bulk
         * ({@link #forEachRemaining forEachRemaining()}).
         *
         * <p>A Spliterator may also partition off some of its elements (using
         * {@link #trySplit}) as another Spliterator, to be used in
         * possibly-parallel operations.  Operations using a Spliterator that
         * cannot split, or does so in a highly imbalanced or inefficient
         * manner, are unlikely to benefit from parallelism.  Traversal
         * and splitting exhaust elements; each Spliterator is useful for only a single
         * bulk computation.
         *
         * <p>A Spliterator also reports a set of {@link #characteristics()} of its
         * structure, source, and elements from among {@link #ORDERED},
         * {@link #DISTINCT}, {@link #SORTED}, {@link #SIZED}, {@link #NONNULL},
         * {@link #IMMUTABLE}, {@link #CONCURRENT}, and {@link #SUBSIZED}. These may
         * be employed by Spliterator clients to control, specialize or simplify
         * computation.  For example, a Spliterator for a {@link Collection} would
         * report {@code SIZED}, a Spliterator for a {@link Set} would report
         * {@code DISTINCT}, and a Spliterator for a {@link SortedSet} would also
         * report {@code SORTED}.  Characteristics are reported as a simple unioned bit
         * set.
         *
         * Some characteristics additionally constrain method behavior; for example if
         * {@code ORDERED}, traversal methods must conform to their documented ordering.
         * New characteristics may be defined in the future, so implementors should not
         * assign meanings to unlisted values.
         *
         * <p><a id="binding">A Spliterator that does not report {@code IMMUTABLE} or
         * {@code CONCURRENT} is expected to have a documented policy concerning:
         * when the spliterator <em>binds</em> to the element source; and detection of
         * structural interference of the element source detected after binding.</a>  A
         * <em>late-binding</em> Spliterator binds to the source of elements at the
         * point of first traversal, first split, or first query for estimated size,
         * rather than at the time the Spliterator is created.  A Spliterator that is
         * not <em>late-binding</em> binds to the source of elements at the point of
         * construction or first invocation of any method.  Modifications made to the
         * source prior to binding are reflected when the Spliterator is traversed.
         * After binding a Spliterator should, on a best-effort basis, throw
         * {@link ConcurrentModificationException} if structural interference is
         * detected.  Spliterators that do this are called <em>fail-fast</em>.  The
         * bulk traversal method ({@link #forEachRemaining forEachRemaining()}) of a
         * Spliterator may optimize traversal and check for structural interference
         * after all elements have been traversed, rather than checking per-element and
         * failing immediately.
         *
         * <p>Spliterators can provide an estimate of the number of remaining elements
         * via the {@link #estimateSize} method.  Ideally, as reflected in characteristic
         * {@link #SIZED}, this value corresponds exactly to the number of elements
         * that would be encountered in a successful traversal.  However, even when not
         * exactly known, an estimated value may still be useful to operations
         * being performed on the source, such as helping to determine whether it is
         * preferable to split further or traverse the remaining elements sequentially.
         *
         * <p>Despite their obvious utility in parallel algorithms, spliterators are not
         * expected to be thread-safe; instead, implementations of parallel algorithms
         * using spliterators should ensure that the spliterator is only used by one
         * thread at a time.  This is generally easy to attain via <em>serial
         * thread-confinement</em>, which often is a natural consequence of typical
         * parallel algorithms that work by recursive decomposition.  A thread calling
         * {@link #trySplit()} may hand over the returned Spliterator to another thread,
         * which in turn may traverse or further split that Spliterator.  The behaviour
         * of splitting and traversal is undefined if two or more threads operate
         * concurrently on the same spliterator.  If the original thread hands a
         * spliterator off to another thread for processing, it is best if that handoff
         * occurs before any elements are consumed with {@link #tryAdvance(Consumer)
         * tryAdvance()}, as certain guarantees (such as the accuracy of
         * {@link #estimateSize()} for {@code SIZED} spliterators) are only valid before
         * traversal has begun.
         *
         * <p>Primitive subtype specializations of {@code Spliterator} are provided for
         * {@link OfInt int}, {@link OfLong long}, and {@link OfDouble double} values.
         * The subtype default implementations of
         * {@link Spliterator#tryAdvance(java.util.function.Consumer)}
         * and {@link Spliterator#forEachRemaining(java.util.function.Consumer)} box
         * primitive values to instances of their corresponding wrapper class.  Such
         * boxing may undermine any performance advantages gained by using the primitive
         * specializations.  To avoid boxing, the corresponding primitive-based methods
         * should be used.  For example,
         * {@link Spliterator.OfInt#tryAdvance(java.util.function.IntConsumer)}
         * and {@link Spliterator.OfInt#forEachRemaining(java.util.function.IntConsumer)}
         * should be used in preference to
         * {@link Spliterator.OfInt#tryAdvance(java.util.function.Consumer)} and
         * {@link Spliterator.OfInt#forEachRemaining(java.util.function.Consumer)}.
         * Traversal of primitive values using boxing-based methods
         * {@link #tryAdvance tryAdvance()} and
         * {@link #forEachRemaining(java.util.function.Consumer) forEachRemaining()}
         * does not affect the order in which the values, transformed to boxed values,
         * are encountered.
         *
         * @apiNote
         * <p>Spliterators, like {@code Iterator}s, are for traversing the elements of
         * a source.  The {@code Spliterator} API was designed to support efficient
         * parallel traversal in addition to sequential traversal, by supporting
         * decomposition as well as single-element iteration.  In addition, the
         * protocol for accessing elements via a Spliterator is designed to impose
         * smaller per-element overhead than {@code Iterator}, and to avoid the inherent
         * race involved in having separate methods for {@code hasNext()} and
         * {@code next()}.
         *
         * <p>For mutable sources, arbitrary and non-deterministic behavior may occur if
         * the source is structurally interfered with (elements added, replaced, or
         * removed) between the time that the Spliterator binds to its data source and
         * the end of traversal.  For example, such interference will produce arbitrary,
         * non-deterministic results when using the {@code java.util.stream} framework.
         *
         * <p>Structural interference of a source can be managed in the following ways
         * (in approximate order of decreasing desirability):
         * <ul>
         * <li>The source cannot be structurally interfered with.
         * <br>For example, an instance of
         * {@link java.util.concurrent.CopyOnWriteArrayList} is an immutable source.
         * A Spliterator created from the source reports a characteristic of
         * {@code IMMUTABLE}.</li>
         * <li>The source manages concurrent modifications.
         * <br>For example, a key set of a {@link java.util.concurrent.ConcurrentHashMap}
         * is a concurrent source.  A Spliterator created from the source reports a
         * characteristic of {@code CONCURRENT}.</li>
         * <li>The mutable source provides a late-binding and fail-fast Spliterator.
         * <br>Late binding narrows the window during which interference can affect
         * the calculation; fail-fast detects, on a best-effort basis, that structural
         * interference has occurred after traversal has commenced and throws
         * {@link ConcurrentModificationException}.  For example, {@link ArrayList},
         * and many other non-concurrent {@code Collection} classes in the JDK, provide
         * a late-binding, fail-fast spliterator.</li>
         * <li>The mutable source provides a non-late-binding but fail-fast Spliterator.
         * <br>The source increases the likelihood of throwing
         * {@code ConcurrentModificationException} since the window of potential
         * interference is larger.</li>
         * <li>The mutable source provides a late-binding and non-fail-fast Spliterator.
         * <br>The source risks arbitrary, non-deterministic behavior after traversal
         * has commenced since interference is not detected.
         * </li>
         * <li>The mutable source provides a non-late-binding and non-fail-fast
         * Spliterator.
         * <br>The source increases the risk of arbitrary, non-deterministic behavior
         * since non-detected interference may occur after construction.
         * </li>
         * </ul>
         *
         * <p><b>Example.</b> Here is a class (not a very useful one, except
         * for illustration) that maintains an array in which the actual data
         * are held in even locations, and unrelated tag data are held in odd
         * locations. Its Spliterator ignores the tags.
         *
         * <pre> {@code
         * class TaggedArray<T> {
         *   private final Object[] elements; // immutable after construction
         *   TaggedArray(T[] data, Object[] tags) {
         *     int size = data.length;
         *     if (tags.length != size) throw new IllegalArgumentException();
         *     this.elements = new Object[2 * size];
         *     for (int i = 0, j = 0; i < size; ++i) {
         *       elements[j++] = data[i];
         *       elements[j++] = tags[i];
         *     }
         *   }
         *
         *   public Spliterator<T> spliterator() {
         *     return new TaggedArraySpliterator<>(elements, 0, elements.length);
         *   }
         *
         *   static class TaggedArraySpliterator<T> implements Spliterator<T> {
         *     private final Object[] array;
         *     private int origin; // current index, advanced on split or traversal
         *     private final int fence; // one past the greatest index
         *
         *     TaggedArraySpliterator(Object[] array, int origin, int fence) {
         *       this.array = array; this.origin = origin; this.fence = fence;
         *     }
         *
         *     public void forEachRemaining(Consumer<? super T> action) {
         *       for (; origin < fence; origin += 2)
         *         action.accept((T) array[origin]);
         *     }
         *
         *     public boolean tryAdvance(Consumer<? super T> action) {
         *       if (origin < fence) {
         *         action.accept((T) array[origin]);
         *         origin += 2;
         *         return true;
         *       }
         *       else // cannot advance
         *         return false;
         *     }
         *
         *     public Spliterator<T> trySplit() {
         *       int lo = origin; // divide range in half
         *       int mid = ((lo + fence) >>> 1) & ~1; // force midpoint to be even
         *       if (lo < mid) { // split out left half
         *         origin = mid; // reset this Spliterator's origin
         *         return new TaggedArraySpliterator<>(array, lo, mid);
         *       }
         *       else       // too small to split
         *         return null;
         *     }
         *
         *     public long estimateSize() {
         *       return (long)((fence - origin) / 2);
         *     }
         *
         *     public int characteristics() {
         *       return ORDERED | SIZED | IMMUTABLE | SUBSIZED;
         *     }
         *   }
         * }}</pre>
         *
         * <p>As an example how a parallel computation framework, such as the
         * {@code java.util.stream} package, would use Spliterator in a parallel
         * computation, here is one way to implement an associated parallel forEach,
         * that illustrates the primary usage idiom of splitting off subtasks until
         * the estimated amount of work is small enough to perform
         * sequentially. Here we assume that the order of processing across
         * subtasks doesn't matter; different (forked) tasks may further split
         * and process elements concurrently in undetermined order.  This
         * example uses a {@link java.util.concurrent.CountedCompleter};
         * similar usages apply to other parallel task constructions.
         *
         * <pre>{@code
         * static <T> void parEach(TaggedArray<T> a, Consumer<T> action) {
         *   Spliterator<T> s = a.spliterator();
         *   long targetBatchSize = s.estimateSize() / (ForkJoinPool.getCommonPoolParallelism() * 8);
         *   new ParEach(null, s, action, targetBatchSize).invoke();
         * }
         *
         * static class ParEach<T> extends CountedCompleter<Void> {
         *   final Spliterator<T> spliterator;
         *   final Consumer<T> action;
         *   final long targetBatchSize;
         *
         *   ParEach(ParEach<T> parent, Spliterator<T> spliterator,
         *           Consumer<T> action, long targetBatchSize) {
         *     super(parent);
         *     this.spliterator = spliterator; this.action = action;
         *     this.targetBatchSize = targetBatchSize;
         *   }
         *
         *   public void compute() {
         *     Spliterator<T> sub;
         *     while (spliterator.estimateSize() > targetBatchSize &&
         *            (sub = spliterator.trySplit()) != null) {
         *       addToPendingCount(1);
         *       new ParEach<>(this, sub, action, targetBatchSize).fork();
         *     }
         *     spliterator.forEachRemaining(action);
         *     propagateCompletion();
         *   }
         * }}</pre>
         *
         * @implNote
         * If the boolean system property {@systemProperty org.openjdk.java.util.stream.tripwire}
         * is set to {@code true} then diagnostic warnings are reported if boxing of
         * primitive values occur when operating on primitive subtype specializations.
         *
         * @tparam T the type of elements returned by this Spliterator
         *
         * @see Collection
         */
        template<class T>
        class SplitIterator: public Object {
        public:
            /**
             * If a remaining element exists, performs the given action on it,
             * returning {@code true}; else returns {@code false}.  If this
             * Spliterator is {@link #ORDERED} the action is performed on the
             * next element in encounter order.  Exceptions thrown by the
             * action are relayed to the caller.
             * <p>
             * Subsequent behavior of a spliterator is unspecified if the action throws
             * an exception.
             *
             * @param action The action
             * @return {@code false} if no remaining elements existed
             * upon entry to this method, else {@code true}.
             * @throws NullPointerException if the specified action is null
             */
             virtual gbool tryAdvance(Consumer<T> const& action) = 0;

            /**
             * Performs the given action for each remaining element, sequentially in
             * the current thread, until all elements have been processed or the action
             * throws an exception.  If this Spliterator is {@link #ORDERED}, actions
             * are performed in encounter order.  Exceptions thrown by the action
             * are relayed to the caller.
             * <p>
             * Subsequent behavior of a spliterator is unspecified if the action throws
             * an exception.
             *
             * @implSpec
             * The default implementation repeatedly invokes {@link #tryAdvance} until
             * it returns {@code false}.  It should be overridden whenever possible.
             *
             * @param action The action
             * @throws NullPointerException if the specified action is null
             */
             virtual void forEach(Consumer<T> const& action) {
                 do {} while (tryAdvance(action));
             }
        };

    } // util
} // core

#endif //CORE23_SPLITITERATOR_H

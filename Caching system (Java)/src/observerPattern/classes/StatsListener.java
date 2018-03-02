package observerPattern.classes;

import observerPattern.interfaces.CacheListener;

/**
 * The StatsListener collects hit / miss / update stats for a cache.
 *
 * @param <K>
 * @param <V>
 */
public class StatsListener<K, V> implements CacheListener<K, V> {
    private int hits;
    private int misses;
    private int updates;

    public StatsListener() {
    }

    /**
     * Get the number of hits for the cache.
     *
     * @return number of hits
     */
    public int getHits() {
        return hits;
    }

    /**
     * Get the number of misses for the cache.
     *
     * @return number of misses
     */
    public int getMisses() {
        return misses;
    }

    /**
     * Get the number of updates (put operations) for the cache.
     *
     * @return number of updates
     */
    public int getUpdates() {
        return updates;
    }

    /**
     * Increment the number of cache hits.
     *
     * @param key
     */
    @Override
    public void onHit(final K key) {
        hits++;
    }

    /**
     * Increment the number of cache misses.
     *
     * @param key
     */
    @Override
    public void onMiss(final K key) {
        misses++;
    }

    /**
     * Increment the number of cache updates.
     *
     * @param key
     */
    @Override
    public void onPut(final K key, final V value) {
        updates++;
    }
}

package cachingSystem.classes;

import cachingSystem.interfaces.CacheStalePolicy;
import dataStructures.classes.Pair;
import observerPattern.interfaces.CacheListener;

/**
 * Class that adapts the FIFOCache class to the ObservableCache abstract class.
 */
public class ObservableFIFOCache<K, V> extends ObservableCache<K, V> {
    private FIFOCache<K, V> cache;
    private CacheListener<K, V> cacheListener;
    private CacheStalePolicy<K, V> stalePolicy;

    public ObservableFIFOCache() {
        cache = new FIFOCache<>();
    }

    /**
     * Returns the value of searched key in cache. If it exists, it's a cache
     * hit and calls listener's onHit, else it's a cache miss and calls
     * listener's onMiss.
     *
     * @param key
     *            searched key
     * @return searched key's value
     */
    @Override
    public V get(final K key) {
        V cacheEntry = cache.get(key);

        if (cacheEntry == null) {
            cacheListener.onMiss(key);
        } else {
            cacheListener.onHit(key);
        }

        return cacheEntry;
    }

    /**
     * Adds a pair with key and value in cache. Calls listener's onPut and
     * clearStaleEntries.
     *
     * @param key
     *            new key
     * @param value
     *            new value
     */
    @Override
    public void put(final K key, final V value) {
        cache.put(key, value);
        cacheListener.onPut(key, value);
        clearStaleEntries();
    }

    /**
     * Returns the size of the cache.
     *
     * @return size of cache
     */
    @Override
    public int size() {
        return cache.size();
    }

    /**
     * Checks if cache is empty.
     *
     * @return true if cache is empty, false otherwise
     */
    @Override
    public boolean isEmpty() {
        return cache.size() == 0;
    }

    /**
     * Removes a key from cache.
     *
     * @param key
     *            key to be removed
     * @return removed key's value
     */
    @Override
    public V remove(final K key) {
        return cache.remove(key);
    }

    /**
     * Removes all entries from cache.
     */
    @Override
    public void clearAll() {
        cache.clearAll();
    }

    /**
     * Returns the eldest entry from cache, based on its custom logic. In the
     * case of FIFO, the eldest entry is the first element in the queue. If
     * cache is empty, it returns null.
     *
     * @return key-value pair of the eldest entry
     */
    @Override
    public Pair<K, V> getEldestEntry() {
        if (isEmpty()) {
            return null;
        }

        Pair<K, V> eldestEntry = cache.getEldestEntry();
        return eldestEntry;
    }

    /**
     * Set a policy for removing stale elements from the cacheList.
     *
     * @param stalePolicy
     */
    @Override
    public void setStalePolicy(final CacheStalePolicy<K, V> stalePolicy) {
        this.stalePolicy = stalePolicy;
    }

    /**
     * Set a listener for the cache.
     *
     * @param cacheListener
     */
    @Override
    public void setCacheListener(final CacheListener<K, V> cacheListener) {
        this.cacheListener = cacheListener;
    }

    /**
     * Clear the stale elements from the cache. This method must make use of the
     * stale policy.
     *
     */
    @Override
    public void clearStaleEntries() {
        while (stalePolicy.shouldRemoveEldestEntry(getEldestEntry())) {
            remove(getEldestEntry().getKey());
        }
    }
}

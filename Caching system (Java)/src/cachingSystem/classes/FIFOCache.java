package cachingSystem.classes;

import cachingSystem.interfaces.Cache;
import dataStructures.classes.Pair;

import java.util.LinkedHashMap;
import java.util.Map;

/**
 * The FIFOCache class should be considered a blackbox. All you need is its API!
 */
public class FIFOCache<K, V> implements Cache<K, V> {

    private LinkedHashMap<K, V> cache;

    public FIFOCache() {
        cache = new LinkedHashMap<>();
    }

    /**
     * Returns the value of searched key in cache.
     *
     * @param key
     *            searched key
     * @return searched key's value
     */
    @Override
    public V get(final K key) {
        return cache.get(key);
    }

    /**
     * Adds a pair with key and value in cache.
     *
     * @param key
     *            new key
     * @param value
     *            new value
     */
    @Override
    public void put(final K key, final V value) {
        cache.put(key, value);
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
        return size() == 0;
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
        cache.clear();
    }

    /**
     * Returns the eldest entry from cache, based on its custom logic. In the
     * case of FIFO, the eldest entry is the first element in the queue.
     *
     * @return key-value pair of the eldest entry
     */
    @Override
    public Pair<K, V> getEldestEntry() {
        if (isEmpty()) {
            return null;
        }

        Map.Entry<K, V> eldest = cache.entrySet().iterator().next();

        return new Pair<K, V>(eldest.getKey(), eldest.getValue());
    }
}

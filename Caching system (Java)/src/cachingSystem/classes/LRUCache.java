package cachingSystem.classes;

import dataStructures.classes.Pair;
import observerPattern.interfaces.CacheListener;

import java.util.HashMap;

import cachingSystem.interfaces.CacheStalePolicy;
import dataStructures.classes.DoublyLinkedList;
import dataStructures.classes.ListNode;

/**
 * This cacheList is very similar to the FIFOCache, but guarantees O(1)
 * complexity for the get, put and remove operations.
 */
public class LRUCache<K, V> extends ObservableCache<K, V> {
    private DoublyLinkedList<K, V> cacheList;
    private HashMap<K, ListNode<K, V>> cacheMap;
    private int size;
    private CacheStalePolicy<K, V> stalePolicy;
    private CacheListener<K, V> cacheListener;

    public LRUCache() {
        size = 0;
        cacheList = new DoublyLinkedList<>();
        cacheMap = new HashMap<>();
    }

    public final void setSize(final int size) {
        this.size = size;
    }

    public final DoublyLinkedList<K, V> getCacheList() {
        return cacheList;
    }

    public final HashMap<K, ListNode<K, V>> getCacheMap() {
        return cacheMap;
    }

    public final CacheStalePolicy<K, V> getStalePolicy() {
        return stalePolicy;
    }

    public final CacheListener<K, V> getCacheListener() {
        return cacheListener;
    }

    /**
     * Returns the value of searched key in cache. It checks if specified key
     * exists in cache's hash map. If it does, it's a cache hit and it moves the
     * key to the end of the doubly linked list, so that it becomes the most
     * recently used. It also calls listener's onHit. Otherwise, it's a cache
     * miss and calls listener's onMiss.
     *
     * @param key
     *            searched key
     * @return searched key's value
     */
    @Override
    public V get(final K key) {
        if (isEmpty() || !cacheMap.containsKey(key)) {
            cacheListener.onMiss(key);
            return null;
        }

        cacheList.delete(cacheMap.get(key));
        cacheList.insert(cacheMap.get(key));
        cacheListener.onHit(key);

        return cacheMap.get(key).getPair().getValue();
    }

    /**
     * Adds a pair with key and value in cache. If it exists already in the
     * cache, it updates its value. It calls listener's onPut method.
     *
     * @param key
     *            new key
     * @param value
     *            new value
     */
    @Override
    public void put(final K key, final V value) {
        ListNode<K, V> newNode = new ListNode<>(key, value);

        if (!cacheMap.containsKey(key)) {
            cacheList.insert(newNode);
            cacheMap.put(key, newNode);
            size++;
        } else {
            cacheList.delete(cacheMap.get(key));
            cacheMap.get(key).getPair().setValue(value);
            cacheList.insert(cacheMap.get(key));
        }

        cacheListener.onPut(key, value);
        clearStaleEntries();
    }

    /**
     * Returns the size of the cache.
     *
     * @return size of cache
     */
    @Override
    public final int size() {
        return size;
    }

    /**
     * Checks if cache is empty.
     *
     * @return true if cache is empty, false otherwise
     */
    @Override
    public final boolean isEmpty() {
        return size == 0;
    }

    /**
     * Removes a key from cache. It is removed from both the hash map and doubly
     * linked list.
     *
     * @param key
     *            key to be removed
     * @return removed key's value
     */
    @Override
    public final V remove(final K key) {
        if (!cacheMap.containsKey(key)) {
            return null;
        }

        V cacheEntryValue = cacheMap.get(key).getPair().getValue();

        size--;
        cacheList.delete(cacheMap.get(key));
        cacheMap.remove(key);
        return cacheEntryValue;
    }

    /**
     * Removes all entries from cache. The hash map and the doubly linked list
     * are cleared as well.
     */
    @Override
    public final void clearAll() {
        if (isEmpty()) {
            return;
        }

        size = 0;
        cacheList.destroy();
        cacheMap.clear();
    }

    /**
     * Returns the eldest entry from cache, based on its custom logic. In the
     * case of LRU, the eldest entry is the least recently used. It is the first
     * element in the doubly linked list.
     *
     * @return key-value pair of the eldest entry
     */
    @Override
    public final Pair<K, V> getEldestEntry() {
        if (isEmpty()) {
            return null;
        }

        return cacheList.getHead().getPair();
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

package cachingSystem.classes;

import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import dataStructures.classes.ListNode;
import dataStructures.classes.Pair;

/**
 * The TimeAwareCache offers the same functionality as the LRUCache, but also
 * stores a timestamp for each element. The timestamp is updated after each get
 * / put operation for a key. This functionality allows for time based cache
 * stale policies (e.g. removing entries that are older than 1 second).
 */
public class TimeAwareCache<K, V> extends LRUCache<K, V> {
    private long expirePolicy;
    private HashMap<K, Pair<K, Timestamp>> keyTimestamps;

    public TimeAwareCache() {
        keyTimestamps = new HashMap<>();
    }

    /**
     * Returns the value of searched key in cache. First, it clears stale
     * entries in the cache. It checks if specified key exists in cache's hash
     * map. If it does, it's a cache hit and it moves the key to the end of the
     * doubly linked list, so that it becomes the most recently used. It also
     * calls listener's onHit. Otherwise, it's a cache miss and calls listener's
     * onMiss.
     *
     * @param key
     *            searched key
     * @return searched key's value
     */
    @Override
    public final V get(final K key) {
        clearStaleEntries();

        if (super.isEmpty()) {
            super.getCacheListener().onMiss(key);
            return null;
        }

        if (!super.getCacheMap().containsKey(key)) {
            super.getCacheListener().onMiss(key);
            return null;
        }

        super.getCacheList().delete(super.getCacheMap().get(key));
        super.getCacheList().insert(super.getCacheMap().get(key));
        super.getCacheListener().onHit(key);

        return super.getCacheMap().get(key).getPair().getValue();
    }

    /**
     * Adds a pair with key and value in cache and adds a pair with key and the
     * current time in the keyTimestamps hash map to keep track of each cache
     * entry's time when it was added. If it exists already in the cache, it
     * updates its value and its time in the keyTimestamps hash map. It calls
     * listener's onPut method.
     *
     * @param key
     *            new key
     * @param value
     *            new value
     */
    @Override
    public final void put(final K key, final V value) {
        ListNode<K, V> newNode = new ListNode<>(key, value);

        if (!super.getCacheMap().containsKey(key)) {
            super.getCacheList().insert(newNode);
            super.getCacheMap().put(key, super.getCacheList().getTail());
            super.setSize(super.size() + 1);
            keyTimestamps.put(key, new Pair<>(key, new Timestamp(System.currentTimeMillis())));
        } else {
            super.getCacheList().delete(super.getCacheMap().get(key));
            super.getCacheMap().get(key).getPair().setValue(value);
            keyTimestamps.get(key).setValue(new Timestamp(System.currentTimeMillis()));
            super.getCacheList().insert(super.getCacheMap().get(key));
        }

        super.getCacheListener().onPut(key, value);
        clearStaleEntries();
    }

    /**
     * Get the timestamp associated with a key, or null if the key is not stored
     * in the cache.
     *
     * @param key
     *            the key
     * @return the timestamp, or null
     */
    public Timestamp getTimestampOfKey(final K key) {
        if (!super.getCacheMap().containsKey(key)) {
            return null;
        }

        return keyTimestamps.get(key).getValue();
    }

    /**
     * Set a cache stale policy that should remove all elements older
     * than @millisToExpire milliseconds. This is a convenience method for
     * setting a time based policy for the cache.
     *
     * @param millisToExpire
     *            the expiration time, in milliseconds
     */
    public void setExpirePolicy(final long millisToExpire) {
        this.expirePolicy = millisToExpire;
    }

    /**
     * Clear the stale elements from the cache. It creates an array list from
     * the values of the keyTimestamps hash map, iterates through it and removes
     * any cache entry that passed the expire policy from the keyTimestamps hash
     * map and the cache itself.
     */
    @Override
    public final void clearStaleEntries() {
        List<Pair<K, Timestamp>> keyTimestampsList = new ArrayList<>(keyTimestamps.values());
        for (int i = 0; i < keyTimestampsList.size(); i++) {
            if (System.currentTimeMillis()
                    - keyTimestampsList.get(i).getValue().getTime() >= expirePolicy) {
                keyTimestamps.remove(keyTimestampsList.get(i).getKey());
                super.remove(keyTimestampsList.get(i).getKey());
            }
        }
    }
}

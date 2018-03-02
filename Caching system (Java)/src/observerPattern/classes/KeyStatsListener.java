package observerPattern.classes;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;

import dataStructures.classes.Pair;
import observerPattern.interfaces.CacheListener;

/**
 * The KeyStatsListener collects key-level stats for cache operations.
 *
 * @param <K>
 * @param <V>
 */
public class KeyStatsListener<K, V> implements CacheListener<K, V> {
    private HashMap<K, Pair<K, Integer>> keyHits;
    private HashMap<K, Pair<K, Integer>> keyMisses;
    private HashMap<K, Pair<K, Integer>> keyUpdates;

    public KeyStatsListener() {
        keyHits = new HashMap<>();
        keyMisses = new HashMap<>();
        keyUpdates = new HashMap<>();
    }

    /**
     * Get the number of hits for a key.
     *
     * @param key
     *            the key
     * @return number of hits
     */
    public int getKeyHits(final K key) {
        return keyHits.get(key).getValue();
    }

    /**
     * Get the number of misses for a key.
     *
     * @param key
     *            the key
     * @return number of misses
     */
    public int getKeyMisses(final K key) {
        return keyMisses.get(key).getValue();
    }

    /**
     * Get the number of updates for a key.
     *
     * @param key
     *            the key
     * @return number of updates
     */
    public int getKeyUpdates(final K key) {
        return keyUpdates.get(key).getValue();
    }

    /**
     * Get the @top most hit keys.
     *
     * @param top
     *            number of top keys
     * @return the list of keys
     */
    public List<K> getTopHitKeys(final int top) {
        List<Pair<K, Integer>> sortedKeyHits = new ArrayList<>(keyHits.values());

        /*
         * Sort the elements in the list by the number of hits/misses/updates.
         */
        Collections.sort(sortedKeyHits, new Comparator<Pair<K, Integer>>() {
            @Override
            public int compare(final Pair<K, Integer> pair1, final Pair<K, Integer> pair2) {
                if (pair1.getValue() > pair2.getValue()) {
                    return -1;
                } else if (pair1.getValue() == pair2.getValue()) {
                    return 0;
                } else {
                    return 1;
                }
            }
        });

        /*
         * Create a new list with the first top sorted elements.
         */
        List<K> topHitKeys = new ArrayList<>();
        for (int i = 0; i < top; i++) {
            topHitKeys.add(sortedKeyHits.get(i).getKey());
        }

        return topHitKeys;
    }

    /**
     * Get the @top most missed keys.
     *
     * @param top
     *            number of top keys
     * @return the list of keys
     */
    public List<K> getTopMissedKeys(final int top) {
        List<Pair<K, Integer>> sortedKeyMisses = new ArrayList<>(keyMisses.values());

        /*
         * Sort the elements in the list by the number of hits/misses/updates.
         */
        Collections.sort(sortedKeyMisses, new Comparator<Pair<K, Integer>>() {
            @Override
            public int compare(final Pair<K, Integer> pair1, final Pair<K, Integer> pair2) {
                if (pair1.getValue() > pair2.getValue()) {
                    return -1;
                } else if (pair1.getValue() == pair2.getValue()) {
                    return 0;
                } else {
                    return 1;
                }
            }
        });

        /*
         * Create a new list with the first top sorted elements.
         */
        List<K> topMissedKeys = new ArrayList<>();
        for (int i = 0; i < top; i++) {
            topMissedKeys.add(sortedKeyMisses.get(i).getKey());
        }

        return topMissedKeys;
    }

    /**
     * Get the @top most updated keys.
     *
     * @param top
     *            number of top keys
     * @return the list of keys
     */
    public List<K> getTopUpdatedKeys(final int top) {
        List<Pair<K, Integer>> sortedKeyUpdates = new ArrayList<>(keyUpdates.values());

        /*
         * Sort the elements in the list by the number of hits/misses/updates.
         */
        Collections.sort(sortedKeyUpdates, new Comparator<Pair<K, Integer>>() {
            @Override
            public int compare(final Pair<K, Integer> pair1, final Pair<K, Integer> pair2) {
                if (pair1.getValue() > pair2.getValue()) {
                    return -1;
                } else if (pair1.getValue() == pair2.getValue()) {
                    return 0;
                } else {
                    return 1;
                }
            }
        });

        /*
         * Create a new list with the first top sorted elements.
         */
        List<K> topUpdatedKeys = new ArrayList<>();
        for (int i = 0; i < top; i++) {
            topUpdatedKeys.add(sortedKeyUpdates.get(i).getKey());
        }

        return topUpdatedKeys;
    }

    /**
     * Increment the number of hits for the specified key if it exists in the
     * keyHits hash map, else add it in the hash map with the value 1.
     *
     * @param key
     *            searched key
     */
    @Override
    public void onHit(final K key) {
        if (keyHits.containsKey(key)) {
            keyHits.get(key).setValue(keyHits.get(key).getValue() + 1);
        } else {
            keyHits.put(key, new Pair<>(key, 1));
        }
    }

    /**
     * Increment the number of misses for the specified key if it exists in the
     * keyMisses hash map, else add it in the hash map with the value 1.
     *
     * @param key
     *            searched key
     */
    @Override
    public void onMiss(final K key) {
        if (keyMisses.containsKey(key)) {
            keyMisses.get(key).setValue(keyMisses.get(key).getValue() + 1);
        } else {
            keyMisses.put(key, new Pair<>(key, 1));
        }
    }

    /**
     * Increment the number of updates for the specified key if it exists in the
     * keyUpdates hash map, else add it in the hash map with the value 1.
     *
     * @param key
     *            searched key
     * @param value
     *            searched value
     */
    @Override
    public void onPut(final K key, final V value) {
        if (keyUpdates.containsKey(key)) {
            keyUpdates.get(key).setValue(keyUpdates.get(key).getValue() + 1);
        } else {
            keyUpdates.put(key, new Pair<>(key, 1));
        }
    }
}

package observerPattern.classes;

import java.util.ArrayList;
import java.util.List;

import observerPattern.interfaces.CacheListener;

/**
 * The BroadcastListener broadcasts cache events to other listeners that have been added to it.
 */
public class BroadcastListener<K, V> implements CacheListener<K, V> {
    private List<CacheListener<K, V>> listenersList;

    public BroadcastListener() {
        listenersList = new ArrayList<>();
    }
    /**
     * Add a listener to the broadcast list.
     *
     * @param listener the listener
     */
    public void addListener(final CacheListener<K, V> listener) {
        listenersList.add(listener);
    }

    /**
     * For each listener in listenersList, calls their own onHit.
     *
     * @param key
     */
    @Override
    public void onHit(final K key) {
        for (CacheListener<K, V> listener : listenersList) {
            listener.onHit(key);
        }
    }

    /**
     * For each listener in listenersList, calls their own onMiss.
     *
     * @param key
     */
    @Override
    public void onMiss(final K key) {
        for (CacheListener<K, V> listener : listenersList) {
            listener.onMiss(key);
        }
    }

    /**
     * For each listener in listenersList, calls their own onPut.
     *
     * @param key
     * @param value
     */
    @Override
    public void onPut(final K key, final V value) {
        for (CacheListener<K, V> listener : listenersList) {
            listener.onPut(key, value);
        }
    }
}

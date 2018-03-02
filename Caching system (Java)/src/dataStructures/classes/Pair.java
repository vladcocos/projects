package dataStructures.classes;

/**
 * The Pair class is a convenient way of storing key-value pairs.
 *
 * @param <K>
 * @param <V>
 */
public class Pair<K, V> {
    private K key;
    private V value;

    public Pair(final K key, final V value) {
        this.key = key;
        this.value = value;
    }

    public final K getKey() {
        return key;
    }

    public final void setKey(final K key) {
        this.key = key;
    }

    public final V getValue() {
        return value;
    }

    public final void setValue(final V value) {
        this.value = value;
    }
}

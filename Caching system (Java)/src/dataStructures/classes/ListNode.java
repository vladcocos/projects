package dataStructures.classes;

public class ListNode<K, V> {
    private Pair<K, V> pair;
    private ListNode<K, V> previous;
    private ListNode<K, V> next;

    public ListNode(final K key, final V value) {
        pair = new Pair<>(key, value);
        previous = null;
        next = null;
    }

    public final ListNode<K, V> getPrevious() {
        return previous;
    }

    public final void setPrevious(final ListNode<K, V> previous) {
        this.previous = previous;
    }

    public final ListNode<K, V> getNext() {
        return next;
    }

    public final void setNext(final ListNode<K, V> next) {
        this.next = next;
    }

    public final Pair<K, V> getPair() {
        return pair;
    }
}

package dataStructures.classes;

public class DoublyLinkedList<K, V> {

    private ListNode<K, V> head;
    private ListNode<K, V> tail;
    private int size;

    public DoublyLinkedList() {
    }

    public final ListNode<K, V> getHead() {
        return head;
    }

    public final ListNode<K, V> getTail() {
        return tail;
    }

    private boolean isEmpty() {
        return size == 0;
    }

    /**
     * Adds a new node to the end of the list. The new node becomes the tail.
     *
     * @param newNode
     *            node to be added
     */
    public final void insert(final ListNode<K, V> newNode) {
        if (isEmpty()) {
            head = newNode;
            tail = newNode;
            size = 1;
            return;
        }

        /*
         * Set links to the tail.
         */
        newNode.setPrevious(tail);
        tail.setNext(newNode);
        tail = newNode;

        /*
         * If the list contained only 1 element before adding the new one, set
         * the links between the head and the new element.
         */
        if (size == 1) {
            head.setNext(tail);
            tail.setPrevious(head);
        }

        size++;
    }

    /**
     * Deletes a node from the list.
     *
     * @param nodeToDelete
     *            node to be deleted
     */
    public final void delete(final ListNode<K, V> nodeToDelete) {
        if (isEmpty()) {
            return;
        }

        if (size == 1) {
            head = null;
            tail = null;
            size = 0;
            return;
        }

        /*
         * The node to be deleted is the head and there are more than 1 element
         * in the list.
         */
        if (nodeToDelete == head) {
            nodeToDelete.getNext().setPrevious(null);
            head = head.getNext();
            size--;
            return;
        }

        /*
         * The node to be deleted is the tail and there are more than 1 element
         * in the list.
         */
        if (nodeToDelete == tail) {
            nodeToDelete.getPrevious().setNext(null);
            tail = tail.getPrevious();
            size--;
            return;
        }

        /*
         * The node is neither the head nor the tail.
         */
        nodeToDelete.getPrevious().setNext(nodeToDelete.getNext());
        nodeToDelete.getNext().setPrevious(nodeToDelete.getPrevious());
        size--;
    }

    /**
     * Removes all nodes from the list.
     */
    public final void destroy() {
        head = null;
        tail = null;
        size = 0;
    }
}

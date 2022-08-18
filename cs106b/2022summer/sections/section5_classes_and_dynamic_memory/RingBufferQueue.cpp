#include "RingBufferQueue.h"
#include "testing/SimpleTest.h"

const int defaultCapacity = 10;

using namespace std;

/**
 * constructor of RBQueue
 */
RBQueue::RBQueue() {
    capacity = defaultCapacity;
    array = new int[capacity];
    qSize = 0;
    head = 0;
}

/**
 * destructor of RBQueue
 */
RBQueue::~RBQueue() {
    delete[] array;
}

/**
 * enqueues an element to the queue
 * @elem element to be enqueued
 * throws an error if the queue is full
 */
void RBQueue::enqueue(int elem) {
    if (isFull()) {
        //error("the queue is full");
        expandCapacity();
    }
    array[(head + qSize) % capacity] = elem;
    qSize++;
}

/**
 * dequeues an element at the front of the queue
 * throws a string exception if the queue is empty
 */
int RBQueue::dequeue() {
    if (isEmpty()) {
        error("the queue is empty");
    }
    int front = array[head];
    head = (head + 1) % capacity;
    qSize--;
    return front;
}

/**
 * returns elements at the front of the queue
 * throws a string exception if the queue is empty
 */
int RBQueue::peek() const {
    if (isEmpty()) {
        error("the queue is empty");
    }
    return array[head];
}

/**
 * returns true if the queue is empty and false otherwise
 */
bool RBQueue::isEmpty() const {
    return qSize == 0;
}

/**
 * returns true is the queue is full and false otherwise
 */
bool RBQueue::isFull() const {
    return qSize == capacity;
}

/**
 * returns the number of elements in the queue
 */
int RBQueue::size() const {
    // The following lines are just here to
    // disable compiler warnings and errors.
    // Delete these lines, then implement
    // this method.
    return qSize;
}

void RBQueue::expandCapacity() {
    int *oldArray = array;
    array = new int[2 * capacity];
    for (int i = 0; i < qSize; i++) {
        array[i] = oldArray[(head + i) % capacity];
    }
    capacity *= 2;
    head = 0;
    delete[] oldArray;
}

/**
 * TODO: write a method comment
 */
ostream& operator<<(ostream& out, const RBQueue& queue) {
    out << "{";
    if (!queue.isEmpty()) {
        out << queue.array[queue.head];
        for (int i = 1; i < queue.qSize; i++) {
            out << ", " << queue.array[(queue.head + i) % queue.capacity];
        }
    }
    out << "}";
    return out;
}

/* Provided Tests Below This Line */

PROVIDED_TEST("Simple Example From Handout"){
    RBQueue q;
    q.enqueue(1);
    q.enqueue(2);
    q.enqueue(3);
    EXPECT_EQUAL(q.peek(), 1);
    q.enqueue(4);
    q.enqueue(5);
    cout << q << endl;
    //EXPECT(!q.isFull());
    EXPECT_EQUAL(q.dequeue(), 1);
    EXPECT_EQUAL(q.dequeue(), 2);
    EXPECT_EQUAL(q.peek(), 3);
    q.enqueue(6);
    EXPECT_EQUAL(q.dequeue(), 3);
    EXPECT_EQUAL(q.dequeue(), 4);
    EXPECT_EQUAL(q.dequeue(), 5);
    EXPECT_EQUAL(q.dequeue(), 6);
    EXPECT(q.isEmpty());
}

STUDENT_TEST("Enqueue and dequeu repeatedly to exceed the boundaries") {
    RBQueue q;
    for (int i = 0; i <= 1000; i++) {
        if (i % 2 == 0) {
            q.enqueue(i);
        } else {
            EXPECT_EQUAL(q.dequeue(), i-1);
        }
    }
}

STUDENT_TEST("Enqueue, dequeue half, and enqueue much more") {
    RBQueue q;
    int numElements = 1000;
    for (int i = 0; i < numElements; i++) {
        q.enqueue(i);
    }
    EXPECT_EQUAL(q.size(), numElements);
    for (int i = 0; i < numElements/2; i++) {
        EXPECT_EQUAL(q.dequeue(), i);
    }
    for (int i = numElements; i < 3 * numElements; i++) {
        q.enqueue(i);
    }
    for (int i = 0; i < numElements * 2; i++) {
        EXPECT_EQUAL(q.dequeue(), i + numElements / 2);
    }
}

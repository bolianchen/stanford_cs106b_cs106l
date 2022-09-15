#include "pqheap.h"
#include "error.h"
#include "random.h"
#include "strlib.h"
#include "datapoint.h"
#include "testing/SimpleTest.h"
using namespace std;

const int INITIAL_CAPACITY = 10;
const int NONE = -1; // used as sentinel index

/*
 * The constructor initializes all of the member variables
 */
PQHeap::PQHeap() {
    _numAllocated = INITIAL_CAPACITY;
    _elements = new DataPoint[_numAllocated];
    _numFilled = 0;
}

/*
 * The destructor is responsible for cleaning up any resources
 * used by this instance of the PQHeap class. The array memory that
 * was allocated for the PQArray is deleted here.
 */
PQHeap::~PQHeap() {
    delete[] _elements;
}

/*
 * place the element at the end of the internal array
 * bubble up the element
 */
void PQHeap::enqueue(DataPoint elem) {
    if (size() == _numAllocated) expandCapacity();
    int index = _numFilled;
    _elements[index] = elem;
    _numFilled++;
    bubbleUp(index);
}

/*
 * returns but does not remove the element that is frontmost
 * if the priority queue is empty, this function calls error()
 */
DataPoint PQHeap::peek() const {
    if (isEmpty()) {
        error("PQueue is empty!");
    }
    return _elements[0];
}

/*
 * removes and returns the frontmost element in the priority queue
 * if the priority queue is empty, this function calls error()
 */
DataPoint PQHeap::dequeue() {
    if (isEmpty()) {
        error("PQueue is empty!");
    }
    DataPoint result = _elements[0];
    swap(0, _numFilled - 1);
    _numFilled--;
    bubbleDown(0);
    return result;
}

/*
 * private helper function that swaps an element with its parent if
 * it has lower priority than its parent
 */
void PQHeap::bubbleUp(int index) {
    int parentIndex = getParentIndex(index);
    if (parentIndex != -1) {
        if (morePrioritized(parentIndex, index)) {
            swap(index, parentIndex);
            bubbleUp(parentIndex);
        }
    }
}

/*
 * private helper function that swaps an element with its child having lower
 * priority if it has higher priority than that child;
 * it has lower priority than its parent
 */
void PQHeap::bubbleDown(int index) {
    int leftIndex = getLeftChildIndex(index);
    int rightIndex = getRightChildIndex(index);

    if (leftIndex != -1) {
        int indexOfSmallerChild = leftIndex;
        if (rightIndex != -1 && morePrioritized(leftIndex, rightIndex)) {
            indexOfSmallerChild = rightIndex;
        }
        if (morePrioritized(index, indexOfSmallerChild)) {
            swap(index, indexOfSmallerChild);
            bubbleDown(indexOfSmallerChild);
        }
    }
}

/*
 * private helper function that checks if the element at indexA has
 * greater priority than the element at indexB
 */
bool PQHeap::morePrioritized(int indexA, int indexB) {
    return _elements[indexA].priority > _elements[indexB].priority;
}

/*
 * private member function. This helper exchanges the element at
 * indexA with the element at indexB.
 */
void PQHeap::swap(int indexA, int indexB) {
    DataPoint tmp = _elements[indexA];
    _elements[indexA] = _elements[indexB];
    _elements[indexB] = tmp;
}

/*
 * private member function.
 * it expands the queue capacity when it is full
 */
void PQHeap::expandCapacity() {
    DataPoint *oldElements = _elements;
    _numAllocated *= 2;
    _elements = new DataPoint[_numAllocated];
    for (int i = 0; i < _numFilled; i++) {
        _elements[i] = oldElements[i];
    }
    delete[] oldElements;
}

/*
 * returns whether this priority queue is empty by checking if its size equals to 0
 */
bool PQHeap::isEmpty() const {
    return size() == 0;
}

/*
 * returns the count of enqueued elements, that is tracked with the member variable _numFilled
 */
int PQHeap::size() const {
    return _numFilled;
}

/*
 * removes all elements from the priority queue.
 * the function simply set the count of enqueued elements to 0 without
 * changing the values of the previously stored elements in the allocated array.
 */
void PQHeap::clear() {
    _numFilled = 0;
}

/*
 * prints the contents of internal array for debugging purposes
 */
void PQHeap::printDebugInfo(string msg) const {
    cout << msg << endl;
    for (int i = 0; i < size(); i++) {
        cout << "[" << i << "] = " << _elements[i] << endl;
    }
}

/*
 * traverses the heap array and ensure each element is larger than
 * its two childs if they exist. An error is thrown if any element
 * violates the heap property
 */
void PQHeap::validateInternalState() const {
    for (int i = 0; i < size(); i++) {
        int leftChild = getLeftChildIndex(i);
        int rightChild = getRightChildIndex(i);
        if (leftChild != -1 && _elements[i].priority > _elements[leftChild].priority) {
            error("PQHeap has an element larger than it left child");
        }
        if (rightChild != -1 && _elements[i].priority > _elements[rightChild].priority) {
            error("PQHeap has an element larger than it right child");
        }
    }
}

/*
 * This helper function calculates the index of the element
 * that is the parent of the specified child index.
 * If this child has no parent, return the sentinel value -1.
 */
int PQHeap::getParentIndex(int child) const {
    if (child == 0) {
        return -1;
    }
    return (child - 1)/2;
}

/*
 * This helper function calculates the index of the element that is the left child of the
 * specified parent index. If this parent has no left child return the sentinel value -1.
 */
int PQHeap::getLeftChildIndex(int parent) const {
    int index = 2 * parent + 1;
    if (index >= _numFilled) {
        return -1;
    }
    return index;
}

/*
 * This helper function calculates the index of the element that is the right child of the
 * specified parent index. If this parent has no right child, return the sentinel value -1.
 */
int PQHeap::getRightChildIndex(int parent) const {
    int index = 2 * parent + 2;
    if (index >= _numFilled) {
        return -1;
    }
    return index;
}

/* * * * * * Test Cases Below This Point * * * * * */

/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("PQHeap example from writeup, validate each step") {
    PQHeap pq;
    Vector<DataPoint> input = {
        { "R", 4 }, { "A", 5 }, { "B", 3 }, { "K", 7 }, { "G", 2 },
        { "V", 9 }, { "T", 1 }, { "O", 8 }, { "S", 6 } };

    pq.validateInternalState();
    for (DataPoint dp : input) {
        pq.enqueue(dp);
        pq.validateInternalState();
    }

    while (!pq.isEmpty()) {
        pq.dequeue();
        pq.validateInternalState();
    }
}

/* TODO: Add your own custom tests here! */

STUDENT_TEST("PQHeap: operations size/isEmpty/clear") {
    PQHeap pq;

    EXPECT(pq.isEmpty());
    pq.clear();
    EXPECT_EQUAL(pq.isEmpty(), pq.size() == 0);
    pq.enqueue({ "", 7 });
    EXPECT_EQUAL(pq.size(), 1);
    pq.enqueue({ "", 5 });
    EXPECT_EQUAL(pq.size(), 2);
    pq.enqueue({ "", 5 });
    EXPECT_EQUAL(pq.size(), 3);
    pq.validateInternalState();
    pq.clear();
    pq.validateInternalState();
    EXPECT(pq.isEmpty());
    EXPECT_EQUAL(pq.size(), 0);
}

STUDENT_TEST("PQHeap: enqueue, dequene and peek, size, isEmpty") {
    PQHeap pq;
    EXPECT(pq.isEmpty());
    pq.enqueue({ "A", 7 });
    pq.enqueue({ "C", 2 });
    pq.enqueue({ "B", 5 });
    pq.enqueue({ "D", 3 });
    pq.enqueue({ "E", 1 });
    EXPECT(!pq.isEmpty());
    EXPECT_EQUAL(pq.size(), 5);
    EXPECT_EQUAL(pq.peek(), {"E", 1});
    EXPECT_EQUAL(pq.dequeue(), {"E", 1});
    EXPECT_EQUAL(pq.size(), 4);
    EXPECT_EQUAL(pq.peek(), {"C", 2});
    EXPECT_EQUAL(pq.dequeue(), {"C", 2});
    EXPECT_EQUAL(pq.size(), 3);
    EXPECT_EQUAL(pq.peek(), {"D", 3});
    EXPECT_EQUAL(pq.dequeue(), {"D", 3});
    EXPECT_EQUAL(pq.size(), 2);
    EXPECT(!pq.isEmpty());
    EXPECT_EQUAL(pq.peek(), {"B", 5});
    EXPECT_EQUAL(pq.dequeue(), {"B", 5});
    EXPECT_EQUAL(pq.size(), 1);
    EXPECT_EQUAL(pq.peek(), {"A", 7});
    EXPECT_EQUAL(pq.dequeue(), {"A", 7});
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT(pq.isEmpty());
}

STUDENT_TEST("PQHeap: test enlarge array memory") {
    for (int size = 5; size <= 500; size *= 5) {
        PQHeap pq;

        for (int i = 1; i <= size; i++) {
            pq.enqueue({"", double(i) });
        }
        pq.validateInternalState();

        for (int i = 1; i <= size; i++) {
            DataPoint expected = {"", double(i) };
            EXPECT_EQUAL(pq.dequeue(), expected);
        }
    }

}

STUDENT_TEST("PQHeap, sequence of mixed operations") {
    PQHeap pq;
    int size = 30;
    double val = 0;

    for (int i = 0; i < size; i++) {
        pq.enqueue({"", --val });
    }
    val = 0;
    for (int i = 0; i < pq.size(); i++) {
        DataPoint front = pq.peek();
        EXPECT_EQUAL(pq.dequeue(), front);
        pq.enqueue( { "", ++val });
    }
    EXPECT_EQUAL(pq.size(), size);
    val = 0;
    while (!pq.isEmpty()) {
        DataPoint expected = { "", ++val };
        EXPECT_EQUAL(pq.dequeue(), expected);
    }
}

STUDENT_TEST("PQHeap stress test, cycle many random elements in and out") {
    PQHeap pq;
    int n = 0, maxEnqueues = 1000;
    double sumEnqueued = 0, sumDequeued = 0;

    setRandomSeed(42); // make test behavior deterministic

    DataPoint mostUrgent = { "", 0 };
    pq.enqueue(mostUrgent);
    while (true) {
        if (++n < maxEnqueues && randomChance(0.9)) {
            DataPoint elem = { "", randomInteger(-10, 10) + 0.5 };
            if (elem.priority < mostUrgent.priority) {
                mostUrgent = elem;
            }
            sumEnqueued += elem.priority;
            pq.enqueue(elem);
        } else {
            DataPoint elem = pq.dequeue();
            sumDequeued += elem.priority;
            EXPECT_EQUAL(elem, mostUrgent);
            if (pq.isEmpty()) break;
            mostUrgent = pq.peek();
        }
    }
    EXPECT_EQUAL(sumEnqueued, sumDequeued);
}

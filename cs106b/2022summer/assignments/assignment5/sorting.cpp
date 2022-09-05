#include "listnode.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * prototypes of helper functions
 */
ListNode* concatenateHelper(ListNode*& list1, ListNode* tail, ListNode* list2);
void concatenate(ListNode*& list, ListNode* less, ListNode* equal, ListNode* greater);
void partition(ListNode* head, ListNode*& less, ListNode*& equal, ListNode*& greater, int pivot);

/*
 * quick sort that always choose the first element as the pivot
 */
void quickSort(ListNode*& front) {
    if (front == nullptr) return;
    ListNode *less, *equal, *greater;
    int pivot = front->data;
    partition(front, less, equal, greater, pivot);
    quickSort(less);
    quickSort(greater);
    concatenate(front, less, equal, greater);
}

/*
 * a helper function to split the input list into three partitions according the the pivot value
 */
void partition(ListNode* head, ListNode*& less, ListNode*& equal, ListNode*& greater, int pivot) {
    ListNode *nextHead;
    ListNode *curLess, *curEqual, *curGreater;
    less = equal = greater = curLess = curEqual = curGreater = nullptr;
    while (head != nullptr) {
        nextHead = head->next;
        if (head->data < pivot) {
            if (less == nullptr) {
                less = curLess = head;
            } else {
                curLess->next = head;
                curLess = curLess->next;
            }
            curLess->next = nullptr;
        } else if (head->data > pivot) {
            if (greater == nullptr) {
                greater = curGreater = head;
            } else {
                curGreater->next = head;
                curGreater = curGreater->next;
            }
            curGreater->next = nullptr;

        } else {
            if (equal == nullptr) {
                equal = curEqual = head;
            } else {
                curEqual->next = head;
                curEqual = curEqual->next;
            }
            curEqual->next = nullptr;
        }
        head = nextHead;
    }
}


/*
 * a helper function to merge three linked lists, less, equal and greater, into list
 */
void concatenate(ListNode*& list, ListNode* less, ListNode* equal, ListNode* greater) {

    ListNode *tail = nullptr;
    tail = concatenateHelper(list, tail, less);
    tail = concatenateHelper(list, tail, equal);
    tail = concatenateHelper(list, tail, greater);
}

/*
 * an auxiliary function for the concatenate function. It has a side effect of expanding list1 by appending list2 to its back.
 * it returns the tail node of the combined linked list
 */
ListNode* concatenateHelper(ListNode*& list1, ListNode* tail, ListNode* list2) {
    if (tail == nullptr) {
        tail = list1 = list2;
    } else {
        tail->next = list2;
    }
    while (tail != nullptr && tail->next != nullptr) {
        tail = tail->next;
    }
    return tail;
}


/* * * * * * Test Code Below This Point * * * * * */

/* TODO: Write your own student tests below to test your sorting helper
 * functions and overall sorting algorithm correctness. We have provided
 * the skeleton of some tests below to help you get started with this
 * process.
 */

/*
 * We have provided this utility function as a convenience. This
 * function traverses the linked list represented by the provided
 * front pointer and prints out the data stored in the list along
 * the way in a convenient format. You may find it to be helpful to
 * use this function as a debugging aid.
 */
void printList(ListNode* front) {
   cout << "{";
   for (ListNode *cur = front; cur != nullptr; cur = cur->next) {
       cout << cur->data;
       if (cur->next != nullptr){
           cout << ", ";
       }
   }
   cout << "}" << endl;
}

/*
 * This utility function deallocates the memory for all the nodes in a
 * given linked list by iteration. It can be used to recycle the memory allocated
 * during a test case and avoid memory leaks.
 */
void deallocateList(ListNode* front) {
    while (front != nullptr) {
        ListNode* cur = front;
        front = front->next;
        delete cur;
    }
}

/*
 * This utility function is given a vector of values and constructs
 * an equivalent linked list containing the same values in the same order.
 * It can be used to construct a linked list to use as an input to
 * a function being tested.
 */
ListNode* createList(Vector<int> values){
    ListNode* list = nullptr;
    for (int i = values.size()-1; i >= 0; i--) {
        list = new ListNode(values.get(i), list);
    }
    return list;
}

/*
 * This utility function compares a linked list and a vector for
 * equivalence and returns true if both contain the same values in the
 * same order. It can be used to confirm a linked list's contents match
 * the expected.
 */
bool areEquivalent(ListNode* front, Vector<int> v){
    int compared = 0;
    while (front != nullptr && compared < v.size()) {
        if (front->data != v[compared++]) {
            return false;
        }
        front = front->next;
    }
    if (front == nullptr && compared == v.size()) {
        return true;
    }
    return false;
}


PROVIDED_TEST("This tests some of the testing helper functions you are recommended to implement."){
    /* Creates a small test list containing the values 1->2->3. */
    ListNode* testList = nullptr;
    testList = new ListNode(3, testList);
    testList = new ListNode(2, testList);
    testList = new ListNode(1, testList);

    /* Test equality comparison function. */
    EXPECT(areEquivalent(testList, {1, 2, 3}));
    EXPECT(!areEquivalent(testList, {1, 2}));
    EXPECT(!areEquivalent(testList, {1, 2, 3, 4}));
    EXPECT(!areEquivalent(testList, {1, 3, 2}));

    /* Test create list function. */
    ListNode* studentList = createList({1, 2, 3, 4});
    printList(studentList);
    ListNode* cur = studentList;
    for (int i = 1; i <= 4; i++){
        EXPECT(cur != nullptr);
        EXPECT_EQUAL(cur->data, i);
        cur = cur->next;
    }

    /* Test deallocate list functions. There should be no memory leaks from this test. */
    deallocateList(studentList);
    deallocateList(testList);
}

/*
 * The provided test case demonstrates a comparsion between the same
 * algorithm (quicksort) operating on a linked list versus a vector.
 */
PROVIDED_TEST("Time linked list quicksort vs vector quicksort") {
    int startSize = 50000;

    for(int n = startSize; n < 10*startSize; n *= 2) {
        Vector<int> v(n);
        ListNode* list = nullptr;

        /* Create linked list and vector with the same random sequence. */
        for (int i = n-1; i >= 0; i--) {
            v[i] = randomInteger(-10000, 10000);
            list = new ListNode(v[i], list);
        }

        /* NOTE: This test does not check correctness of the linked list sort
         * function. It only times the two operations to compare relative speed. */
        TIME_OPERATION(n, quickSort(list));
        TIME_OPERATION(n, v.sort());        /* Standard vector sort operation is backed
                                               with quicksort algorithm. */

        deallocateList(list);
    }
}

STUDENT_TEST("Test deallocateList, on different sizes of linked lists") {
    int startSize = 1;

    for (int n = startSize; n <= 10000*startSize; n *= 10) {
        ListNode* list = nullptr;
        for (int i = 0; i < n; i++) {
            list = new ListNode(randomInteger(-10000, 10000), list);
        }
        deallocateList(list);
    }
}

STUDENT_TEST("Test areEquivalent, on different sizes of linked lists") {
    int startSize = 1;
    ListNode* list = nullptr;

    for (int n = startSize; n <= 1000*startSize; n++) {
        Vector<int> v(n);
        ListNode* list = nullptr;
        for (int i = n-1; i >= 0; i--) {
            int value = randomInteger(-10000, 10000);
            v.set(i, value);
            list = new ListNode(value, list);
        }
        EXPECT(areEquivalent(list, v));

        int indexToChange = randomInteger(0, n-1);
        v.set(indexToChange, v.get(indexToChange) + 1);

        EXPECT(!areEquivalent(list, v));
        deallocateList(list);
    }
}

STUDENT_TEST("Test createList, on different sizes of linked lists") {
    int startSize = 50000;

    for(int n = startSize; n < 10*startSize; n *= 2) {
        Vector<int> v(n);
        ListNode* list = nullptr;

        /* Create linked list and vector with the same random sequence. */
        for (int i = n-1; i >= 0; i--) {
            v[i] = randomInteger(-10000, 10000);
        }

        list = createList(v);
        EXPECT(areEquivalent(list, v));

        deallocateList(list);
    }
}

STUDENT_TEST("Test partition, on linked lists of different sizes filled with random numbers") {
    for (int size = 0; size <= 10000; size += 1000) {
        Vector<int> vec;
        for (int i = 0; i < size; i++) {
            vec.add(randomInteger(-10000, 10000));
        }
        ListNode* list = createList(vec);
        ListNode *less, *equal, *greater;
        int pivot = randomInteger(-10000, 10000);
        partition(list, less, equal, greater, pivot);

        int elementCount = 0;
        ListNode* cur = less;
        while (cur != nullptr) {
            EXPECT(cur->data < pivot);
            cur = cur->next;
            elementCount++;
        }
        cur = equal;
        while (cur != nullptr) {
            EXPECT(cur->data == pivot);
            cur = cur->next;
            elementCount++;
        }
        cur = greater;
        while (cur != nullptr) {
            EXPECT(cur->data > pivot);
            cur = cur->next;
            elementCount++;
        }
        EXPECT_EQUAL(elementCount, size);
        deallocateList(less);
        deallocateList(equal);
        deallocateList(greater);
    }
}

STUDENT_TEST("Test concatenate on linked lists of random sizes") {
    int testRuns = 100000;
    for (int i = 0; i < testRuns; i++) {
        Vector<int> v1, v2, v3, expected;
        int size1 = randomInteger(-5, 5);
        int size2 = randomInteger(-5, 5);
        int size3 = randomInteger(-5, 5);

        for (int i = 0; i < size1; i++) {
            int element = randomInteger(-10000, 10000);
            v1.add(element);
            expected.add(element);
        }
        for (int i = 0; i < size2; i++) {
            int element = randomInteger(-10000, 10000);
            v2.add(element);
            expected.add(element);
        }
        for (int i = 0; i < size3; i++) {
            int element = randomInteger(-10000, 10000);
            v3.add(element);
            expected.add(element);
        }

        ListNode* list1 = createList(v1);
        ListNode* list2 = createList(v2);
        ListNode* list3 = createList(v3);
        ListNode* merged;
        concatenate(merged, list1, list2, list3);
        EXPECT(areEquivalent(merged, expected));
        deallocateList(merged);
    }
}


STUDENT_TEST("End to End Test for quickSort"){
    int testRuns = 1000;
    for (int run = 0; run < testRuns; run++) {
        int size = randomInteger(0, 1000);
        /* Initialize vector to sequence of values */
        Vector<int> values;
        for (int i = 0; i < size; i++) {
            values.add(randomInteger(-1000, 1000));
        }
        /* Create equivalent list to use as test input */
        ListNode* list = createList(values);
        /* Sort the list */
        quickSort(list);
        /* Sort the equivalent vector */
        values.sort();
        /* Confirm sorted list matches sorted vector */
        EXPECT(areEquivalent(list, values));
        /* Avoid memory leaks by deallocating list */
        deallocateList(list);
    }
}

STUDENT_TEST("Time linked list quicksort on random inputs") {
    int startSize = 4000000;

    for(int n = startSize; n < 10*startSize; n *= 2) {
        ListNode* list = nullptr;

        /* Create linked list and vector with the same random sequence. */
        for (int i = 0; i < n; i++) {
            list = new ListNode(randomInteger(-10000, 10000), list);
        }

        /* NOTE: This test does not check correctness of the linked list sort
         * function. It only times the linked list quicksort
         */
        TIME_OPERATION(n, quickSort(list));

        deallocateList(list);
    }
}

STUDENT_TEST("Time linked list quicksort on sorted inputs as the worst case") {
    int startSize = 1000;

    for(int n = startSize; n < 10*startSize; n *= 2) {
        ListNode* list = nullptr;

        /* Create linked list and vector with the same random sequence. */
        for (int i = 0; i < n; i++) {
            list = new ListNode(i, list);
        }

        /* NOTE: This test does not check correctness of the linked list sort
         * function. It only times the linked list quicksort
         */
        TIME_OPERATION(n, quickSort(list));

        deallocateList(list);
    }
}

STUDENT_TEST("Time linked list quicksort vs vector quicksort") {
    int startSize = 500000;

    for(int n = startSize; n < 10*startSize; n *= 2) {
        Vector<int> v(n);
        ListNode* list = nullptr;

        /* Create linked list and vector with the same random sequence. */
        for (int i = n-1; i >= 0; i--) {
            v[i] = randomInteger(-100, 100);
            list = new ListNode(v[i], list);
        }

        /* NOTE: This test does not check correctness of the linked list sort
         * function. It only times the two operations to compare relative speed. */
        TIME_OPERATION(n, quickSort(list));
        TIME_OPERATION(n, v.sort());        /* Standard vector sort operation is backed
                                               with quicksort algorithm. */

        deallocateList(list);
    }
}



/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include "queue.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
Queue<int> binaryMerge(Queue<int> a, Queue<int> b) {
    Queue<int> result;
    int totalLength = a.size() + b.size();
    int prevEnqued = INT_MIN;
    int currEnqued = INT_MIN;
    for (int i = 0; i < totalLength; i++) {
        if (!a.isEmpty() && !b.isEmpty()) {
            if (a.peek() > b.peek()) {
                currEnqued = b.dequeue();
            } else {
                currEnqued = a.dequeue();
            }
        } else if (!a.isEmpty()) {
            currEnqued = a.dequeue();

        } else {
            currEnqued = b.dequeue();
        }

        if (prevEnqued > currEnqued) {
            error("either input queue is not sorted ascendingly");
        }
        result.enqueue(currEnqued);
        prevEnqued = currEnqued;

    }
    return result;
}

/*
 * This function assumes correct functionality of the previously
 * defined binaryMerge function and makes use of this function to
 * iteratively merge a collection of sequences, starting off with
 * an empty sequence and progressively merging in one sequence at
 * a time. This function is provided for you fully implemented –
 * it does not need to modified at all.
 */
Queue<int> naiveMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;

    for (Queue<int>& q : all) {
        result = binaryMerge(q, result);
    }
    return result;
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
Queue<int> recMultiMerge(Vector<Queue<int>>& all) {
    int numOfSequences = all.size();
    if (numOfSequences == 0) {
        return {};
    } else if (numOfSequences == 1) {
        return all[0];
    } else {
        Vector<Queue<int>> left = all.subList(0, numOfSequences / 2);
        Vector<Queue<int>> right = all.subList(numOfSequences / 2);
        return binaryMerge(recMultiMerge(left), recMultiMerge(right));
    }
}


/* * * * * * Test Cases * * * * * */

Queue<int> createSequence(int size);
void distribute(Queue<int> input, Vector<Queue<int>>& all);

PROVIDED_TEST("binaryMerge, two short sequences") {
    Queue<int> a = {2, 4, 5};
    Queue<int> b = {1, 3, 3};
    Queue<int> expected = {1, 2, 3, 3, 4, 5};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}

PROVIDED_TEST("naiveMultiMerge, small collection of short sequences") {
    Vector<Queue<int>> all = {{3, 6, 9, 9, 100},
                             {1, 5, 9, 9, 12},
                             {5},
                             {},
                             {-5, -5},
                             {3402}
                            };
    Queue<int> expected = {-5, -5, 1, 3, 5, 5, 6, 9, 9, 9, 9, 12, 100, 3402};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}

PROVIDED_TEST("recMultiMerge, compare to naiveMultiMerge") {
    int n = 20;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(n);
    distribute(input, all);
    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
}

PROVIDED_TEST("Time binaryMerge operation") {
    int n = 1000000;
    Queue<int> a = createSequence(n);
    Queue<int> b = createSequence(n);
    TIME_OPERATION(a.size() + b.size(), binaryMerge(a, b));
}

PROVIDED_TEST("Time naiveMultiMerge operation") {
    int n = 11000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), naiveMultiMerge(all));
}
PROVIDED_TEST("Time recMultiMerge operation") {
    int n = 90000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}


/* Test helper to fill queue with sorted sequence */
Queue<int> createSequence(int size) {
    Queue<int> q;
    for (int i = 0; i < size; i++) {
        q.enqueue(i);
    }
    return q;
}

/* Test helper to distribute elements of sorted sequence across k sequences,
   k is size of Vector */
void distribute(Queue<int> input, Vector<Queue<int>>& all) {
    while (!input.isEmpty()) {
        all[randomInteger(0, all.size()-1)].enqueue(input.dequeue());
    }
}

/* * * * * * Test Cases * * * * * */
STUDENT_TEST("binaryMerge, divide 0 ~ 9999 into two sorted sequences") {
    Queue<int> a, b, expected;
    for (int i = 0; i < 10000; i++) {
        if (i % 3 == 0) {
            a.enqueue(i);
        } else {
            b.enqueue(i);
        }
        expected.enqueue(i);
    }
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}

STUDENT_TEST("binaryMerge, one short sequence and one empty") {
    Queue<int> a = {1, 3, 5};
    Queue<int> b = {};
    Queue<int> expected = {1, 3, 5};

    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}
STUDENT_TEST("binaryMerge, two empty sequences") {
    Queue<int> a = {};
    Queue<int> b = {};
    Queue<int> expected = {};

    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}
STUDENT_TEST("binaryMerge, divide 0 ~ 9999 into two sorted sequences") {
    Queue<int> a, b, expected;
    for (int i = 0; i < 10000; i++) {
        if (i % 3 == 0) {
            a.enqueue(i);
        } else {
            b.enqueue(i);
        }
        expected.enqueue(i);
    }
    EXPECT_EQUAL(binaryMerge(a, b), expected);
}

STUDENT_TEST("binaryMerge, unsorted sequences") {
    Queue<int> a = {3, 2, 1};
    Queue<int> b = {1, 2, 3};
    EXPECT_ERROR(binaryMerge(a, b));
    EXPECT_ERROR(binaryMerge(b, a));
}

//STUDENT_TEST("binaryMerge, input size = 10000000 ~ 80000000") {
//    int n = 2;
//    int initialSize = 10000000;
//    Queue<int> input;
//    Vector<Queue<int>> all;
//    for (int size = initialSize; size <= initialSize * 8; size *= 2) {
//        input = createSequence(size);
//        all = {{}, {}};
//        distribute(input, all);
//        TIME_OPERATION(size, binaryMerge(all[0], all[1]));
//    }
//}

STUDENT_TEST("naiveMultiMerge, empty vector and vector of empty queues") {
    Vector<Queue<int>> zeroQueues = {};
    Vector<Queue<int>> tenEmptyQueues(10);
    Queue<int> expected = {};
    EXPECT_EQUAL(naiveMultiMerge(zeroQueues), expected);
    EXPECT_EQUAL(naiveMultiMerge(tenEmptyQueues), expected);
}

//STUDENT_TEST("Time naiveMultiMerge operation, n = 500000 ~ 8000000, k fixed as 10") {
//    int k = 10;
//    Queue<int> input;
//    Vector<Queue<int>> all;
//    for (int n = 500000; n <= 8000000; n *= 2) {
//        all.clear();
//        for (int i = 0; i < k; i++) {
//            all.add({});
//        }
//        input = createSequence(n);
//        distribute(input, all);
//        TIME_OPERATION(n, naiveMultiMerge(all));
//    }
//}

//STUDENT_TEST("Time naiveMultiMerge operation, k = 10 ~ 160, n fixed as 500,000") {
//    int n = 500000;
//    Queue<int> input;
//    Vector<Queue<int>> all;
//    for (int k = 20; k <= 320; k *= 2) {
//        all.clear();
//        for (int i = 0; i < k; i++) {
//            all.add({});
//        }
//        input = createSequence(n);
//        distribute(input, all);
//        TIME_OPERATION(k, naiveMultiMerge(all));
//    }
//}

STUDENT_TEST("recMultiMerge, empty vector and vector of empty queues") {
    Vector<Queue<int>> zeroQueues = {};
    Vector<Queue<int>> tenEmptyQueues(10);
    Queue<int> expected = {};
    EXPECT_EQUAL(recMultiMerge(zeroQueues), expected);
    EXPECT_EQUAL(recMultiMerge(tenEmptyQueues), expected);
}

PROVIDED_TEST("recMultiMerge, small collection of short sequences") {
    Vector<Queue<int>> all = {{3, 6, 9, 9, 100},
                             {1, 5, 9, 9, 12},
                             {5},
                             {},
                             {-5, -5},
                             {3402}
                            };
    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
}

PROVIDED_TEST("recMultiMerge, compare to naiveMultiMerge with various n and k") {
    Queue<int> input;
    Vector<Queue<int>> all;
    for (int n = 100; n <= 10000; n *= 10) {
        for (int k = n/10; k <= n; k *= 2) {
            all.clear();
            for (int i = 0; i < k; i++) {
                all.add({});
            }
            input = createSequence(n);
            distribute(input, all);
            EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
        }
    }
}

STUDENT_TEST("Time naiveMultiMerge operation, n = 500000 ~ 8000000, k fixed as 100") {
    int k = 100;
    Queue<int> input;
    Vector<Queue<int>> all;
    for (int n = 1000000; n <= 16000000; n *= 2) {
        all.clear();
        for (int i = 0; i < k; i++) {
            all.add({});
        }
        input = createSequence(n);
        distribute(input, all);
        TIME_OPERATION(n, recMultiMerge(all));
    }
}

STUDENT_TEST("Time naiveMultiMerge operation, k = 2 ~ 65536 growing quardratically, n fixed as 10,000,000") {
    int n = 10000000;
    Queue<int> input;
    Vector<Queue<int>> all;
    for (long k = 2; k <= 65536 ; k *= k) {
        all.clear();
        for (long i = 0; i < k; i++) {
            all.add({});
        }
        input = createSequence(n);
        distribute(input, all);
        TIME_OPERATION(k, recMultiMerge(all));
    }
}

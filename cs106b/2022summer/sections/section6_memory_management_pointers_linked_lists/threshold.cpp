/*
 * CS106B Section Handout Test Harness: Section 6
 * ----------------------------------------------
 * These problems have been galvanized from years of
 * section handouts that have been worked on by numerous
 * instructors and TA's. Codified by Trip Master and Nick
 * Bowman for CS106B Spring 2020.
 *
 * A huge thank you to Keith Schwarz and Julie Zelenski
 * for creating an amazing testing harness!
 */

#include <iostream>
#include "testing/SimpleTest.h"
#include "node.h"
#include "utility.h"
#include "error.h"
#include "stack.h"
#include "set.h"
using namespace std;

bool valueWithinThreshold (double value, double target, double threshold) {
    return value >= target - threshold && value <= target + threshold;
}

/*
 * recursively remvoe nodes whose value is not within the threshold
 * easier to write. However, one call stack is required for each single element,
 * so it is not scalable to long linked lists
 */
void recursiveRemover(DoubleNode*& front, double value, double threshold) {
    if (front == nullptr) {
        return;
    }
    if (valueWithinThreshold(front->data, value, threshold)) {
        front = front->next;
        recursiveRemover(front, value, threshold);
    } else {
        recursiveRemover(front->next, value, threshold);
    }
}

/*
 * iteratively remvoe nodes whose value is not within the threshold
 * in the first while loop, remove the consecutive elements at the front
 * of the linked list by rewiring the head of the original linked list to the first
 * node having value within the threshold.
 * in the second while loop, rewiring the intermediate nodes if necessary
 */
void iterativeRemover(DoubleNode*& front, double value, double threshold) {
    if (front == nullptr) {
        return;
    }
    while (front != nullptr
           && valueWithinThreshold(front->data, value, threshold)) {
        DoubleNode* trash = front;
        front = front->next;
        delete trash;
    }
    if (front != nullptr) {
        DoubleNode* cur = front;
        while (cur->next != nullptr) {
            if (valueWithinThreshold(cur->next->data, value, threshold)) {
                DoubleNode* trash = cur->next;
                cur->next = cur->next->next;
                delete trash;
            } else {
                cur = cur->next;
            }
        }
    }
}

void removeAllThreshold(DoubleNode*& front, double value, double threshold) {
    return iterativeRemover(front, value, threshold);
}

/* * * * * Provided Tests Below This Point * * * * */
PROVIDED_TEST("Example from handout") {
    DoubleNode *originalList = createDoubleListFromVector({ 3.0, 9.0, 4.2, 2.1, 3.3, 2.3, 3.4, 4.0, 2.9, 2.7, 3.1, 18.2});
    DoubleNode *solnList = createDoubleListFromVector({9.0, 4.2, 2.1, 2.3, 3.4, 4.0, 18.2});

    removeAllThreshold(originalList, 3.0, 0.3);
    EXPECT(doubleListEqual(originalList, solnList));
    freeDoubleList(originalList);
    freeDoubleList(solnList);
}


STUDENT_TEST("Test the case when all nodes are removed") {
    DoubleNode *originalList = createDoubleListFromVector({ 3.0, 9.0, 4.2, 2.1, 3.3, 2.3, 3.4, 4.0, 2.9, 2.7, 3.1, 18.2});
    DoubleNode *solnList = createDoubleListFromVector({});

    removeAllThreshold(originalList, 10.0, 10.0);

    EXPECT(doubleListEqual(originalList, solnList));
    freeDoubleList(originalList);
    freeDoubleList(solnList);
}

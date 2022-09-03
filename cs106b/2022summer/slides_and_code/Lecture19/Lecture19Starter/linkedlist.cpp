/*************************************************
 * File: LinkedLists.cpp
 *
 * Lots of fucntions that show off the many different ways
 * to do linked list operations!
 */

#include <iostream>
#include <string>
#include "linkedlist.h"
using namespace std;

/* Given a linked list, returns the length of that list. */
int lengthOf(Node* list) {
    /* TODO: Implement me! */
    return 0;
}

/* Prints the contents of a linked list, in order. */
void printList(Node* list) {
    while (list != nullptr) {
        cout << list->data << endl;
        list = list->next;
    }
}

/* Frees all the memory used by a linked list. */
void freeList(Node* list) {
    while (list != nullptr) {
        /* Store where to go next, since we're about to blow up our linked
         * list Node.
         */
        Node* next = list->next;
        delete list;
        list = next;
    }
}

/* Reads a linked list from the user one element at a time, stopping when
 * the user enters an empty list. This returns the list, constructed in
 * reverse order.
 */
Node* readList() {
    Node* result = nullptr;
    while (true) {
        string line = getLine("Next item: ");
        if (line == "") break;

        Node* newNode = new Node;
        newNode->data = line;

        newNode->next = result;
        result = newNode;
    }
    return result;
}

/* Given a linked list, returns the length of that list. Operates recursively. */
int lengthOfRec(Node* list) {
    /* Base Case: The empty list has length zero. */
    if (list == nullptr) return 0;

    /* Recursive Case: If there's a Node at the front of the list, the list
     * has length 1 (for that Node) plus the length of the rest of the list.
     */
    return 1 + lengthOf(list->next);
}

/* Prints the contents of a linked list, in order. Operates recursively. */
void printListRec(Node* list) {
    /* Base Case: There's nothing to print if the list is empty. */
    if (list == nullptr) return;

    /* Recursive Case: Print the first Node, then the rest of the list.
     * Question to ponder: What happens if we swap the next two lines, and why? */
    cout << list->data << endl;
    printList(list->next);
}

/* Frees all the memory used by a linked list. Operates recursively. */
void freeListRec(Node* list) {
    /* Base Case: If the list is empty, there's nothing to do. */
    if (list == nullptr) return;

    /* Recursive Case: Delete the rest of the list, then the first Node.
     *
     * Question to ponder: Why is it a Bad Thing to reorder these lines?
     */
    freeListRec(list->next);
    delete list;
}

void prependTo(Node* list, string data) {
    /* TODO: Implement me! */
}









/* Appends to a linked list represented by the given head pointer. */
void appendTo(Node*& list, string data) {
    /* TODO: Implement me! */
}

/*
 * This helper function is provided a vector of integer values and
 * returns a pointer to the beginning of a linked list containing
 * those values in the specified order. It uses appendTo();
 */
Node* createListWithAppend(Vector<string> values) {
    // From section 6 utility.cpp
    if (values.isEmpty()) return nullptr;
    Node* head = new Node;
    head->data = values[0];
    head->next  = nullptr;

    for (int i = 1; i < values.size(); i++) {
        appendTo(head, values[i]);
    }
    return head;
}

/*
 * This helper function is provided a vector of integer values and
 * returns a pointer to the beginning of a linked list containing
 * those values in the specified order.
 */
Node* createListWithTailPtr(Vector<string> values) {
    /* TODO: Implement me! */
    return nullptr;
}

/* Adds data to a linked list in alphabetical order. Assumes existing list is already
 * sorted alphabetically. */
void alphabeticalAdd(Node*& list, string data) {
    /* TODO: Implement me! */
}

/* Removes all nodes matching dataToRemove from the passed in list (if the data exists). */
void remove(Node*& list, string dataToRemove) {
    /* TODO: Implement me! */
}

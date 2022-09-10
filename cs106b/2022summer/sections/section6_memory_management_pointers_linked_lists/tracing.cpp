/*
 * CS106B 2022 Summer Section 6
 * ----------------------------------------------
 * Tracing Pointers by Reference
 */

#include <iostream>
#include "testing/SimpleTest.h"
#include "node.h"
#include "utility.h"
#include "error.h"
#include "vector.h"
using namespace std;


void confuse(Node* list) {
    list->data = 137;
}

void befuddle(Node* list) {
    list = new Node;
    list->data = 42;
    list->next = nullptr;
}

void confound(Node* list) {
    list->next = new Node;
    list->next->data = 2718;
    list->next->next = nullptr;
}

void bamboozle(Node*& list) {
    list->data = 137;
}

void mystify(Node*& list) {
    list = new Node;
    list->data = 161;
    list->next = nullptr;
}

STUDENT_TEST("Tracing pointers by reference") {
    Node* list = new Node;
    list->data = 1;
    list->next = new Node;
    list->next->data = 3;
    list->next->next = new Node;
    list->next->next->data = 5;
    list->next->next->next = nullptr;

    confuse(list);
    printList(list); // print 137, 3, 5

    befuddle(list); // leaked the created node 42 in the function
    printList(list); // print 137, 3, 5

    confound(list); // leaked 2 nodes 3, 5
    printList(list); // print 137, 2718

    bamboozle(list);
    printList(list); // print 42, 2718

    mystify(list); // leaked 2 nodes 42, 2718
    printList(list); // 161

    freeList(list); // defined at beginning of section handout
}

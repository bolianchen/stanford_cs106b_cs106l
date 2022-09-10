/*
 * CS106B 2022 Summer Section 6
 * ----------------------------------------------
 * Rewiring Linked Lists
 */

#include <iostream>
#include "testing/SimpleTest.h"
#include "node.h"
#include "utility.h"
#include "error.h"
#include "vector.h"
using namespace std;

STUDENT_TEST("Rewiring linked lists - 1") {
    Vector<int> nums = {5, 4, 3};
    Node* list = createListFromVector(nums);
    Node* temp = list->next->next;
    temp->next = list->next;
    temp->next->next = list;
    list->next = nullptr;
    list = temp;
}

STUDENT_TEST("Rewiring linked lists - 2") {
    Vector<int> nums = {5, 4, 3};
    Node* list = createListFromVector(nums);
    list->next->next->next = list;
    Node* list2 = list->next;
    list->next = nullptr;
    list = list2->next;
}

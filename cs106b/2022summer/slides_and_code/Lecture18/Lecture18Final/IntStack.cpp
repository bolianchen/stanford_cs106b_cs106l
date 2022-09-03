#include "IntStack.h"
#include "testing/SimpleTest.h"
using namespace std;

void printList(Node* list) {
    while (list != nullptr) {
        cout << list->data << endl;
        list = list->next;
    }
}


IntStack::IntStack() {
    top = nullptr;
}

IntStack::~IntStack() {
    while (top != nullptr){
        Node* temp = top;
        top = top->next;
        delete temp;
    }
}

void IntStack::push(int value) {
    Node* newNode = new Node;
    newNode->data = value;
    newNode->next = top;
    top = newNode;
}

int IntStack::pop() {
    if (isEmpty()) {
        error("Trying to pop from an empty stack.");
    }
    int value = top->data;
    Node* temp = top;
    top = top->next;
    delete temp;
    //we need to deallocate any node we remove
   // somehow get the data field of the top node and return that
    return value;
}

bool IntStack::isEmpty(){
    return top == nullptr;
}


PROVIDED_TEST("Reverse simple set of values"){
    IntStack stack;

    for (int i = 0; i < 10; i++){
        stack.push(i);
    }

    for (int i = 9; i >= 0; i--){
        EXPECT_EQUAL(i, stack.pop());
    }
}

#include "IntStack.h"
#include "testing/SimpleTest.h"
using namespace std;


IntStack::IntStack() {
    top = nullptr;
}

IntStack::~IntStack() {
    /* FILL ME IN */
}

void IntStack::push(int value) {
    /* FILL ME IN */
}

int IntStack::pop() {
    /* FILL ME IN */
    return 0;
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

/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "recursion.h"
#include "testing/SimpleTest.h"

using namespace std;

/*
 * Keep only the bracketing operators in the input string with recursion
 */
string operatorsFrom(string str) {
    string bracketing = "()[]{}";
    if (str.empty()) {
        return str;
    } else if (bracketing.find(str.substr(0, 1)) != string::npos) {
        return str.substr(0, 1) + operatorsFrom(str.substr(1));
    } else {
        return operatorsFrom(str.substr(1));
    }
}

/*
 * Recursively check if the input string consisting of only bracketing characters is balanced
 */
bool operatorsAreMatched(string ops) {
    if (ops.empty()) {
        return true;
    } else if (ops.find("()") != string::npos) {
        return operatorsAreMatched(ops.replace(ops.find("()"), 2, ""));
    } else if (ops.find("[]") != string::npos) {
        return operatorsAreMatched(ops.replace(ops.find("[]"), 2, ""));
    } else if (ops.find("{}") != string::npos) {
        return operatorsAreMatched(ops.replace(ops.find("{}"), 2, ""));
    } else {
        return false;
    }
}

/*
 * The isBalanced function assumes correct implementation of
 * the above two functions operatorsFrom and operatorsMatch.
 * It uses operatorsFrom to extract the operator characters
 * from the input string and then confirms that those
 * operators are balanced by using operatorsMatch.
 * You should not modify the provided code in the isBalanced
 * function.  If the previous two functions have been implemented
 * correctly, the provided isBalanced will correctly report whether
 * the input string has balanced bracketing operators.
 */
bool isBalanced(string str) {
    string ops = operatorsFrom(str);
    return operatorsAreMatched(ops);
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("operatorsFrom on simple example") {
    EXPECT_EQUAL(operatorsFrom("vec[3]"), "[]");
}

PROVIDED_TEST("operatorsAreMatched on simple example") {
    EXPECT(operatorsAreMatched("{}"));
}

PROVIDED_TEST("isBalanced on example from writeup") {
    string example ="int main() { int x = 2 * (vec[2] + 3); x = (1 + random()); }";
    EXPECT(isBalanced(example));
}

PROVIDED_TEST("isBalanced on non-balanced examples from writeup") {
    EXPECT(!isBalanced("( ( [ a ] )"));
    EXPECT(!isBalanced("3 ) ("));
    EXPECT(!isBalanced("{ ( x } y )"));
}

STUDENT_TEST("operatorsFrom on complex examples") {
    EXPECT_EQUAL(operatorsFrom("*()[[[}}abc"), "()[[[}}");
    EXPECT_EQUAL(operatorsFrom("123[(it is good)} yah]"), "[()}]");
    EXPECT_EQUAL(operatorsFrom("{9[8}6]4()"), "{[}]()");
}

STUDENT_TEST("operatorsAreMatched on negative examples") {
    EXPECT(!operatorsAreMatched("{)"));
    EXPECT(!operatorsAreMatched("a"));
    EXPECT(!operatorsAreMatched("("));
    EXPECT(!operatorsAreMatched(")"));
    EXPECT(!operatorsAreMatched("()}["));
    EXPECT(!operatorsAreMatched("(ABCD)"));
}

STUDENT_TEST("operatorsAreMatched on positive examples") {
    EXPECT(operatorsAreMatched("{([])}"));
    EXPECT(operatorsAreMatched("{{}}[()()]"));
}

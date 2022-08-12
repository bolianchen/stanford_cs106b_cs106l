/*
 * The program is to experience how huge program efficiency can be
 * according to algorithm choices by actually trying different algorihms
 * to compute perfect numbers.
 */

#include "console.h"
#include <iostream>
#include <cmath>
#include "testing/SimpleTest.h"
using namespace std;

/* This function takes one argument `n` and calculates the sum
 * of all proper divisors of `n` excluding itself. To find divisors
 * a loop iterates over all numbers from 1 to n-1, testing for a
 * zero remainder from the division.
 *
 * Note: long is a C++ type is a variant of int that allows for a
 * larger range of values. For all intents and purposes, you can
 * treat it like you would an int.
 */
long divisorSum(long n) {
    long total = 0;
    for (long divisor = 1; divisor < n; divisor++) {
        if (n % divisor == 0) {
            total += divisor;
        }
    }
    return total;
}


/* This function takes one argument `n` and returns a boolean
 * (true/false) value indicating whether or not `n` is perfect.
 * A perfect number is a non-zero positive number whose sum
 * of its proper divisors is equal to itself.
 */
bool isPerfect(long n) {
    return (n != 0) && (n == divisorSum(n));
}

/* This function does an exhaustive search for perfect numbers.
 * It takes one argument `stop` and searches the range 1 to `stop`,
 * checking each number to see whether it is perfect and if so,
 * printing it to the console.
 */
void findPerfects(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfect(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
long smarterSum(long n) {
    long total = 0;
    if (n == 1 || n < 0) {
        return total;
    }
    double squareRoot = sqrt(n);
    for (long divisor = 1; divisor <= squareRoot; divisor++) {
        if (n % divisor == 0) {
            if (divisor == 1 || divisor == squareRoot) {
                total += divisor;
            } else {
                total += divisor + n / divisor;
            }
        }
    }
    return total;
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
bool isPerfectSmarter(long n) {
    return (n != 0) && (n == smarterSum(n));
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void findPerfectsSmarter(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfectSmarter(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
long findNthPerfectEuclid(long n) {
    long found = 0;
    long k = 1;
    while (true) {
        // cast the power of 2 to k from double to long; otherwise, substraction of 1 could be ignored
        long kThMersenne = (long) pow(2, k) - 1;
        if (smarterSum(kThMersenne) == 1) {
            found += 1;
            if (found == n) {
                return (long) pow(2, k-1) * ((long) pow(2, k) - 1);
            }
        }
        k += 1;
    }
}


/* * * * * * Test Cases * * * * * */

/* Note: Do not add or remove any of the PROVIDED_TEST tests.
 * You should add your own STUDENT_TEST tests below the
 * provided tests.
 */

PROVIDED_TEST("Confirm divisorSum of small inputs") {
    EXPECT_EQUAL(divisorSum(1), 0);
    EXPECT_EQUAL(divisorSum(6), 6);
    EXPECT_EQUAL(divisorSum(12), 16);
}

PROVIDED_TEST("Confirm 6 and 28 are perfect") {
    EXPECT(isPerfect(6));
    EXPECT(isPerfect(28));
}

PROVIDED_TEST("Confirm 12 and 98765 are not perfect") {
    EXPECT(!isPerfect(12));
    EXPECT(!isPerfect(98765));
}

PROVIDED_TEST("Test oddballs: 0 and 1 are not perfect") {
    EXPECT(!isPerfect(0));
    EXPECT(!isPerfect(1));
}

PROVIDED_TEST("Confirm 33550336 is perfect") {
    EXPECT(isPerfect(33550336));
}

PROVIDED_TEST("Time trials of findPerfects on doubling input sizes") {
    TIME_OPERATION(10000, findPerfects(10000));
    TIME_OPERATION(20000, findPerfects(20000));
    TIME_OPERATION(40000, findPerfects(40000));
}

// TODO: add your student test cases here
STUDENT_TEST("Inspect if isPerfect would return false for negative numbers") {
    EXPECT(!isPerfect(-1));
    EXPECT(!isPerfect(-100));
    EXPECT(!isPerfect(-1000));
    EXPECT(!isPerfect(-10000));

}

STUDENT_TEST("Time trials of findPerfects to find the maximum size search my computer can complete in one minute") {
    int size = 14500;
    for (int i = 0; i < 4; i += 1) {
        TIME_OPERATION(size, findPerfects(size));
        size *= 2;
    }
}

STUDENT_TEST("Check the correctness of smarterSum") {
    EXPECT_EQUAL(smarterSum(-10), divisorSum(-10));
    EXPECT_EQUAL(smarterSum(-1), divisorSum(-1));
    EXPECT_EQUAL(smarterSum(1), divisorSum(1));
    EXPECT_EQUAL(smarterSum(6), divisorSum(6));
    EXPECT_EQUAL(smarterSum(12), divisorSum(12));
    EXPECT_EQUAL(smarterSum(25), divisorSum(25));
    EXPECT_EQUAL(smarterSum(28), divisorSum(28));
    EXPECT_EQUAL(smarterSum(100), divisorSum(100));
    EXPECT_EQUAL(smarterSum(496), divisorSum(496));
    EXPECT_EQUAL(smarterSum(8128), divisorSum(8128));
}

STUDENT_TEST("Confirm the correctness of isPerfectSmarter") {
    EXPECT(isPerfectSmarter(6));
    EXPECT(isPerfectSmarter(28));
    EXPECT(!isPerfectSmarter(-1));
    EXPECT(!isPerfectSmarter(-100));
    EXPECT(!isPerfectSmarter(0));
    EXPECT(!isPerfectSmarter(1));
    EXPECT(!isPerfectSmarter(12));
    EXPECT(!isPerfectSmarter(98765));
    EXPECT(!isPerfectSmarter(-1));
    EXPECT(!isPerfectSmarter(-100));
    EXPECT(!isPerfectSmarter(-1000));
    EXPECT(!isPerfectSmarter(-10000));
}

STUDENT_TEST("Time trials of findPerfectsSmarter to find the maximum size search my computer can complete in one minute") {
    // 110000 is the maximum size search to be completed in one minute
    int size = 575000;
    for (int i = 0; i < 4; i += 1) {
        TIME_OPERATION(size, findPerfectsSmarter(size));
        size *= 2;
    }
}

STUDENT_TEST("Check the correctness of findNthPerfectEuclid") {
    EXPECT_EQUAL(findNthPerfectEuclid(1), 6);
    EXPECT_EQUAL(findNthPerfectEuclid(2), 28);
    EXPECT_EQUAL(findNthPerfectEuclid(3), 496);
    EXPECT_EQUAL(findNthPerfectEuclid(4), 8128);
    EXPECT_EQUAL(findNthPerfectEuclid(5), 33550336);
    EXPECT_EQUAL(findNthPerfectEuclid(6), 8589869056);
    EXPECT_EQUAL(findNthPerfectEuclid(7), 137438691328);
    EXPECT_EQUAL(findNthPerfectEuclid(8), 2305843008139952128);
}

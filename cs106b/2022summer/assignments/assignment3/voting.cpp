// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "voting.h"
#include "testing/SimpleTest.h"
using namespace std;

int computePowerIndexPerBlock(Vector<int>& blocks, int majorityVote, int coalitionVotesSoFar,
                              int targetIndex, int index) {
    if (blocks.isEmpty() || index >= blocks.size()) {
        return 0;
    } else if (targetIndex == index || coalitionVotesSoFar + blocks[index] >= majorityVote) {
        return computePowerIndexPerBlock(blocks, majorityVote, coalitionVotesSoFar, targetIndex, index + 1);
    } else {
        if (coalitionVotesSoFar + blocks[index] + blocks[targetIndex] >= majorityVote) {
            return 1 + computePowerIndexPerBlock(blocks, majorityVote, coalitionVotesSoFar + blocks[index],
                                                 targetIndex, index + 1) +
                   computePowerIndexPerBlock(blocks, majorityVote, coalitionVotesSoFar, targetIndex, index + 1);
        } else {
            return computePowerIndexPerBlock(blocks, majorityVote, coalitionVotesSoFar + blocks[index], targetIndex, index + 1) +
                   computePowerIndexPerBlock(blocks, majorityVote, coalitionVotesSoFar, targetIndex, index + 1);
        }
    }
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
Vector<int> computePowerIndexes(Vector<int>& blocks)
{
    int totalVotes = 0;
    for (int vote: blocks) {
        totalVotes += vote;
    }
    Vector<int> result;
    for (int targetIndex = 0; targetIndex < blocks.size(); targetIndex++) {
        result.add(computePowerIndexPerBlock(blocks, totalVotes / 2 + 1, 0, targetIndex, 0));
    }
    int totalCriticalVotes = 0;
    for (int criticalVote: result) {
        totalCriticalVotes += criticalVote;
    }
    for (int i = 0; i < result.size(); i++) {
        result[i] = result[i] * 100 / totalCriticalVotes;
    }

    return result;
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("Test power index, blocks 50-49-1") {
    Vector<int> blocks = {50, 49, 1};
    Vector<int> expected = {60, 20, 20};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks Hempshead 1-1-3-7-9-9") {
    Vector<int> blocks = {1, 1, 3, 7, 9, 9};
    Vector<int> expected = {0, 0, 0, 33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks CA-TX-NY 55-38-39") {
    Vector<int> blocks = {55, 38, 29};
    Vector<int> expected = {33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks CA-TX-GA 55-38-16") {
    Vector<int> blocks = {55, 38, 16};
    Vector<int> expected = {100, 0, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks EU post-Nice") {
    // Estonia is one of those 4s!!
    Vector<int> blocks = {29,29,29,29,27,27,14,13,12,12,12,12,12,10,10,10,7,7,7,7,7,4,4,4,4,4,3};
    Vector<int> expected = {8, 8, 8, 8, 7, 7, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Time power index operation") {
    Vector<int> blocks;
    for (int i = 0; i < 15; i++) {
        blocks.add(randomInteger(1, 10));
    }
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
}

STUDENT_TEST("Time power index operation") {
    Vector<int> blocks;
    for (int size = 14; size <= 28 ; size+=2) {
        blocks.clear();
        for (int i = 0; i < size; i++) {
            blocks.add(randomInteger(1, 100));
        }
        TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
    }
}



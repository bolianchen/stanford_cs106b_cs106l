#include <iostream>
#include "testing/SimpleTest.h"
#include "console.h"
#include "vector.h"
using namespace std;

struct BackpackItem {
    int survivalValue;  // You can assume this value will always >= 0
    int weight;         // You can assume this value will always >= 0
};

int fillBackpackHelper(Vector<BackpackItem>& items, int remainingCapacity, int curValue, int index) {
    // return the current value when all the items have been considered or no remaining capacity
    if (index >= items.size() || remainingCapacity == 0) {
        return curValue;
    } else {
        BackpackItem item = items[index];
        int choice1 = 0;
        // add an item only when its weight does not exceed the remaining capacity
        if (item.weight <= remainingCapacity) {
            choice1 = fillBackpackHelper(items, remainingCapacity - item.weight,
                                         curValue + item.survivalValue, index + 1);
        }
        int choice2 = fillBackpackHelper(items, remainingCapacity, curValue, index + 1);
        return max(choice1, choice2);
    }
}

/*
 * Return the maximum survival
 */

int fillBackpack(Vector<BackpackItem>& items, int targetWeight){
    return fillBackpackHelper(items, targetWeight, 0, 0);
}

PROVIDED_TEST("Simple knapsack examples.") {
    int targetWeight = 18;
    Vector<BackpackItem> items = {
        {12, 4},
        {10,6},
        {8, 5},
        {11, 7},
        {14, 3},
        {7, 1},
        {9, 6}
    };
    EXPECT_EQUAL(fillBackpack(items, targetWeight), 44);

    targetWeight = 25;
    items = {
       {5, 6},
       {20, 15},
       {3, 11},
       {50, 12},
       {5, 6},
       {4, 11},
       {15, 13},
       {12, 7},
       {6, 17},
       {7, 13}
    };

    EXPECT_EQUAL(fillBackpack(items, targetWeight), 67);

    targetWeight = 5;
    items = {
       {3, 2},
       {4, 3},
       {5, 4},
       {6, 5}
    };

    EXPECT_EQUAL(fillBackpack(items, targetWeight), 7);
}

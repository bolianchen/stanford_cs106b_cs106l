#pragma once
#include <string>
#include "testing/MemoryDiagnostics.h"

struct Node {
    int data;
    Node *next;
    TRACK_ALLOCATIONS_OF(Node);
};

struct StringNode {
    std::string data;
    StringNode *next;
};

struct DoubleNode {
    double data;
    DoubleNode *next;
    TRACK_ALLOCATIONS_OF(DoubleNode);
};

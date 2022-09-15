#include "bits.h"
#include "treenode.h"
#include "huffman.h"
#include "map.h"
#include "vector.h"
#include "priorityqueue.h"
#include "strlib.h"
#include "testing/SimpleTest.h"
using namespace std;

/**
 * Given a Queue<Bit> containing the compressed message bits and the encoding tree
 * used to encode those bits, decode the bits back to the original message text.
 *
 * You can assume that tree is a well-formed non-empty encoding tree and
 * messageBits queue contains a valid sequence of encoded bits.
 *
 * Your implementation may change the messageBits queue however you like. There
 * are no requirements about what it should look like after this function
 * returns. The encoding tree should be unchanged.
 *
 * Implementation Details
 * if messageBits is not empty, dequeue a Bit to know which side to traverse the tree from the root
 * once a leaf is reached, concatenate the character to the output and traverse from the root again
 */
string decodeText(EncodingTreeNode* tree, Queue<Bit>& messageBits) {
    if (tree == nullptr || tree->isLeaf()) {
        error("the input tree is empty or a leaf, it is invalid");
    }
    string text = "";
    EncodingTreeNode* cur = tree;
    while (!messageBits.isEmpty()) {
        if (messageBits.dequeue() == Bit(0)) {
            cur = cur->zero;
        } else {
            cur = cur->one;
        }
        if (cur == nullptr) {
            error("the input tree has a node with one non-null child, it is invalid");
        }
        if (messageBits.isEmpty() && !cur->isLeaf()) {
            error("the input tree and message bits are not a valid pair");
        }
        if (cur->isLeaf()) {
            text += cur->getChar();
            cur = tree;
        }
    }
    return text;
}

/**
 * Reconstruct encoding tree from flattened form Queue<Bit> and Queue<char>.
 *
 * You can assume that the queues are well-formed and represent
 * a valid encoding tree.
 *
 * Your implementation may change the queue parameters however you like. There
 * are no requirements about what they should look like after this function
 * returns.
 *
 * Implementation Details
 * construct trees with depth-first search along zero and then one
 */


EncodingTreeNode* unflattenTree(Queue<Bit>& treeShape, Queue<char>& treeLeaves) {
    if (!treeShape.isEmpty()) {
        bool isLeaf = (treeShape.dequeue() == Bit(0));
        if (isLeaf) {
            return new EncodingTreeNode(treeLeaves.dequeue());
        } else {
            return new EncodingTreeNode(
                        unflattenTree(treeShape, treeLeaves),
                        unflattenTree(treeShape, treeLeaves));
        }
    }
    return nullptr;
}

/**
 * Decompress the given EncodedData and return the original text.
 *
 * You can assume the input data is well-formed and was created by a correct
 * implementation of compress.
 *
 * Your implementation may change the data parameter however you like. There
 * are no requirements about what it should look like after this function
 * returns.
 */
string decompress(EncodedData& data) {
    EncodingTreeNode* tree = unflattenTree(data.treeShape, data.treeLeaves);
    string text = decodeText(tree, data.messageBits);
    deallocateTree(tree);
    return text;
}

/**
 * Constructs an optimal Huffman coding tree for the given text, using
 * the algorithm described in lecture.
 *
 * Reports an error if the input text does not contain at least
 * two distinct characters.
 *
 * When assembling larger trees out of smaller ones, make sure to set the first
 * tree dequeued from the queue to be the zero subtree of the new tree and the
 * second tree as the one subtree.
 *
 * Implementation Details
 * 1. Build a map between characters and their frequency
 *
 * 2. For each the key, value pair in the map,
 * convert key to a subtree containing a leaf node and
 * enqueue the subtree and its corresponding value to a priority queue
 *
 * 3. If the priority queue is not empty, dequeue two elements from it
 * and build a new subtree with a corresponding value that is the sum
 * of the priorities of the two dequeued elements. Enqueue the new subtree and the value
 * to the priority queue.
 *
 * Return the tree built finally
 */
EncodingTreeNode* buildHuffmanTree(string text) {
    Map<char, int> chToFrequency;
    EncodingTreeNode *tree;
    for (int i = 0; i < text.size(); i++) {
        char ch = text[i];
        int frequency = 1;
        if (chToFrequency.containsKey(ch)) {
            frequency += chToFrequency.get(ch);
        }
        chToFrequency.put(ch, frequency);
    }

    if (chToFrequency.size() <= 1) {
        error("the input text does not contain at least two distinct characters");
    }

    PriorityQueue<EncodingTreeNode*> queue;
    PriorityQueue<char> queue2;
    for (char ch: chToFrequency) {
        queue.enqueue(new EncodingTreeNode(ch), chToFrequency.get(ch));
        queue2.enqueue(ch, chToFrequency.get(ch));
    }

    while (!queue2.isEmpty()) {
        cout << queue2.dequeue() << endl;
    }

    while (!queue.isEmpty()) {
        int pZero = queue.peekPriority();
        EncodingTreeNode *zero = queue.dequeue();
        int pOne = queue.peekPriority();
        EncodingTreeNode *one = queue.dequeue();
        tree = new EncodingTreeNode(zero, one);
        if (!queue.isEmpty()) {
            queue.enqueue(tree, pZero + pOne);
        } else {
            break;
        }
    }
    return tree;
}

/**
 * Given a string and an encoding tree, encode the text using the tree
 * and return a Queue<Bit> of the encoded bit sequence.
 *
 * You can assume tree is a valid non-empty encoding tree and contains an
 * encoding for every character in the text.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */

void buildCharToBitsMap(EncodingTreeNode* tree, Queue<Bit> bits, Map<char, Queue<Bit>>& map) {
    if (tree == nullptr) {
        return;
    } else {
        if (tree->isLeaf()) {
            map.put(tree->getChar(), bits);
        } else {
            Queue<Bit> bitsZero = bits;
            bitsZero.enqueue(Bit(0));
            buildCharToBitsMap(tree->zero, bitsZero, map);
            Queue<Bit> bitsOne = bits;
            bitsOne.enqueue(Bit(1));
            buildCharToBitsMap(tree->one, bitsOne, map);
        }
    }
}

Queue<Bit> encodeText(EncodingTreeNode* tree, string text) {
    Map<char, Queue<Bit>> map;
    buildCharToBitsMap(tree, {}, map);
    Queue<Bit> messageBits;
    for (int i = 0; i < text.size(); i++) {
        Queue<Bit> charBits = map.get(text[i]);
        while (!charBits.isEmpty()) {
            messageBits.enqueue(charBits.dequeue());
        }
    }
    return messageBits;
}

/**
 * Flatten the given tree into a Queue<Bit> and Queue<char> in the manner
 * specified in the assignment writeup.
 *
 * You can assume the input queues are empty on entry to this function.
 *
 * You can assume tree is a valid well-formed encoding tree.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
void flattenTree(EncodingTreeNode* tree, Queue<Bit>& treeShape, Queue<char>& treeLeaves) {
    if (tree != nullptr) {
        if (tree->isLeaf()) {
            treeShape.enqueue(Bit(0));
            treeLeaves.enqueue(tree->getChar());
        } else {
            treeShape.enqueue(Bit(1));
            flattenTree(tree->zero, treeShape, treeLeaves);
            flattenTree(tree->one, treeShape, treeLeaves);
        }
    }
}

/**
 * Compress the input text using Huffman coding, producing as output
 * an EncodedData containing the encoded message and flattened
 * encoding tree used.
 *
 * Reports an error if the message text does not contain at least
 * two distinct characters.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
EncodedData compress(string messageText) {
    EncodingTreeNode *huffmanTree = buildHuffmanTree(messageText);
    Queue<Bit> messageBits = encodeText(huffmanTree, messageText);
    Queue<Bit> treeShape;
    Queue<char> treeLeaves;
    flattenTree(huffmanTree, treeShape, treeLeaves);
    deallocateTree(huffmanTree);
    return {treeShape, treeLeaves, messageBits};
}

/* * * * * * Testing Helper Functions Below This Point * * * * * */

EncodingTreeNode* createExampleTree() {
    /* Example encoding tree used in multiple test cases:
     *                *
     *              /   \
     *             T     *
     *                  / \
     *                 *   E
     *                / \
     *               R   S
     */
    /* TODO: Implement this utility function needed for testing. */
    EncodingTreeNode *left = new EncodingTreeNode('R');
    EncodingTreeNode *right = new EncodingTreeNode('S');
    left = new EncodingTreeNode(left, right);
    right = new EncodingTreeNode('E');
    right = new EncodingTreeNode(left, right);
    left = new EncodingTreeNode('T');
    return new EncodingTreeNode(left, right);
}

void deallocateTree(EncodingTreeNode* t) {
    if (t != nullptr) {
        deallocateTree(t->zero);
        deallocateTree(t->one);
        delete t;
    }
}

bool areEqual(EncodingTreeNode* a, EncodingTreeNode* b) {
    if (a == nullptr || b == nullptr) {
        return a == b;
    } else if (a->isLeaf() != b->isLeaf()) {
        return false;
    } else {
        if (a->isLeaf()) return a->ch == b->ch;
        return areEqual(a->zero, b->zero) && areEqual(a->one, b->one);
    }
}

/* * * * * * Test Cases Below This Point * * * * * */

/* TODO: Write your own student tests. */

STUDENT_TEST("Test deallocateTree") {
    EncodingTreeNode *tree = createExampleTree();
    deallocateTree(tree);
}

STUDENT_TEST("Test areEqual") {
    EncodingTreeNode* sTree = new EncodingTreeNode('A');
    EncodingTreeNode* sTree2 = new EncodingTreeNode('A');
    EncodingTreeNode* sTree3 = new EncodingTreeNode('Z');
    EXPECT(areEqual(sTree, sTree2));
    EXPECT(areEqual(sTree2, sTree));
    EXPECT(!areEqual(sTree, sTree3));
    EXPECT(!areEqual(sTree3, sTree));

    EncodingTreeNode* exTree = createExampleTree();
    EncodingTreeNode* exTree2 = createExampleTree();
    EXPECT(areEqual(exTree, exTree2));
    EXPECT(areEqual(exTree->zero, exTree2->zero));
    EXPECT(areEqual(exTree->one, exTree2->one));
    EXPECT(!areEqual(exTree, exTree2->zero));
    EXPECT(!areEqual(exTree, exTree2->one));
    EXPECT(!areEqual(exTree->zero, exTree2->one));

    EXPECT(areEqual(nullptr, nullptr));
    EXPECT(!areEqual(sTree, nullptr));
    EXPECT(!areEqual(nullptr, sTree));

    deallocateTree(sTree);
    deallocateTree(sTree2);
    deallocateTree(sTree3);
    deallocateTree(exTree);
    deallocateTree(exTree2);
}

STUDENT_TEST("Test decodeText with a larger tree") {
    /* expand the example tree to the tree below
     *
     * 			*
     * 		  /	 \
     * 	     M	   *
     * 			  /  \
     *           O    *
     *              /   \
     *             T     *
     *                  / \
     *                 *   E
     *                / \
     *               R   S
     */
    EncodingTreeNode* tree = createExampleTree();
    tree = new EncodingTreeNode(new EncodingTreeNode('O'), tree);
    tree = new EncodingTreeNode(new EncodingTreeNode('M'), tree);
    Queue<Bit> messageBits = {1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0};
    EXPECT_EQUAL(decodeText(tree, messageBits), "REST");
    messageBits = {1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0,
                   1, 1, 1, 0, 0, 1, 0, 1, 0, 0};
    EXPECT_EQUAL(decodeText(tree, messageBits), "RESTROOM");
    deallocateTree(tree);
}

STUDENT_TEST("Test unflattenTree") {
    EncodingTreeNode* tree1 = createExampleTree(); // see diagram above
    EncodingTreeNode* tree2 = new EncodingTreeNode(new EncodingTreeNode('O'), tree1);
    tree2 = new EncodingTreeNode(new EncodingTreeNode('M'), tree2);
    EXPECT(!areEqual(tree1, tree2));
    Queue<Bit>  treeShape  = { 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeLeaves = { 'M', 'O', 'T', 'R', 'S', 'E' };
    tree1 = unflattenTree(treeShape, treeLeaves);
    EXPECT(areEqual(tree1, tree1));

    deallocateTree(tree1);
    deallocateTree(tree2);
}

STUDENT_TEST("Test decompress with the examples in the warmup") {
    EncodedData data = {
        {1, 1, 0, 1, 0, 0, 0}, // treeShape
        { 'N', 'M', 'S', 'O' },  // treeLeaves
        {0, 1, 0, 1, 1, 0, 0, 0, 1, 1} // messageBits
    };

    EXPECT_EQUAL(decompress(data), "MOONS");

    data = {
        {1, 0, 1, 0, 0}, // treeShape
        {'E', 'W', 'K'},  // treeLeaves
        {0, 0, 1, 0, 1, 1} // messageBits
    };
    EXPECT_EQUAL(decompress(data), "EEWK");

    data = {
        {1, 1, 0, 1, 0, 0, 0}, // treeShape
        {'N', 'M', 'S', 'O'},  // treeLeaves
        {0, 1, 1, 1, 0, 0} // messageBits
    };
    EXPECT_EQUAL(decompress(data), "SON");
}

STUDENT_TEST("Test encodeText") {
    /* expand the example tree to the tree below
     *
     * 			*
     * 		  /	 \
     * 	     M	   *
     * 			  /  \
     *           O    *
     *              /   \
     *             T     *
     *                  / \
     *                 *   E
     *                / \
     *               R   S
     */
    EncodingTreeNode* tree = createExampleTree();
    tree = new EncodingTreeNode(new EncodingTreeNode('O'), tree);
    tree = new EncodingTreeNode(new EncodingTreeNode('M'), tree);
    Queue<Bit> messageBits = encodeText(tree, "RESTROOM");
    EXPECT_EQUAL(decodeText(tree, messageBits), "RESTROOM");
    messageBits = encodeText(tree, "MEOSTR");
    EXPECT_EQUAL(decodeText(tree, messageBits), "MEOSTR");
    deallocateTree(tree);
}

STUDENT_TEST("Test flattenTree") {
    EncodingTreeNode* tree = createExampleTree();
    tree = new EncodingTreeNode(new EncodingTreeNode('O'), tree);
    tree = new EncodingTreeNode(new EncodingTreeNode('M'), tree);
    Queue<Bit> treeShape;
    Queue<char> treeLeaves;
    flattenTree(tree, treeShape, treeLeaves);
    EncodingTreeNode* recoveredTree = unflattenTree(treeShape, treeLeaves);
    EXPECT(areEqual(tree, recoveredTree));
    deallocateTree(tree);
    deallocateTree(recoveredTree);
}

STUDENT_TEST("Test buildHuffmanTree with ties breaking and with invalid input texts") {

    EncodingTreeNode *huffmanTree = buildHuffmanTree("BOOKKEEPER");

    Queue<Bit>  treeShape  = {1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0};
    Queue<char> treeLeaves = {'O', 'K', 'B', 'R', 'P', 'E'};
    EncodingTreeNode *expectedTree = unflattenTree(treeShape, treeLeaves);

    EXPECT(areEqual(huffmanTree, expectedTree));
    deallocateTree(huffmanTree);
    deallocateTree(expectedTree);

    EXPECT_ERROR(buildHuffmanTree("aaaaa"));
    EXPECT_ERROR(buildHuffmanTree(""));
}

STUDENT_TEST("Test the whole pipeline") {
    /*
     * manually testings with the console main program
     * 1. uncompress mystery.jpg.huf -> compress -> uncompress (OK)
     * 2. compress excellent.wav -> decompress (OK)
     * 3. compress dream.txt -> decomress (OK)
     * 4. compress mystery.jpg.huf -> compress -> compress -> uncompress -> uncompress -> uncompress (OK)
     * 5. compress ascii.txt -> uncompress (OK)
     */
}


/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("decodeText, small example encoding tree") {
    EncodingTreeNode* tree = createExampleTree(); // see diagram above
    EXPECT(tree != nullptr);

    Queue<Bit> messageBits = { 1, 1 }; // E
    EXPECT_EQUAL(decodeText(tree, messageBits), "E");

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(decodeText(tree, messageBits), "SET");

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1}; // STREETS
    EXPECT_EQUAL(decodeText(tree, messageBits), "STREETS");

    deallocateTree(tree);
}

PROVIDED_TEST("unflattenTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit>  treeShape  = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeLeaves = { 'T', 'R', 'S', 'E' };
    EncodingTreeNode* tree = unflattenTree(treeShape, treeLeaves);

    EXPECT(areEqual(tree, reference));

    deallocateTree(tree);
    deallocateTree(reference);
}

PROVIDED_TEST("decompress, small example input") {
    EncodedData data = {
        { 1, 0, 1, 1, 0, 0, 0 }, // treeShape
        { 'T', 'R', 'S', 'E' },  // treeLeaves
        { 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1 } // messageBits
    };

    EXPECT_EQUAL(decompress(data), "TRESS");
}

PROVIDED_TEST("buildHuffmanTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    EncodingTreeNode* tree = buildHuffmanTree("STREETTEST");
    EXPECT(areEqual(tree, reference));

    deallocateTree(reference);
    deallocateTree(tree);
}

PROVIDED_TEST("encodeText, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above

    Queue<Bit> messageBits = { 1, 1 }; // E
    EXPECT_EQUAL(encodeText(reference, "E"), messageBits);

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(encodeText(reference, "SET"), messageBits);

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1 }; // STREETS
    EXPECT_EQUAL(encodeText(reference, "STREETS"), messageBits);

    deallocateTree(reference);
}

PROVIDED_TEST("flattenTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit>  expectedShape  = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> expectedLeaves = { 'T', 'R', 'S', 'E' };

    Queue<Bit>  treeShape;
    Queue<char> treeLeaves;
    flattenTree(reference, treeShape, treeLeaves);

    EXPECT_EQUAL(treeShape,  expectedShape);
    EXPECT_EQUAL(treeLeaves, expectedLeaves);

    deallocateTree(reference);
}

PROVIDED_TEST("compress, small example input") {
    EncodedData data = compress("STREETTEST");
    Queue<Bit>  treeShape   = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeChars   = { 'T', 'R', 'S', 'E' };
    Queue<Bit>  messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0 };

    EXPECT_EQUAL(data.treeShape, treeShape);
    EXPECT_EQUAL(data.treeLeaves, treeChars);
    EXPECT_EQUAL(data.messageBits, messageBits);
}

PROVIDED_TEST("Test end-to-end compress -> decompress") {
    Vector<string> inputs = {
        "HAPPY HIP HOP",
        "Nana Nana Nana Nana Nana Nana Nana Nana Batman"
        "Research is formalized curiosity. It is poking and prying with a purpose. – Zora Neale Hurston",
    };

    for (string input: inputs) {
        EncodedData data = compress(input);
        string output = decompress(data);

        EXPECT_EQUAL(input, output);
    }
}


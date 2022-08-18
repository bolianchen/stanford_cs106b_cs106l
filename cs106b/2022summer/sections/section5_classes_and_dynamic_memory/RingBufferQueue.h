/***********************************************************************
 * File: RingBufferQueue.h
 * This file defines the interface of a first-in, first-out data
 * structure called the RingBufferQueue, as defined in Section Handout
 * #4 on Classes and Objects. This data structure is implemented using
 * a dynamically-allocated array that acts a circular buffer.
 *
 * The section handout was created by Marty Stepp, Julie Zelenski, and
 * Kate Rydberg. Links to the section handout and the solution PDF are
 * available from the CS106B course website. This starter code project
 * was put together by Anand Shankar.
 */
#pragma once

#include <iostream>

class RBQueue {

public:

    RBQueue();

    ~RBQueue();

    void enqueue(int elem);

    int dequeue();

    int peek() const;

    bool isEmpty() const;

    bool isFull() const;

    int size() const;

private:
    // TODO: Include any private member functions
    // or member variables here as you deem appropriate.
    // Then, delete these lines.
    int *array;
    int capacity, qSize;
    int head;

    void expandCapacity();

    /* Note: since this is a header file, we've opted to refer to
     * ostream by its "full name" of std::ostream.
     *
     * By listing this method as a "friend", this method can break
     * object encapsulation and access private member variables
     * of the RBQueue object.
     *
     * TODO: delete these lines and write a method comment
     */
    friend std::ostream& operator<<(std::ostream& out, const RBQueue& queue);
};

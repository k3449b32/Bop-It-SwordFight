#ifndef QUEUE_H
#define QUEUE_H

#include <Arduino.h>

#define MAX_QUEUE_SIZE 10  // Define the maximum size of the queue

class Queue {
  private:
    String queue[MAX_QUEUE_SIZE];
    int front;
    int rear;
    int count;
  
  public:
    Queue();
    bool isFull();
    bool isEmpty();
    void enqueue(String value);
    String dequeue();
    String peek();
    int size();
};

#endif

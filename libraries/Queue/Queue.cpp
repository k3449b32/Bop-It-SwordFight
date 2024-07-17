#include "Queue.h"
#include <Arduino.h>

Queue::Queue() {
  front = 0;
  rear = -1;
  count = 0;
}

bool Queue::isFull() {
  return count == MAX_QUEUE_SIZE;
}

bool Queue::isEmpty() {
  return count == 0;
}

void Queue::enqueue(String value) {
  if (!isFull()) {
    rear = (rear + 1) % MAX_QUEUE_SIZE;
    queue[rear] = value;
    count++;
  } else {
    Serial.println("Queue is full");
  }
}

String Queue::dequeue() {
  if (!isEmpty()) {
    String value = queue[front];
    front = (front + 1) % MAX_QUEUE_SIZE;
    count--;
    return value;
  } else {
    Serial.println("Queue is empty");
    return ""; // Indicating error
  }
}

String Queue::peek() {
  if (!isEmpty()) {
    return queue[front];
  } else {
    Serial.println("Queue is empty");
    return ""; // Indicating error
  }
}

int Queue::size() {
  return count;
}

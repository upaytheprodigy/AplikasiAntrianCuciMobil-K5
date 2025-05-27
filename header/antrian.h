#ifndef ANTRIAN_H
#define ANTRIAN_H
#include "mobil.h"

typedef struct NodeAntrian {
    Mobil data;
    struct NodeAntrian* next;
} NodeAntrian;

// Prototipe fungsi
void enqueue(NodeAntrian** front, Mobil data);
Mobil dequeue(NodeAntrian** front);
void printQueue(NodeAntrian* front);

#endif

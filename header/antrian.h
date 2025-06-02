#ifndef ANTRIAN_H
#define ANTRIAN_H
#include "mobil.h"

typedef struct NodeAntrian {
    Mobil data;
    struct NodeAntrian* next;
    struct NodeAntrian* queuePembilasan;
} NodeAntrian;

// Prototipe fungsi
void enqueue(NodeAntrian** front, Mobil data);
Mobil dequeue(NodeAntrian** front);
void printQueue(NodeAntrian* front, const char* jenis); // "VIP" atau "Reguler"
int countQueue(NodeAntrian* front);  // OPSIONAL, untuk membandingkan reguler 1 dan 2
Mobil* findMobil(NodeAntrian* front, int id);  // untuk pembatalan
void deleteMobil(NodeAntrian** front, int id); // untuk menghapus mobil dari queue saat dibatalkan

#endif

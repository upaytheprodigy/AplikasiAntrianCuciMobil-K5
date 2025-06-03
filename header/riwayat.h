#ifndef RIWAYAT_H
#define RIWAYAT_H
#include "mobil.h"

typedef struct NodeRiwayat {
    Mobil data;
    struct NodeRiwayat* next;
    struct NodeRiwayat* prev;
} NodeRiwayat;

void insertRiwayat(NodeRiwayat** head, Mobil data); // Insert data ke riwayat
void printRiwayat(NodeRiwayat* head); // traversal forward
void printRiwayatTerbalik(NodeRiwayat* tail); // traversal backward

#endif

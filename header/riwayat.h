#ifndef RIWAYAT_H
#define RIWAYAT_H
#include "mobil.h"

typedef struct NodeRiwayat {
    Mobil data;
    struct NodeRiwayat* next;
    struct NodeRiwayat* prev;
} NodeRiwayat;

void insertRiwayat(NodeRiwayat** head, Mobil data);
void printRiwayat(NodeRiwayat* head);
void printRiwayatTerbalik(NodeRiwayat* tail); // traversal backward

#endif

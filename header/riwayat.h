#ifndef RIWAYAT_H
#define RIWAYAT_H
#include "mobil.h"
#include "treewaktu.h"

typedef struct NodeRiwayat {
    Mobil data;
    struct NodeRiwayat* next;
    struct NodeRiwayat* prev;
} NodeRiwayat;

extern NodeRiwayat* riwayat;
extern TreeWaktu* rootTreeWaktu;

// Riwayat
void insertRiwayat(NodeRiwayat** head, Mobil data);
void simpanRiwayatKeFile(NodeRiwayat* head);
void tampilkanRiwayatTanggal();

#endif
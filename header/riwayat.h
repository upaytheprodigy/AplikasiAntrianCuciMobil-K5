#ifndef RIWAYAT_H
#define RIWAYAT_H
#include "mobil.h"

typedef struct NodeRiwayat {
    Mobil data;
    struct NodeRiwayat* next;
    struct NodeRiwayat* prev;
} NodeRiwayat;

extern NodeRiwayat* riwayat;

void insertRiwayat(NodeRiwayat** head, Mobil data); // Insert data ke riwayat
void simpanRiwayatKeFile(NodeRiwayat* head); // Simpan riwayat ke file
void printRiwayatFilter(NodeRiwayat* head, int mode, const char* keyword); // Tampilkan riwayat dengan filter
void cariRiwayatMobil(NodeRiwayat* head, int mode, const char* keyword); // Cari riwayat berdasarkan ID atau nama

#endif

/*
Program        : Riwayat Pencucian Mobil
Deskripsi      : Program ini menyimpan dan mengelola riwayat pencucian mobil
                 yang telah selesai diproses menggunakan struktur data Linked List
Pembuat        : Muhammad Naufal Nurmayadi
NIM            : 241511053
*/

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
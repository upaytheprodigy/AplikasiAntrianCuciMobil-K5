#ifndef TREEWAKTU_H
#define TREEWAKTU_H
#include "mobil.h"

typedef struct TreeWaktu {
    Mobil data;
    int waktuSelesai;
    struct TreeWaktu* left;
    struct TreeWaktu* right;
} TreeWaktu;

TreeWaktu* insertTreeWaktu(TreeWaktu* root, Mobil data, int waktuSelesai);
void rekapWaktu(TreeWaktu* root, int waktuAwal, int waktuAkhir, int* total, int* vip, int* reguler);
void rekapPerJam(TreeWaktu* root, int jam[24], int vip[24], int reguler[24]);
void printRekapPerJam(TreeWaktu* root);
void printMobilDalamRentang(TreeWaktu* root, int waktuAwal, int waktuAkhir);
void tampilkanRekapPerJam(TreeWaktu* root);
void tampilkanRekapRentangWaktu(TreeWaktu* root, int jamAwal, int menitAwal, int detikAwal, int jamAkhir, int menitAkhir, int detikAkhir);

#endif

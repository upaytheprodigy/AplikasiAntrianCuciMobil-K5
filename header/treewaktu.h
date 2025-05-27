#ifndef TREEWAKTU_H
#define TREEWAKTU_H
#include "mobil.h"

typedef struct TreeWaktu {
    Mobil data;
    int waktuSelesai;
    struct TreeWaktu* left;
    struct TreeWaktu* right;
} TreeWaktu;

TreeWaktu* insertTree(TreeWaktu* root, Mobil data);
void inorderTraversal(TreeWaktu* root);
void rekapWaktu(TreeWaktu* root, int awal, int akhir, int* total);
void searchByWaktu(TreeWaktu* root, int waktu);

#endif

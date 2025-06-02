#ifndef TREEWAKTU_H
#define TREEWAKTU_H
#include "mobil.h"

typedef struct TreeWaktu {
    Mobil data;
    int waktuSelesai;
    struct TreeWaktu* left;
    struct TreeWaktu* right;
} TreeWaktu;

TreeWaktu* insertTree(TreeWaktu* root, Mobil data); // Insert data ke dalam tree berdasarkan waktu selesai
void inorderTraversal(TreeWaktu* root); // Traversal inorder untuk menampilkan data
void rekapWaktu(TreeWaktu* root, int awal, int akhir, int* total); // Rekap total waktu dari awal hingga akhir
void searchByWaktu(TreeWaktu* root, int waktu); // Cari mobil berdasarkan waktu selesai

#endif

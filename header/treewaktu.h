/*
Program        : Tree Waktu Pencucian Mobil
Deskripsi      : Program ini mengelola dan mencari data waktu pencucian mobil
                 menggunakan struktur data Binary Search Tree (BST) untuk
                 memudahkan pencarian dan pengurutan berdasarkan waktu
Pembuat        : Muhammad Naufal Nurmayadi
NIM            : 241511053
*/

#ifndef TREEWAKTU_H
#define TREEWAKTU_H
#include "mobil.h"

typedef struct TreeWaktu {
    Mobil data;
    int waktuSelesai;
    struct TreeWaktu* left;
    struct TreeWaktu* right;
} TreeWaktu;

// Tree waktu
TreeWaktu* insertTreeWaktu(TreeWaktu* root, Mobil data, int waktuSelesai);
void simpanTreeWaktu(TreeWaktu* root, const char* filename);
TreeWaktu* loadTreeWaktu(const char* filename);
void rekapWaktu(TreeWaktu* root, int waktuAwal, int waktuAkhir, int* total, int* vip, int* reguler);
void rekapPerJam(TreeWaktu* root, int jam[24], int vip[24], int reguler[24]);
void printRekapPerJam(TreeWaktu* root);
void printMobilDalamRentang(TreeWaktu* root, int waktuAwal, int waktuAkhir);
void tampilkanRekapPerJam(TreeWaktu* root);
void tampilkanRekapRentangWaktu(TreeWaktu* root, int jamAwal, int menitAwal, int detikAwal, int jamAkhir, int menitAkhir, int detikAkhir);
void tampilkanRekapPerHari(TreeWaktu* root);
void tampilkanRekapPerJamRentang(TreeWaktu* root);

#endif
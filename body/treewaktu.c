#include "../header/treewaktu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Insert data mobil ke BST berdasarkan waktu selesai
TreeWaktu* insertTreeWaktu(TreeWaktu* root, Mobil data, int waktuSelesai) {
    if (root == NULL) {
        TreeWaktu* node = (TreeWaktu*)malloc(sizeof(TreeWaktu));
        node->data = data;
        node->waktuSelesai = waktuSelesai;
        node->left = node->right = NULL;
        return node;
    }
    if (waktuSelesai < root->waktuSelesai)
        root->left = insertTreeWaktu(root->left, data, waktuSelesai);
    else
        root->right = insertTreeWaktu(root->right, data, waktuSelesai);
    return root;
}

void tampilkanRekapPerJam(TreeWaktu* root) {
    printRekapPerJam(root);
}

void tampilkanRekapRentangWaktu(TreeWaktu* root, int jamAwal, int menitAwal, int detikAwal, int jamAkhir, int menitAkhir, int detikAkhir) {
    time_t now = time(NULL);
    struct tm tAwal = *localtime(&now);
    struct tm tAkhir = *localtime(&now);
    tAwal.tm_hour = jamAwal; tAwal.tm_min = menitAwal; tAwal.tm_sec = detikAwal;
    tAkhir.tm_hour = jamAkhir; tAkhir.tm_min = menitAkhir; tAkhir.tm_sec = detikAkhir;
    time_t waktuAwal = mktime(&tAwal);
    time_t waktuAkhir = mktime(&tAkhir);
    printMobilDalamRentang(root, waktuAwal, waktuAkhir);
}

// Rekap jumlah mobil dalam rentang waktu tertentu (total, VIP, Reguler)
void rekapWaktu(TreeWaktu* root, int waktuAwal, int waktuAkhir, int* total, int* vip, int* reguler) {
    if (!root) return;
    if (root->waktuSelesai >= waktuAwal)
        rekapWaktu(root->left, waktuAwal, waktuAkhir, total, vip, reguler);
    if (root->waktuSelesai >= waktuAwal && root->waktuSelesai <= waktuAkhir) {
        (*total)++;
        if (strcasecmp(root->data.jalur, "VIP") == 0) (*vip)++;
        else (*reguler)++;
    }
    if (root->waktuSelesai <= waktuAkhir)
        rekapWaktu(root->right, waktuAwal, waktuAkhir, total, vip, reguler);
}

// Rekap per jam (berapa mobil selesai per jam, VIP/Reguler)
void rekapPerJam(TreeWaktu* root, int jam[24], int vip[24], int reguler[24]) {
    if (!root) return;
    struct tm* waktu = localtime((time_t*)&root->waktuSelesai);
    int j = waktu->tm_hour;
    jam[j]++;
    if (strcasecmp(root->data.jalur, "VIP") == 0) vip[j]++;
    else reguler[j]++;
    rekapPerJam(root->left, jam, vip, reguler);
    rekapPerJam(root->right, jam, vip, reguler);
}

// Print rekap per jam
void printRekapPerJam(TreeWaktu* root) {
    int jam[24] = {0}, vip[24] = {0}, reguler[24] = {0};
    rekapPerJam(root, jam, vip, reguler);
    printf("=====================================================\n");
    printf("| Jam  | Total Mobil |   VIP   |  Reguler |\n");
    printf("-----------------------------------------------------\n");
    for (int i = 0; i < 24; i++) {
        printf("| %02d:00 |     %-7d |  %-6d |   %-7d |\n", i, jam[i], vip[i], reguler[i]);
    }
    printf("=====================================================\n");
}

// Print semua data mobil dalam rentang waktu tertentu
void printMobilDalamRentang(TreeWaktu* root, int waktuAwal, int waktuAkhir) {
    static int headerPrinted = 0;
    if (!headerPrinted) {
        printf("====================================================================================================\n");
        printf("| %-4s | %-20s | %-8s | %-10s | %-8s | %-8s |\n",
            "ID", "Nama", "Jenis", "Plat", "Jalur", "Selesai");
        printf("----------------------------------------------------------------------------------------------------\n");
        headerPrinted = 1;
    }
    if (!root) return;
    if (root->waktuSelesai >= waktuAwal)
        printMobilDalamRentang(root->left, waktuAwal, waktuAkhir);
    if (root->waktuSelesai >= waktuAwal && root->waktuSelesai <= waktuAkhir) {
        printf("| %-4d | %-20s | %-8s | %-10s | %-8s | %-8s |\n",
            root->data.id,
            root->data.nama,
            root->data.jenisMobil,
            root->data.platNomor,
            root->data.jalur,
            root->data.waktuSelesaiStr
        );
    }
    if (root->waktuSelesai <= waktuAkhir)
        printMobilDalamRentang(root->right, waktuAwal, waktuAkhir);

    // Print footer hanya sekali di akhir
    if (root->left == NULL && root->right == NULL) {
        printf("====================================================================================================\n");
        headerPrinted = 0;
    }
}
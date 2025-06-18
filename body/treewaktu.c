/*
Program        : Tree Waktu Pencucian Mobil
Deskripsi     : Program ini mengelola dan mencari data waktu pencucian mobil
                menggunakan struktur data Binary Search Tree (BST) untuk
                memudahkan pencarian dan pengurutan berdasarkan waktu
Pembuat        : Muhammad Naufal Nurmayadi
NIM           : 241511053
*/

#include "../header/treewaktu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

extern TreeWaktu* rootTreeWaktu;

// ===================== INSERT TREE WAKTU =====================
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

// ===================== REKAP WAKTU (RENTANG) =====================
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

// ===================== REKAP PER JAM =====================
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

// ===================== PRINT REKAP PER JAM =====================
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

// ===================== PRINT MOBIL DALAM RENTANG WAKTU =====================
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

// ===================== TAMPILKAN REKAP PER JAM (WRAPPER) =====================
void tampilkanRekapPerJam(TreeWaktu* root) {
    printRekapPerJam(root);
}

// ===================== TAMPILKAN REKAP RENTANG WAKTU (WRAPPER) =====================
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

void simpanTreeWaktuKeFile(TreeWaktu* root, FILE* file) {
    if (!root) return;
    Mobil* m = &root->data;
    fprintf(file, "%d;%s;%s;%s;%s;%d;%s\n",
        m->id, m->nama, m->jenisMobil, m->platNomor, m->jalur,
        root->waktuSelesai, m->waktuSelesaiStr
    );
    simpanTreeWaktuKeFile(root->left, file);
    simpanTreeWaktuKeFile(root->right, file);
}

void simpanTreeWaktu(TreeWaktu* root, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Gagal membuka file %s untuk penulisan!\n", filename);
        return;
    }
    if (!root) {
        printf("[DEBUG] Tree waktu kosong, tidak ada data yang disimpan.\n");
    }
    simpanTreeWaktuKeFile(root, file);
    fclose(file);
}

TreeWaktu* loadTreeWaktu(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) return NULL;
    TreeWaktu* root = NULL;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        Mobil m;
        int waktuSelesai;
        sscanf(line, "%d;%49[^;];%9[^;];%19[^;];%9[^;];%d;%19[^\n]",
            &m.id, m.nama, m.jenisMobil, m.platNomor, m.jalur,
            &waktuSelesai, m.waktuSelesaiStr
        );
        // parsing field lain jika ada
        root = insertTreeWaktu(root, m, waktuSelesai);
    }
    fclose(file);
    return root;
}

void tampilkanRekapPerHari(TreeWaktu* root) {
    int tgl, bln, thn;
    printf("Masukkan tanggal (dd mm yyyy): ");
    scanf("%d %d %d", &tgl, &bln, &thn);
    getchar();

    struct tm tAwal = {0}, tAkhir = {0};
    tAwal.tm_mday = tgl;
    tAwal.tm_mon = bln - 1;
    tAwal.tm_year = thn - 1900;
    tAwal.tm_hour = 0; tAwal.tm_min = 0; tAwal.tm_sec = 0;
    tAkhir = tAwal;
    tAkhir.tm_hour = 23; tAkhir.tm_min = 59; tAkhir.tm_sec = 59;

    time_t waktuAwal = mktime(&tAwal);
    time_t waktuAkhir = mktime(&tAkhir);

    printMobilDalamRentang(root, waktuAwal, waktuAkhir);
}

void tampilkanRekapPerJamRentang(TreeWaktu* root) {
    int tgl, bln, thn, jamAwal, menitAwal, detikAwal, jamAkhir, menitAkhir, detikAkhir;
    printf("Masukkan tanggal (dd mm yyyy): ");
    scanf("%d %d %d", &tgl, &bln, &thn);
    printf("Masukkan jam awal (hh mm ss): ");
    scanf("%d %d %d", &jamAwal, &menitAwal, &detikAwal);
    printf("Masukkan jam akhir (hh mm ss): ");
    scanf("%d %d %d", &jamAkhir, &menitAkhir, &detikAkhir);
    getchar();

    struct tm tAwal = {0}, tAkhir = {0};
    tAwal.tm_mday = tgl;
    tAwal.tm_mon = bln - 1;
    tAwal.tm_year = thn - 1900;
    tAwal.tm_hour = jamAwal; tAwal.tm_min = menitAwal; tAwal.tm_sec = detikAwal;
    tAkhir.tm_mday = tgl;
    tAkhir.tm_mon = bln - 1;
    tAkhir.tm_year = thn - 1900;
    tAkhir.tm_hour = jamAkhir; tAkhir.tm_min = menitAkhir; tAkhir.tm_sec = detikAkhir;

    time_t waktuAwal = mktime(&tAwal);
    time_t waktuAkhir = mktime(&tAkhir);

    printMobilDalamRentang(root, waktuAwal, waktuAkhir);
}
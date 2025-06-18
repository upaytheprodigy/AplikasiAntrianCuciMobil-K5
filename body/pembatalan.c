/*
Program        : Pembatalan Antrian Cuci Mobil
Deskripsi      : Program ini bertujuan untuk menangani pembatalan antrian mobil
                yang belum masuk proses pencucian dengan menggunakan struktur
                data Stack untuk menyimpan data mobil yang dibatalkan
Pembuat        : Muhammad Naufal Nurmayadi
NIM            : 241511053
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header/pembatalan.h"

// Global stack pointer for pembatalan
NodePembatalan* stackPembatalan = NULL;

// ===================== PUSH (Tambah ke Stack) =====================
void pushPembatalan(NodePembatalan** top, Mobil data) {
    NodePembatalan* newNode = (NodePembatalan*)malloc(sizeof(NodePembatalan));
    if (newNode == NULL) {
        printf("\nMemory allocation failed!\n");
        return;
    }
    newNode->data = data;
    newNode->next = *top;
    *top = newNode;
    printf("\nMobil dengan ID %d berhasil disimpan ke dalam daftar pembatalan\n", data.id);
}

// ===================== POP (Ambil dari Stack) =====================
Mobil popPembatalan(NodePembatalan** top) {
    Mobil emptyMobil = {
        .id = -1,
        .nama = "",
        .jenisMobil = "",
        .platNomor = "",
        .jalur = "",
        .waktuDatang = 0,
        .estimasiSelesai = 0,
        .durasiCuci = 0,
        .durasiBilas = 0,
        .durasiKering = 0,
        .statusProses = 0
    };
    if (*top == NULL) {
        printf("\nDaftar pembatalan kosong!\n");
        return emptyMobil;
    }
    NodePembatalan* temp = *top;
    Mobil poppedMobil = temp->data;
    *top = temp->next;
    free(temp);
    return poppedMobil;
}

// ===================== PRINT STACK =====================
void printStackPembatalan(NodePembatalan* top) {
    if (top == NULL) {
        printf("\nDaftar pembatalan kosong!\n");
        return;
    }
    printf("\n============= DAFTAR MOBIL YANG DIBATALKAN =============\n");
    printf("ID\tNama\t\tPlat\t\tJenis\tJalur\n");
    printf("-----------------------------------------------------\n");
    NodePembatalan* current = top;
    while (current != NULL) {
        printf("%d\t%-15s%-15s%-10s%s\n", 
               current->data.id,
               current->data.nama,
               current->data.platNomor,
               current->data.jenisMobil,
               current->data.jalur);
        current = current->next;
    }
    printf("-----------------------------------------------------\n");
}

// ===================== SIMPAN KE FILE =====================
void simpanPembatalanKeFile(NodePembatalan* top) {
    FILE* file = fopen("pembatalan/pembatalan.txt", "w");
    if (!file) {
        printf("Gagal membuka file pembatalan/pembatalan.txt\n");
        return;
    }
    // VIP
    fprintf(file, "==================== PEMBATALAN VIP ====================\n");
    NodePembatalan* curr = top;
    int adaVIP = 0;
    while (curr) {
        if (strcasecmp(curr->data.jalur, "VIP") == 0) {
            fprintf(file, "ID: %d | Nama: %s | Plat: %s | Jenis: %s | Jalur: %s | Waktu: %s\n",
                curr->data.id, curr->data.nama, curr->data.platNomor, curr->data.jenisMobil, curr->data.jalur, curr->data.waktuDatangStr);
            adaVIP = 1;
        }
        curr = curr->next;
    }
    if (!adaVIP) fprintf(file, "Tidak ada pembatalan VIP.\n");

    // Reguler
    fprintf(file, "\n==================== PEMBATALAN REGULER ====================\n");
    curr = top;
    int adaReg = 0;
    while (curr) {
        if (strcasecmp(curr->data.jalur, "Reguler") == 0) {
            fprintf(file, "ID: %d | Nama: %s | Plat: %s | Jenis: %s | Jalur: %s | Waktu: %s\n",
                curr->data.id, curr->data.nama, curr->data.platNomor, curr->data.jenisMobil, curr->data.jalur, curr->data.waktuDatangStr);
            adaReg = 1;
        }
        curr = curr->next;
    }
    if (!adaReg) fprintf(file, "Tidak ada pembatalan Reguler.\n");
    fclose(file);
}

// ===================== BACA FILE PEMBATALAN =====================
void openPembatalanFile() {
    FILE* file = fopen("pembatalan/pembatalan.txt", "r");
    if (!file) {
        printf("File pembatalan/pembatalan.txt tidak ditemukan.\n");
        return;
    }
    char line[256];
    printf("\n");
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    fclose(file);
    printf("\n");
}
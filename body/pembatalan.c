#include <stdio.h>
#include <stdlib.h>
#include "../header/pembatalan.h"

// Global stack pointer for pembatalan
NodePembatalan* stackPembatalan = NULL;

// Push data mobil ke stack pembatalan
void pushPembatalan(NodePembatalan** top, Mobil data) {
    // Alokasi node baru
    NodePembatalan* newNode = (NodePembatalan*)malloc(sizeof(NodePembatalan));
    
    if (newNode == NULL) {
        printf("\nMemory allocation failed!\n");
        return;
    }
    
    // Copy data mobil ke node baru
    newNode->data = data;
    
    // Set next node baru ke top sebelumnya
    newNode->next = *top;
    
    // Update top ke node baru
    *top = newNode;
    
    printf("\nMobil dengan ID %d berhasil disimpan ke dalam daftar pembatalan\n", data.id);
}

// Pop data mobil dari stack pembatalan
Mobil popPembatalan(NodePembatalan** top) {
    // Inisialisasi mobil kosong dengan nilai default
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
    
    // Cek apakah stack kosong
    if (*top == NULL) {
        printf("\nDaftar pembatalan kosong!\n");
        return emptyMobil;
    }
    
    // Simpan node top sementara
    NodePembatalan* temp = *top;
    
    // Simpan data mobil yang akan di-pop
    Mobil poppedMobil = temp->data;
    
    // Update top ke next node
    *top = temp->next;
    
    // Bebaskan memory node yang di-pop
    free(temp);
    
    return poppedMobil;
}

// Print semua data dalam stack pembatalan
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